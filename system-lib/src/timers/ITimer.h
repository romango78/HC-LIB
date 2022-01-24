// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _I_TIMER_H_
#define _I_TIMER_H_

#include <stdint.h>

#define MILLISECONDS_IN_SECOND 1000

class ITimer
{
    public:
        ITimer() {};
        virtual ~ITimer() = default;

        // Gets the interval, expressed in milliseconds, at which the IsElapsed flag is set in true.
        virtual uint32_t getInterval() = 0;

        // Sets the interval, expressed in milliseconds, at which the IsElapsed flag is set in true.
        virtual void setInterval(const uint32_t t_interval) = 0;

        // Starts counting interval.
        virtual void start() = 0;

        // Stops counting interval and resets the timer in the initial state.
        virtual void stop() = 0;

        // Checks if the interval elapses.
        virtual bool isElapsed() = 0;

        // Checks if counting interval is started.
        virtual bool isStarted() = 0;
};

#endif