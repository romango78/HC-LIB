// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _ZMPT101B_READERS_H_
#define _ZMPT101B_READERS_H_

#include "sensors/readers/ISensorReader.h"
#include "sensors/ZMPT101B.h"
#include "timers/ITimer.h"
#include "deviceerrdef.h"

#define AC_NETWORK_FREQUENCY 50

class ZMPT101BRmsReader : public ISensorReader<ZMPT101B_ACVoltage, ZMPT101BSensor>
{
    private:
        ITimer* const m_timer;
    public:
        ZMPT101BRmsReader(ITimer* const t_timer)
            : m_timer(t_timer) {};
        virtual ~ZMPT101BRmsReader() = default;

        ZMPT101B_ACVoltage read(const ZMPT101BSensor& t_sensor) override;
};

class ZMPT101BTrueRmsReader : public ISensorReader<ZMPT101B_ACVoltage, ZMPT101BSensor>
{
    private:
        ITimer* const m_timer;
    public:
        ZMPT101BTrueRmsReader(ITimer* const t_timer)
            : m_timer(t_timer) {};
        virtual ~ZMPT101BTrueRmsReader() = default;

        ZMPT101B_ACVoltage read(const ZMPT101BSensor& t_sensor) override;
};

#endif