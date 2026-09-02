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
#include "DigitalDeviceControllerTests.h"
#include "RelayDeviceControllerTests.h"
#include "DeviceTests.h"
#include "SensorTests.h"

void testsProcess()
{
    UNITY_BEGIN();

    RUN_TEST(Should_SetState_WhenDeviceIsInitialized);
    RUN_TEST(Should_RaiseError_IfSetState_WhenStreamIsNotSet);
    RUN_TEST(Should_GetState_WhenDeviceIsInitialized);
    RUN_TEST(Should_RaiseError_IfGetState_WhenStreamIsNotSet);

    RUN_TEST(ShouldSwitchRepayInOnStatus);
    RUN_TEST(Should_RaiseError_WhenTryOnRelay_AndStreamIsNotSet);
    RUN_TEST(ShouldSwitchRepayInOffStatus);
    RUN_TEST(Should_RaiseError_WhenTryOffRelay_AndStreamIsNotSet);
    RUN_TEST(ShouldGetRelayState);
    RUN_TEST(Should_RaiseError_WhenTryGetRelayState_AndStreamIsNotSet);

    RUN_TEST(ShouldConvert_AnalogDeviceToAnalogDevice);
    RUN_TEST(ShouldConvert_AnalogDeviceToIDevice);
    RUN_TEST(ShouldMove_AnalogDeviceToAnalogDevice);
    RUN_TEST(ShouldAssignCopy_AnalogDevice);
    RUN_TEST(ShouldConvert_DigitalDeviceToDigitalDevice);
    RUN_TEST(ShouldConvert_DigitalDeviceToIDevice);
    RUN_TEST(ShouldMove_DigitalDeviceToDigitalDevice);
    RUN_TEST(ShouldAssignCopy_DigitalDevice);
    RUN_TEST(ShouldConvert_RelayDeviceToRelayDevice);
    RUN_TEST(ShouldConvert_RelayDeviceToDigitalDevice);
    RUN_TEST(ShouldConvert_RelayDeviceToIDevice);
    RUN_TEST(ShouldMove_RelayDeviceToRelayDevice);
    RUN_TEST(ShouldAssignCopy_RelayDevice);

    RUN_TEST(ShouldConvert_AnalogSensorToAnalogSensor);
    RUN_TEST(ShouldConvert_AnalogSensorToISensor);
    RUN_TEST(ShouldMove_AnalogSensorToAnalogSensor);
    RUN_TEST(ShouldAssignCopy_AnalogSensor);

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
