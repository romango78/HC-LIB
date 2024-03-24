// Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "TwiUnit.h"
#include "TwiBuffer.h"

void TwiUnitInternal::enable(const uint8_t t_slave_address)
{
    // Initialize TWI in Slave mode
    twi_unit_enable();
    twi_unit_setAddress(t_slave_address);
}

void TwiUnitInternal::disable()
{
    twi_unit_disable();
}

bool TwiUnitInternal::isReady()
{
    return (twi_unit_getState() != TWI_DISABLED_STATE);
}

void TwiUnitInternal::dataReceivedHandler()
{
    if(TwiUnit.TwiDataReceivedEvent)
    {
        TwiUnit.TwiDataReceivedEvent(TwiBuffer.inboxCount());
    }
}

void TwiUnitInternal::dataRequestingHandler()
{
    if(TwiUnit.TwiDataRequestingEvent)
    {
        TwiUnit.TwiDataRequestingEvent();
    }
}

void TwiUnitInternal::dataSentHandler()
{
    if(TwiUnit.TwiDataSentEvent)
    {
        TwiUnit.TwiDataSentEvent();
    }  
}