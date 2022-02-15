// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "AnalogPortAdapter.h"

#if defined(ARDUINO)
#include <Arduino.h>
#endif

void AnalogPortAdapter::setInputMode()
{
    #if defined(ARDUINO)
    pinMode(m_pin, INPUT);
    #endif
};

void AnalogPortAdapter::setOutputMode()
{
    #if defined(ARDUINO)
    pinMode(m_pin, OUTPUT);
    #endif
};

int AnalogPortAdapter::read()
{
    #if defined(ARDUINO)
    return analogRead(m_pin);
    #else
    return NO_DATA;
    #endif
};

void AnalogPortAdapter::write(const int t_value)
{
    #if defined(ARDUINO)
    analogWrite(m_pin, t_value);
    #endif
};

uint8_t AnalogPortAdapter::getState()
{
    #if defined(ARDUINO)
    if(m_pin < 8)
    {
        return bitRead(PORTC, m_pin);
    }
    return NO_DATA;
    #else
    return NO_DATA;
    #endif
};