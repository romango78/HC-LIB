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
#include "unity_extensions.h"
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
    const uint32_t expectedValue = 1382u;

    FakeTimer timer;
    ZMPT101BSensor sensor(0, new FakeStream(0, 1023));
    sensor.zero = 512;
    ZMPT101BTrueRmsReader sut(&timer);

    Expected<ZMPT101B_ACVoltage, Error> result = sut.read(sensor);

    TEST_ASSERT_TRUE(result.hasValue());
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(expectedValue, result.getValue().data, F("The True RMS calculation is falied."));
    TEST_ASSERT_EQUAL(VOLTAGE_SENSOR_TYPE, result.getValue().sensor.type);
    TEST_ASSERT_FALSE_MESSAGE(sensor.stream->hasError(), F("No errors expected."));
}

void ZMPT101BTrueRmsReader_Read_WhenStreamAlreadyReadable()
{
    const uint32_t expectedValue = 1382u;

    FakeTimer timer;
    ZMPT101BSensor sensor(0, new FakeStream(0, 1023));
    sensor.zero = 512;
    sensor.stream->begin(StreamMode::Read);
    ZMPT101BTrueRmsReader sut(&timer);

    Expected<ZMPT101B_ACVoltage, Error> result = sut.read(sensor);

    TEST_ASSERT_TRUE(result.hasValue());
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(expectedValue, result.getValue().data, F("The True RMS calculation is falied."));
    TEST_ASSERT_FALSE(sensor.stream->hasError());
}

void ZMPT101BTrueRmsReader_Read_WhenTimerAlreadyStarted()
{
    const uint32_t expectedValue = 1382u;

    FakeTimer timer;
    timer.start();
    ZMPT101BSensor sensor(0, new FakeStream(0, 1023));
    sensor.zero = 512;
    ZMPT101BTrueRmsReader sut(&timer);

    Expected<ZMPT101B_ACVoltage, Error> result = sut.read(sensor);

    TEST_ASSERT_TRUE(result.hasValue());
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(expectedValue, result.getValue().data, F("The True RMS calculation is falied."));
    TEST_ASSERT_FALSE(timer.isStarted());
}

void ZMPT101BTrueRmsReader_Read_Twice()
{
    const uint32_t expectedValue = 1382u;

    FakeTimer timer;
    ZMPT101BSensor sensor(0, new FakeStream(0, 1023));
    sensor.zero = 512;
    ZMPT101BTrueRmsReader sut(&timer);

    Expected<ZMPT101B_ACVoltage, Error> first = sut.read(sensor);
    Expected<ZMPT101B_ACVoltage, Error> second = sut.read(sensor);

    TEST_ASSERT_TRUE(first.hasValue());
    TEST_ASSERT_TRUE(second.hasValue());
    TEST_ASSERT_EQUAL_UINT32(expectedValue, first.getValue().data);
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
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0u, result.getValue().data, F("No samples must yield 0 mV True RMS."));
}

void ZMPT101BTrueRmsReader_Read_ConstantMidScale()
{
    FakeTimer timer;
    ZMPT101BSensor sensor(0, new FakeConstantStream(512));
    sensor.zero = 512;
    ZMPT101BTrueRmsReader sut(&timer);

    Expected<ZMPT101B_ACVoltage, Error> result = sut.read(sensor);

    TEST_ASSERT_TRUE(result.hasValue());
    TEST_ASSERT_FALSE(sensor.stream->hasError());
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0u, result.getValue().data,
        F("Constant mid-scale ADC yields 0 mV True RMS."));
}

#endif
#endif
