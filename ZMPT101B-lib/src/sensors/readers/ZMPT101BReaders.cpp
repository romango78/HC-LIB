// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "ZMPT101BReaders.h"
#include <math.h>

// The Ploynominal Equation 3
#define PolynomialEquation(x) (0.00000412*x*x*x - 0.000857*x*x + 2.675*x - 3.198)

uint16_t ZMPT101BReaderBase::getZero(ZMPT101BSensor* const t_sensor)
{
    if(!t_sensor)
    {
        return 0;
    }
    return reinterpret_cast<ZMPT101BSensor*>(t_sensor)->zero;
};

ZMPT101B_ACVoltage ZMPT101BRmsReader::read(ZMPT101BSensor* const t_sensor)
{   
    if(!m_timer)
    {
        return ZMPT101B_ACVoltage((ZMPT101BSensor *)t_sensor, (err_t)DEVICE_ERROR_TIMER_IS_NOT_INITIALIZED);
    }
    if(!t_sensor)
    {
        return ZMPT101B_ACVoltage((ZMPT101BSensor *)t_sensor, (err_t)ERROR_ARGUMENT_IS_NULL);
    }
    if(!t_sensor->analogStream)
    {
        return ZMPT101B_ACVoltage((ZMPT101BSensor *)t_sensor, (err_t)IO_ERROR_STREAM_NOTCREATED);
    };
    if(!t_sensor->analogStream->canRead())
    {
        t_sensor->analogStream->begin(StreamMode::Read);
    };
    
    if(m_timer->isStarted())
    {
        m_timer->stop();
    }

    m_timer->setInterval(2*static_cast<uint32_t>(MILLISECONDS_IN_SECOND/AC_NETWORK_FREQUENCY));
    m_timer->start();

    float maxVoltage = 0.0f;
    float minVoltage = 1000.0f;            
    while(!m_timer->isElapsed())
    {
        int16_t adjAdcValue = (int16_t)t_sensor->analogStream->read() - getZero(t_sensor);
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

    float result = (maxVoltage - minVoltage)/2/sqrt(2);
    return ZMPT101B_ACVoltage((ZMPT101BSensor *)t_sensor, result);
};

ZMPT101B_ACVoltage ZMPT101BTrueRmsReader::read(ZMPT101BSensor* const t_sensor)
{   
    if(!m_timer)
    {
        return ZMPT101B_ACVoltage((ZMPT101BSensor *)t_sensor, (err_t)DEVICE_ERROR_TIMER_IS_NOT_INITIALIZED);
    }
    if(!t_sensor)
    {
        return ZMPT101B_ACVoltage((ZMPT101BSensor *)t_sensor, (err_t)ERROR_ARGUMENT_IS_NULL);
    }
    if(!t_sensor->analogStream)
    {
        return ZMPT101B_ACVoltage((ZMPT101BSensor *)t_sensor, (err_t)IO_ERROR_STREAM_NOTCREATED);
    };
    if(!t_sensor->analogStream->canRead())
    {
        t_sensor->analogStream->begin(StreamMode::Read);
    };
    
    if(m_timer->isStarted())
    {
        m_timer->stop();
    }

    m_timer->setInterval(2*static_cast<uint32_t>(MILLISECONDS_IN_SECOND/AC_NETWORK_FREQUENCY));
    m_timer->start();

    float totalVoltage = 0.0f;
    uint16_t sampleCount = 0;
    while(!m_timer->isElapsed())
    {
        int16_t adjAdcValue = (int16_t)t_sensor->analogStream->read() - getZero(t_sensor);
        float voltage = PolynomialEquation(adjAdcValue);
        totalVoltage += voltage * voltage;
        sampleCount++;
    }
    m_timer->stop();

    float result = sqrt(totalVoltage/sampleCount);
    return ZMPT101B_ACVoltage((ZMPT101BSensor *)t_sensor, result);
};