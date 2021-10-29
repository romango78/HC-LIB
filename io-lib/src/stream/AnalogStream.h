// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _ANALOG_STREAM_H_
#define _ANALOG_STREAM_H_

#include "adapter/IPortAdapter.h"
#include "BaseStream.h"

#define NO_DATA -1

#define ADC_SCALE 1023.0
#define V_REF 5.0

class IVoltageStream
{
    public:
        virtual float getVoltage() = 0;
};

class AnalogStream : BaseStream<int>, IVoltageStream
{
    private:
        IPortAdapter *m_adapter;                
    public:
        AnalogStream(IPortAdapter* t_adapter) 
            : BaseStream(), m_adapter(t_adapter) {};

        ~AnalogStream() = default;

        void begin(StreamMode t_mode) override;
        int read() override;

        float getVoltage() override;
};

#endif