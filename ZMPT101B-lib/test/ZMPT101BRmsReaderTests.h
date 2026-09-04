// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _HC_LIB_ZMPT101B_RMS_READER_TESTS_H_
#define _HC_LIB_ZMPT101B_RMS_READER_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "sensors/readers/ZMPT101BReaders.h"
#include "FakeTimer.h"
#include "FakeStream.h"

void ZMPT101BRmsReader_Raise_Error_When_TimerIsNotInitialized()
{
    ZMPT101BSensor sensor(0, nullptr);
    ZMPT101BRmsReader sut(nullptr);

    Expected<ZMPT101B_ACVoltage, Error> result = sut.read(sensor);

    TEST_ASSERT_FALSE(result.hasValue());
    TEST_ASSERT_TRUE(result.getError() == DeviceError::TimerIsNotInitialized);
}

void ZMPT101BRmsReader_Raise_Error_When_StreamIsNotInitialized()
{
    FakeTimer timer;
    ZMPT101BSensor sensor(0, nullptr);
    ZMPT101BRmsReader sut(&timer);

    Expected<ZMPT101B_ACVoltage, Error> result = sut.read(sensor);

    TEST_ASSERT_FALSE(result.hasValue());
    TEST_ASSERT_TRUE(result.getError() == IoError::StreamNotCreated);
}

void ZMPT101BRmsReader_Read_Data_And_Calculate_Rms()
{
    const float expectedValue = 1357.688158f;

    FakeTimer timer;
    ZMPT101BSensor sensor(0, new FakeStream(0, 1023));
    sensor.zero = 512;
    ZMPT101BRmsReader sut(&timer);

    Expected<ZMPT101B_ACVoltage, Error> result = sut.read(sensor);

    TEST_ASSERT_TRUE(result.hasValue());
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.001f, expectedValue, result.getValue().data, "The RMS calculation is failed.");
    TEST_ASSERT_EQUAL(VOLTAGE_SENSOR_TYPE, result.getValue().sensor.type);
    TEST_ASSERT_FALSE_MESSAGE(sensor.stream->hasError(), "No errors expected.");
}

void ZMPT101BRmsReader_Read_WhenStreamAlreadyReadable()
{
    const float expectedValue = 1357.688158f;

    FakeTimer timer;
    ZMPT101BSensor sensor(0, new FakeStream(0, 1023));
    sensor.zero = 512;
    sensor.stream->begin(StreamMode::Read);
    ZMPT101BRmsReader sut(&timer);

    Expected<ZMPT101B_ACVoltage, Error> result = sut.read(sensor);

    TEST_ASSERT_TRUE(result.hasValue());
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.001f, expectedValue, result.getValue().data, "The RMS calculation is failed.");
    TEST_ASSERT_FALSE(sensor.stream->hasError());
}

void ZMPT101BRmsReader_Read_WhenTimerAlreadyStarted()
{
    const float expectedValue = 1357.688158f;

    FakeTimer timer;
    timer.start();
    ZMPT101BSensor sensor(0, new FakeStream(0, 1023));
    sensor.zero = 512;
    ZMPT101BRmsReader sut(&timer);

    Expected<ZMPT101B_ACVoltage, Error> result = sut.read(sensor);

    TEST_ASSERT_TRUE(result.hasValue());
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.001f, expectedValue, result.getValue().data, "The RMS calculation is failed.");
    TEST_ASSERT_FALSE(timer.isStarted());
}

void ZMPT101BRmsReader_Read_Twice()
{
    const float expectedValue = 1357.688158f;

    FakeTimer timer;
    ZMPT101BSensor sensor(0, new FakeStream(0, 1023));
    sensor.zero = 512;
    ZMPT101BRmsReader sut(&timer);

    Expected<ZMPT101B_ACVoltage, Error> first = sut.read(sensor);
    Expected<ZMPT101B_ACVoltage, Error> second = sut.read(sensor);

    TEST_ASSERT_TRUE(first.hasValue());
    TEST_ASSERT_TRUE(second.hasValue());
    TEST_ASSERT_FLOAT_WITHIN(0.001f, expectedValue, first.getValue().data);
    TEST_ASSERT_FALSE(sensor.stream->hasError());
}

void ZMPT101BRmsReader_Read_WhenTimerElapsedImmediately()
{
    const float expectedValue = -353.55339059f;

    FakeTimer timer(0);
    ZMPT101BSensor sensor(0, new FakeConstantStream(512));
    sensor.zero = 512;
    ZMPT101BRmsReader sut(&timer);

    Expected<ZMPT101B_ACVoltage, Error> result = sut.read(sensor);

    TEST_ASSERT_TRUE(result.hasValue());
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.001f, expectedValue, result.getValue().data,
        "No samples keep max=0 and min=1000.");
}

void ZMPT101BRmsReader_Read_ConstantMidScale()
{
    FakeTimer timer;
    ZMPT101BSensor sensor(0, new FakeConstantStream(512));
    sensor.zero = 512;
    ZMPT101BRmsReader sut(&timer);

    Expected<ZMPT101B_ACVoltage, Error> result = sut.read(sensor);

    TEST_ASSERT_TRUE(result.hasValue());
    TEST_ASSERT_FALSE(sensor.stream->hasError());
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.001f, 1.13137085f, result.getValue().data,
        "Constant zero offset: max stays 0, min is PolynomialEquation(0).");
}

#endif
#endif
