// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#if defined(UNIT_TEST) && !defined(ARDUINO)

#include <unity.h>
#include "log/LogTests.h"

void setUp(void)
{
}

void tearDown(void) 
{
}

int main( int argc, char **argv) {
    UNITY_BEGIN();

    /*****************************************
     * Log Tests (LogTests.h)
     *****************************************/
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

    UNITY_END();
}

#endif

#if defined(UNIT_TEST) && defined(ARDUINO)

int main( int argc, char **argv) {
    // STUB
}

#endif