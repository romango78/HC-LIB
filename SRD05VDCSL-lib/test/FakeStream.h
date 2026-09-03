// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _HC_LIB_FAKE_STREAM_H_
#define _HC_LIB_FAKE_STREAM_H_

#include <inttypes.h>
#include "stream/DigitalStream.h"
#include "errors/IoErrors.h"

uint8_t g_PortState = 0;

class FakeDigitalStream : public DigitalStream
{
    private:
        bool m_isSetToWrite;
        bool m_hasError;
    public:
        FakeDigitalStream()
            : m_isSetToWrite(false), m_hasError(false) {};

        FakeDigitalStream(uint8_t t_initialValue)
            : m_isSetToWrite(false), m_hasError(false)
            {
                g_PortState = t_initialValue;
            };

        virtual ~FakeDigitalStream() = default;

        void begin(const StreamMode t_mode) override
        {
            m_hasError = false;
            m_isSetToWrite = (t_mode == StreamMode::Write);
        };

        uint8_t read() override
        {
            return 0;
        };

        void write(const uint8_t t_data) override
        {
            if (canWrite())
            {
                g_PortState = t_data;
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
            return g_PortState;
        }

        bool canRead() const override
        {
            return false;
        };

        bool canWrite() const override
        {
            return m_isSetToWrite;
        };

        bool hasError() const override
        {
            return m_hasError;
        };

        Error getLastError() const override
        {
            if(hasError())
            {
                return to_error(IoError::StreamClosed);
            }
            return to_error(GenericError::NoError);
        };

        uint8_t getWrittenValue() const
        {
            return g_PortState;
        }

        IStream<uint8_t>* clone() const override
        {
            auto stream = new FakeDigitalStream();
            stream->m_isSetToWrite = m_isSetToWrite;
            stream->m_hasError = m_hasError;
            return stream;
        }
};

#endif
