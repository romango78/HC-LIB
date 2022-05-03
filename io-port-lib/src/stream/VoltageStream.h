// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _VOLTAGE_STREAM_H_
#define _VOLTAGE_STREAM_H_

#include "AnalogStream.h"

// ADC scale was taken from ATMega328P datasheet
#define ADC_SCALE 1024
#define V_REF 5.0f
#define PWM_MAX 255

class IVoltageStream : public AnalogStream
{
    protected:
        IVoltageStream(IPortAdapter<int>* const t_adapter)
            : AnalogStream(t_adapter) {};
    public:        
        virtual ~IVoltageStream() = default;

        virtual float getVoltage() = 0;
        virtual void setPwm(const int t_percentage) = 0;
};

class VoltageStream : public IVoltageStream
{
    public:
        VoltageStream() = delete;
        VoltageStream(IPortAdapter<int>* const t_adapter) 
            : IVoltageStream(t_adapter) {};

        virtual ~VoltageStream() = default;

        float getVoltage() override;
        void setPwm(const int t_percentage) override; 

        IStream<uint16_t>* clone() const override;
};

#endif