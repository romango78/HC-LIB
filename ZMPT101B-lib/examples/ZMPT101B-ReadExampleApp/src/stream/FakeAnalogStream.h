// Copyright (c) 2026 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _FAKE_ANALOG_STREAM_H_
#define _FAKE_ANALOG_STREAM_H_

#if !defined(ARDUINO)

#include "stream/AnalogStream.h"
#include "errors/IoErrors.h"

/// @brief Native stand-in for AnalogStream: mid-scale ADC with a small swing.
class FakeAnalogStream : public AnalogStream
{
    private:
        uint16_t m_value;
        int16_t m_step;
        bool m_readable;
    public:
        FakeAnalogStream()
            : m_value(512), m_step(8), m_readable(false) {};

        void begin(const StreamMode t_mode) override
        {
            m_readable = (t_mode == StreamMode::Read);
        };

        uint16_t read() override
        {
            if(!m_readable)
            {
                return 0;
            }
            m_value = static_cast<uint16_t>(static_cast<int16_t>(m_value) + m_step);
            if(m_value > 600)
            {
                m_step = -8;
            }
            if(m_value < 424)
            {
                m_step = 8;
            }
            return m_value;
        };

        void write(const uint16_t t_data) override {};

        void end() override
        {
            m_readable = false;
        };

        uint8_t getState() override
        {
            return 0;
        };

        bool canRead() const override
        {
            return m_readable;
        };

        bool canWrite() const override
        {
            return false;
        };

        bool hasError() const override
        {
            return false;
        };

        Error getLastError() const override
        {
            return to_error(GenericError::NoError);
        };

        IStream<uint16_t>* clone() const override
        {
            auto stream = new FakeAnalogStream();
            stream->m_value = m_value;
            stream->m_step = m_step;
            stream->m_readable = m_readable;
            return stream;
        };
};

#endif
#endif
