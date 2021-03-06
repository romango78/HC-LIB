// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#if defined(UNIT_TEST) 

#include <unity.h>

#include "AnalogStreamTests.h"
#include "DigitalStreamTests.h"
#include "VoltageStreamTests.h"

void dummyTest()
{
    TEST_ASSERT_EQUAL(2, 1+1);
}

void testsProcess() 
{
    UNITY_BEGIN();

    /*****************************************
     * Analog Stream Tests (AnalogStreamTests.h)
     *****************************************/
    RUN_TEST(AnalogStream_ShouldRaiseError_WhenAdaptorIsNotSet);
    RUN_TEST(AnalogStream_ShouldReadData_WhenStreamIsOpenForRead);
    RUN_TEST(AnalogStream_ShouldBeInReadMode_WhenStreamIsOpenForRead);
    RUN_TEST(AnalogStream_ShouldBeInUndefinedMode_WhenStreamIsClosed);
    RUN_TEST(AnalogStream_ShouldRaiseError_WhenTryRead_And_StreamIsNotOpenForRead);
    RUN_TEST(AnalogStream_ShouldWriteData_WhenStreamIsOpenForWrite);
    RUN_TEST(AnalogStream_ShouldBeInWriteMode_WhenStreamIsOpenForWrite);
    RUN_TEST(AnalogStream_ShouldBeInSpecificMode_WhenStreamIsOpenedSeveralTimes);
    RUN_TEST(AnalogStream_ShouldRaiseError_WhenTryWrite_And_StreamIsNotOpenForWrite);

    /*****************************************
     * Digital Stream Tests (DigitalStreamTests.h)
     *****************************************/
    RUN_TEST(DigitalStream_ShouldRaiseError_WhenAdaptorIsNotSet);
    RUN_TEST(DigitalStream_ShouldReadData_WhenStreamIsOpenForRead);
    RUN_TEST(DigitalStream_ShouldBeInReadMode_WhenStreamIsOpenForRead);
    RUN_TEST(DigitalStream_ShouldBeInUndefinedMode_WhenStreamIsClosed);
    RUN_TEST(DigitalStream_ShouldRaiseError_WhenTryRead_And_StreamIsNotOpenForRead);
    RUN_TEST(DigitalStream_ShouldWriteData_WhenStreamIsOpenForWrite);
    RUN_TEST(DigitalStream_ShouldBeInWriteMode_WhenStreamIsOpenForWrite);
    RUN_TEST(DigitalStream_ShouldBeInSpecificMode_WhenStreamIsOpenedSeveralTimes);
    RUN_TEST(DigitalStream_ShouldRaiseError_WhenTryWrite_And_StreamIsNotOpenForWrite);

    /*****************************************
     * Voltage Stream Tests (VoltageStreamTests.h)
     *****************************************/
    RUN_TEST(VoltageStream_ShouldReadVoltage);
    RUN_TEST(VoltageStream_ShouldSetPWM);

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