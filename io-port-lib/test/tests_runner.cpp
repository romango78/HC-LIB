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
#include "AnalogStreamTests.h"
#include "DigitalStreamTests.h"
#include "VoltageStreamTests.h"
#include "PortAdapterTests.h"

void testsProcess()
{
    UNITY_BEGIN();

    RUN_TEST(AnalogStream_ShouldRaiseError_WhenAdaptorIsNotSet);
    RUN_TEST(AnalogStream_ShouldReadData_WhenStreamIsOpenForRead);
    RUN_TEST(AnalogStream_ShouldBeInReadMode_WhenStreamIsOpenForRead);
    RUN_TEST(AnalogStream_ShouldBeInUndefinedMode_WhenStreamIsClosed);
    RUN_TEST(AnalogStream_ShouldRaiseError_WhenTryRead_And_StreamIsNotOpenForRead);
    RUN_TEST(AnalogStream_ShouldWriteData_WhenStreamIsOpenForWrite);
    RUN_TEST(AnalogStream_ShouldBeInWriteMode_WhenStreamIsOpenForWrite);
    RUN_TEST(AnalogStream_ShouldBeInSpecificMode_WhenStreamIsOpenedSeveralTimes);
    RUN_TEST(AnalogStream_ShouldRaiseError_WhenTryWrite_And_StreamIsNotOpenForWrite);
    RUN_TEST(AnalogStream_ShouldRaiseError_WhenBegin_AndAdaptorIsNotSet);
    RUN_TEST(AnalogStream_ShouldGetState_WhenAdaptorIsSet);
    RUN_TEST(AnalogStream_ShouldRaiseError_WhenGetState_AndAdaptorIsNotSet);
    RUN_TEST(AnalogStream_ShouldClone_WhenAdaptorIsSet);
    RUN_TEST(AnalogStream_ShouldClone_WhenAdaptorIsNotSet);

    RUN_TEST(DigitalStream_ShouldRaiseError_WhenAdaptorIsNotSet);
    RUN_TEST(DigitalStream_ShouldReadData_WhenStreamIsOpenForRead);
    RUN_TEST(DigitalStream_ShouldBeInReadMode_WhenStreamIsOpenForRead);
    RUN_TEST(DigitalStream_ShouldBeInUndefinedMode_WhenStreamIsClosed);
    RUN_TEST(DigitalStream_ShouldRaiseError_WhenTryRead_And_StreamIsNotOpenForRead);
    RUN_TEST(DigitalStream_ShouldWriteData_WhenStreamIsOpenForWrite);
    RUN_TEST(DigitalStream_ShouldBeInWriteMode_WhenStreamIsOpenForWrite);
    RUN_TEST(DigitalStream_ShouldBeInSpecificMode_WhenStreamIsOpenedSeveralTimes);
    RUN_TEST(DigitalStream_ShouldRaiseError_WhenTryWrite_And_StreamIsNotOpenForWrite);
    RUN_TEST(DigitalStream_ShouldRaiseError_WhenBegin_AndAdaptorIsNotSet);
    RUN_TEST(DigitalStream_ShouldGetState_WhenAdaptorIsSet);
    RUN_TEST(DigitalStream_ShouldRaiseError_WhenGetState_AndAdaptorIsNotSet);
    RUN_TEST(DigitalStream_ShouldClone_WhenAdaptorIsSet);
    RUN_TEST(DigitalStream_ShouldClone_WhenAdaptorIsNotSet);

    RUN_TEST(VoltageStream_ShouldReadVoltage);
    RUN_TEST(VoltageStream_ShouldSetPWM);
    RUN_TEST(VoltageStream_ShouldClampPwm_WhenPercentageIsBelowZero);
    RUN_TEST(VoltageStream_ShouldClampPwm_WhenPercentageIsAbove100);
    RUN_TEST(VoltageStream_ShouldClone);

    RUN_TEST(AnalogPortAdapter_ShouldReturnNoData_WhenNotOnArduino);
    RUN_TEST(AnalogPortAdapter_ShouldClone);
    RUN_TEST(DigitalPortAdapter_ShouldReturnNoData_WhenNotOnArduino);
    RUN_TEST(DigitalPortAdapter_ShouldClone);

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
