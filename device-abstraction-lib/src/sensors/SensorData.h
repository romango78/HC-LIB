// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _SENSOR_DATA_H_
#define _SENSOR_DATA_H_

#include <stdint.h>
#include "Sensor.h"
#include "errdef.h"

template<typename T>
struct SensorData
{
    const ISensor sensor;
    const T data;
    const err_t error;

    SensorData() = delete;    
    SensorData(const ISensor& t_sensor, const T t_data)
        : sensor(t_sensor), data(t_data), error(NO_ERROR) {};
    SensorData(const ISensor& t_sensor, const err_t t_error)
        : sensor(t_sensor), data(T()), error(t_error) {};
};

#endif