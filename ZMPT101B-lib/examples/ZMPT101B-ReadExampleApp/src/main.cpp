// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file main.cpp
/// @brief ZMPT101B RMS / True RMS demo.
#include <Arduino.h>
#include "adapter/AnalogPortAdapter.h"
#include "stream/AnalogStream.h"
#include "sensors/ZMPT101B.h"
#include "sensors/readers/ZMPT101BReaders.h"
#include "timers/ArduinoTimer.h"

#define ZMPT101B_PIN A0
#define ZMPT101B_CALIBRATION_FACTOR 0.275f

static AnalogStream *const stream = new AnalogStream(new AnalogPortAdapter(ZMPT101B_PIN));
static ZMPT101BSensor sensor(ZMPT101B_PIN, stream, ZMPT101B_CALIBRATION_FACTOR);
static ArduinoTimer timer;
static ZMPT101BRmsReader rmsReader(&timer);
static ZMPT101BTrueRmsReader trueRmsReader(&timer);

void setup()
{
    Serial.begin(115200);
    delay(2000);
}

void loop()
{
    Expected<ZMPT101B_ACVoltage, Error> rms = rmsReader.read(sensor);
    Expected<ZMPT101B_ACVoltage, Error> trueRms = trueRmsReader.read(sensor);

    Serial.print(F("RMS_mV:"));
    if(rms.hasValue())
    {
        Serial.print(rms.getValue().data);
    }
    else
    {
        Serial.print(rms.getError().message());
    }
    Serial.print(F(", TrueRMS_mV:"));
    if(trueRms.hasValue())
    {
        Serial.println(trueRms.getValue().data);
    }
    else
    {
        Serial.println(trueRms.getError().message());
    }
    delay(500);
}
