// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file ITimer.h
/// @brief Interval countdown abstraction used by ArduinoTimer and device code.
#ifndef _HC_LIB_I_TIMER_H_
#define _HC_LIB_I_TIMER_H_

#include <stdint.h>

#define MILLISECONDS_IN_SECOND 1000

/// @brief Counts down a millisecond interval and reports when it has elapsed.
/// @note isElapsed() may latch and is therefore non-const. start() restarts an
/// already running timer. stop() clears started/elapsed and keeps the interval.
class ITimer
{
    public:
        ITimer() = default;
        virtual ~ITimer() = default;

        /// @brief Returns the interval in milliseconds.
        virtual uint32_t getInterval() const = 0;

        /// @brief Sets the interval in milliseconds.
        /// @param t_interval The countdown length.
        /// @note ArduinoTimer ignores this while the timer is started.
        virtual void setInterval(const uint32_t t_interval) = 0;

        /// @brief Starts or restarts the countdown from now.
        virtual void start() = 0;

        /// @brief Stops the countdown and clears the elapsed flag. The interval is kept.
        virtual void stop() = 0;

        /// @brief true after the interval has expired since the last start().
        /// @note Once true, stays true until stop() or start().
        virtual bool isElapsed() = 0;

        /// @brief true after start() and before stop().
        virtual bool isStarted() const = 0;
};

#endif
