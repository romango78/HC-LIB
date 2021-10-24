// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _PORT_STREAM_H_
#define _PORT_STREAM_H_

#include <Arduino.h>
#include "IStream.h"

#define ADC_SCALE 1024.0

class AnalogPortStream : IStream<int>
{
    private:
        uint8_t m_pin;
        float m_vref;
        bool m_isReadInitialized;
    public:
        AnalogPortStream(uint8_t t_pin, float t_vref) 
            : m_pin(t_pin), m_vref(t_vref), m_isReadInitialized(false) {};

        ~AnalogPortStream() = default;

        bool canRead() override
        {
            return m_isReadInitialized;
        }

        void startRead() override
        {
            pinMode(m_pin, INPUT);            
            m_isReadInitialized = true;
        };

        int read() override
        {
            return analogRead(m_pin);
        };

        float readVoltage()
        {
            return (float)read() * (m_vref/ADC_SCALE);
        }

        void endRead() override
        {
            m_isReadInitialized = false;
        }

};

#endif