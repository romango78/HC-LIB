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

/// @brief Analog-to-Digital Converter resolution in bits.
#define ADC_BITS  10
/// @brief Analog-to-Digital Converter counts (2^ADC_BITS).
#define ADC_COUNTS (1<<ADC_BITS)
/// @brief AC network frequency in Hertz.
#define AC_NETWORK_FREQUENCY 50
/// @brief Measurement resolution in wave counts.
#define MESURE_RESOLUTION_IN_WAVE_COUNT 4
/// @brief Measurement time in milliseconds.
#define MESURE_TIME_IN_MILLISECONDS (MESURE_RESOLUTION_IN_WAVE_COUNT * static_cast<uint32_t>(MILLISECONDS_IN_SECOND/ AC_NETWORK_FREQUENCY))


class ZMPT101BAcReaderBase : public ISensorReader<ZMPT101B_ACVoltage, ZMPT101BSensor>
{
    protected:
        ITimer* const m_timer;

        void waitUntilWaveCloseToZero(const ZMPT101BSensor& t_sensor) const;
        uint16_t readAdcRawValue(const ZMPT101BSensor& t_sensor) const;
        float toVolts(const float t_adcValue) const;
    public:
        ZMPT101BAcReaderBase() = delete;

        explicit ZMPT101BAcReaderBase(ITimer* const t_timer)
            : m_timer(t_timer) {
                if(m_timer)
                {
                    if(m_timer->isStarted())
                    {
                        m_timer->stop();
                    }
                    m_timer->setInterval(MESURE_TIME_IN_MILLISECONDS);
                }
            };

        virtual ~ZMPT101BAcReaderBase() = default;        
};

/// @brief Peak-to-peak RMS over MESURE_RESOLUTION_IN_WAVE_COUNT AC periods (default 3).
class ZMPT101BRmsReader : public ZMPT101BAcReaderBase
{
    public:
        ZMPT101BRmsReader() = delete;

        /// @brief Initializes the reader with _t_timer_. Does not take ownership.
        explicit ZMPT101BRmsReader(ITimer* const t_timer)
            : ZMPT101BAcReaderBase(t_timer) {};         

        virtual ~ZMPT101BRmsReader() = default;

        /// @brief Returns RMS volts, or DeviceError::TimerIsNotInitialized / IoError::StreamNotCreated.
        Expected<ZMPT101B_ACVoltage, Error> read(const ZMPT101BSensor& t_sensor) const override;
};

/// @brief True RMS (sqrt of mean square) over two AC periods (40 ms at 50 Hz).
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
