// Copyright (c) 2026 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file SensorData.h
/// @brief A successful sensor reading. Errors are returned via Expected.
#ifndef _HC_LIB_SENSOR_DATA_H_
#define _HC_LIB_SENSOR_DATA_H_

#include "Sensor.h"

/// @brief A measurement _T_ taken from an _ISensor_.
/// @tparam T The measurement type. Must be trivially copyable.
/// @note Trivially copyable when _T_ is trivially copyable. The copy copies the _ISensor_ slice and _data_.
/// Failures belong on _Expected{SensorData, Error}_, not on this type.
/// const members delete assignment, so a _SensorData_ cannot be stored in an _Array_.
template<typename T>
struct SensorData
{
    static_assert(__is_trivially_copyable(T), "SensorData measurement type must be trivially copyable.");

    const ISensor sensor;
    const T data;

    SensorData() = delete;

    /// @brief Initializes a successful reading.
    /// @param t_sensor The sensor that produced _t_data_. Copied as an ISensor slice.
    /// @param t_data The measurement.
    SensorData(const ISensor& t_sensor, const T& t_data)
        : sensor(t_sensor), data(t_data) {};
};

static_assert(__is_trivially_copyable(SensorData<uint8_t>), "SensorData must be trivially copyable.");

#endif
