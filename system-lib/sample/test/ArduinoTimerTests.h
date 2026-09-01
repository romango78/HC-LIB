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

void ArduinoTimer_Ctor_ShouldBeStopped_WhenDefaultConstructed()
{
    ArduinoTimer sut;

    TEST_ASSERT_EQUAL_MESSAGE(0, sut.getInterval(), "Default interval should be 0.");
    TEST_ASSERT_EQUAL_MESSAGE(false, sut.isStarted(), "Timer should not be started.");
    TEST_ASSERT_EQUAL_MESSAGE(false, sut.isElapsed(), "Timer should not elapse before start.");
}

void ArduinoTimer_ConstAccessors_ShouldReadState_WhenNotStarted()
{
    ArduinoTimer sut;
    sut.setInterval(15);
    const ArduinoTimer &ref = sut;

    TEST_ASSERT_EQUAL_MESSAGE(15, ref.getInterval(), "const getInterval() should return the interval.");
    TEST_ASSERT_EQUAL_MESSAGE(false, ref.isStarted(), "const isStarted() should return false.");
}

void ArduinoTimer_SetInterval_ShouldUpdateInterval_WhenNotStarted()
{
    ArduinoTimer sut;

    sut.setInterval(50);

    TEST_ASSERT_EQUAL_MESSAGE(50, sut.getInterval(), "setInterval should store the interval when stopped.");
}

void ArduinoTimer_SetInterval_ShouldKeepInterval_WhenStarted()
{
    ArduinoTimer sut;
    sut.setInterval(20);
    sut.start();

    sut.setInterval(9999);

    TEST_ASSERT_EQUAL_MESSAGE(20, sut.getInterval(), "setInterval should be ignored while the timer is started.");
}

void ArduinoTimer_IsElapsed_ShouldBeTrueImmediately_WhenIntervalIsZero()
{
    ArduinoTimer sut;
    sut.start();

    TEST_ASSERT_EQUAL_MESSAGE(true, sut.isElapsed(), "A zero interval should elapse immediately after start.");
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
}

void ArduinoTimer_IsElapsed_ShouldStayTrue_UntilStopped()
{
    ArduinoTimer sut;
    sut.setInterval(20);
    sut.start();
    while(!sut.isElapsed())
    {
    }

    TEST_ASSERT_EQUAL_MESSAGE(true, sut.isElapsed(), "Elapsed state should latch.");
    TEST_ASSERT_EQUAL_MESSAGE(true, sut.isStarted(), "Elapsed timer should still be started.");
}

void ArduinoTimer_Stop_ShouldClearStartedAndElapsed_WhenStopped()
{
    ArduinoTimer sut;
    sut.setInterval(20);
    sut.start();
    while(!sut.isElapsed())
    {
    }

    sut.stop();

    TEST_ASSERT_EQUAL_MESSAGE(false, sut.isStarted(), "stop() should clear started.");
    TEST_ASSERT_EQUAL_MESSAGE(false, sut.isElapsed(), "stop() should clear elapsed.");
    TEST_ASSERT_EQUAL_MESSAGE(20, sut.getInterval(), "stop() should keep the interval.");
}

void ArduinoTimer_Start_ShouldRestartCountdown_WhenAlreadyElapsed()
{
    ArduinoTimer sut;
    sut.setInterval(20);
    sut.start();
    while(!sut.isElapsed())
    {
    }

    sut.start();

    TEST_ASSERT_EQUAL_MESSAGE(true, sut.isStarted(), "start() should keep the timer started.");
    TEST_ASSERT_EQUAL_MESSAGE(false, sut.isElapsed(), "start() should restart the countdown.");
}

void ArduinoTimer_Start_ShouldRunNewInterval_WhenRestartedAfterStop()
{
    ArduinoTimer sut;
    sut.setInterval(20);
    sut.start();
    while(!sut.isElapsed())
    {
    }
    sut.stop();

    sut.start();

    TEST_ASSERT_EQUAL_MESSAGE(true, sut.isStarted(), "start() after stop() should start again.");
    TEST_ASSERT_EQUAL_MESSAGE(false, sut.isElapsed(), "A new countdown should not be elapsed immediately.");
}

#endif
#endif
