// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _ANALOG_DEVICES_H_
#define _ANALOG_DEVICES_H_

#include "devices/Device.h"
#include "stream/IStream.h"

struct AnalogDevice : public IDevice
{
    const uint8_t pin;
    IStream<uint16_t>* const stream;

    AnalogDevice() = delete;
    AnalogDevice(const uint8_t t_type, const uint8_t t_pin, IStream<uint16_t>* const t_stream) 
        : IDevice(t_type, DeviceCategory::analog), pin(t_pin), stream(t_stream) {};
};

#endif