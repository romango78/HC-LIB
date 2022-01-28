// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _RELAY_DEVICE_CONTROLLER_H_
#define _RELAY_DEVICE_CONTROLLER_H_

#include "devices/DigitalDevices.h"
#include "devices/controllers/DigitalDeviceController.h"

#ifndef LOW
#define LOW 0x0
#endif

#ifndef HIGH
#define HIGH 0x1
#endif

enum RelayState
{
    on = LOW,
    off = HIGH
};

class RelayDeviceController : private DigitalDeviceController
{
    public:
        RelayDeviceController() {};
        ~RelayDeviceController() = default;

        err_t on(RelayDevice t_device);
        err_t off(RelayDevice t_device);

        Expected<RelayState> getState(RelayDevice t_device);
};

#endif