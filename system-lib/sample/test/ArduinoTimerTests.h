// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _ARDUINO_TIMER_TESTS_H_
#define _ARDUINO_TIMER_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#include "timers/ArduinoTimer.h"

void ArduinoTimer_IsElapsed_ShouldBeFalse_WhenNotStarted()
{
    ArduinoTimer sut;

    TEST_ASSERT_EQUAL_MESSAGE(false, sut.isStarted(), "Timer should not be started.");
    TEST_ASSERT_EQUAL_MESSAGE(false, sut.isElapsed(), "Timer should not elapse before start.");
}

void ArduinoTimer_IsElapsed_ShouldBecomeTrue_AfterInterval()
{
    ArduinoTimer sut;
    sut.setInterval(20);
    sut.start();

    TEST_ASSERT_EQUAL_MESSAGE(true, sut.isStarted(), "Timer should be started.");
    TEST_ASSERT_EQUAL_MESSAGE(false, sut.isElapsed(), "Timer should not elapse immediately.");

    while(!sut.isElapsed())
    {
    }

    TEST_ASSERT_EQUAL_MESSAGE(true, sut.isElapsed(), "Timer should elapse after the interval.");

    sut.stop();
    TEST_ASSERT_EQUAL_MESSAGE(false, sut.isElapsed(), "Stopped timer should not stay elapsed.");
}

#endif
#endif
