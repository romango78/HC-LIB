// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _SENSOR_READER_H_
#define _SENSOR_READER_H_

#include "sensors/Sensor.h"

template<typename T>
class SensorReader
{
    protected:
        ISensor* m_sensor;
    public:
        SensorReader(ISensor *t_sensor) : m_sensor(t_sensor) {};
        ~SensorReader() = default;

        virtual T read() = 0;
};

#endif