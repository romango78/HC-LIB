// Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _SENSOR_TESTS_H_
#define _SENSOR_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "sensors/AnalogSensor.h"
#include "FakeStream.h"
#include "move.h"

AnalogSensor createAnalogSensor()
{
    AnalogSensor result{10, 5, (IStream<uint16_t>*)new FakeStream()};
    return result;
}

AnalogSensor createAnalogSensor2()
{
    AnalogSensor result{12, 10, (IStream<uint16_t>*)new FakeStream()};
    return result;
}

void ShouldConvert_AnalogSensorToAnalogSensor()
{
    // Arrange
    AnalogSensor source = createAnalogSensor();

    // Act
    AnalogSensor sut = source;

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(source.type, sut.type, "The 'type' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.category, sut.category, "The 'category' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.pin, sut.pin, "The 'pin' should be same as in origin object.");
    TEST_ASSERT_NOT_NULL_MESSAGE(sut.stream, "The 'stream' should be cloned.");
}

void ShouldConvert_AnalogSensorToISensor()
{
    // Arrange
    AnalogSensor source = createAnalogSensor();

    // Act
    ISensor sut = (ISensor)source;

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(source.type, sut.type, "The 'type' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.category, sut.category, "The 'category' should be same as in origin object.");
}

void ShouldMove_AnalogSensorToAnalogSensor()
{
    // Arrange
    AnalogSensor source = createAnalogSensor();
    auto sourceStreamAddr = reinterpret_cast<uintptr_t>(source.stream);

    // Act
    AnalogSensor sut(std::move(source));

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(source.type, sut.type, "The 'type' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.category, sut.category, "The 'category' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.pin, sut.pin, "The 'pin' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(sourceStreamAddr, sut.stream, "The 'stream' should be same as in origin object.");
}

void ShouldAssignCopy_AnalogSensor()
{
    // Arrange
    AnalogSensor source = createAnalogSensor();
    AnalogSensor sut = createAnalogSensor2();

    // Act
    sut = source;

    TEST_ASSERT_EQUAL_MESSAGE(source.type, sut.type, "The 'type' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.category, sut.category, "The 'category' should be same as in origin object.");
    TEST_ASSERT_EQUAL_MESSAGE(source.pin, sut.pin, "The 'pin' should be same as in origin object.");
    TEST_ASSERT_TRUE_MESSAGE(source.stream != sut.stream, "The 'stream' should be cloned.");
}

#endif
#endif