// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "RelayDeviceController.h"

err_t RelayDeviceController::on(RelayDevice* t_device)
{
    return setState(t_device, LOW);
}

err_t RelayDeviceController::off(RelayDevice* t_device)
{
    return setState(t_device, HIGH);
}

RelayState RelayDeviceController::getState(RelayDevice* t_device)
{
    return (RelayState)DigitalDeviceController::getState(t_device);
}