// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "ZMPT101BReaders.h"
#include <math.h>

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

    if(m_timer->isStarted())
    {
        m_timer->stop();
    }

    m_timer->setInterval(2 * static_cast<uint32_t>(MILLISECONDS_IN_SECOND / AC_NETWORK_FREQUENCY));
    m_timer->start();

    float maxVoltage = 0.0f;
    float minVoltage = 1000.0f;
    while(!m_timer->isElapsed())
    {
        int16_t adjAdcValue = static_cast<int16_t>(t_sensor.stream->read()) - static_cast<int16_t>(t_sensor.zero);
        float voltage = PolynomialEquation(adjAdcValue);
        if(maxVoltage < voltage)
        {
            maxVoltage = voltage;
        }
        if(minVoltage > voltage)
        {
            minVoltage = voltage;
        }
    }
    m_timer->stop();

    float result = (maxVoltage - minVoltage) / 2 / sqrt(2);
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
