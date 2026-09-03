// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file main.cpp
/// @brief Main file for the logging example application.
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
#endif
#include "app.config/app.config.h"
#include "log/LogManager.h"

Log *logger;
// Constructed in setup(): AVR F()/PSTR() is a statement-expression and cannot
// initialize a global. The name literal still lives in flash.
LogModule module;

void setup()
{
#if defined(ARDUINO)
    Serial.begin(115200);
#endif
    module = LogModule(F("Logging Example App"));
    logger = LogManager::getLogger();
}

void loop()
{
    logger->debug(module, F("Here can be your debug message."));
    logger->info(module, F("Here can be your info message."));
    logger->warn(module, F("Here can be your warning message."));
    logger->error(module, F("Here can be your error message."));
    logger->fatal(module, F("Here can be your critical message."));
#if defined(ARDUINO)
    Serial.write('\n');
#else
    putchar('\n');
    fflush(stdout);
#endif

    delay(1000);
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
