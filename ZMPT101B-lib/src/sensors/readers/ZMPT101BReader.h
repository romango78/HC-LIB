// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _ZMPT101B_READER_H_
#define _ZMPT101B_READER_H_

#include "sensors/readers/SensorReader.h"
#include "sensors/ZMPT101B.h"
#include "timers/ITimer.h"

#define AC_NETWORK_FREQUENCY 50

class ZMPT101BRmsReader : public SensorReader<ZMPT101B_ACVoltage>
{
    private:
        IVoltageStream *m_stream;
        ITimer *m_timer;

        float getSensitivity();
    public:
        ZMPT101BRmsReader(ZMPT101BSensor *t_sensor, IVoltageStream *t_stream, ITimer *t_timer)
            : SensorReader(t_sensor), m_stream(t_stream), m_timer(t_timer) {};

        ~ZMPT101BRmsReader() {};

        ZMPT101B_ACVoltage read() override;
};

class ZMPT101BTrueRmsReader : public SensorReader<ZMPT101B_ACVoltage>
{
    private:
        IVoltageStream *m_stream;
        ITimer *m_timer;

        float getZero();
        float getSensitivity();
    public:
        ZMPT101BTrueRmsReader(ZMPT101BSensor *t_sensor, IVoltageStream *t_stream, ITimer *t_timer)
            : SensorReader(t_sensor), m_stream(t_stream), m_timer(t_timer) {};

        ~ZMPT101BTrueRmsReader() {};

        ZMPT101B_ACVoltage read() override;
};

#endif