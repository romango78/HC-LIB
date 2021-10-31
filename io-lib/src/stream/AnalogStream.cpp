// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "AnalogStream.h"

void AnalogStream::begin(StreamMode t_mode)
{   
    BaseStream::begin(t_mode);
    if(canRead())
    {        
        m_adapter->setInputMode();
    }
    else
    {
        m_adapter->setOutputMode();
    }
};

int AnalogStream::read()
{
    BaseStream::read();
    if(!canRead())
    {
        BaseStream::setLastError(IO_ERROR_STREAM_CLOSED);
        return NO_DATA;
    }
    return m_adapter->read();
};

void AnalogStream::write(int t_data)
{
    BaseStream::write(t_data);
    if(!canWrite())
    {
        BaseStream::setLastError(IO_ERROR_STREAM_CLOSED);
        return;
    }
    m_adapter->write(t_data);    
};


float AnalogStream::getVoltage()
{
    return (float)read() * (V_REF/ADC_SCALE);
};

void AnalogStream::setPwm(int t_percentage)
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
    write(data);
};