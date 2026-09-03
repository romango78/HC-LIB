// Copyright (c) 2022 Roman Gorielov. All Rights Reserved.
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
#include "SRD05VDCSLTests.h"

void testsProcess()
{
    UNITY_BEGIN();

    RUN_TEST(ShouldConvert_SRD05VDCSLToSRD05VDCSL);
    RUN_TEST(ShouldConvert_SRD05VDCSLToRelayDevice);
    RUN_TEST(ShouldConvert_SRD05VDCSLToDigitalDevice);
    RUN_TEST(ShouldConvert_SRD05VDCSLToIDevice);
    RUN_TEST(ShouldMove_SRD05VDCSLToSRD05VDCSL);
    RUN_TEST(ShouldAssignCopy_SRD05VDCSL);
    RUN_TEST(ShouldSelfAssign_SRD05VDCSL);
    RUN_TEST(ShouldConstruct_SRD05VDCSL_WithNullStream);
    RUN_TEST(ShouldClassify_SRD05VDCSLAsRelay);
    RUN_TEST(ShouldSwitch_SRD05VDCSL_OnAndOff);
    RUN_TEST(Should_RaiseError_WhenStreamIsNull);

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
