// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _FAKE_STREAM_H_
#define _FAKE_STREAM_H_

#include <inttypes.h>
#include <stdlib.h>
#include "stream/IStream.h"

class FakeStream : public IStream<uint8_t>
{
    private:
        uint8_t m_currentValue;
        bool m_isSetToWrite;
        bool m_hasError;
    public:
        FakeStream()
            : FakeStream(0) {};
        FakeStream(uint8_t t_initialValue) 
            : m_isSetToWrite(false), m_hasError(false), m_currentValue(t_initialValue) {};

        ~FakeStream() = default;

        void begin(const StreamMode t_mode) override
        {
            m_hasError = false;
            if(t_mode == StreamMode::Write)
            {
                m_isSetToWrite = true;
            }
            else
            {
                m_isSetToWrite = false;
            };
        };

        uint8_t read() override 
        {
            return 0;
        };

        void write(const uint8_t t_data) override 
        {
            if (canWrite())
            {
                m_currentValue = t_data;
            }
            else
            {
                m_hasError = true;
            }
        };

        void end() override
        {
            m_isSetToWrite = false;
            m_hasError = false;
        };

        uint8_t getState() override
        {
            return m_currentValue;
        }

        bool canRead() override
        {
            return false;
        };

        bool canWrite() override
        {
            return m_isSetToWrite;
        };
        
        bool hasError() override
        {
            return m_hasError;
        };

        err_t getLastError() override
        {
            if(hasError())
            {
                return STREAM_CLOSED_IO_ERROR;
            };
            return NO_ERROR;
        };

        uint8_t getWrittenValue()
        {
            return m_currentValue;
        }
};

#endif