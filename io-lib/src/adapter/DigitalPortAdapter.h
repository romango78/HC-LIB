// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _DIGITAL_PORT_ADAPTER_H_
#define _DIGITAL_PORT_ADAPTER_H_

#include "IPortAdapter.h"

class DigitalPortAdapter : IPortAdapter<uint8_t>
{
    public:
        DigitalPortAdapter(const uint8_t t_pin) : IPortAdapter(t_pin) {};
        ~DigitalPortAdapter() = default;
        
        void setInputMode() override;
        void setOutputMode() override;

        uint8_t read() override;
        void write(uint8_t t_value) override;

        uint8_t getState() override;
};

#endif