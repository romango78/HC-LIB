// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _ZMPT101B_RMS_READER_TESTS_H_
#define _ZMPT101B_RMS_READER_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "sensors/readers/ZMPT101BReaders.h"
#include "FakeTimer.h"
#include "FakeStream.h"

void ZMPT101BRmsReader_Raise_Error_When_TimerIsNotInitialized()
{
    // Arrange
    ZMPT101BSensor sensor = {0, nullptr};
    ZMPT101BRmsReader* sut = new ZMPT101BRmsReader(nullptr);

    // Act
    ZMPT101B_ACVoltage result = sut->read(sensor);

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(TIMER_IS_NOT_INITIALIZED_DEVICE_ERROR, result.error, "Expected Timer Is Not Initialized error.");

    delete sut;
};

void ZMPT101BRmsReader_Raise_Error_When_StreamIsNotInitialized()
{
    // Arrange
    ITimer* timer = new FakeTimer();

    ZMPT101BSensor sensor = {0, nullptr};
    ZMPT101BRmsReader* sut = new ZMPT101BRmsReader(timer);

    // Act
    ZMPT101B_ACVoltage result = sut->read(sensor);

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(STREAM_NOTCREATED_IO_ERROR, result.error, "Expected Stream Is Not Created error.");

    delete sut;
    delete timer;
};

void ZMPT101BRmsReader_Read_Data_And_Calculate_Rms()
{
    // Arrange
    const float expectedValue = 1357.688158f;

    ITimer* timer = new FakeTimer();
    IStream<uint16_t>* stream = new FakeStream(0, 1023);

    ZMPT101BSensor sensor = {0, stream};
    sensor.zero = 512;
    ZMPT101BRmsReader* sut = new ZMPT101BRmsReader(timer);

    // Act
    ZMPT101B_ACVoltage result = sut->read(sensor);

    // Asserts   
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.001f, expectedValue, result.data, "The RMS calculation is failed.");
    TEST_ASSERT_EQUAL_MESSAGE(false, stream->hasError(), "No errors expected.");

    delete sut;
    delete timer;
};

#endif
#endif