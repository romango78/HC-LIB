// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "VoltageStream.h"

float VoltageStream::getVoltage()
{
    return (float)AnalogStream::read() * (V_REF/ADC_SCALE);
};

void VoltageStream::setPwm(const int t_percentage)
{
    uint8_t percentage;
    if(t_percentage < 0)
    {
        percentage = 0;
    } 
    else if (t_percentage > 100)
    {
        percentage = 100;
    }
    else
    {
        percentage = t_percentage;
    }

    int data = static_cast<int>((percentage * PWM_MAX)/100);
    AnalogStream::write(data);
};

IStream<uint16_t>* VoltageStream::clone() const
{
    return AnalogStream::clone();
}