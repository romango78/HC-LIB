// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _ARDUINO_TIMER_H_
#define _ARDUINO_TIMER_H_

#include "ITimer.h"

class ArduinoTimer : public ITimer
{
    private:
        uint32_t m_interval;
        uint32_t m_startedAt;
        bool m_started;
        bool m_elapsed;
    public:
        ArduinoTimer()
            : m_interval(0), m_started(false), m_elapsed(false) {};
        ~ArduinoTimer() = default;

        uint32_t getInterval() override;
        void setInterval(uint32_t t_interval) override;

        void start() override;
        void stop() override;

        bool isElapsed() override;
        bool isStarted() override;
};

#endif