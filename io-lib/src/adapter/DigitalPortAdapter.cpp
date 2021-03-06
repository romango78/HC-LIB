// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "DigitalPortAdapter.h"

#if defined(ARDUINO)
#include <Arduino.h>
#endif

void DigitalPortAdapter::setInputMode()
{
    #if defined(ARDUINO)
    pinMode(m_pin, INPUT);
    #endif
};

void DigitalPortAdapter::setOutputMode()
{
    #if defined(ARDUINO)
    pinMode(m_pin, OUTPUT);
    #endif
};

uint8_t DigitalPortAdapter::read()
{
    #if defined(ARDUINO)
    return digitalRead(m_pin);
    #else
    return NO_DATA;
    #endif
};

void DigitalPortAdapter::write(const uint8_t t_value)
{
    #if defined(ARDUINO)
    digitalWrite(m_pin, t_value);
    #endif
};

uint8_t DigitalPortAdapter::getState()
{
    #if defined(ARDUINO)
    if(m_pin < 8)
    {
        return bitRead(PORTD, m_pin);
    }
    if (m_pin < 14)
    {
        return bitRead(PORTB, m_pin);
    }
    return NO_DATA;
    #else
    return NO_DATA;
    #endif
};

IPortAdapter<uint8_t>* DigitalPortAdapter::clone() const 
{
    return new DigitalPortAdapter(m_pin);
}