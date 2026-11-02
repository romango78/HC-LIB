// Copyright (c) 2026 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file ISensorReader.h
/// @brief Abstraction for reading one sensor, or several sensors in one call, into SensorData or an Error.
#ifndef _HC_LIB_I_SENSOR_READER_H_
#define _HC_LIB_I_SENSOR_READER_H_

#include "Expected.h"
#include "errors/DeviceErrors.h"
#include "sensors/SensorData.h"
#include "collections/generic/IFixedCollection.h"

/// @brief Reads one _TSensor_, or several sensors in one call.
/// @tparam TData Measurement stored in _SensorData{TData}_. Must be trivially copyable.
/// @tparam TSensor Concrete sensor type.
template<typename TData, typename TSensor>
class ISensorReader
{
    static_assert(__is_base_of(ISensor, TSensor), "TSensor must inherit ISensor.");

    public:
        ISensorReader() = default;
        virtual ~ISensorReader() = default;

        /// @brief Reads _t_sensor_ and stores the reading in _t_reading_.
        /// @param t_sensor Sensor to read.
        /// @param t_reading Receives the reading. The caller deletes it. Left unchanged on failure.
        /// @return True, or an _Error_ when the sensor cannot be read.
        virtual Expected<bool, Error> read(const TSensor& t_sensor, SensorData<TData>*& t_reading) const = 0;

        /// @brief Reads every sensor in _t_sensors_ in one call.
        /// @param t_sensors Sensors to read. Each slot is a pointer to the caller's sensor. _t_readings.getCount()_ must equal _t_sensors.getCount()_.
        /// @param t_readings Receives one _SensorData{TData}*__ per sensor, in the same order. The caller deletes each reading.
        /// @return True, or an _Error_ when the group cannot be read.
        /// @note On failure, _t_readings_ is unchanged. An implementation that stores a reading and then fails deletes that reading before returning.
        /// Pointers are used because _SensorData_ cannot be assigned and a sensor that owns a stream is not trivially copyable. An _Array_ of those pointers satisfies both arguments.
        virtual Expected<bool, Error> read(IFixedCollection<const TSensor*>& t_sensors, IFixedCollection<SensorData<TData>*>& t_readings) const = 0;
};

#endif
