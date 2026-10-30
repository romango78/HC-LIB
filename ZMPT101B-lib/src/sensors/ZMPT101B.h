// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file ZMPT101B.h
/// @brief ZMPT101B AC voltage sensor and its measurement type.
#ifndef _HC_LIB_ZMPT101B_H_
#define _HC_LIB_ZMPT101B_H_

#include "sensors/AnalogSensor.h"
#include "sensors/sensordef.h"
#include "sensors/SensorData.h"
#include "stream/IStream.h"
#include "Q10.h"

/// @brief ZMPT101B analog voltage sensor.
struct ZMPT101BSensor : AnalogSensor
{
    /// @brief ADC mid-point, in counts. Readers start at mid-scale and refine it.
    /// @note Mutable so a const reader can refine the mid-point.
    mutable Q10 zero;
    
    /// @brief Scale applied when converting ADC to millivolts.
    Q10 calibration_factor;

    /// @brief Initializes the sensor.
    ZMPT101BSensor() = delete;

    /// @brief Initializes the sensor on _t_pin_ with _t_stream_.
    /// @param t_pin Analog pin connected to the module OUT.
    /// @param t_stream Analog stream for that pin. The sensor takes ownership.
    /// @param t_calibration_factor Scale applied when converting ADC to millivolts (default 1.0).    
    ZMPT101BSensor(const uint8_t t_pin, IStream<uint16_t>* const t_stream,
        const float t_calibration_factor = 1.0f)
        : AnalogSensor(VOLTAGE_SENSOR_TYPE, t_pin, t_stream),
          zero(Q10()), calibration_factor(Q10(t_calibration_factor)) {};

    /// @brief Sets the ADC mid-point.
    /// @param t_zero The ADC mid-point, in ADC counts.
    void setZero(const uint16_t t_zero) const
    {
        zero = Q10(t_zero);
    }

    /// @brief Adjusts the ADC mid-point based on a new value.
    /// @param t_adcValue The new ADC value.
    /// @note EMA (Exponential Moving Average) filter used here allows to correct zero point on fly.
    void adjustZero(const uint16_t t_adcValue) const
    {
        zero += Q10(0.001) * (Q10(t_adcValue) - zero);
    }

    /// @brief Checks if the value is within ±10% of the mid-point.
    /// @param t_value The ADC value to check.
    /// @return True if the value is close to the mid-point, false otherwise.
    bool isCloseToZero(const uint16_t t_value) const
    {
        const uint32_t scaledValue = static_cast<uint32_t>(t_value) * 10u;
        const uint32_t scaledZero = static_cast<uint32_t>(zero.toInt32());
        return scaledValue > scaledZero * 9u && scaledValue < scaledZero * 11u;
    }
};

/// @brief AC RMS voltage reading in millivolts from a ZMPT101BSensor.
struct ZMPT101B_ACVoltage : SensorData<uint32_t>
{
    ZMPT101B_ACVoltage() = delete;

    /// @brief Initializes a successful reading in millivolts.
    ZMPT101B_ACVoltage(const ZMPT101BSensor& t_sensor, const uint32_t t_data)
        : SensorData<uint32_t>(t_sensor, t_data) {};
};

/// @brief Calibration helpers for ZMPT101BSensor.
class ZMPT101B
{
    public:
        /// @brief Sets _t_sensor.calibration_factor_ so a later read matches the etalon.
        /// @param t_sensor Sensor to scale. The existing factor is left unchanged on failure.
        /// @param t_etalonVoltageInMillivolts Reference RMS from a meter, in millivolts. Must be greater than 0.
        /// @note Reads one RMS window with the factor forced to 1, then stores etalon / measured. Closes the stream.
        static void calibrate(ZMPT101BSensor& t_sensor, uint32_t t_etalonVoltageInMillivolts);
};

#endif
