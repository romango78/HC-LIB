// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _HC_LIB_VOLTAGE_STREAM_TESTS_H_
#define _HC_LIB_VOLTAGE_STREAM_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "FakePortAdapter.h"
#include "stream/VoltageStream.h"

void VoltageStream_ShouldReadVoltage()
{
    float expectedValue = 2.5;
    FakePortAdapter<int>* adapter = new FakePortAdapter<int>();
    adapter->setData(static_cast<int>(expectedValue * ADC_SCALE / V_REF));
    IVoltageStream* sut = new VoltageStream(adapter);

    sut->begin(StreamMode::Read);
    float actualValue = sut->getVoltage();
    sut->end();

    TEST_ASSERT_FLOAT_WITHIN_MESSAGE(0.01, expectedValue, actualValue, "The read voltage is not equal the expected value.");

    delete sut;
}

void VoltageStream_ShouldSetPWM()
{
    int expectedValue = 25;
    FakePortAdapter<int>* adapter = new FakePortAdapter<int>();
    IVoltageStream* sut = new VoltageStream(adapter);

    sut->begin(StreamMode::Write);
    sut->setPwm(expectedValue);
    sut->end();

    TEST_ASSERT_EQUAL_INT_MESSAGE(static_cast<int>(expectedValue * PWM_MAX / 100), adapter->getData(),
        "The write PWM is not equal the expected value.");

    delete sut;
}

#endif
#endif
