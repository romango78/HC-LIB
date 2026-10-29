// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "ZMPT101BReaders.h"
#include "board.h"

uint16_t ZMPT101BAcReaderBase::m_supplyVoltageMv = 0;

/// @brief Ensures the timer is initialized and the stream is created.
/// @param t_sensor The sensor to check.
/// @return True if the timer is initialized and the stream is created, false otherwise.
Expected<bool, Error> ZMPT101BAcReaderBase::ensureValidInput(const ZMPT101BSensor& t_sensor) const
{
    if(!m_timer)
    {
        return make_error(DeviceError::TimerIsNotInitialized);
    }
    if(!t_sensor.stream)
    {
        return make_error(IoError::StreamNotCreated);
    }
    return true;
}

/// @brief Waits until the wave is close to zero.
/// @param t_sensor The sensor to wait for.
/// @note The wave is close to zero if the ADC value is close to the mid-point.
void ZMPT101BAcReaderBase::waitUntilWaveCloseToZero(const ZMPT101BSensor& t_sensor) const
{   
    if(m_timer->isStarted())
    {
        m_timer->stop();
    }
    m_timer->start();
    while(!m_timer->isElapsed())
    {
        uint16_t rawValue = static_cast<uint16_t>(t_sensor.stream->read());
        if(t_sensor.isCloseToZero(rawValue))
        {
            break;
        }
    }
    m_timer->stop();
}

/// @brief Reads one ADC sample and adjusts the zero point.
/// @param t_sensor The sensor to read the value from.
/// @return The ADC raw value.
uint16_t ZMPT101BAcReaderBase::readAdcRawValue(const ZMPT101BSensor& t_sensor) const
{
    uint16_t rawValue = static_cast<uint16_t>(t_sensor.stream->read());
    t_sensor.adjustZero(rawValue);
    return rawValue;
}

/// @brief Converts an ADC amplitude to millivolts.
/// @param t_adcValue The ADC RMS amplitude in counts.
/// @param t_calibration_factor Scale applied when converting ADC to millivolts (default 1.0).
/// @return The voltage in millivolts. Zero inputs become 0.
uint32_t ZMPT101BAcReaderBase::toMillivolts(const Q10 t_adcValue, const Q10 t_calibration_factor) const
{
    if(t_adcValue == 0 || t_calibration_factor == 0)
    {
        return 0;
    }
    if(m_supplyVoltageMv == 0)
    {
        m_supplyVoltageMv = board::getSupplyVoltage();
    }
    const Q10 product = t_adcValue * t_calibration_factor * (Q10(m_supplyVoltageMv) / Q10(ADC_COUNTS));
    return static_cast<uint32_t>(product.toInt32());
}
