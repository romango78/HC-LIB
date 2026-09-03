// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file main.cpp
/// @brief Toggles an SRD-05VDC-SL-C relay and prints its state.
#if defined(ARDUINO)
    #include <Arduino.h>
    #include "adapter/DigitalPortAdapter.h"
    #include "stream/DigitalStream.h"
#else
    #include <cstdio>
    #include <thread>
    #include <chrono>
    #include "stream/FakeDigitalStream.h"
    inline void delay(unsigned long t_milliseconds)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(t_milliseconds));
    }
#ifndef F
#define F(string_literal) (string_literal)
#endif
#endif
#include "devices/controllers/RelayDeviceController.h"
#include "devices/SRD05VDCSL.h"

#define SRD05VDCSL_PIN 3

SRD05VDCSLDevice *relayDevice;
RelayDeviceController controller;

#if defined(ARDUINO)
static void printLine(const __FlashStringHelper *t_text)
{
    Serial.println(t_text);
}

static void printError(const Error& t_error)
{
    Serial.print(F("Error: "));
    Serial.println(t_error.message());
}
#else
static void printLine(const char *t_text)
{
    std::puts(t_text);
}

static void printError(const Error& t_error)
{
    std::printf("Error: %s\n", t_error.message());
}
#endif

void setup()
{
#if defined(ARDUINO)
    Serial.begin(115200);
    DigitalStream *stream = new DigitalStream(new DigitalPortAdapter(SRD05VDCSL_PIN));
#else
    DigitalStream *stream = new FakeDigitalStream();
#endif
    static SRD05VDCSLDevice device(SRD05VDCSL_PIN, stream);
    relayDevice = &device;

    delay(2000);
    controller.off(*relayDevice);
    delay(3000);
}

void loop()
{
    Expected<RelayState, Error> state = controller.getState(*relayDevice);
    if(state.hasValue())
    {
        if(state.getValue() == RelayState::Off)
        {
            printLine(F("State: off"));
            Error error = controller.on(*relayDevice);
            if(error)
            {
                printError(error);
            }
        }
        else
        {
            printLine(F("State: on"));
            Error error = controller.off(*relayDevice);
            if(error)
            {
                printError(error);
            }
        }
        printLine(F("Ok!"));
    }
    else
    {
        printError(state.getError());
    }
    delay(5000);
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
