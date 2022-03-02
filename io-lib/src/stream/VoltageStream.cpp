// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "VoltageStream.h"

void VoltageStream::begin(const StreamMode t_mode) 
{
    AnalogStream::begin(t_mode);
};

uint16_t VoltageStream::read()  
{
    return AnalogStream::read();
};

void VoltageStream::write(const uint16_t t_data) 
{
    AnalogStream::write(t_data);
};

uint8_t VoltageStream::getState()
{
    return AnalogStream::getState();  
}

void VoltageStream::end() 
{
    AnalogStream::end();
};

bool VoltageStream::canRead() 
{
    return AnalogStream::canRead();
};

bool VoltageStream::canWrite() 
{
    return AnalogStream::canWrite();
};

bool VoltageStream::hasError() 
{
    return AnalogStream::hasError();
};

err_t VoltageStream::getLastError() 
{
    return AnalogStream::getLastError();
};

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