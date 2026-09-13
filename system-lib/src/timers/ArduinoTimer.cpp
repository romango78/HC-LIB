// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "ArduinoTimer.h"

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <chrono>
#endif

static uint32_t nowMs()
{
#if defined(ARDUINO)
    return millis();
#else
    using namespace std::chrono;
    static const steady_clock::time_point origin = steady_clock::now();
    return static_cast<uint32_t>(duration_cast<milliseconds>(steady_clock::now() - origin).count());
#endif
}

uint32_t ArduinoTimer::getInterval() const
{
    return m_interval;
}

void ArduinoTimer::setInterval(const uint32_t t_interval)
{
    if(!m_started)
    {
        m_interval = t_interval;
    }
}

void ArduinoTimer::start()
{
    m_started = true;
    m_elapsed = false;
    m_startedAt = nowMs();
}

void ArduinoTimer::stop()
{
    m_started = false;
    m_startedAt = 0;
    m_elapsed = false;
}

bool ArduinoTimer::isElapsed()
{
    if(!m_started)
    {
        return false;
    }
    if(!m_elapsed)
    {
        m_elapsed = (nowMs() - m_startedAt >= m_interval);
    }
    return m_elapsed;
}

bool ArduinoTimer::isStarted() const
{
    return m_started;
}
