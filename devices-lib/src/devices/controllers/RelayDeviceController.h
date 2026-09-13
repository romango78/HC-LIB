// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file RelayDeviceController.h
/// @brief Relay device controller implementation.
#ifndef _HC_LIB_RELAY_DEVICE_CONTROLLER_H_
#define _HC_LIB_RELAY_DEVICE_CONTROLLER_H_

#include "devices/DigitalDevices.h"
#include "devices/controllers/DigitalDeviceController.h"

#ifndef LOW
#define LOW 0x0
#endif

#ifndef HIGH
#define HIGH 0x1
#endif

/// @brief Relay coil state. On is LOW (active-low).
enum class RelayState : uint8_t
{
    On = LOW,
    Off = HIGH
};

/// @brief Turns a RelayDevice on or off.
class RelayDeviceController : private DigitalDeviceController
{
    public:
        /// @brief Initializes the device controller.
        RelayDeviceController() = default;

        /// @brief Destructor.
        ~RelayDeviceController() = default;

        /// @brief Energizes the relay (writes LOW).
        /// @param t_device The device to energize.
        /// @return The error if the relay was not energized.
        Error on(const RelayDevice& t_device);

        /// @brief De-energizes the relay (writes HIGH).
        /// @param t_device The device to de-energize.
        /// @return The error if the relay was not de-energized.
        Error off(const RelayDevice& t_device);

        /// @brief Reads the relay state from the device stream.
        /// @param t_device The device to get the state of.
        /// @return The state of the relay.
        /// @return The error if the state was not read.
        Expected<RelayState, Error> getState(const RelayDevice& t_device) const;
};

#endif
