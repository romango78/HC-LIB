// Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "TwiBuffer.h"

uint16_t TwiBufferInternal::inboxCount() const
{
    return m_inboxBuffer->count();
}

void TwiBufferInternal::inboxClear()
{
    m_inboxBuffer->clear();
}

Expected<uint8_t> TwiBufferInternal::inboxRead()
{
    return m_inboxBuffer->dequeue();
}

void TwiBufferInternal::inboxWrite(const uint8_t t_data)
{
    m_inboxBuffer->enqueue(t_data);
}

uint16_t TwiBufferInternal::outboxCount() const
{
    return m_outboxBuffer->count();
}

void TwiBufferInternal::outboxClear()
{
    m_outboxBuffer->clear();
}

Expected<uint8_t> TwiBufferInternal::outboxRead()
{
    return m_outboxBuffer->dequeue();
}

void TwiBufferInternal::outboxWrite(const uint8_t t_data)
{
    m_outboxBuffer->enqueue(t_data);
}