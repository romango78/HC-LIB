// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include <Arduino.h>
#include "app.config/app.config.h"

#ifdef VOLTAGE_READ_EXAMPLE_APP

// This sample demonstrates how to read voltage from
// Arduino port.
// The analog port #3 is used in this sample

#include "stream/IStream.h"
#include "adapter/AnalogPortAdapter.h"
#include "stream/AnalogStream.h"

AnalogStream *stream;

void setup() 
{
    // Setup device
    Serial.begin(115200);

    // Setup Application
    IPortAdapter *adapter = (IPortAdapter*)new AnalogPortAdapter(A0);
    stream = new AnalogStream(adapter);
    stream->begin(StreamMode::Read);
}

void loop() 
{
    float value = ((IVoltageStream*)stream)->getVoltage();
    Serial.println(value);
    delay(100);
}

#endif

#ifdef VOLTAGE_LED_FADE_EXAMPLE_APP

// This sample demonstrates how to config the PWM (pulse-width modulation).
// The digital port #3 is used in this sample.
// Note: not all ports support PWM.

#include "stream/IStream.h"
#include "adapter/AnalogPortAdapter.h"
#include "stream/AnalogStream.h"

#define V_MIN 0
#define V_MAX 100
#define V_STEP 1
#define ASC_DIR 1
#define DESC_DIR -1

AnalogStream *stream;
int _currPwm;
int _direction;

void setup() 
{
    // Setup device
    Serial.begin(115200);

    // Setup Application
    _currPwm = V_MIN;
    _direction = ASC_DIR;

    IPortAdapter *adapter = (IPortAdapter*)new AnalogPortAdapter(3);
    stream = new AnalogStream(adapter);
    stream->begin(StreamMode::Write);
}

void loop() 
{
    if(_currPwm < V_MIN && _direction == DESC_DIR)
    {
        _direction = ASC_DIR;
    }
    if(_currPwm > V_MAX && _direction == ASC_DIR)
    {
        _direction = DESC_DIR;
    }

    Serial.println(_currPwm);
    ((IVoltageStream*)stream)->setPwm(_currPwm);    

    _currPwm += V_STEP * _direction;
    delay(30);
}

#endif