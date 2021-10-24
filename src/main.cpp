// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include <Arduino.h>
#include "app.config/app.config.h"

#ifdef LOGGING_EXAMPLE_APP

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

#endif

#ifdef ZMPT101B_DC_EXAMPLE_APP

#include "readers/zmpt101b.h"
#include "PortStream.h"

ZMPT101BVoltageDCReader* reader;

void setup() 
{
    // Setup device
    //Serial.begin(115200);
    Serial.begin(9600);

    delay(2000);

    // Setup Application      
    ZMPT101BSensor* sensor = new ZMPT101BSensor(A0);
    sensor->sensitivity = 0.001;
    sensor->zero = 0;
    
    AnalogPortStream *stream = new AnalogPortStream(sensor->pin, VREF);
    //ZMPT101B::calibrate(sensor, stream);    
/*
    sout::printf("The ZMPT101B was initialized and calibrated. It contains the following data:\n");
    sout::printf("\tType: %X\n", sensor->type);
    sout::printf("\tCategory: %u\n", sensor->category);
    sout::printf("\tPin: %u\n", sensor->pin);
    sout::printf("\tZero: %d\n", sensor->zero);
    sout::printf("\tSensitivity: %f\n", sensor->sensitivity);
*/
    reader = new ZMPT101BVoltageDCReader(sensor, stream);
}

void loop() 
{
    ZMPT101BVoltageDC sensorData = reader->read();

    /*const ZMPT101BSensor* sensor = reinterpret_cast<const ZMPT101BSensor*>(sensorData.sensor);
    sout::printf("The ZMPT101B sensor info:\n");
    sout::printf("\tType: %X\n", sensor->type);
    sout::printf("\tCategory: %u\n", sensor->category);
    sout::printf("\tPin: %u\n", sensor->pin);
    sout::printf("\tZero: %d\n", sensor->zero);
    sout::printf("\tSensitivity: %f\n", sensor->sensitivity);    
    sout::printf("Read data: %f\n", sensorData.data);
    delay(2000);*/
    Serial.println(sensorData.data, 6);
    //delay(100);
    //Serial.print("\n");
}

#endif