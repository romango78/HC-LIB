// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "ZMPT101BReaders.h"
#include <math.h>
#include "board.h"

/// @brief Ensures the timer is initialized and the stream is created.
/// @param t_sensor The sensor to check.
/// @return True if the timer is initialized and the stream is created, false otherwise.
Expected<bool, Error> ZMPT101BAcReaderBase::ensureValidInput(const ZMPT101BSensor& t_sensor) const
{
    if(!m_timer)
    {
        return make_error(DeviceError::TimerIsNotInitialized);
    }
    if(!t_sensor.stream)
    {
        return make_error(IoError::StreamNotCreated);
    }
    return true;
}

/// @brief Waits until the wave is close to zero.
/// @param t_sensor The sensor to wait for.
/// @note The wave is close to zero if the ADC value is close to the mid-point.
void ZMPT101BAcReaderBase::waitUntilWaveCloseToZero(const ZMPT101BSensor& t_sensor) const
{   
    if(m_timer->isStarted())
    {
        m_timer->stop();
    }
    m_timer->start();
    while(!m_timer->isElapsed())
    {
        uint16_t rawValue = static_cast<uint16_t>(t_sensor.stream->read());
        if(t_sensor.isCloseToZero(rawValue))
        {
            break;
        }
    }
    m_timer->stop();
}

/// @brief Reads the ADC raw value.
/// @param t_sensor The sensor to read the value from.
/// @return The ADC raw value.
uint16_t ZMPT101BAcReaderBase::readAdcRawValue(const ZMPT101BSensor& t_sensor) const
{
    uint16_t rawValue = static_cast<uint16_t>(t_sensor.stream->read());
    t_sensor.zero += (rawValue - t_sensor.zero) / ADC_COUNTS;
    return rawValue;
}

/// @brief Converts the ADC value to voltage.
/// @param t_adcValue The ADC value to convert.
/// @param t_calibration_factor The calibration factor.
/// @return The voltage.
float ZMPT101BAcReaderBase::toVoltage(const float t_adcValue, const float t_calibration_factor) const
{
    auto supply_voltage = board::getSupplyVoltage();
    return t_adcValue * t_calibration_factor * supply_voltage / ADC_COUNTS;
}

// The Polynomial Equation 3 (ADC offset from zero -> volts).
#define PolynomialEquation(x) (0.00000412*x*x*x - 0.000857*x*x + 2.675*x - 3.198)

/// @brief Returns RMS volts, or DeviceError::TimerIsNotInitialized / IoError::StreamNotCreated.
/// @param t_sensor The sensor to read from.
/// @return The RMS volts, or DeviceError::TimerIsNotInitialized / IoError::StreamNotCreated.
Expected<ZMPT101B_ACVoltage, Error> ZMPT101BRmsReader::read(const ZMPT101BSensor& t_sensor) const
{
    auto validationResult = ensureValidInput(t_sensor);
    if(!validationResult)
    {
        return make_error(validationResult.getError());
    }
    
    if(!t_sensor.stream->canRead())
    {
        t_sensor.stream->begin(StreamMode::Read);
    }
    // Set initial sensor zero
    t_sensor.zero = ADC_COUNTS>>1;
    // Wait until the wave is close to zero (mid-scale adc) part in sin curve.
    waitUntilWaveCloseToZero(t_sensor);    

    float maxAdcValue = 0.0f;
    float minAdcValue = 1000.0f;

    // Start measurement loop to calculate RMS voltage.
    m_timer->start();
    while(!m_timer->isElapsed())
    {
        uint16_t adcRawValue = readAdcRawValue(t_sensor);        
        float adcAdjValue = adcRawValue - t_sensor.zero;
        
        if(maxAdcValue < adcAdjValue)
        {
            maxAdcValue = adcAdjValue;
        }
        if(minAdcValue > adcAdjValue)
        {
            minAdcValue = adcAdjValue;
        }     
    }
    m_timer->stop();

    float result = toVoltage((maxAdcValue - minAdcValue) / 2 / sqrt(2), t_sensor.calibration_factor);
    return ZMPT101B_ACVoltage(t_sensor, result);
}

/// @brief Returns True RMS volts, or DeviceError::TimerIsNotInitialized / IoError::StreamNotCreated.
/// @param t_sensor The sensor to read from.
/// @return The True RMS volts, or DeviceError::TimerIsNotInitialized / IoError::StreamNotCreated.
Expected<ZMPT101B_ACVoltage, Error> ZMPT101BTrueRmsReader::read(const ZMPT101BSensor& t_sensor) const
{
    auto validationResult = ensureValidInput(t_sensor);
    if(!validationResult)
    {
        return make_error(validationResult.getError());
    }
    
    if(!t_sensor.stream->canRead())
    {
        t_sensor.stream->begin(StreamMode::Read);
    }
    // Set initial sensor zero
    t_sensor.zero = ADC_COUNTS>>1;
    // Wait until the wave is close to zero (mid-scale adc) part in sin curve.
    waitUntilWaveCloseToZero(t_sensor);  

    double totalAdcAdjValue = 0.0;
    uint16_t sampleCount = 0;

    // Start measurement loop to calculate True RMS voltage.
    m_timer->start();
    while(!m_timer->isElapsed())
    {
        uint16_t adcRawValue = readAdcRawValue(t_sensor);        
        float adcAdjValue = adcRawValue - t_sensor.zero;

        totalAdcAdjValue += adcAdjValue * adcAdjValue;
        sampleCount++;
    }
    m_timer->stop();

    float result = (sampleCount == 0) 
        ? 0.0f 
        : toVoltage(static_cast<float>(sqrt(totalAdcAdjValue / sampleCount)), t_sensor.calibration_factor);
    return ZMPT101B_ACVoltage(t_sensor, result);
}

