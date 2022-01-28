// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _ZMPT101B_H_
#define _ZMPT101B_H_

#include "sensors/AnalogSensor.h"
#include "sensors/sensordef.h"
#include "sensors/SensorData.h"
#include "stream/IStream.h"

#define ZMPT101B_READ_ITERATIONS 1000

struct ZMPT101BSensor : AnalogSensor
{
    uint16_t zero;

    ZMPT101BSensor(const uint8_t t_pin, IStream<uint16_t>* const t_analogStream)
        : AnalogSensor(VOLTAGE_SENSOR_TYPE, t_pin, t_analogStream), zero(0) {};
};

struct ZMPT101B_ACVoltage : SensorData<float>
{
    ZMPT101B_ACVoltage() : SensorData() {};
    
    ZMPT101B_ACVoltage(const ZMPT101BSensor* const t_sensor, const float t_data)
        : SensorData<float>(t_sensor, t_data) {};    

    ZMPT101B_ACVoltage(const ZMPT101BSensor* const t_sensor, const err_t t_error)
        : SensorData<float>(t_sensor, t_error) {};  
};

class ZMPT101B
{
    public:
        static void calibrate(ZMPT101BSensor* const t_sensor)
        {
            if(!t_sensor)
            {
                return;
            }
            if(!t_sensor->analogStream)
            {
                return;
            }
            if(!t_sensor->analogStream->canRead())
            {
                t_sensor->analogStream->begin(StreamMode::Read);
            }
            uint32_t adcValue = 0;
            for(int index = 0; index < ZMPT101B_READ_ITERATIONS; index++)
            {
                adcValue += t_sensor->analogStream->read();
            }
            t_sensor->zero = static_cast<uint16_t>(adcValue / ZMPT101B_READ_ITERATIONS);
            t_sensor->analogStream->end();
        };
};

#endif