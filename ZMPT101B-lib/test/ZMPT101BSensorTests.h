// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _HC_LIB_ZMPT101B_SENSOR_TESTS_H_
#define _HC_LIB_ZMPT101B_SENSOR_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "lib-utility.h"
#include "sensors/ZMPT101B.h"
#include "FakeStream.h"

ZMPT101BSensor createZMPT101BSensor()
{
    ZMPT101BSensor result{14, new FakeConstantStream(512)};
    return result;
}

ZMPT101BSensor createZMPT101BSensor2()
{
    ZMPT101BSensor result{3, new FakeConstantStream(400)};
    return result;
}

void ZMPT101BSensor_ShouldConstruct_WithPinAndStream()
{
    IStream<uint16_t>* stream = new FakeConstantStream(512);
    ZMPT101BSensor sut(14, stream);

    TEST_ASSERT_EQUAL(VOLTAGE_SENSOR_TYPE, sut.type);
    TEST_ASSERT_EQUAL(DeviceCategory::Analog, sut.category);
    TEST_ASSERT_EQUAL(14, sut.pin);
    TEST_ASSERT_EQUAL(stream, sut.stream);
    TEST_ASSERT_EQUAL(0, sut.zero);
    TEST_ASSERT_TRUE(device::is_analog(sut));
    TEST_ASSERT_FALSE(device::is_digital(sut));
    TEST_ASSERT_FALSE(device::is_relay(sut));
}

void ZMPT101BSensor_ShouldConstruct_WithNullStream()
{
    ZMPT101BSensor sut(7, nullptr);

    TEST_ASSERT_EQUAL(VOLTAGE_SENSOR_TYPE, sut.type);
    TEST_ASSERT_EQUAL(DeviceCategory::Analog, sut.category);
    TEST_ASSERT_EQUAL(7, sut.pin);
    TEST_ASSERT_NULL(sut.stream);
    TEST_ASSERT_EQUAL(0, sut.zero);
}

void ZMPT101BSensor_ShouldCopy_PreservesZeroAndClonesStream()
{
    ZMPT101BSensor source = createZMPT101BSensor();
    source.zero = 512;

    ZMPT101BSensor sut = source;

    TEST_ASSERT_EQUAL(source.type, sut.type);
    TEST_ASSERT_EQUAL(source.category, sut.category);
    TEST_ASSERT_EQUAL(source.pin, sut.pin);
    TEST_ASSERT_EQUAL(512, sut.zero);
    TEST_ASSERT_NOT_NULL(sut.stream);
    TEST_ASSERT_TRUE(source.stream != sut.stream);
}

void ZMPT101BSensor_ShouldCopy_WithNullStream()
{
    ZMPT101BSensor source(7, nullptr);
    source.zero = 99;

    ZMPT101BSensor sut = source;

    TEST_ASSERT_EQUAL(source.pin, sut.pin);
    TEST_ASSERT_EQUAL(99, sut.zero);
    TEST_ASSERT_NULL(sut.stream);
}

void ZMPT101BSensor_ShouldCopy_FromMovedSource()
{
    ZMPT101BSensor source = createZMPT101BSensor();
    source.zero = 400;
    ZMPT101BSensor owner(std::move(source));

    ZMPT101BSensor sut(source);

    TEST_ASSERT_EQUAL(source.pin, sut.pin);
    TEST_ASSERT_EQUAL(400, sut.zero);
    TEST_ASSERT_NULL_MESSAGE(sut.stream, "Copy of a moved-from sensor should not clone the stream.");
    TEST_ASSERT_NOT_NULL(owner.stream);
}

void ZMPT101BSensor_ShouldMove()
{
    ZMPT101BSensor source = createZMPT101BSensor();
    source.zero = 480;
    IStream<uint16_t>* const stream = source.stream;

    ZMPT101BSensor sut(std::move(source));

    TEST_ASSERT_EQUAL(14, sut.pin);
    TEST_ASSERT_EQUAL(480, sut.zero);
    TEST_ASSERT_EQUAL(stream, sut.stream);
}

void ZMPT101BSensor_ShouldAssignCopy()
{
    ZMPT101BSensor source = createZMPT101BSensor();
    source.zero = 512;
    ZMPT101BSensor sut = createZMPT101BSensor2();

    sut = source;

    TEST_ASSERT_EQUAL(source.pin, sut.pin);
    TEST_ASSERT_EQUAL(512, sut.zero);
    TEST_ASSERT_TRUE(source.stream != sut.stream);
}

void ZMPT101BSensor_ShouldAssign_FromNullStream()
{
    ZMPT101BSensor source(7, nullptr);
    ZMPT101BSensor sut = createZMPT101BSensor();

    sut = source;

    TEST_ASSERT_EQUAL(7, sut.pin);
    TEST_ASSERT_NULL_MESSAGE(sut.stream, "Assignment from a null-stream sensor should clear the stream.");
}

void ZMPT101BSensor_ShouldSelfAssign()
{
    ZMPT101BSensor sut = createZMPT101BSensor();
    sut.zero = 333;
    IStream<uint16_t>* const stream = sut.stream;

    sut = sut;

    TEST_ASSERT_EQUAL(333, sut.zero);
    TEST_ASSERT_EQUAL(stream, sut.stream);
}

void ZMPT101B_Calibrate_DoesNothing_WhenSensorIsNull()
{
    ZMPT101B::calibrate(nullptr);
}

void ZMPT101B_Calibrate_DoesNothing_WhenStreamIsNull()
{
    ZMPT101BSensor sensor(3, nullptr);
    sensor.zero = 99;

    ZMPT101B::calibrate(&sensor);

    TEST_ASSERT_EQUAL_MESSAGE(99, sensor.zero, "Null stream must leave zero unchanged.");
}

void ZMPT101B_Calibrate_AveragesAdcIntoZero()
{
    FakeConstantStream* stream = new FakeConstantStream(512);
    ZMPT101BSensor sensor(14, stream);

    ZMPT101B::calibrate(&sensor);

    TEST_ASSERT_EQUAL(512, sensor.zero);
    TEST_ASSERT_FALSE_MESSAGE(stream->canRead(), "calibrate() must call end().");
    TEST_ASSERT_FALSE(stream->hasError());
}

void ZMPT101B_Calibrate_WhenStreamAlreadyReadable()
{
    FakeConstantStream* stream = new FakeConstantStream(400);
    ZMPT101BSensor sensor(14, stream);
    stream->begin(StreamMode::Read);

    ZMPT101B::calibrate(&sensor);

    TEST_ASSERT_EQUAL(400, sensor.zero);
    TEST_ASSERT_FALSE_MESSAGE(stream->canRead(), "calibrate() must call end() even if the stream was already readable.");
    TEST_ASSERT_FALSE(stream->hasError());
}

#endif
#endif
