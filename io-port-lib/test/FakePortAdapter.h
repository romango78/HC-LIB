// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _HC_LIB_FAKE_PORT_ADAPTER_H_
#define _HC_LIB_FAKE_PORT_ADAPTER_H_

#include "adapter/IPortAdapter.h"

#define NO_MODE -1
#define INPUT_MODE 0
#define OUTPUT_MODE 1

template<typename T>
class FakePortAdapter : public IPortAdapter<T>
{
    private:
        int m_mode;
        T m_data;
    public:
        FakePortAdapter()
            : IPortAdapter<T>(0), m_mode(NO_MODE), m_data(T()) {}
        virtual ~FakePortAdapter() = default;

        void setInputMode() override
        {
            m_mode = INPUT_MODE;
        }

        void setOutputMode() override
        {
            m_mode = OUTPUT_MODE;
        }

        T read() override
        {
            return m_data;
        }

        void write(const T t_value) override
        {
            m_data = t_value;
        }

        uint8_t getState() override
        {
            return 0;
        }

        IPortAdapter<T>* clone() const override
        {
            auto clone = new FakePortAdapter<T>();
            clone->m_mode = m_mode;
            clone->m_data = m_data;
            return clone;
        }

        int getMode() const
        {
            return m_mode;
        }

        T getData() const
        {
            return m_data;
        }

        void setData(const T t_data)
        {
            m_data = t_data;
        }
};

#endif
