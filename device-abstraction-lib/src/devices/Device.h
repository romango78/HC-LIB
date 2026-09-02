// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file Device.h
/// @brief Device category tags and the IDevice value header.
#ifndef _HC_LIB_DEVICE_H_
#define _HC_LIB_DEVICE_H_

#include <stdint.h>
#include "devicedef.h"

/// @brief Broad class of a device (analog vs digital).
enum class DeviceCategory : uint8_t
{
    Analog = DEVICE_CATEGORY_ANALOG,
    Digital = DEVICE_CATEGORY_DIGITAL
};

/// @brief Type and category of a device.
/// @note Copying IDevice slices derived types (pin, stream, and other fields are dropped).
///       Pass concrete devices by const reference when those fields are needed.
struct IDevice
{
    const uint8_t type;
    const DeviceCategory category;

    /// @brief Initializes the type and category.
    /// @param t_type Device type code (see _devicedef.h_).
    /// @param t_category Analog or digital.
    IDevice(const uint8_t t_type, const DeviceCategory t_category)
        : type(t_type), category(t_category) {};

    virtual ~IDevice() = default;
};

namespace device
{
    /// @brief True when _t_device_ is digital.
    inline bool is_digital(const IDevice& t_device) noexcept
    {
        return t_device.category == DeviceCategory::Digital;
    }

    /// @brief True when _t_device_ is analog.
    inline bool is_analog(const IDevice& t_device) noexcept
    {
        return t_device.category == DeviceCategory::Analog;
    }

    /// @brief True when _t_device_ is a digital relay.
    inline bool is_relay(const IDevice& t_device) noexcept
    {
        return is_digital(t_device) && t_device.type == RELAY_DEVICE_TYPE;
    }
}

#endif
