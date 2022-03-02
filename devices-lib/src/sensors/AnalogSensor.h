// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _ANALOG_SENSOR_H_
#define _ANALOG_SENSOR_H_

#include "sensors/Sensor.h"
#include "stream/IStream.h"
#include <new>

struct AnalogSensor : ISensor
{
private:
    bool m_isDisposed = false;
    IStream<uint16_t>* clone_stream(const AnalogSensor& source) const
    {
        if(!source.m_isDisposed && source.stream)
        {
            return source.stream->clone();
        }
        return nullptr;
    }

public:
    const uint8_t pin;
    IStream<uint16_t>* const stream;

    AnalogSensor() = delete;
    AnalogSensor(const uint8_t t_type, const uint8_t t_pin, IStream<uint16_t>* const t_stream) 
        : ISensor(t_type, DeviceCategory::analog), pin(t_pin), stream(t_stream) {};
    AnalogSensor(const AnalogSensor& source) noexcept
        : ISensor(source), pin(source.pin), stream(clone_stream(source)) 
        {
            m_isDisposed = false;
        };
    AnalogSensor(AnalogSensor&& source) noexcept
        : ISensor(source), pin(source.pin), stream(source.stream) 
        {
            source.m_isDisposed = true;
        };        
    virtual ~AnalogSensor()
    {
        if(!m_isDisposed && stream)
        {
            delete stream;
        }
        m_isDisposed = true;
    }

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

    AnalogSensor& operator= (const AnalogSensor&& source) = delete; 
};

#endif