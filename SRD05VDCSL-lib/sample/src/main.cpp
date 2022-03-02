// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include <Arduino.h>

#include "adapter/DigitalPortAdapter.h"
#include "stream/DigitalStream.h"
#include "devices/controllers/RelayDeviceController.h"
#include "devices/SRD05VDCSL.h"

#define SRD05VDCSL_PIN 3

RelayDevice* relayDevice;
RelayDeviceController* relayController;

void setup() 
{
    // Setup device
    Serial.begin(115200);

    // Setup Application      
    IPortAdapter<uint8_t> *adapter = (IPortAdapter<uint8_t>*)new DigitalPortAdapter(SRD05VDCSL_PIN);
    IStream<uint8_t> *stream = (IStream<uint8_t> *)new DigitalStream(adapter);

    SRD05VDCSLDevice device{SRD05VDCSL_PIN, stream};
    relayDevice = &device;

    relayController = new RelayDeviceController();
    
    delay(2000);
    relayController->off(*relayDevice);
    delay(3000);
}

void loop() 
{
    err_t error;
    auto state = relayController->getState(*relayDevice);
    if(state.hasValue())
    {  
        if(state.getValue() == RelayState::off)
        {
            Serial.println("State: off");
            error = relayController->on(*relayDevice);
        }
        else
        {
            Serial.println("State: on");
            error = relayController->off(*relayDevice);
        }
        Serial.println("Ok!");
    }
    else
    {
        Serial.print("Error: ");
        Serial.println(error);
    }
    delay(5000);
}