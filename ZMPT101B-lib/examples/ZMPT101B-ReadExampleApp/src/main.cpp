// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file main.cpp
/// @brief ZMPT101B calibration helper or RMS / True RMS demo.
#include <Arduino.h>
#include "adapter/AnalogPortAdapter.h"
#include "stream/AnalogStream.h"
#include "sensors/ZMPT101B.h"
#include "sensors/readers/ZMPT101BReaders.h"
#include "timers/ArduinoTimer.h"

#define ZMPT101B_PIN A0

ZMPT101BSensor *sensor;
ZMPT101BRmsReader *rmsReader;
ZMPT101BTrueRmsReader *trueRmsReader;

void setup()
{
    Serial.begin(115200);
    AnalogStream *stream = new AnalogStream(new AnalogPortAdapter(ZMPT101B_PIN));

    static ZMPT101BSensor zmpt(ZMPT101B_PIN, stream, 0.275f);
    sensor = &zmpt;

    static ArduinoTimer timer;
    static ZMPT101BRmsReader rms(&timer);
    static ZMPT101BTrueRmsReader trueRms(&timer);
    rmsReader = &rms;
    trueRmsReader = &trueRms;

    delay(2000);
}

void loop()
{
    Expected<ZMPT101B_ACVoltage, Error> rms = rmsReader->read(*sensor);
    Expected<ZMPT101B_ACVoltage, Error> trueRms = trueRmsReader->read(*sensor);

    Serial.print(F("RMS_V:"));
    if(rms.hasValue())
    {
        Serial.print(rms.getValue().data, 2);
    }
    else
    {
        Serial.print(rms.getError().message());
    }
    Serial.print(F(", TrueRMS_V:"));
    if(trueRms.hasValue())
    {
        Serial.println(trueRms.getValue().data, 2);
    }
    else
    {
        Serial.println(trueRms.getError().message());
    }
    delay(500);
}