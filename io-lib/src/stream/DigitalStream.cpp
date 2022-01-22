// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "DigitalStream.h"

void DigitalStream::begin(StreamMode t_mode)
{   
    if(m_adapter)
    {
        BaseStream::begin(t_mode);
        if(canRead())
        {        
            m_adapter->setInputMode();
        }
        else
        {
            m_adapter->setOutputMode();
        }
    }
    else
    {
       BaseStream::setLastError(IO_ERROR_STREAM_CLOSED); 
    }
};

uint8_t DigitalStream::read()
{
    BaseStream::read();
    if(!canRead())
    {
        BaseStream::setLastError(IO_ERROR_STREAM_CLOSED);
        return NO_DATA;
    }
    return static_cast<uint8_t>(m_adapter->read());
};

void DigitalStream::write(uint8_t t_data)
{
    BaseStream::write(t_data);
    if(!canWrite())
    {
        BaseStream::setLastError(IO_ERROR_STREAM_CLOSED);
        return;
    }
    m_adapter->write(t_data);    
};

uint8_t DigitalStream::getState()
{
    if(m_adapter)
    {
        return m_adapter->getState();
    }
    BaseStream::setLastError(IO_ERROR_STREAM_CLOSED);
    return 0; 
}