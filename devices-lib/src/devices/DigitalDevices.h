// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _DIGITAL_DEVICES_H_
#define _DIGITAL_DEVICES_H_

#include "devices/Device.h"
#include "stream/IStream.h"
#include <new>

struct DigitalDevice : public IDevice
{
private:
    bool m_isDisposed = false;
    IStream<uint8_t>* clone_stream(const DigitalDevice& source) const
    {
        if(!source.m_isDisposed && source.stream)
        {
            return source.stream->clone();
        }
        return nullptr;
    }

public:
    const uint8_t pin;
    IStream<uint8_t>* const stream;

    DigitalDevice() = delete;
    DigitalDevice(const uint8_t t_type, const uint8_t t_pin, IStream<uint8_t>* const t_stream) 
        : IDevice(t_type, DeviceCategory::digital), pin(t_pin), stream(t_stream) {};
    DigitalDevice(const DigitalDevice& source) noexcept
        : IDevice(source), pin(source.pin), stream(clone_stream(source)) 
        {
            m_isDisposed = false;
        };
    DigitalDevice(DigitalDevice&& source) noexcept
        : IDevice(source), pin(source.pin), stream(source.stream) 
        {
            source.m_isDisposed = true;
        };

    virtual ~DigitalDevice()
    {
        if(!m_isDisposed && stream)
        {
            delete stream;
        }
        m_isDisposed = true;
    }

    DigitalDevice& operator= (const DigitalDevice& source) noexcept
    {
        if(this == &source)
        {
            return *this;
        }

        this->~DigitalDevice();
        new (this) DigitalDevice(source);

        return *this;
    }

    DigitalDevice& operator= (const DigitalDevice&& source) = delete;    
};

struct RelayDevice : public DigitalDevice
{
protected:    
    RelayDevice(const uint8_t t_type, const uint8_t t_pin, IStream<uint8_t>* const t_stream) 
        : DigitalDevice(t_type, t_pin, t_stream) {};
public:
    RelayDevice() = delete;
    RelayDevice(const uint8_t t_pin, IStream<uint8_t>* const t_stream) 
        : RelayDevice(RELAY_DEVICE_TYPE, t_pin, t_stream) {};
    RelayDevice(const RelayDevice& source) noexcept
        : DigitalDevice(source) {};
    RelayDevice(RelayDevice&& source) noexcept
        : DigitalDevice(dynamic_cast<DigitalDevice&&>(source)) {};
    virtual ~RelayDevice() = default;

    RelayDevice& operator= (const RelayDevice& source) noexcept
    {
        if(this != &source)
        {
            DigitalDevice::operator=(source);
        }
        return *this;
    }
};

#endif