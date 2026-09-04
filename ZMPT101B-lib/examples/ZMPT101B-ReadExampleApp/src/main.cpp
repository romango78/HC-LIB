// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file main.cpp
/// @brief ZMPT101B calibration helper or RMS / True RMS demo.
#if defined(ARDUINO)
    #include <Arduino.h>
#else
    #include <cstdio>
    #include <thread>
    #include <chrono>
    inline void delay(unsigned long t_milliseconds)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(t_milliseconds));
    }
#ifndef F
#define F(string_literal) (string_literal)
#endif
#ifndef A0
#define A0 14
#endif
#endif

#if defined(ARDUINO)
    #include "adapter/AnalogPortAdapter.h"
    #include "stream/AnalogStream.h"
#else
    #include "stream/FakeAnalogStream.h"
#endif
#include "sensors/ZMPT101B.h"
#include "sensors/readers/ZMPT101BReaders.h"
#include "timers/ArduinoTimer.h"

#define ZMPT101B_PIN A0

ZMPT101BSensor *sensor;
ZMPT101BRmsReader *rmsReader;
ZMPT101BTrueRmsReader *trueRmsReader;

void setup()
{
#if defined(ARDUINO)
    Serial.begin(115200);
    AnalogStream *stream = new AnalogStream(new AnalogPortAdapter(ZMPT101B_PIN));
#else
    AnalogStream *stream = new FakeAnalogStream();
#endif
    static ZMPT101BSensor zmpt(ZMPT101B_PIN, stream);
    sensor = &zmpt;
    ZMPT101B::calibrate(sensor);

    static ArduinoTimer timer;
    static ZMPT101BRmsReader rms(&timer);
    static ZMPT101BTrueRmsReader trueRms(&timer);
    rmsReader = &rms;
    trueRmsReader = &trueRms;

#if defined(ARDUINO)
    Serial.println(F("220V,RMS,TrueRMS"));
#else
    std::puts("220V,RMS,TrueRMS");
#endif
    delay(2000);
}

void loop()
{
    Expected<ZMPT101B_ACVoltage, Error> rms = rmsReader->read(*sensor);
    Expected<ZMPT101B_ACVoltage, Error> trueRms = trueRmsReader->read(*sensor);

#if defined(ARDUINO)
    Serial.print(220.00);
    Serial.write(' ');
    if(rms.hasValue())
    {
        Serial.print(rms.getValue().data, 2);
    }
    else
    {
        Serial.print(rms.getError().message());
    }
    Serial.write(' ');
    if(trueRms.hasValue())
    {
        Serial.println(trueRms.getValue().data, 2);
    }
    else
    {
        Serial.println(trueRms.getError().message());
    }
#else
    std::printf("220.00 %s%f %s%f\n",
        rms.hasValue() ? "" : "ERR ",
        rms.hasValue() ? rms.getValue().data : 0.0f,
        trueRms.hasValue() ? "" : "ERR ",
        trueRms.hasValue() ? trueRms.getValue().data : 0.0f);
#endif
    delay(500);
}

#if !defined(ARDUINO)
int main()
{
    setup();
    for(;;)
    {
        loop();
    }
    return 0;
}

#endif