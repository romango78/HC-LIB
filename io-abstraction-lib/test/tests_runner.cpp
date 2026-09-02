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
#include "IoErrorTests.h"
#include "BaseStreamTests.h"

void testsProcess()
{
    UNITY_BEGIN();

    RUN_TEST(IoError_ToError_ShouldMatchStreamNotCreated);
    RUN_TEST(IoError_ToError_ShouldMatchStreamClosed);
    RUN_TEST(IoError_MakeError_ShouldWrapIoError);

    RUN_TEST(BaseStream_Ctor_ShouldBeClosed_WithoutError);
    RUN_TEST(BaseStream_Begin_ShouldAllowRead_WhenOpenedForRead);
    RUN_TEST(BaseStream_Begin_ShouldAllowWrite_WhenOpenedForWrite);
    RUN_TEST(BaseStream_End_ShouldCloseStream);
    RUN_TEST(BaseStream_SetLastError_ShouldStoreIoError);

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
