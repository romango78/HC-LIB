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
#include "FakeTimer.h"

class FakeStream : public IStream<uint16_t>
{
    private:
        const uint16_t m_minValue;
        const uint16_t m_maxValue;
        uint16_t m_stepValue;
        uint16_t m_currentValue;
        bool m_isSetToRead;
        bool m_hasError;
    public:
        FakeStream(const uint16_t t_minValue, const uint16_t t_maxValue)
            : m_minValue(t_minValue), m_maxValue(t_maxValue), 
              m_isSetToRead(false), m_hasError(false)
        {
            m_stepValue = static_cast<uint16_t>((abs(t_maxValue) - abs(t_minValue))/TIMER_TACTS) + 1;
        };

        ~FakeStream() = default;

        void begin(const StreamMode t_mode) override
        {
            m_hasError = false;
            if(t_mode == StreamMode::Read)
            {
                m_isSetToRead = true;
            }
            else
            {
                m_isSetToRead = false;
            };
            m_currentValue = m_minValue - m_stepValue;
        };

        uint16_t read() override
        {
            if (canRead())
            {
                m_currentValue += m_stepValue;
                
                if(m_currentValue > m_maxValue)
                {
                    m_currentValue = m_maxValue;
                    m_stepValue = -m_stepValue;
                }

                if(m_currentValue < m_minValue)
                {
                    m_currentValue = m_minValue;
                    m_stepValue = -m_stepValue;
                }                

                return m_currentValue;
            }
            else
            {
                m_hasError = true;
                return 0;
            }
        };

        void write(const uint16_t t_data) override {};

        void end() override
        {
            m_isSetToRead = false;
            m_hasError = false;
        };

        uint8_t getState() override
        {
            return 0;
        }

        bool canRead() override
        {
            return m_isSetToRead;
        };

        bool canWrite() override
        {
            return false;
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

        IStream<uint16_t>* clone() const override
        {
            auto stream = new FakeStream(m_minValue, m_maxValue);
            stream->m_isSetToRead = m_isSetToRead;
            stream->m_currentValue = m_currentValue;
            stream->m_hasError = m_hasError;

            return stream;
        }        
};

#endif