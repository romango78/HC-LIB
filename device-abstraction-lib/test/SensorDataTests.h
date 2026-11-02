// Copyright (c) 2026 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _HC_LIB_SENSOR_DATA_TESTS_H_
#define _HC_LIB_SENSOR_DATA_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "collections/generic/Array.h"
#include "collections/generic/IFixedCollection.h"
#include "collections/generic/IEnumerator.h"
#include "unity_extensions.h"
#include "sensors/SensorData.h"
#include "sensors/sensordef.h"

/// @brief Tests SensorData construction from an ISensor and a measurement.
/// @note The reading stores an ISensor slice (type, category) and the data value.
void SensorData_Ctor_ShouldStoreSensorSliceAndData()
{
    ISensor sensor(VOLTAGE_SENSOR_TYPE, DeviceCategory::Analog);
    SensorData<uint32_t> sut(sensor, 2200u);

    TEST_ASSERT_EQUAL_MESSAGE(VOLTAGE_SENSOR_TYPE, sut.getSensor().getType(), F("Sensor type should be preserved."));
    TEST_ASSERT_EQUAL_MESSAGE(static_cast<uint8_t>(DeviceCategory::Analog),
        static_cast<uint8_t>(sut.getSensor().getCategory()), F("Sensor category should be preserved."));
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(2200u, sut.getData(), F("Measurement should be stored."));
    TEST_ASSERT_TRUE_MESSAGE(device::is_analog(sut.getSensor()), F("Sliced sensor should still be analog."));
}

/// @brief Tests SensorData with a digital sensor type.
/// @note The category on the slice is Digital even when the type code is a sensor type.
void SensorData_Ctor_ShouldAcceptDigitalSensor()
{
    ISensor sensor(VOLTAGE_SENSOR_TYPE, DeviceCategory::Digital);
    SensorData<uint16_t> sut(sensor, 512u);

    TEST_ASSERT_EQUAL(VOLTAGE_SENSOR_TYPE, sut.getSensor().getType());
    TEST_ASSERT_EQUAL(static_cast<uint8_t>(DeviceCategory::Digital),
        static_cast<uint8_t>(sut.getSensor().getCategory()));
    TEST_ASSERT_EQUAL_UINT16(512u, sut.getData());
    TEST_ASSERT_TRUE(device::is_digital(sut.getSensor()));
}

/// @brief Tests SensorData copy construction.
/// @note The copy keeps the ISensor slice and the measurement.
void SensorData_CopyCtor_ShouldCopySensorAndData()
{
    ISensor sensor(VOLTAGE_SENSOR_TYPE, DeviceCategory::Analog);
    SensorData<uint32_t> source(sensor, 1381u);

    SensorData<uint32_t> sut = source;

    TEST_ASSERT_EQUAL(source.getSensor().getType(), sut.getSensor().getType());
    TEST_ASSERT_EQUAL(static_cast<uint8_t>(source.getSensor().getCategory()),
        static_cast<uint8_t>(sut.getSensor().getCategory()));
    TEST_ASSERT_EQUAL_UINT32(source.getData(), sut.getData());
}

/// @brief Tests SensorData with a trivially copyable struct measurement.
/// @note The measurement type must be trivially copyable.
struct FakeReading
{
    uint16_t raw;
    uint16_t millivolts;
};

void SensorData_Ctor_ShouldStoreStructMeasurement()
{
    ISensor sensor(VOLTAGE_SENSOR_TYPE, DeviceCategory::Analog);
    FakeReading reading = { 512u, 2500u };
    SensorData<FakeReading> sut(sensor, reading);

    TEST_ASSERT_EQUAL_UINT16(512u, sut.getData().raw);
    TEST_ASSERT_EQUAL_UINT16(2500u, sut.getData().millivolts);
    TEST_ASSERT_EQUAL(VOLTAGE_SENSOR_TYPE, sut.getSensor().getType());
}

/// @brief Tests storing SensorData in a fixed Array.
/// @note Array assigns into existing slots, so SensorData must be assignable.
void SensorData_Array_ShouldBeUsedForStoring()
{
    Array<SensorData<uint16_t>, 3> readings;
    readings[0] = SensorData<uint16_t>(ISensor(VOLTAGE_SENSOR_TYPE, DeviceCategory::Analog), 512u);
    readings[1] = SensorData<uint16_t>(ISensor(VOLTAGE_SENSOR_TYPE, DeviceCategory::Analog), 1024u);
    readings[2] = SensorData<uint16_t>(ISensor(VOLTAGE_SENSOR_TYPE, DeviceCategory::Analog), 2048u);

    // Tests of using the IFixedCollection interface.
    IFixedCollection<SensorData<uint16_t>>& sut = readings;

    TEST_ASSERT_EQUAL(3, sut.getCount());
    TEST_ASSERT_EQUAL_UINT16(512u, sut[0].getData());
    TEST_ASSERT_EQUAL_UINT16(1024u, sut[1].getData());
    TEST_ASSERT_EQUAL_UINT16(2048u, sut[2].getData());
    TEST_ASSERT_EQUAL(VOLTAGE_SENSOR_TYPE, sut[0].getSensor().getType());
    TEST_ASSERT_EQUAL(VOLTAGE_SENSOR_TYPE, sut[1].getSensor().getType());
    TEST_ASSERT_EQUAL(VOLTAGE_SENSOR_TYPE, sut[2].getSensor().getType());

    // Tests of using the IEnumerator interface.
    std::unique_ptr<IEnumerator<SensorData<uint16_t>>> enumerator = sut.getEnumerator();
    enumerator->reset();
    uint16_t index = 0;
    while (enumerator->moveNext())
    {
        Expected<SensorData<uint16_t>, Error> current = enumerator->getCurrent();
        TEST_ASSERT_TRUE(current.hasValue());
        TEST_ASSERT_EQUAL_UINT16(readings[index].getData(), current.getValue().getData());
        index++;
    }    
}

#endif
#endif
