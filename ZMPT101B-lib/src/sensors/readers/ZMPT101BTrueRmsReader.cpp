// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "ZMPT101BReaders.h"

/// @brief Returns True RMS millivolts, or DeviceError::TimerIsNotInitialized / IoError::StreamNotCreated.
/// @param t_sensor The sensor to read from.
/// @return The True RMS millivolts, or DeviceError::TimerIsNotInitialized / IoError::StreamNotCreated.
Expected<ZMPT101B_ACVoltage, Error> ZMPT101BTrueRmsReader::read(const ZMPT101BSensor& t_sensor) const
{
    auto validationResult = ensureValidInput(t_sensor);
    if(!validationResult)
    {
        return make_error(validationResult.getError());
    }
    
    if(!t_sensor.stream->canRead())
    {
        t_sensor.stream->begin(StreamMode::Read);
    }
    // Set the zero point to the middle of the ADC range.
    t_sensor.setZero(ADC_COUNTS >> 1);

    // Wait until the wave is close to zero (mid-scale adc) part in sin curve.
    waitUntilWaveCloseToZero(t_sensor);

    // A Q10 sum of squares overflows int32. Keep the integer squares in int64.
    int64_t totalAdcAdjSquared = 0;
    uint16_t sampleCount = 0;

    // Start measurement loop to calculate True RMS voltage.
    m_timer->start();
    while(!m_timer->isElapsed())
    {
        uint16_t adcRawValue = readAdcRawValue(t_sensor);
        const Q10 adcAdjValue = Q10(adcRawValue) - t_sensor.zero;

        totalAdcAdjSquared += (adcAdjValue * adcAdjValue).toInt32();
        sampleCount++;
    }
    m_timer->stop();

    if(sampleCount == 0)
    {
        return ZMPT101B_ACVoltage(t_sensor, 0);
    }

    const Q10 meanSquare(static_cast<int32_t>(totalAdcAdjSquared / sampleCount));
    const Expected<Q10, Error> trueRms = meanSquare.sqrt();
    if(!trueRms.hasValue())
    {
        return make_error(trueRms.getError());
    }

    return ZMPT101B_ACVoltage(t_sensor, toMillivolts(trueRms.getValue(), t_sensor.calibration_factor));
}
