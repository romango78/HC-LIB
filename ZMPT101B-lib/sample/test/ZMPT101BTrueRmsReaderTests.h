// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _ZMPT101B_TRUE_RMS_READER_TESTS_H_
#define _ZMPT101B_TRUE_RMS_READER_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "sensors/readers/ZMPT101BReaders.h"
#include "FakeTimer.h"
#include "FakeStream.h"

void ZMPT101BTrueRmsReader_Raise_Error_When_TimerIsNotInitialized()
{
    // Arrange
    ZMPT101BSensor* sensor = new ZMPT101BSensor(0, nullptr);
    ZMPT101BTrueRmsReader* sut = new ZMPT101BTrueRmsReader(nullptr);

    // Act
    ZMPT101B_ACVoltage result = sut->read(sensor);

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(DEVICE_ERROR_TIMER_IS_NOT_INITIALIZED, result.error, "Expected Timer Is Not Initialized error.");

    delete sut;
    delete sensor;
};

void ZMPT101BTrueRmsReader_Raise_Error_When_ArgumentIsNull()
{
    // Arrange
    ITimer* timer = new FakeTimer();
    ZMPT101BTrueRmsReader* sut = new ZMPT101BTrueRmsReader(timer);

    // Act
    ZMPT101B_ACVoltage result = sut->read(nullptr);

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(ERROR_ARGUMENT_IS_NULL, result.error, "Expected Argumet Is Null error.");

    delete sut;
    delete timer;
};

void ZMPT101BTrueRmsReader_Raise_Error_When_StreamIsNotInitialized()
{
    // Arrange
    ITimer* timer = new FakeTimer();

    ZMPT101BSensor* sensor = new ZMPT101BSensor(0, nullptr);
    ZMPT101BTrueRmsReader* sut = new ZMPT101BTrueRmsReader(timer);

    // Act
    ZMPT101B_ACVoltage result = sut->read(sensor);

    // Asserts
    TEST_ASSERT_EQUAL_MESSAGE(IO_ERROR_STREAM_NOTCREATED, result.error, "Expected Stream Is Not Created error.");

    delete sut;
    delete sensor;
    delete timer;
};

void ZMPT101BTrueRmsReader_Read_Data_And_Calculate_TrueRms()
{
    // Arrange
    const float expectedValue = 954.69072732f;

    ITimer* timer = new FakeTimer();
    IStream<uint16_t>* stream = new FakeStream(0, 1023);

    ZMPT101BSensor* sensor = new ZMPT101BSensor(0, stream);
    sensor->zero = 512;
    ZMPT101BTrueRmsReader* sut = new ZMPT101BTrueRmsReader(timer);

    // Act
    ZMPT101B_ACVoltage result = sut->read(sensor);

    // Asserts   
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.001f, expectedValue, result.data, "The True RMS calculation is falied.");
    TEST_ASSERT_EQUAL_MESSAGE(false, stream->hasError(), "No errors expected.");

    delete sut;
    delete sensor;
    delete stream;
    delete timer;
};

#endif
#endif