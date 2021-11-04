// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include <Arduino.h>
#include "app.config/app.config.h"

#include "log/LogManager.h"

Log *logger;
LogModule module = { "Logging Example App" };

void setup() 
{
    // Setup device
    Serial.begin(115200);

    // Setup Application
    logger = LogManager::getLogger();
}

void loop() 
{
    logger->debug(module, "Here can be your debug message.");
    logger->info(module, "Here can be your info message.");
    logger->warn(module, "Here can be your warning message.");
    logger->error(module, "Here can be your error message.");
    logger->fatal(module, "Here can be your critical message.");
    sout::printf("\n");

    delay(1000);
}