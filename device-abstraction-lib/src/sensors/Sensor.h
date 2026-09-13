// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file Sensor.h
/// @brief Sensor specialization of IDevice.
#ifndef _HC_LIB_SENSOR_H_
#define _HC_LIB_SENSOR_H_

#include "devices/Device.h"

/// @brief A device that produces measurements.
/// @note Copying ISensor slices derived types the same way as IDevice.
struct ISensor : IDevice
{
    /// @brief Initializes the sensor type and category.
    /// @param t_type Sensor type code (see _sensordef.h_).
    /// @param t_category Analog or digital.
    ISensor(const uint8_t t_type, const DeviceCategory t_category)
        : IDevice(t_type, t_category) {};

    virtual ~ISensor() = default;
};

#endif
