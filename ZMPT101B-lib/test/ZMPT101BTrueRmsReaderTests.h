// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _HC_LIB_ZMPT101B_TRUE_RMS_READER_TESTS_H_
#define _HC_LIB_ZMPT101B_TRUE_RMS_READER_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "sensors/readers/ZMPT101BReaders.h"
#include "FakeTimer.h"
#include "FakeStream.h"

void ZMPT101BTrueRmsReader_Raise_Error_When_TimerIsNotInitialized()
{
    ZMPT101BSensor sensor(0, nullptr);
    ZMPT101BTrueRmsReader sut(nullptr);

    Expected<ZMPT101B_ACVoltage, Error> result = sut.read(sensor);

    TEST_ASSERT_FALSE(result.hasValue());
    TEST_ASSERT_TRUE(result.getError() == DeviceError::TimerIsNotInitialized);
}

void ZMPT101BTrueRmsReader_Raise_Error_When_StreamIsNotInitialized()
{
    FakeTimer timer;
    ZMPT101BSensor sensor(0, nullptr);
    ZMPT101BTrueRmsReader sut(&timer);

    Expected<ZMPT101B_ACVoltage, Error> result = sut.read(sensor);

    TEST_ASSERT_FALSE(result.hasValue());
    TEST_ASSERT_TRUE(result.getError() == IoError::StreamNotCreated);
}

void ZMPT101BTrueRmsReader_Read_Data_And_Calculate_TrueRms()
{
    const float expectedValue = 954.69072732f;

    FakeTimer timer;
    ZMPT101BSensor sensor(0, new FakeStream(0, 1023));
    sensor.zero = 512;
    ZMPT101BTrueRmsReader sut(&timer);

    Expected<ZMPT101B_ACVoltage, Error> result = sut.read(sensor);

    TEST_ASSERT_TRUE(result.hasValue());
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.001f, expectedValue, result.getValue().data, "The True RMS calculation is falied.");
    TEST_ASSERT_EQUAL(VOLTAGE_SENSOR_TYPE, result.getValue().sensor.type);
    TEST_ASSERT_FALSE_MESSAGE(sensor.stream->hasError(), "No errors expected.");
}

void ZMPT101BTrueRmsReader_Read_WhenStreamAlreadyReadable()
{
    const float expectedValue = 954.69072732f;

    FakeTimer timer;
    ZMPT101BSensor sensor(0, new FakeStream(0, 1023));
    sensor.zero = 512;
    sensor.stream->begin(StreamMode::Read);
    ZMPT101BTrueRmsReader sut(&timer);

    Expected<ZMPT101B_ACVoltage, Error> result = sut.read(sensor);

    TEST_ASSERT_TRUE(result.hasValue());
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.001f, expectedValue, result.getValue().data, "The True RMS calculation is falied.");
    TEST_ASSERT_FALSE(sensor.stream->hasError());
}

void ZMPT101BTrueRmsReader_Read_WhenTimerAlreadyStarted()
{
    const float expectedValue = 954.69072732f;

    FakeTimer timer;
    timer.start();
    ZMPT101BSensor sensor(0, new FakeStream(0, 1023));
    sensor.zero = 512;
    ZMPT101BTrueRmsReader sut(&timer);

    Expected<ZMPT101B_ACVoltage, Error> result = sut.read(sensor);

    TEST_ASSERT_TRUE(result.hasValue());
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.001f, expectedValue, result.getValue().data, "The True RMS calculation is falied.");
    TEST_ASSERT_FALSE(timer.isStarted());
}

void ZMPT101BTrueRmsReader_Read_Twice()
{
    const float expectedValue = 954.69072732f;

    FakeTimer timer;
    ZMPT101BSensor sensor(0, new FakeStream(0, 1023));
    sensor.zero = 512;
    ZMPT101BTrueRmsReader sut(&timer);

    Expected<ZMPT101B_ACVoltage, Error> first = sut.read(sensor);
    Expected<ZMPT101B_ACVoltage, Error> second = sut.read(sensor);

    TEST_ASSERT_TRUE(first.hasValue());
    TEST_ASSERT_TRUE(second.hasValue());
    TEST_ASSERT_FLOAT_WITHIN(0.001f, expectedValue, first.getValue().data);
    TEST_ASSERT_FALSE(sensor.stream->hasError());
}

void ZMPT101BTrueRmsReader_Read_WhenTimerElapsedImmediately()
{
    FakeTimer timer(0);
    ZMPT101BSensor sensor(0, new FakeConstantStream(512));
    sensor.zero = 512;
    ZMPT101BTrueRmsReader sut(&timer);

    Expected<ZMPT101B_ACVoltage, Error> result = sut.read(sensor);

    TEST_ASSERT_TRUE(result.hasValue());
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.001f, 0.0f, result.getValue().data, "No samples must yield 0 V True RMS.");
}

void ZMPT101BTrueRmsReader_Read_ConstantMidScale()
{
    const float expectedValue = 3.198f;

    FakeTimer timer;
    ZMPT101BSensor sensor(0, new FakeConstantStream(512));
    sensor.zero = 512;
    ZMPT101BTrueRmsReader sut(&timer);

    Expected<ZMPT101B_ACVoltage, Error> result = sut.read(sensor);

    TEST_ASSERT_TRUE(result.hasValue());
    TEST_ASSERT_FALSE(sensor.stream->hasError());
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.001f, expectedValue, result.getValue().data,
        "True RMS of PolynomialEquation(0) is |offset|.");
}

#endif
#endif
