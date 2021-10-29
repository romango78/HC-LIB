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

#include "IStream.h"
#include "AnalogPortAdapter.h"
#include "AnalogStream.h"

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