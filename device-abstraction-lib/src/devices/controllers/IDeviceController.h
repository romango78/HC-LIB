// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file IDeviceController.h
/// @brief Abstraction for reading and writing device state.
#ifndef _HC_LIB_I_DEVICE_CONTROLLER_H_
#define _HC_LIB_I_DEVICE_CONTROLLER_H_

#include "Expected.h"
#include "errors/DeviceErrors.h"

/// @brief Reads and writes _State_ on a _Device_.
/// @tparam State The device state type.
/// @tparam Device The concrete device type. Passed by const reference.
template<typename State, typename Device>
class IDeviceController
{
    public:
        IDeviceController() = default;
        virtual ~IDeviceController() = default;

        /// @brief Applies _t_state_ to _t_device_.
        /// @return _GenericError::NoError_ on success, otherwise an _Error_.
        virtual Error setState(const Device& t_device, const State& t_state) = 0;

        /// @brief Reads the current state of _t_device_.
        /// @return The state, or an _Error_ when the device cannot be read.
        virtual Expected<State, Error> getState(const Device& t_device) const = 0;
};

#endif
