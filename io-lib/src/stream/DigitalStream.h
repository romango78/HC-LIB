// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _DIGITAL_STREAM_H_
#define _DIGITAL_STREAM_H_

#include <inttypes.h>
#include "adapter/IPortAdapter.h"
#include "BaseStream.h"

#define NO_DATA 0x0

class DigitalStream : public BaseStream<uint8_t>
{
    private:
        IPortAdapter<uint8_t> *m_adapter;                
    public:
        DigitalStream(IPortAdapter<uint8_t>* t_adapter) 
            : BaseStream(), m_adapter(t_adapter) {};

        ~DigitalStream() = default;

        void begin(StreamMode t_mode) override;
        uint8_t read() override;
        void write(uint8_t t_data) override;

        uint8_t getState() override;
};

#endif