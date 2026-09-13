// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _HC_LIB_FAKE_TIMER_H_
#define _HC_LIB_FAKE_TIMER_H_

#include <inttypes.h>
#include "timers/ITimer.h"

#define TIMER_TACTS 1000

class FakeTimer : public ITimer
{
    private:
        const uint16_t m_tacts;
        uint16_t m_iterationCount;
        uint16_t m_currentIteration;
        bool m_isElapsed;
        bool m_isStarted;
    public:
        FakeTimer()
            : FakeTimer(TIMER_TACTS)
        {}

        /// @brief Uses _t_tacts_ samples per window. 0 means isElapsed() is true immediately.
        explicit FakeTimer(const uint16_t t_tacts)
            : m_tacts(t_tacts), m_iterationCount(t_tacts),
              m_currentIteration(0), m_isElapsed(false), m_isStarted(false)
        {}

        uint32_t getInterval() const override
        {
            return m_iterationCount;
        }

        void setInterval(const uint32_t t_interval) override
        {
            m_iterationCount = m_tacts;
        }

        void start() override
        {
            m_currentIteration = 1;
            m_isElapsed = false;
            m_isStarted = true;
        }

        void stop() override
        {
            m_isElapsed = false;
            m_isStarted = false;
        }

        bool isElapsed() override
        {
            if(m_currentIteration > m_iterationCount)
            {
                m_isElapsed = true;
            }
            m_currentIteration++;
            return m_isElapsed;
        }

        bool isStarted() const override
        {
            return m_isStarted;
        }
};

#endif
