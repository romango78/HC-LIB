// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _HC_LIB_PORT_ADAPTER_TESTS_H_
#define _HC_LIB_PORT_ADAPTER_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "adapter/AnalogPortAdapter.h"
#include "adapter/DigitalPortAdapter.h"

void AnalogPortAdapter_ShouldReturnNoData_WhenNotOnArduino()
{
    AnalogPortAdapter sut(3);

    sut.setInputMode();
    sut.setOutputMode();
    sut.write(128);

    TEST_ASSERT_EQUAL_MESSAGE(NO_DATA, sut.read(), "Native analog read has no hardware pin.");
    TEST_ASSERT_EQUAL_MESSAGE(NO_DATA, sut.getState(), "Native analog state has no hardware pin.");
}

void AnalogPortAdapter_ShouldClone()
{
    AnalogPortAdapter sut(5);

    IPortAdapter<int>* clone = sut.clone();

    TEST_ASSERT_NOT_NULL_MESSAGE(clone, "A clone is expected.");
    TEST_ASSERT_EQUAL_MESSAGE(NO_DATA, clone->read(), "The clone should behave as a native analog adapter.");
    TEST_ASSERT_EQUAL_MESSAGE(NO_DATA, clone->getState(), "The clone should behave as a native analog adapter.");

    delete clone;
}

void DigitalPortAdapter_ShouldReturnNoData_WhenNotOnArduino()
{
    DigitalPortAdapter sut(2);

    sut.setInputMode();
    sut.setOutputMode();
    sut.write(1);

    TEST_ASSERT_EQUAL_MESSAGE(NO_DATA, sut.read(), "Native digital read has no hardware pin.");
    TEST_ASSERT_EQUAL_MESSAGE(NO_DATA, sut.getState(), "Native digital state has no hardware pin.");
}

void DigitalPortAdapter_ShouldClone()
{
    DigitalPortAdapter sut(4);

    IPortAdapter<uint8_t>* clone = sut.clone();

    TEST_ASSERT_NOT_NULL_MESSAGE(clone, "A clone is expected.");
    TEST_ASSERT_EQUAL_MESSAGE(NO_DATA, clone->read(), "The clone should behave as a native digital adapter.");
    TEST_ASSERT_EQUAL_MESSAGE(NO_DATA, clone->getState(), "The clone should behave as a native digital adapter.");

    delete clone;
}

#endif
#endif
