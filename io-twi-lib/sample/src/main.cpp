// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include <Arduino.h>
#include "app.config/app.config.h"
#include "adapter/TwiUnitAdapter.h"
#include "comm/twi/TwiUnit.h"
#include "iotwierrdef.h"

void handleError(err_t t_error)
{
    Serial.print("The following error occurred: ");
    switch(t_error)
    {
        case UNIT_DISABLED_TWI_ERROR:
            Serial.println("TWI UNIT is disabled.");
            break;
        case TIMEOUT_OCCURRED_TWI_ERROR:
            Serial.println("Timeout.");
            break;
        case DEVICE_NOT_FOUND_TWI_ERROR:
            Serial.println("Device is not found.");
            break;
        case TRANSMITTION_TWI_ERROR:
            Serial.println("Data has not been transmitted, NACK has been received.");
            break;
        case ARBITRATION_LOST_TWI_ERROR:
            Serial.println("Arbitration is lost.");
            break;
        case INVALID_OPERATION_ERROR:
            Serial.println("Current operation is not allowed.");
            break;        
        default:
            Serial.println(t_error);
            break;
    }
    ////////////////////////////////
    digitalWrite(LED_BUILTIN, HIGH); 
}

#ifdef TWI_MASTER_EXAMPLE_APP

#include "stream/TwiMasterStream.h"

ITwiMasterStream *stream;

void setup() 
{   
    // Setup device
    Serial.begin(115200);
    delay(2000);    
    Serial.println(".....:: TWI MASTER EXAMPLE APPLICATION ::.....");

    pinMode(LED_BUILTIN, OUTPUT);
    
    // Setup Application
    TwiUnit.enable(TWI_MASTER_ADDRESS);

    ITwiAdapter *adapter = new TwiUnitAdapter();
    stream = (ITwiMasterStream *)new TwiMasterStream(adapter);
}

void loop() 
{
    digitalWrite(LED_BUILTIN, LOW);
    ////////////////////////////////
    char data[] = "TEST MESSAGE";
    
    stream->begin(StreamMode::Write, TWI_SLAVE_ADDRESS);
    if(stream->hasError())
    {   
        handleError(stream->getLastError());
    }
    else
    {
        Serial.println("Transmission is started!");
        for(unsigned int i=0;i<sizeof(data)-1;i++)
        {
            stream->write(data[i]);
            if(stream->hasError())
            {   
                Serial.print("The following error occurred by sending '");
                Serial.print(data[i]);
                Serial.println("' char.");
                handleError(stream->getLastError());
                break;
            }
        }
        if(!stream->hasError())
        {
            Serial.print("The message '");
            Serial.print(data);
            Serial.println("' has been transmitted.");
        }

        stream->end();
        if(stream->hasError())
        {   
            handleError(stream->getLastError());
        }
        Serial.println("Transmission is finished!");
    }
    ////////////////////////////////
    delay(10000);
}

#endif

#ifdef TWI_SLAVE_EXAMPLE_APP

#include "stream/TwiSlaveStream.h"

void slave_onDataReceived(const uint16_t t_count)
{
    digitalWrite(LED_BUILTIN, HIGH);
    ////////////////////////////////
    Serial.print("Received ");
    Serial.print(t_count);
    Serial.println(" bytes.");
    ////////////////////////////////
    ITwiAdapter *adapter = new TwiUnitAdapter();
    TwiSlaveStream *stream = new TwiSlaveStream(adapter);

    stream->begin(StreamMode::Read); 
    if(stream->hasError())
    {   
        handleError(stream->getLastError());        
    }
    else
    {     
        while(true)
        {
            uint8_t data = stream->read();
            if(stream->hasError())
            {
                break;
            }
            char c = data;
            Serial.print(c);
        };
    }
    stream->end();
    ////////////////////////////////
    Serial.println("");
    ////////////////////////////////
    digitalWrite(LED_BUILTIN, LOW);
}

void slave_onDataRequesting()
{
    digitalWrite(LED_BUILTIN, HIGH);
    ////////////////////////////////
    ITwiAdapter *adapter = new TwiUnitAdapter();
    TwiSlaveStream *stream = new TwiSlaveStream(adapter);

    stream->begin(StreamMode::Write);
    char message[] = "Received.";
    unsigned int i = 0;
    for(;i<sizeof(message)-1;i++)
    {
        stream->write(message[i]);
        if(stream->hasError())
        {   
            handleError(stream->getLastError());
            break;
        }        
    }    
    stream->end();
    ////////////////////////////////
    Serial.print(i+1);
    Serial.println(" bytes are pending to be sent.");
    ////////////////////////////////
    digitalWrite(LED_BUILTIN, LOW);
}

void slave_onDataSent()
{
    Serial.println("Data has been sent...");
}

void setup() 
{
    // Setup device
    Serial.begin(115200);
    Serial.println(".....:: TWI SLAVE EXAMPLE APPLICATION ::.....");

    pinMode(LED_BUILTIN, OUTPUT);

    // Setup Application    
    TwiUnit.onTwiDataReceived(slave_onDataReceived);
    TwiUnit.onTwiDataRequesting(slave_onDataRequesting);
    TwiUnit.onTwiDataSent(slave_onDataSent);
    TwiUnit.enable(TWI_SLAVE_ADDRESS);
}

void loop() 
{
    delay(100);
}

#endif