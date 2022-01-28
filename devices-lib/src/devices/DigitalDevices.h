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

struct DigitalDevice : public IDevice
{
    const uint8_t pin;
    IStream<uint8_t>* const stream;

    DigitalDevice(const uint8_t t_type, const uint8_t t_pin, IStream<uint8_t>* const t_stream) 
        : IDevice(t_type, DeviceCategory::digital), pin(t_pin), stream(t_stream) {};
    
    DigitalDevice(const DigitalDevice &t_origin)
        : IDevice(t_origin), pin(t_origin.pin)
        , stream(t_origin.stream) {};
};

struct RelayDevice : public DigitalDevice
{
    protected:    
        RelayDevice(const uint8_t t_type, const uint8_t t_pin, IStream<uint8_t>* const t_stream) 
            : DigitalDevice(t_type, t_pin, t_stream) {};
    public:
        RelayDevice(const uint8_t t_pin, IStream<uint8_t>* const t_stream) 
            : RelayDevice(RELAY_DEVICE_TYPE, t_pin, t_stream) {};        
};

#endif