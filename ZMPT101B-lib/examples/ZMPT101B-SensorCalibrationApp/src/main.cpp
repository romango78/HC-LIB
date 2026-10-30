// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file main.cpp
/// @brief Finds ZMPT101BSensor::calibration_factor against a meter reading.
/// @note Set ETALON_VOLTAGE_MV to the RMS millivolts shown by a multimeter on the same mains.
/// Mains on the module screw terminals is hazardous. Do not touch them while they are live.
#include <Arduino.h>
#include "adapter/AnalogPortAdapter.h"
#include "stream/AnalogStream.h"
#include "sensors/ZMPT101B.h"
#include "sensors/readers/ZMPT101BReaders.h"
#include "timers/ArduinoTimer.h"

#define ZMPT101B_PIN A0
#define ETALON_VOLTAGE_MV 230000u

static AnalogStream *const stream = new AnalogStream(new AnalogPortAdapter(ZMPT101B_PIN));
static ZMPT101BSensor sensor(ZMPT101B_PIN, stream);
static ArduinoTimer timer;
static ZMPT101BRmsReader rmsReader(&timer);

void setup()
{
    Serial.begin(115200);
    delay(2000);

    Serial.print(F("ETALON (mV): "));
    Serial.println(ETALON_VOLTAGE_MV);

    ZMPT101B::calibrate(sensor, ETALON_VOLTAGE_MV);

    Serial.print(F("FACTOR: "));
    Serial.println(sensor.calibration_factor.toFloat(), 4);
}

void loop()
{
    Expected<ZMPT101B_ACVoltage, Error> rms = rmsReader.read(sensor);

    Serial.print(F("RMS (mV): "));
    if(rms.hasValue())
    {
        Serial.println(rms.getValue().data);
    }
    else
    {
        Serial.println(rms.getError().message());
    }
    delay(1000);
}
