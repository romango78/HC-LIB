// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _I_SENSOR_READER_H_
#define _I_SENSOR_READER_H_

template<typename SensorData, typename Sensor>
class ISensorReader
{
    public:
        ISensorReader() {};
        virtual ~ISensorReader() = default;

        virtual SensorData read(const Sensor& t_sensor) = 0;
};

#endif