// Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

extern "C" {
    #include "comm/twi/TwiUnitHardware.h"
}
#include "TwiUnitAdapter.h"
#include "comm/twi/TwiUnit.h"
#include "comm/twi/TwiBuffer.h"
#include "stream/IStream.h"

TwiUnitAdapter::~TwiUnitAdapter()
{
    close();
}

err_t TwiUnitAdapter::openInMasterMode(const uint8_t t_readWriteMode, const uint8_t t_address)
{    
    if(!TwiUnit.isReady() || m_mode == STR_TWI_MODE)
    {
        return INVALID_OPERATION_ERROR;
    }    
    switch(t_readWriteMode)
    {
        case READ_MODE:
            m_mode = MR_TWI_MODE;
            break;
        case WRITE_MODE:
            m_mode = MT_TWI_MODE;
            break;
    }
    return twi_unit_start(t_address, m_mode == MT_TWI_MODE ? TWI_WRITE_MODE : TWI_READ_MODE);    
}

err_t TwiUnitAdapter::openInSlaveMode()
{
    if(!TwiUnit.isReady() || m_mode != UNDEF_TWI_MODE)
    {
        return INVALID_OPERATION_ERROR;
    }
    m_mode = STR_TWI_MODE;
    return NO_ERROR;
}

err_t TwiUnitAdapter::close()
{
    err_t error = NO_ERROR;
    switch(m_mode)
    {
        case  MT_TWI_MODE:
            error = twi_unit_stop();
            break;
    }    

    m_mode = UNDEF_TWI_MODE;
    return error;
}

Expected<uint8_t> TwiUnitAdapter::read()
{
    return TwiBuffer.inboxRead();
}

err_t TwiUnitAdapter::write(const uint8_t t_data)
{
    switch(m_mode)
    {
        case MT_TWI_MODE:            
            return twi_unit_transmitt(t_data);
        case STR_TWI_MODE:
            TwiBuffer.outboxWrite(t_data);
            break;
        default:
            return INVALID_OPERATION_ERROR;
    };
    return NO_ERROR;
}

ITwiAdapter* TwiUnitAdapter::clone() const
{
    auto clone = new TwiUnitAdapter();
    clone->m_address = m_address;
    clone->m_mode = m_mode;
    return (ITwiAdapter*) clone;
}