// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file ISensorReader.h
/// @brief Abstraction for reading a sensor into SensorData or an Error.
#ifndef _HC_LIB_I_SENSOR_READER_H_
#define _HC_LIB_I_SENSOR_READER_H_

#include "Expected.h"
#include "errors/DeviceErrors.h"

/// @brief Reads _Sensor_ and returns _SensorData_ or an _Error_.
/// @tparam SensorData The reading payload (usually _SensorData{T}_ or a derived type).
/// @tparam Sensor The concrete sensor type. Passed by const reference.
template<typename SensorData, typename Sensor>
class ISensorReader
{
    public:
        ISensorReader() = default;
        virtual ~ISensorReader() = default;

        /// @brief Reads _t_sensor_.
        /// @return The reading, or an _Error_ when the sensor cannot be read.
        virtual Expected<SensorData, Error> read(const Sensor& t_sensor) const = 0;
};

#endif
