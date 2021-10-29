// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _ANALOG_PORT_ADAPTER_H_
#define _ANALOG_PORT_ADAPTER_H_

#include "IPortAdapter.h"

class AnalogPortAdapter : IPortAdapter
{
    public:
        AnalogPortAdapter(uint8_t t_pin) : IPortAdapter(t_pin) {};
        
        void setInputMode() override;
        void setOutputMode() override;

        int read() override;
        void write(int t_value) override;
};

#endif