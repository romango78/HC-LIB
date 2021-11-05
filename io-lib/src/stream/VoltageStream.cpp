// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "VoltageStream.h"

void VoltageStream::begin(StreamMode t_mode) 
{
    AnalogStream::begin(t_mode);
};

int VoltageStream::read()  
{
    return AnalogStream::read();
};

void VoltageStream::write(int t_data) 
{
    AnalogStream::write(t_data);
};

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

void VoltageStream::setPwm(int t_percentage)
{
    if(t_percentage < 0)
    {
        t_percentage = 0;
    } 
    else if (t_percentage > 100)
    {
        t_percentage = 100;
    }

    int data = static_cast<int>((t_percentage * PWM_MAX)/100);
    AnalogStream::write(data);
};