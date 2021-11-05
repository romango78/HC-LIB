// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include <Arduino.h>
#include "app.config/app.config.h"

#ifdef ZMPT101B_DC_EXAMPLE_APP

#include "adapter/AnalogPortAdapter.h"
#include "sensors/ZMPT101B.h"
#include "sensors/readers/ZMPT101BDCReader.h"

ZMPT101BDCReader* dcReader;

void setup() 
{
    // Setup device
    Serial.begin(115200);   

    delay(2000);

    // Setup Application      
    ZMPT101BSensor* sensor = new ZMPT101BSensor(A0);
    sensor->sensitivity = 0.001;
    sensor->zero = 0;
    
    IPortAdapter *adapter = (IPortAdapter*)new AnalogPortAdapter(sensor->pin);
    IVoltageStream *stream = (IVoltageStream *)new VoltageStream(adapter);
    ZMPT101B::calibrate(sensor, stream);    

    Serial.println("The ZMPT101B was initialized and calibrated. It contains the following data:");
    Serial.print("      Type: ");
    Serial.println(sensor->type);
    Serial.print("      Category: ");
    Serial.println(sensor->category);
    Serial.print("      Pin: ");
    Serial.println(sensor->pin);
    Serial.print("      Zero: ");
    Serial.println(sensor->zero);
    Serial.print("      Sensitivity: ");
    Serial.println(sensor->sensitivity);

    dcReader = new ZMPT101BDCReader(sensor, stream);
}

void loop() 
{
    ZMPT101BVoltageDC sensorData = dcReader->read();

    /*const ZMPT101BSensor* sensor = reinterpret_cast<const ZMPT101BSensor*>(sensorData.sensor);
    sout::printf("The ZMPT101B sensor info:\n");
    sout::printf("\tType: %X\n", sensor->type);
    sout::printf("\tCategory: %u\n", sensor->category);
    sout::printf("\tPin: %u\n", sensor->pin);
    sout::printf("\tZero: %d\n", sensor->zero);
    sout::printf("\tSensitivity: %f\n", sensor->sensitivity);    
    sout::printf("Read data: %f\n", sensorData.data);
    delay(2000);*/
    Serial.print("Read data: ");
    Serial.println(sensorData.data, 6);
    delay(1000);
}

#endif