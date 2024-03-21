// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#if defined(UNIT_TEST) 

#include <unity.h>
#include "ZMPT101BRmsReaderTests.h"
#include "ZMPT101BTrueRmsReaderTests.h"

void dummyTest()
{
    TEST_ASSERT_EQUAL(2, 1+1);
}

void testsProcess() 
{
    UNITY_BEGIN();

    /*****************************************
     * ZMPT101BRmsReader
     *****************************************/
    RUN_TEST(ZMPT101BRmsReader_Raise_Error_When_TimerIsNotInitialized);
    RUN_TEST(ZMPT101BRmsReader_Raise_Error_When_StreamIsNotInitialized);
    RUN_TEST(ZMPT101BRmsReader_Read_Data_And_Calculate_Rms);

    /*****************************************
     * ZMPT101BTrueRmsReader
     *****************************************/
    RUN_TEST(ZMPT101BTrueRmsReader_Raise_Error_When_TimerIsNotInitialized);
    RUN_TEST(ZMPT101BTrueRmsReader_Raise_Error_When_StreamIsNotInitialized);
    RUN_TEST(ZMPT101BTrueRmsReader_Read_Data_And_Calculate_TrueRms);

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