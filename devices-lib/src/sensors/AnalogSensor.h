// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file AnalogSensor.h
/// @brief Analog sensor implementation.
#ifndef _HC_LIB_ANALOG_SENSOR_H_
#define _HC_LIB_ANALOG_SENSOR_H_

#include "sensors/Sensor.h"
#include "stream/IStream.h"
#include <new>

/// @brief Analog sensor implementation.
/// @tparam T The type of the value to read or write.
struct AnalogSensor : ISensor
{
private:
    /// @brief Whether the sensor is disposed.
    bool m_isDisposed = false;

    /// @brief Clones the stream.
    /// @param source The source sensor to clone the stream from.
    /// @return A pointer to the cloned stream.
    IStream<uint16_t>* clone_stream(const AnalogSensor& source) const
    {
        if(!source.m_isDisposed && source.stream)
        {
            return source.stream->clone();
        }
        return nullptr;
    }

public:
    /// @brief The pin number of the sensor.
    const uint8_t pin;

    /// @brief The stream of the sensor.
    IStream<uint16_t>* const stream;

    /// @brief Initializes the sensor.
    AnalogSensor() = delete;

    /// @brief Initializes the sensor with the given type, pin, and stream.
    /// @param t_type The type of the sensor.
    /// @param t_pin The pin number of the sensor.
    /// @param t_stream The stream of the sensor.
    /// @return A pointer to the cloned sensor.
    AnalogSensor(const uint8_t t_type, const uint8_t t_pin, IStream<uint16_t>* const t_stream) 
        : ISensor(t_type, DeviceCategory::Analog), pin(t_pin), stream(t_stream) {};
    AnalogSensor(const AnalogSensor& source) noexcept
        : ISensor(source), pin(source.pin), stream(clone_stream(source)) 
        {
            m_isDisposed = false;
        };

    /// @brief Initializes the sensor with the given source sensor.
    /// @param source The source sensor to initialize the sensor from.
    /// @return A pointer to the cloned sensor.
    AnalogSensor(AnalogSensor&& source) noexcept
        : ISensor(source), pin(source.pin), stream(source.stream) 
        {
            source.m_isDisposed = true;
        };        

    /// @brief Destructor.
    virtual ~AnalogSensor()
    {
        if(!m_isDisposed && stream)
        {
            delete stream;
        }
        m_isDisposed = true;
    }

    /// @brief Assigns the sensor with the given source sensor.
    /// @param source The source sensor to assign the sensor to.
    /// @return A pointer to the assigned sensor.
    AnalogSensor& operator= (const AnalogSensor& source) noexcept
    {
        if(this == &source)
        {
            return *this;
        }

        this->~AnalogSensor();
        new (this) AnalogSensor(source);

        return *this;
    }

    /// @brief Assigns the sensor with the given source sensor.
    /// @param source The source sensor to assign the sensor to.
    /// @return A pointer to the assigned sensor.
    AnalogSensor& operator= (const AnalogSensor&& source) = delete; 
};

#endif