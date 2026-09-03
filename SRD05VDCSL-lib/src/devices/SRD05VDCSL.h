// Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file SRD05VDCSL.h
/// @brief Songle SRD-05VDC-SL-C relay device.
#ifndef _HC_LIB_SRD05VDCSL_H_
#define _HC_LIB_SRD05VDCSL_H_

#include "devices/DigitalDevices.h"

/// @brief SRD-05VDC-SL-C relay on a digital pin.
/// @note Drive it with RelayDeviceController (on / off / getState).
struct SRD05VDCSLDevice : public RelayDevice
{
    SRD05VDCSLDevice() = delete;

    /// @brief Initializes the relay on _t_pin_ with _t_stream_.
    /// @param t_pin Digital pin connected to the relay coil input.
    /// @param t_stream Digital stream for that pin. The device takes ownership.
    SRD05VDCSLDevice(const uint8_t t_pin, DigitalStream* const t_stream)
        : RelayDevice(RELAY_DEVICE_TYPE, t_pin, t_stream) {};

    /// @brief Copies type, pin, and a cloned stream.
    SRD05VDCSLDevice(const SRD05VDCSLDevice& source) noexcept
        : RelayDevice(source) {};

    /// @brief Moves the stream pointer. The source no longer owns it.
    SRD05VDCSLDevice(SRD05VDCSLDevice&& source) noexcept
        : RelayDevice(static_cast<RelayDevice&&>(source)) {};

    virtual ~SRD05VDCSLDevice() = default;

    /// @brief Copy-assigns type, pin, and a cloned stream.
    SRD05VDCSLDevice& operator=(const SRD05VDCSLDevice& source) noexcept
    {
        if(this != &source)
        {
            RelayDevice::operator=(source);
        }
        return *this;
    }
};

#endif
