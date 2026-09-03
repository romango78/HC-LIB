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
#include "LogTests.h"

void testsProcess()
{
    UNITY_BEGIN();

    RUN_TEST(Log_ShouldLogDebug_WithoutModule);
    RUN_TEST(Log_ShouldLogDebug_WithModule);
    RUN_TEST(Log_ShouldLogInfo_WithoutModule);
    RUN_TEST(Log_ShouldLogInfo_WithModule);
    RUN_TEST(Log_ShouldLogWarning_WithoutModule);
    RUN_TEST(Log_ShouldLogWarning_WithModule);
    RUN_TEST(Log_ShouldLogError_WithoutModule);
    RUN_TEST(Log_ShouldLogError_WithModule);
    RUN_TEST(Log_ShouldLogFatal_WithoutModule);
    RUN_TEST(Log_ShouldLogFatal_WithModule);
    RUN_TEST(Log_ShouldNotLog_WhenPersisterIsNull);
    RUN_TEST(Log_ShouldLog_WithoutDateTime);
    RUN_TEST(Log_ShouldLog_WithFormatArguments);
    RUN_TEST(Log_ShouldLog_WithFlashFormatString);
    RUN_TEST(Log_ShouldLog_WithFlashModuleName);

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
