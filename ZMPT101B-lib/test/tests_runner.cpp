// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#if defined(ARDUINO) && !defined(UNIT_TEST)

#include <Arduino.h>

void setup()
{
}

void loop()
{
    delay(100);
}

#endif

#if defined(UNIT_TEST)

#include <unity.h>
#include "ZMPT101BSensorTests.h"
#include "ZMPT101BRmsReaderTests.h"
#include "ZMPT101BTrueRmsReaderTests.h"

void testsProcess()
{
    UNITY_BEGIN();

    RUN_TEST(ZMPT101BSensor_ShouldConstruct_WithPinAndStream);
    RUN_TEST(ZMPT101BSensor_ShouldConstruct_WithNullStream);
    RUN_TEST(ZMPT101BSensor_ShouldCopy_PreservesZeroAndClonesStream);
    RUN_TEST(ZMPT101BSensor_ShouldCopy_WithNullStream);
    RUN_TEST(ZMPT101BSensor_ShouldCopy_FromMovedSource);
    RUN_TEST(ZMPT101BSensor_ShouldMove);
    RUN_TEST(ZMPT101BSensor_ShouldAssignCopy);
    RUN_TEST(ZMPT101BSensor_ShouldAssign_FromNullStream);
    RUN_TEST(ZMPT101BSensor_ShouldSelfAssign);
    RUN_TEST(ZMPT101B_Calibrate_DoesNothing_WhenSensorIsNull);
    RUN_TEST(ZMPT101B_Calibrate_DoesNothing_WhenStreamIsNull);
    RUN_TEST(ZMPT101B_Calibrate_AveragesAdcIntoZero);
    RUN_TEST(ZMPT101B_Calibrate_WhenStreamAlreadyReadable);

    RUN_TEST(ZMPT101BRmsReader_Raise_Error_When_TimerIsNotInitialized);
    RUN_TEST(ZMPT101BRmsReader_Raise_Error_When_StreamIsNotInitialized);
    RUN_TEST(ZMPT101BRmsReader_Read_Data_And_Calculate_Rms);
    RUN_TEST(ZMPT101BRmsReader_Read_WhenStreamAlreadyReadable);
    RUN_TEST(ZMPT101BRmsReader_Read_WhenTimerAlreadyStarted);
    RUN_TEST(ZMPT101BRmsReader_Read_Twice);
    RUN_TEST(ZMPT101BRmsReader_Read_WhenTimerElapsedImmediately);
    RUN_TEST(ZMPT101BRmsReader_Read_ConstantMidScale);

    RUN_TEST(ZMPT101BTrueRmsReader_Raise_Error_When_TimerIsNotInitialized);
    RUN_TEST(ZMPT101BTrueRmsReader_Raise_Error_When_StreamIsNotInitialized);
    RUN_TEST(ZMPT101BTrueRmsReader_Read_Data_And_Calculate_TrueRms);
    RUN_TEST(ZMPT101BTrueRmsReader_Read_WhenStreamAlreadyReadable);
    RUN_TEST(ZMPT101BTrueRmsReader_Read_WhenTimerAlreadyStarted);
    RUN_TEST(ZMPT101BTrueRmsReader_Read_Twice);
    RUN_TEST(ZMPT101BTrueRmsReader_Read_WhenTimerElapsedImmediately);
    RUN_TEST(ZMPT101BTrueRmsReader_Read_ConstantMidScale);

    UNITY_END();
};

#if !defined(ARDUINO)

void setUp(void)
{
};

void tearDown(void)
{
};

int main(int argc, char **argv)
{
    testsProcess();
    return 0;
};

#endif

#if defined(ARDUINO)

#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    delay(4000);
    testsProcess();
};

void loop()
{
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(500);
};

#endif
#endif
