// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <stdint.h>
#include "devicedef.h"
#include "stream/IStream.h"

enum DeviceCategory
{
    analog = DEVICE_CATEGORY_ANALOG,
	digital = DEVICE_CATEGORY_DIGITAL
};

struct IDevice
{
    const uint8_t type;
    const DeviceCategory category;
    IDevice(const uint8_t t_type, const DeviceCategory t_category)
        : type(t_type), category(t_category) {};
};

struct DigitalDevice : public IDevice
{
    const uint8_t pin;
    IStream<uint8_t>* const digitalStream;

    DigitalDevice(const uint8_t t_type, const uint8_t t_pin, IStream<uint8_t>* const t_digitalStream) 
        : IDevice(t_type, DeviceCategory::digital), pin(t_pin), digitalStream(t_digitalStream) {};
};

struct RelayDevice : public DigitalDevice
{
    RelayDevice(const uint8_t t_type, const uint8_t t_pin, IStream<uint8_t>* const t_digitalStream) 
        : DigitalDevice(t_type, t_pin, t_digitalStream) {};
};

#endif