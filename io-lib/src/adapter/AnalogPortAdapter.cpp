// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#if defined(ARDUINO)

#include <Arduino.h>
#include "AnalogPortAdapter.h"

void AnalogPortAdapter::setInputMode()
{
    pinMode(m_pin, INPUT);
};

void AnalogPortAdapter::setOutputMode()
{
    pinMode(m_pin, OUTPUT);
};

int AnalogPortAdapter::read()
{
    return analogRead(m_pin);
};

void AnalogPortAdapter::write(int t_value)
{
    analogWrite(m_pin, t_value);
};

#endif