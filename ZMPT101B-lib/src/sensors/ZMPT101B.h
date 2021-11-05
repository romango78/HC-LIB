// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _ZMPT101B_H_
#define _ZMPT101B_H_

#include "sensors/Sensor.h"
#include "sensors/SensorData.h"
#include "stream/VoltageStream.h"

// Sensor Type
#define ZMPT101B_SENSOR_TYPE 0x001

#define ZMPT101B_READ_ITERATIONS 10

struct ZMPT101BSensor : AnalogSensor
{
    float zero;
    float sensitivity;

    ZMPT101BSensor(const uint8_t t_pin)
        : AnalogSensor(ZMPT101B_SENSOR_TYPE, t_pin), zero(0), sensitivity(1) {};
};

struct ZMPT101BVoltageDC : SensorData<float>
{
    ZMPT101BVoltageDC() : SensorData() {};
    
    ZMPT101BVoltageDC(ZMPT101BSensor *t_sensor, float t_data)
        : SensorData<float>(t_sensor, t_data) {};    

    ZMPT101BVoltageDC(ZMPT101BSensor *t_sensor, err_t t_error)
        : SensorData<float>(t_sensor, t_error) {};  
};

class ZMPT101B
{
    public:
        static void calibrate(ZMPT101BSensor *t_sensor, IVoltageStream *t_stream)
        {
            if(!t_sensor)
            {
                return;
            }
            if(!t_stream)
            {
                return;
            }
            if(!t_stream->canRead())
            {
                t_stream->begin(StreamMode::Read);
            }
            float voltage = 0;
            for(int index = 0; index < ZMPT101B_READ_ITERATIONS; index++)
            {
                voltage += t_stream->getVoltage();
            }
            t_sensor->zero = voltage / ZMPT101B_READ_ITERATIONS;
            t_stream->end();
        };
};

#endif