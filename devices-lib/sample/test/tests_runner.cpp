// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#if defined(UNIT_TEST) 

#include <unity.h>
#include "DigitalDeviceControllerTests.h"
#include "RelayDeviceControllerTests.h"
#include "DeviceTests.h"
#include "SensorTests.h"

void dummyTest()
{
    TEST_ASSERT_EQUAL(2, 1+1);
}

void testsProcess() 
{
    UNITY_BEGIN();

    /*****************************************
     * DigitalDeviceController Tests
     *****************************************/
    RUN_TEST(Should_SetState_WhenDeviceIsInitialized);
    RUN_TEST(Should_RaiseError_IfSetState_WhenStreamIsNotSet);
    RUN_TEST(Should_GetState_WhenDeviceIsInitialized);
    RUN_TEST(Should_RaiseError_IfGetState_WhenStreamIsNotSet);

    /*****************************************
     * RelayDeviceController Tests
     *****************************************/
    RUN_TEST(ShouldSwitchRepayInOnStatus);
    RUN_TEST(Should_RaiseError_WhenTryOnRelay_AndStreamIsNotSet);
    RUN_TEST(ShouldSwitchRepayInOffStatus);
    RUN_TEST(Should_RaiseError_WhenTryOffRelay_AndStreamIsNotSet);
    RUN_TEST(ShouldGetRelayState);
    RUN_TEST(Should_RaiseError_WhenTryGetRelayState_AndStreamIsNotSet);

    /*****************************************
     * Device Tests
     *****************************************/
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

    /*****************************************
     * Sensor Tests
     *****************************************/    
    RUN_TEST(ShouldConvert_AnalogSensorToAnalogSensor);
    RUN_TEST(ShouldConvert_AnalogSensorToISensor);
    RUN_TEST(ShouldMove_AnalogSensorToAnalogSensor);
    RUN_TEST(ShouldAssignCopy_AnalogSensor);

    UNITY_END();
};

#if !defined(ARDUINO)

void setUp(void)
{
    // STUB
};

void tearDown(void) 
{
    // STUB
};

int main( int argc, char **argv) 
{
    testsProcess();
    return 0;
};

#endif

#if defined(ARDUINO)

#include <Arduino.h>

void setUp(void)
{
    // STUB
};

void tearDown(void) 
{
    // STUB
};

void setup() {
    // Setup device
    Serial.begin(115200);

    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
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