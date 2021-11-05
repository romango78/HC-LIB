// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _SENSOR_H_
#define _SENSOR_H_

#include <stdint.h>

// Sensor Categories
#define SENSOR_CATEGORY_ANALOG 0

enum SensorCategory
{
	analog = SENSOR_CATEGORY_ANALOG
};

struct ISensor
{
    const uint8_t type;
    const SensorCategory category;
    ISensor(const uint8_t t_type, const SensorCategory t_category)
        : type(t_type), category(t_category) {};
};

struct AnalogSensor : ISensor
{
    const uint8_t pin;
    AnalogSensor(const uint8_t t_type, const uint8_t t_pin) 
        : ISensor(t_type, SensorCategory::analog), pin(t_pin) {};
};

#endif