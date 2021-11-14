// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.
#ifndef _FAKE_PORT_ADAPTER_H_
#define _FAKE_PORT_ADAPTER_H_

#include "adapter/IPortAdapter.h"

#define NO_MODE -1
#define INPUT_MODE 0
#define OUTPUT_MODE 1

class FakePortAdapter : IPortAdapter<int>
{
    private:
        int m_mode;
        int m_data;
    public:
        FakePortAdapter() : IPortAdapter(0), m_mode(NO_MODE) {}
        ~FakePortAdapter() = default;
        
        void setInputMode() override
        {
            m_mode = INPUT_MODE;
        }

        void setOutputMode() override
        {
            m_mode = OUTPUT_MODE;
        }

        int read() override
        {
            return m_data;
        }

        void write(int t_value) override
        {
            m_data = t_value;
        }

        int getMode()
        {
            return m_mode;
        }

        int getData()
        {
            return m_data;
        }

        void setData(int t_data)
        {
            m_data = t_data;
        }
};

#endif