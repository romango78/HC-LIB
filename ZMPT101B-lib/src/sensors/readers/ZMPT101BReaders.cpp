// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "ZMPT101BReaders.h"
#include <math.h>

#define SUPPLY_VOLTAGE 5.0f

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

uint16_t ZMPT101BAcReaderBase::readAdcRawValue(const ZMPT101BSensor& t_sensor) const
{
    uint16_t rawValue = static_cast<uint16_t>(t_sensor.stream->read());
    t_sensor.zero += (rawValue - t_sensor.zero) / ADC_COUNTS;
    return rawValue;
}

float ZMPT101BAcReaderBase::toVolts(const float t_adcValue) const
{
    return t_adcValue * SUPPLY_VOLTAGE / ADC_COUNTS;
}

// The Polynomial Equation 3 (ADC offset from zero -> volts).
#define PolynomialEquation(x) (0.00000412*x*x*x - 0.000857*x*x + 2.675*x - 3.198)

/// @brief Returns RMS volts, or DeviceError::TimerIsNotInitialized / IoError::StreamNotCreated.
/// @param t_sensor The sensor to read from.
/// @return The RMS volts, or DeviceError::TimerIsNotInitialized / IoError::StreamNotCreated.
Expected<ZMPT101B_ACVoltage, Error> ZMPT101BRmsReader::read(const ZMPT101BSensor& t_sensor) const
{
    if(!m_timer)
    {
        return make_error(DeviceError::TimerIsNotInitialized);
    }
    if(!t_sensor.stream)
    {
        return make_error(IoError::StreamNotCreated);
    }
    if(!t_sensor.stream->canRead())
    {
        t_sensor.stream->begin(StreamMode::Read);
    }

    // Wait until the wave is close to zero (mid-scale adc) part in sin curve.
    waitUntilWaveCloseToZero(t_sensor);    

    float maxAdcValue = 0.0f;
    float minAdcValue = 1000.0f;
    uint8_t halfWaveElapsedCount = 0;
    float adcOffset = static_cast<float>(t_sensor.zero);

    // Start measurement loop to calculate RMS voltage.
    m_timer->start();
    while(halfWaveElapsedCount <= MESURE_RESOLUTION_IN_WAVE_COUNT*2 && !m_timer->isElapsed())
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

        if(t_sensor.isCloseToZero(adcRawValue))
        {
            halfWaveElapsedCount++;
        }        
    }
    m_timer->stop();

    float result = toVolts((maxAdcValue - minAdcValue) / 2 / sqrt(2));
    return ZMPT101B_ACVoltage(t_sensor, result);
}

/// @brief Returns True RMS volts, or DeviceError::TimerIsNotInitialized / IoError::StreamNotCreated.
/// @param t_sensor The sensor to read from.
/// @return The True RMS volts, or DeviceError::TimerIsNotInitialized / IoError::StreamNotCreated.
Expected<ZMPT101B_ACVoltage, Error> ZMPT101BTrueRmsReader::read(const ZMPT101BSensor& t_sensor) const
{
    if(!m_timer)
    {
        return make_error(DeviceError::TimerIsNotInitialized);
    }
    if(!t_sensor.stream)
    {
        return make_error(IoError::StreamNotCreated);
    }
    if(!t_sensor.stream->canRead())
    {
        t_sensor.stream->begin(StreamMode::Read);
    }

    if(m_timer->isStarted())
    {
        m_timer->stop();
    }

    m_timer->setInterval(2 * static_cast<uint32_t>(MILLISECONDS_IN_SECOND / AC_NETWORK_FREQUENCY));
    m_timer->start();

    double totalVoltage = 0.0;
    uint16_t sampleCount = 0;
    while(!m_timer->isElapsed())
    {
        int16_t adjAdcValue = static_cast<int16_t>(t_sensor.stream->read()) - static_cast<int16_t>(t_sensor.zero);
        float voltage = PolynomialEquation(adjAdcValue);
        totalVoltage += voltage * voltage;
        sampleCount++;
    }
    m_timer->stop();

    float result = (sampleCount == 0) ? 0.0f : static_cast<float>(sqrt(totalVoltage / sampleCount));
    return ZMPT101B_ACVoltage(t_sensor, result);
}

