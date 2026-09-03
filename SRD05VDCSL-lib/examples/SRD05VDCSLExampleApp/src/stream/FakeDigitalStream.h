// Copyright (c) 2026 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _FAKE_DIGITAL_STREAM_H_
#define _FAKE_DIGITAL_STREAM_H_

#if !defined(ARDUINO)

#include "stream/DigitalStream.h"
#include "errors/IoErrors.h"

/// @brief Native stand-in for DigitalStream: stores the last written pin value.
class FakeDigitalStream : public DigitalStream
{
    private:
        uint8_t m_state;
        bool m_writable;
    public:
        FakeDigitalStream()
            : m_state(static_cast<uint8_t>(0x1)), m_writable(false) {};

        void begin(const StreamMode t_mode) override
        {
            m_writable = (t_mode == StreamMode::Write);
        };

        uint8_t read() override
        {
            return m_state;
        };

        void write(const uint8_t t_data) override
        {
            if(m_writable)
            {
                m_state = t_data;
            }
        };

        void end() override
        {
            m_writable = false;
        };

        uint8_t getState() override
        {
            return m_state;
        };

        bool canRead() const override
        {
            return false;
        };

        bool canWrite() const override
        {
            return m_writable;
        };

        bool hasError() const override
        {
            return false;
        };

        Error getLastError() const override
        {
            return to_error(GenericError::NoError);
        };

        IStream<uint8_t>* clone() const override
        {
            auto stream = new FakeDigitalStream();
            stream->m_state = m_state;
            stream->m_writable = m_writable;
            return stream;
        };
};

#endif
#endif
