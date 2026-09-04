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
    TEST_ASSERT_FALSE_MESSAGE(sensor.stream->hasError(), "No errors expected.");
}

#endif
#endif
