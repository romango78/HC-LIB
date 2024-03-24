// Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "TwiMasterStream.h"

void TwiMasterStream::begin(const StreamMode t_mode, const uint8_t t_address)
{
    if(m_twi)
    {
        BaseStream::begin(t_mode);        
        auto error = m_twi->openInMasterMode(t_mode, t_address);
        if(error != NO_ERROR)
        {
            BaseStream::end();
            m_twi->close();

            BaseStream::setLastError(error);
        }        
    }
    else
    {
       BaseStream::setLastError(STREAM_NOTCREATED_IO_ERROR);
    }
}

uint8_t TwiMasterStream::read()
{
    BaseStream::read();
    if(!canRead())
    {
        BaseStream::setLastError(STREAM_CLOSED_IO_ERROR);
        return NO_DATA;
    }
    return 0;//static_cast<uint8_t>(m_twi->read());
}

void TwiMasterStream::write(const uint8_t t_data)
{
    BaseStream::write(t_data);
    if(!canWrite())
    {
        BaseStream::setLastError(STREAM_CLOSED_IO_ERROR);
        return;
    }
    auto error = m_twi->write(t_data);
    if(error != NO_ERROR)
    {
        BaseStream::setLastError(error);
    }    
}

void TwiMasterStream::end()
{
    BaseStream::end();
    auto error = m_twi->close();
    if(error != NO_ERROR)
    {
        BaseStream::setLastError(error);
    }    
}

IStream<uint8_t>* TwiMasterStream::clone() const
{
    ITwiAdapter* adapter = nullptr;
    if(m_twi)
    {
        adapter = m_twi->clone();
    }
    auto clone = new TwiMasterStream(adapter);
    return BaseStream::clone(clone);
}
