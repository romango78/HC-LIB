// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "ZMPT101BReaders.h"

/// 1/(2√2) ≈ 181/512. Rounded: (peakToPeak * 181 + 256) >> 9.
/// @brief 1/(2√2), used to turn a peak amplitude into RMS.
#define ONE_OVER_SQRT2 Q10(0.353553391f)

/// @brief Returns RMS millivolts, or DeviceError::TimerIsNotInitialized / IoError::StreamNotCreated.
/// @param t_sensor The sensor to read from.
/// @return The RMS millivolts, or DeviceError::TimerIsNotInitialized / IoError::StreamNotCreated.
Expected<ZMPT101B_ACVoltage, Error> ZMPT101BRmsReader::read(const ZMPT101BSensor& t_sensor) const
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
    
    Q10 maxAdcValue = Q10(0);
    Q10 minAdcValue = Q10(1000);    

    // Start measurement loop to calculate RMS voltage.
    m_timer->start();
    while(!m_timer->isElapsed())
    {
        uint16_t adcRawValue = readAdcRawValue(t_sensor);
        const Q10 adcAdjValue = Q10(adcRawValue) - t_sensor.zero;

        if(maxAdcValue < adcAdjValue)
        {
            maxAdcValue = adcAdjValue;
        }
        if(minAdcValue > adcAdjValue)
        {
            minAdcValue = adcAdjValue;
        }
    }
    m_timer->stop();

    // Calculate the peak-to-peak voltage.
    const Q10 peakToPeak = maxAdcValue - minAdcValue;
    if(peakToPeak <= 0)
    {
        return ZMPT101B_ACVoltage(t_sensor, 0);
    }

    const Q10 amplitude = peakToPeak * ONE_OVER_SQRT2;
    return ZMPT101B_ACVoltage(t_sensor, toMillivolts(amplitude, t_sensor.calibration_factor));
}
