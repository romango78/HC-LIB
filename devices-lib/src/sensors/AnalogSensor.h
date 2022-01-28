// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _ANALOG_SENSOR_H_
#define _ANALOG_SENSOR_H_

#include "sensors/Sensor.h"
#include "stream/IStream.h"

struct AnalogSensor : ISensor
{
    const uint8_t pin;
    IStream<uint16_t>* const analogStream;

    AnalogSensor(const uint8_t t_type, const uint8_t t_pin, IStream<uint16_t>* const t_analogStream) 
        : ISensor(t_type, DeviceCategory::analog), pin(t_pin), analogStream(t_analogStream) {};
};

#endif