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

namespace device
{
    inline bool is_digital(IDevice t_device) noexcept
    {
        return t_device.category == DeviceCategory::digital;
    }

    inline bool is_relay(IDevice t_device) noexcept
    {
        if(is_digital(t_device))
        {
            return t_device.type == RELAY_DEVICE_TYPE;
        }
        return false;
    }
}

#endif