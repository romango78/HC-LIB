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

void VoltageStream_ShouldClampPwm_WhenPercentageIsBelowZero()
{
    FakePortAdapter<int>* adapter = new FakePortAdapter<int>();
    VoltageStream sut(adapter);

    sut.begin(StreamMode::Write);
    sut.setPwm(-10);
    sut.end();

    TEST_ASSERT_EQUAL_INT_MESSAGE(0, adapter->getData(), "Negative PWM should clamp to 0.");
    TEST_ASSERT_FALSE_MESSAGE(sut.hasError(), "No errors expected.");
}

void VoltageStream_ShouldClampPwm_WhenPercentageIsAbove100()
{
    FakePortAdapter<int>* adapter = new FakePortAdapter<int>();
    VoltageStream sut(adapter);

    sut.begin(StreamMode::Write);
    sut.setPwm(150);
    sut.end();

    TEST_ASSERT_EQUAL_INT_MESSAGE(PWM_MAX, adapter->getData(), "PWM above 100 should clamp to PWM_MAX.");
    TEST_ASSERT_FALSE_MESSAGE(sut.hasError(), "No errors expected.");
}

void VoltageStream_ShouldClone()
{
    FakePortAdapter<int>* adapter = new FakePortAdapter<int>();
    adapter->setData(static_cast<int>(2.5f * ADC_SCALE / V_REF));
    VoltageStream sut(adapter);
    sut.begin(StreamMode::Read);

    IStream<uint16_t>* clone = sut.clone();

    TEST_ASSERT_NOT_NULL_MESSAGE(clone, "A clone is expected.");
    TEST_ASSERT_TRUE_MESSAGE(clone->canRead(), "The clone should keep the source mode.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(adapter->getData(), clone->read(), "The clone should read the cloned adapter data.");

    delete clone;
}

#endif
#endif
