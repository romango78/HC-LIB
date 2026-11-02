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
/// @note Trivially copyable when _T_ is trivially copyable. Copy and assignment replace the whole reading.
/// getSensor() and getData() are const, so a caller cannot change the fields after construction.
/// Assignment still replaces the whole object, which is what _Array_ requires.
/// Failures belong on _Expected{SensorData, Error}_, not on this type.
template<typename T>
struct SensorData
{
    static_assert(__is_trivially_copyable(T), "SensorData measurement type must be trivially copyable.");

    /// @brief Initializes a null reading.
    /// @note This is necessary for Array assignment to work.
    SensorData()
        : m_sensor(EmptySensor()), m_data(T()) {};

    /// @brief Initializes a successful reading.
    /// @param t_sensor The sensor that produced _t_data_. Copied as an ISensor slice.
    /// @param t_data The measurement.
    SensorData(const ISensor& t_sensor, const T& t_data)
        : m_sensor(t_sensor), m_data(t_data) {};

    /// @brief The sensor slice that produced this reading.
    const ISensor& getSensor() const { return m_sensor; }

    /// @brief The measurement.
    const T& getData() const { return m_data; }

private:
    ISensor m_sensor;
    T m_data;
};

#endif
