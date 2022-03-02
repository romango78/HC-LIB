// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _DIGITAL_DEVICE_CONTROLLER_H_
#define _DIGITAL_DEVICE_CONTROLLER_H_

#include "devices/DigitalDevices.h"
#include "devices/controllers/IDeviceController.h"

class DigitalDeviceController : public IDeviceController<uint8_t, DigitalDevice>
{        
    public:
        DigitalDeviceController() {};
        ~DigitalDeviceController() = default;

        err_t setState(DigitalDevice t_device, const uint8_t t_data) override;
        Expected<uint8_t> getState(DigitalDevice t_device) override;
};

#endif