// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "RelayDeviceController.h"

Error RelayDeviceController::on(const RelayDevice& t_device)
{
    return setState(t_device, static_cast<uint8_t>(RelayState::On));
}

Error RelayDeviceController::off(const RelayDevice& t_device)
{
    return setState(t_device, static_cast<uint8_t>(RelayState::Off));
}

Expected<RelayState, Error> RelayDeviceController::getState(const RelayDevice& t_device) const
{
    Expected<uint8_t, Error> state = DigitalDeviceController::getState(t_device);
    if(state.hasValue())
    {
        return Expected<RelayState, Error>(static_cast<RelayState>(state.getValue()));
    }
    return make_error(state.getError());
}
