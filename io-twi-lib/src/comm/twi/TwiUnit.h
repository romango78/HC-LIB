// Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _TWI_UNIT_H_
#define _TWI_UNIT_H_

extern "C" {
    #include "TwiUnitHardware.h"
}
#include <inttypes.h>

#define TwiUnit (TwiUnitInternal::getInstance())

class TwiUnitInternal
{
private:
    TwiUnitInternal()
    {
        // Register event handlers
        twi_unit_onDataReceived(dataReceivedHandler);
        twi_unit_onDataSent(dataSentHandler);
    }
    
    // Events
    void (*TwiDataReceivedEvent) (const uint16_t t_count); 
    void (*TwiDataRequestingEvent) ();
    void (*TwiDataSentEvent) ();

    // Event handlers
    static void dataReceivedHandler();
    static void dataRequestingHandler();
    static void dataSentHandler();
public:
    TwiUnitInternal(const TwiUnitInternal& source) = delete;
    TwiUnitInternal& operator=(const TwiUnitInternal& copy) = delete;

    static TwiUnitInternal& getInstance()
    {
        // Only one instance
        // Garanteed to be lazy initialized
        // Garanteed that it will be destroyed correctly
        static TwiUnitInternal instance;
        return instance;
    }

    void enable(const uint8_t t_slave_address);
    void disable();
    bool isReady();
    
    void onTwiDataReceived(void (*handler) (const uint16_t))
    {
        TwiDataReceivedEvent = handler;
    }
    void onTwiDataRequesting(void (*handler) ())
    {
        TwiDataRequestingEvent = handler;
    }    
    void onTwiDataSent(void (*handler) ())
    {
        TwiDataSentEvent = handler;
    }    
};

#endif