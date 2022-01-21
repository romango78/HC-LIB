// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.
#ifndef _VOLTAGE_STREAM_TESTS_H_
#define _VOLTAGE_STREAM_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "FakePortAdapter.h"
#include "stream/VoltageStream.h"

void VoltageStream_ShouldReadVoltage()
{
    // Arrange
    float expectedValue = 2.5;
    IPortAdapter<int>* adapter = (IPortAdapter<int> *)new FakePortAdapter();
    ((FakePortAdapter *)adapter)->setData(static_cast<int>(expectedValue*ADC_SCALE/V_REF));

    IVoltageStream* sut =(IVoltageStream *)new VoltageStream(adapter);

    // Act
    sut->begin(StreamMode::Read);
    float actualValue = sut->getVoltage();
    sut->end();

    // Assert
    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.01, expectedValue, actualValue,"The read voltage is not equal the expected value.");

    delete sut;
    delete adapter;
}

void VoltageStream_ShouldSetPWM()
{
    // Arrange
    int expectedValue = 25;
    IPortAdapter<int>* adapter = (IPortAdapter<int> *)new FakePortAdapter();    

    IVoltageStream* sut =(IVoltageStream *)new VoltageStream(adapter);

    // Act
    sut->begin(StreamMode::Write);
    sut->setPwm(expectedValue);
    sut->end();
    
    // Assert
    TEST_ASSERT_EQUAL_INT_MESSAGE(static_cast<int>(expectedValue*PWM_MAX/100), ((FakePortAdapter *)adapter)->getData()
        ,"The write PWM is not equal the expected value.");

    delete sut;
    delete adapter;
}

#endif
#endif