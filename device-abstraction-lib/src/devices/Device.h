// Copyright (c) 2026 Roman Gorielov. All Rights Reserved.
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
    None = DEVICE_CATEGORY_NONE,
    Analog = DEVICE_CATEGORY_ANALOG,
    Digital = DEVICE_CATEGORY_DIGITAL
};

/// @brief Type and category of a device.
/// @note Trivially copyable. It has no virtual functions. Copy and assignment replace the whole value.
/// getType() and getCategory() are const, so a caller cannot change the fields after construction.
/// Copying IDevice slices derived types (pin, stream, and other fields are dropped).
/// Pass concrete devices by const reference when those fields are needed.
struct IDevice
{
    /// @brief Initializes the type and category.
    /// @param t_type Device type code (see _devicedef.h_).
    /// @param t_category Analog, digital, or none.
    IDevice(const uint8_t t_type, const DeviceCategory t_category)
        : m_type(t_type), m_category(t_category) {};

    /// @brief Device type code.
    uint8_t getType() const { return m_type; }

    /// @brief Analog, digital, or none.
    DeviceCategory getCategory() const { return m_category; }

private:
    uint8_t m_type;
    DeviceCategory m_category;
};

namespace device
{
    /// @brief True when _t_device_ is digital.
    inline bool is_digital(const IDevice& t_device) noexcept
    {
        return t_device.getCategory() == DeviceCategory::Digital;
    }

    /// @brief True when _t_device_ is analog.
    inline bool is_analog(const IDevice& t_device) noexcept
    {
        return t_device.getCategory() == DeviceCategory::Analog;
    }

    /// @brief True when _t_device_ is a digital relay.
    inline bool is_relay(const IDevice& t_device) noexcept
    {
        return is_digital(t_device) && t_device.getType() == RELAY_DEVICE_TYPE;
    }
}

#endif
