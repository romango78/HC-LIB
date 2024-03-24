// Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#pragma once

#ifndef _TWI_BUFFER_H_
#define _TWI_BUFFER_H_

#include "collections/generic/Queue.h"

#define TwiBuffer (TwiBufferInternal::getInstance())

class TwiBufferInternal
{
private:
    Queue<uint8_t>* const m_inboxBuffer;
    Queue<uint8_t>* const m_outboxBuffer;

    TwiBufferInternal() :
        m_inboxBuffer(new Queue<uint8_t>()), m_outboxBuffer(new Queue<uint8_t>())
    {};
    ~TwiBufferInternal() 
    {
        m_inboxBuffer->clear();
        delete m_inboxBuffer;
        m_outboxBuffer->clear();
        delete m_outboxBuffer;
    };
public:
    TwiBufferInternal(const TwiBufferInternal& source) = delete;
    TwiBufferInternal& operator=(const TwiBufferInternal& copy) = delete;

    static TwiBufferInternal& getInstance()
    {
        // Only one instance
        // Garanteed to be lazy initialized
        // Garanteed that it will be destroyed correctly
        static TwiBufferInternal instance;
        return instance;
    }

    void inboxClear();
    Expected<uint8_t> inboxRead();
    void inboxWrite(const uint8_t t_data);
    uint16_t inboxCount() const;

    void outboxClear();
    Expected<uint8_t> outboxRead();
    void outboxWrite(const uint8_t t_data);
    uint16_t outboxCount() const;

};

#endif