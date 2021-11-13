// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "ZMPT101BReader.h"
#include <math.h>

// The Ploynominal Equation 3
#define PolynomialEquation(x) (0.00000412*x*x*x - 0.000857*x*x + 2.675*x - 3.198)

float ZMPT101BRmsReader::getSensitivity()
{
    if(!m_sensor)
    {
        return 0.001;
    }
    return reinterpret_cast<ZMPT101BSensor*>(this->m_sensor)->sensitivity;
};

ZMPT101B_ACVoltage ZMPT101BRmsReader::read()
{   
    if(!m_timer)
    {
        return ZMPT101B_ACVoltage((ZMPT101BSensor *)this->m_sensor, (err_t)IO_ERROR_STREAM_NOTCREATED);
    }
    if(!m_stream)
    {
        return ZMPT101B_ACVoltage((ZMPT101BSensor *)this->m_sensor, (err_t)IO_ERROR_STREAM_NOTCREATED);
    };
    if(!m_stream->canRead())
    {
        m_stream->begin(StreamMode::Read);
    };
    
    if(m_timer->isStarted())
    {
        m_timer->stop();
    }

    m_timer->setInterval(2*static_cast<uint32_t>(MILLISECONDS_IN_SECOND/AC_NETWORK_FREQUENCY));
    m_timer->start();

    float maxVoltage = 0.0f;
    float minVoltage = V_REF;            
    while(!m_timer->isElapsed())
    {
        float voltage = m_stream->getVoltage();
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

    float result = (maxVoltage - minVoltage)/2/sqrt(2)/getSensitivity();
    return ZMPT101B_ACVoltage((ZMPT101BSensor *)this->m_sensor, result);
};

float ZMPT101BTrueRmsReader::getZero()
{
    if(!m_sensor)
    {
        return 0;
    }
    return reinterpret_cast<ZMPT101BSensor*>(this->m_sensor)->zero;
};

float ZMPT101BTrueRmsReader::getSensitivity()
{
    if(!m_sensor)
    {
        return 0.001;
    }
    return reinterpret_cast<ZMPT101BSensor*>(this->m_sensor)->sensitivity;
};

ZMPT101B_ACVoltage ZMPT101BTrueRmsReader::read()
{   
    if(!m_timer)
    {
        return ZMPT101B_ACVoltage((ZMPT101BSensor *)this->m_sensor, (err_t)IO_ERROR_STREAM_NOTCREATED);
    }
    if(!m_stream)
    {
        return ZMPT101B_ACVoltage((ZMPT101BSensor *)this->m_sensor, (err_t)IO_ERROR_STREAM_NOTCREATED);
    };
    if(!m_stream->canRead())
    {
        m_stream->begin(StreamMode::Read);
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
        int readData = m_stream->read() - 512;//m_stream->getVoltage() - getZero();
        float voltage = PolynomialEquation(readData);
        totalVoltage += voltage * voltage;
        sampleCount++;
    }
    m_timer->stop();

    float result = sqrt(totalVoltage/sampleCount)/getSensitivity();
    return ZMPT101B_ACVoltage((ZMPT101BSensor *)this->m_sensor, result);
};