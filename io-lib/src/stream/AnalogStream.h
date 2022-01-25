// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _ANALOG_STREAM_H_
#define _ANALOG_STREAM_H_

#include <inttypes.h>
#include "adapter/IPortAdapter.h"
#include "BaseStream.h"

#define NO_DATA UINT16_MAX

class AnalogStream : public BaseStream<uint16_t>
{
    private:
        IPortAdapter<int>* const m_adapter;                
    public:
        AnalogStream(IPortAdapter<int>* const t_adapter) 
            : BaseStream(), m_adapter(t_adapter) {};

        ~AnalogStream() = default;

        void begin(const StreamMode t_mode) override;
        uint16_t read() override;
        void write(const uint16_t t_data) override;

        uint8_t getState() override;
};

#endif