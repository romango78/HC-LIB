// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "ArduinoTimer.h"
#include <Arduino.h>

uint32_t ArduinoTimer::getInterval()
{
    return m_interval;
};

void ArduinoTimer::setInterval(uint32_t t_interval)
{
    if(!isStarted())
    {
        m_interval = t_interval;
    }
};

void ArduinoTimer::start()
{
    m_started = true;
    m_startedAt = millis();
};

void ArduinoTimer::stop()
{
    m_started = false;
    m_startedAt = 0;
    m_elapsed = false;
};

bool ArduinoTimer::isElapsed()
{
    if(!m_elapsed)
    {
        m_elapsed = (millis() - m_startedAt >= m_interval);
    }
    return m_elapsed;
};

bool ArduinoTimer::isStarted()
{
    return m_started;
}