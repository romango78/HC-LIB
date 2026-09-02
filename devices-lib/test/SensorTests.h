// Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _HC_LIB_SENSOR_TESTS_H_
#define _HC_LIB_SENSOR_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "sensors/AnalogSensor.h"
#include "FakeStream.h"
#include "lib-utility.h"

AnalogSensor createAnalogSensor()
{
    AnalogSensor result{10, 5, (IStream<uint16_t>*)new FakeAnalogStream()};
    return result;
}

AnalogSensor createAnalogSensor2()
{
    AnalogSensor result{12, 10, (IStream<uint16_t>*)new FakeAnalogStream()};
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

void ShouldClassify_AnalogSensorAsAnalog()
{
    AnalogSensor sut = createAnalogSensor();

    TEST_ASSERT_TRUE_MESSAGE(device::is_analog(sut), "AnalogSensor should be analog.");
    TEST_ASSERT_FALSE_MESSAGE(device::is_digital(sut), "AnalogSensor should not be digital.");
    TEST_ASSERT_FALSE_MESSAGE(device::is_relay(sut), "AnalogSensor should not be a relay.");
}

void ShouldConstruct_AnalogSensor_WithNullStream()
{
    AnalogSensor sut{10, 5, nullptr};

    TEST_ASSERT_EQUAL(10, sut.type);
    TEST_ASSERT_TRUE(sut.category == DeviceCategory::Analog);
    TEST_ASSERT_EQUAL(5, sut.pin);
    TEST_ASSERT_NULL(sut.stream);
}

void ShouldCopy_AnalogSensor_WithNullStream()
{
    AnalogSensor source{10, 5, nullptr};

    AnalogSensor sut = source;

    TEST_ASSERT_EQUAL(source.type, sut.type);
    TEST_ASSERT_EQUAL(source.pin, sut.pin);
    TEST_ASSERT_NULL(sut.stream);
}

void ShouldCopy_AnalogSensor_FromMovedSource()
{
    AnalogSensor source = createAnalogSensor();
    AnalogSensor owner(std::move(source));

    AnalogSensor sut(source);

    TEST_ASSERT_EQUAL(source.type, sut.type);
    TEST_ASSERT_EQUAL(source.pin, sut.pin);
    TEST_ASSERT_NULL_MESSAGE(sut.stream, "Copy of a moved-from sensor should not clone the stream.");
    TEST_ASSERT_NOT_NULL(owner.stream);
}

void ShouldAssign_AnalogSensor_FromNullStream()
{
    AnalogSensor source{10, 5, nullptr};
    AnalogSensor sut = createAnalogSensor();

    sut = source;

    TEST_ASSERT_EQUAL(source.type, sut.type);
    TEST_ASSERT_EQUAL(source.pin, sut.pin);
    TEST_ASSERT_NULL_MESSAGE(sut.stream, "Assignment from a null-stream sensor should clear the stream.");
}

void ShouldSelfAssign_AnalogSensor()
{
    AnalogSensor sut = createAnalogSensor();
    IStream<uint16_t>* streamBefore = sut.stream;

    sut = sut;

    TEST_ASSERT_EQUAL_MESSAGE(streamBefore, sut.stream, "Self-assignment must leave the stream pointer unchanged.");
    TEST_ASSERT_NOT_NULL(sut.stream);
}

#endif
#endif