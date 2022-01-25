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

class IVoltageStream : public IStream<uint16_t>
{
    public:
        IVoltageStream(){};
        virtual ~IVoltageStream() = default;

        virtual float getVoltage() = 0;
        virtual void setPwm(const int t_percentage) = 0;
};

class VoltageStream : protected AnalogStream, public IVoltageStream
{
    public:
        VoltageStream(IPortAdapter<int>* const t_adapter) 
            : AnalogStream(t_adapter) {};

        ~VoltageStream() = default;

        void begin(const StreamMode t_mode) override;
        uint16_t read() override;
        void write(const uint16_t t_data) override;
        void end() override;

        uint8_t getState() override;
        
        bool canRead() override;
        bool canWrite() override;
        bool hasError() override;

        err_t getLastError() override;

        float getVoltage() override;
        void setPwm(const int t_percentage) override;        
};

#endif