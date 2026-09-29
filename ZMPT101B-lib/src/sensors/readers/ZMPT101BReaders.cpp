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

/// @brief EMA (Exponential Moving Average) filter allows to correct zero point on fly
#define EMA_FILTER(y, y0) (0.001*y + 0.999*y0)

/// @brief 1/sqrt(2), used to turn a peak amplitude into RMS.
#define ONE_OVER_SQRT2 0.70710678118f

uint16_t ZMPT101BAcReaderBase::m_supplyVoltageMv = 0;

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
    t_sensor.zero = EMA_FILTER(rawValue, t_sensor.zero);
    return rawValue;
}

/// @brief Converts an ADC amplitude to millivolts.
/// @param t_adcValue The ADC value to convert.
/// @param t_calibration_factor The calibration factor.
/// @return The voltage in millivolts. Non-positive inputs become 0.
uint32_t ZMPT101BAcReaderBase::toMillivolts(const float t_adcValue, const float t_calibration_factor) const
{
    if(m_supplyVoltageMv == 0)
    {
        m_supplyVoltageMv = board::getSupplyVoltage();
    }
    const float millivolts = t_adcValue * t_calibration_factor * static_cast<float>(m_supplyVoltageMv) / ADC_COUNTS;
    if(millivolts <= 0.0f)
    {
        return 0;
    }
    return static_cast<uint32_t>(millivolts + 0.5f);
}

/// @brief Returns RMS millivolts, or DeviceError::TimerIsNotInitialized / IoError::StreamNotCreated.
/// @param t_sensor The sensor to read from.
/// @return The RMS millivolts, or DeviceError::TimerIsNotInitialized / IoError::StreamNotCreated.
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

    const float amplitude = (maxAdcValue - minAdcValue) * 0.5f * ONE_OVER_SQRT2;
    return ZMPT101B_ACVoltage(t_sensor, toMillivolts(amplitude, t_sensor.calibration_factor));
}

/// @brief Returns True RMS millivolts, or DeviceError::TimerIsNotInitialized / IoError::StreamNotCreated.
/// @param t_sensor The sensor to read from.
/// @return The True RMS millivolts, or DeviceError::TimerIsNotInitialized / IoError::StreamNotCreated.
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

    float totalAdcAdjValue = 0.0f;
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

    const float trueRms = (sampleCount == 0)
        ? 0.0f
        : sqrtf(totalAdcAdjValue / static_cast<float>(sampleCount));
    const uint32_t millivolts = toMillivolts(trueRms, t_sensor.calibration_factor);
    return ZMPT101B_ACVoltage(t_sensor, millivolts);
}
