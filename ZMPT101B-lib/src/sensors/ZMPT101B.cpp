// Copyright (c) 2026 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "ZMPT101B.h"
#include "sensors/readers/ZMPT101BReaders.h"
#include "timers/ArduinoTimer.h"

void ZMPT101B::calibrate(ZMPT101BSensor& t_sensor, uint32_t t_etalonVoltageInMillivolts)
{
    if(!t_sensor.stream || t_etalonVoltageInMillivolts == 0)
    {
        return;
    }

    /// @brief Save the previous calibration factor.
    const Q10 previousFactor = t_sensor.calibration_factor;
    /// @brief Set the calibration factor to 1 for calculating real calibration factor in the current environment.
    t_sensor.calibration_factor = Q10(1);

    ArduinoTimer timer;
    ZMPT101BRmsReader reader(&timer);
    const Expected<ZMPT101B_ACVoltage, Error> rms = reader.read(t_sensor);
    t_sensor.stream->end();
    if(!rms.hasValue() || rms.getValue().data == 0)
    {
        /// @brief Restore the previous calibration factor when the measurement is invalid.
        t_sensor.calibration_factor = previousFactor;
        return;
    }

    const uint32_t measured = rms.getValue().data;
    /// @brief Calculate the calibration factor based on the etalon voltage and the measured voltage.
    t_sensor.calibration_factor =
        Q10(static_cast<int32_t>(t_etalonVoltageInMillivolts)) / Q10(static_cast<int32_t>(measured));
}
