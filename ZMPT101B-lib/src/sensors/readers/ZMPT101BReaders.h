// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file ZMPT101BReaders.h
/// @brief RMS and True RMS readers for ZMPT101BSensor.
#ifndef _HC_LIB_ZMPT101B_READERS_H_
#define _HC_LIB_ZMPT101B_READERS_H_

#include "sensors/readers/ISensorReader.h"
#include "sensors/ZMPT101B.h"
#include "timers/ITimer.h"
#include "errors/IoErrors.h"

#ifndef AC_NETWORK_FREQUENCY
#define AC_NETWORK_FREQUENCY 50
#endif

/// @brief Peak-to-peak RMS over two AC periods.
class ZMPT101BRmsReader : public ISensorReader<ZMPT101B_ACVoltage, ZMPT101BSensor>
{
    private:
        ITimer* const m_timer;
    public:
        ZMPT101BRmsReader() = delete;

        /// @brief Initializes the reader with _t_timer_. Does not take ownership.
        explicit ZMPT101BRmsReader(ITimer* const t_timer)
            : m_timer(t_timer) {};

        virtual ~ZMPT101BRmsReader() = default;

        /// @brief Returns RMS volts, or DeviceError::TimerIsNotInitialized / IoError::StreamNotCreated.
        Expected<ZMPT101B_ACVoltage, Error> read(const ZMPT101BSensor& t_sensor) const override;
};

/// @brief True RMS (sqrt of mean square) over two AC periods.
class ZMPT101BTrueRmsReader : public ISensorReader<ZMPT101B_ACVoltage, ZMPT101BSensor>
{
    private:
        ITimer* const m_timer;
    public:
        ZMPT101BTrueRmsReader() = delete;

        /// @brief Initializes the reader with _t_timer_. Does not take ownership.
        explicit ZMPT101BTrueRmsReader(ITimer* const t_timer)
            : m_timer(t_timer) {};

        virtual ~ZMPT101BTrueRmsReader() = default;

        /// @brief Returns True RMS volts, or DeviceError::TimerIsNotInitialized / IoError::StreamNotCreated.
        Expected<ZMPT101B_ACVoltage, Error> read(const ZMPT101BSensor& t_sensor) const override;
};

#endif
