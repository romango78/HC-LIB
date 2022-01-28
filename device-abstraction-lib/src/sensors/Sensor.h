// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _SENSOR_H_
#define _SENSOR_H_

#include "devices/Device.h"

struct ISensor : IDevice
{
    ISensor(const uint8_t t_type, const DeviceCategory t_category)
        : IDevice(t_type, t_category) {};
};

#endif