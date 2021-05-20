// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _SENSOR_READER_H_
#define _SENSOR_READER_H_

#include "Sensor.h"

template<typename T>
class ISensorReader
{
    protected:
        ISensor* m_sensor;
    public:
        ISensorReader(ISensor *t_sensor) : m_sensor(t_sensor) {};
        virtual ~ISensorReader() = default;
        virtual T read() {};
};

#endif