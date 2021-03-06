// Copyright (c) 20212Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _I_PORT_ADAPTER_H_
#define _I_PORT_ADAPTER_H_

#include <inttypes.h>
#include "ICloneable.h"

#define NO_DATA 0

template<typename T>
class IPortAdapter : public ICloneable<IPortAdapter<T>>
{
    protected:
        const uint8_t m_pin; 
    public:
        IPortAdapter(const uint8_t t_pin) 
            : m_pin(t_pin) {};
        virtual ~IPortAdapter() = default;

        virtual void setInputMode() = 0;
        virtual void setOutputMode() = 0;

        virtual T read() = 0;
        virtual void write(const T t_value) = 0;

        virtual uint8_t getState() = 0;

        virtual IPortAdapter<T>* clone() const = 0;
};

#endif