// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file DigitalDeviceController.h
/// @brief Digital device controller implementation.
#ifndef _HC_LIB_DIGITAL_DEVICE_CONTROLLER_H_
#define _HC_LIB_DIGITAL_DEVICE_CONTROLLER_H_

#include "devices/DigitalDevices.h"
#include "devices/controllers/IDeviceController.h"

/// @brief Writes and reads digital pin state through DigitalDevice::stream.
class DigitalDeviceController : public IDeviceController<uint8_t, DigitalDevice>
{
    public:
        /// @brief Initializes the device controller.
        DigitalDeviceController() = default;

        /// @brief Destructor.
        ~DigitalDeviceController() = default;

        /// @brief Sets the state of the device.
        /// @param t_device The device to set the state of.
        /// @param t_state The state to set the device to.
        /// @return The error if the state was not set.
        Error setState(const DigitalDevice& t_device, const uint8_t& t_state) override;

        /// @brief Gets the state of the device.
        /// @param t_device The device to get the state of.
        /// @return The state of the device.
        /// @return The error if the state was not get.
        Expected<uint8_t, Error> getState(const DigitalDevice& t_device) const override;
};

#endif
