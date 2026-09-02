// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _HC_LIB_LOG_TESTS_H_
#define _HC_LIB_LOG_TESTS_H_

#ifdef UNIT_TEST

#include <unity.h>
#if !defined(ARDUINO)
    #include <cstring>
#else
    #include <Arduino.h>
#endif

#include "FakeDateTimeProvider.h"
#include "FakeLogPersister.h"
#include "log/Log.h"

#ifdef __cplusplus
extern "C" {
#endif
namespace sout {
    #include "printf.h"
    #include "printf.c"
}
#ifdef __cplusplus
}
#endif

#ifndef SOUT_BUFFER_SIZE
#define SOUT_BUFFER_SIZE 128U
#endif

LogModule module = { "Unit Test Module" };

LogLevel gLogLevel()
{
    return LogLevel::Debug;
};

static char ms_outputBuffer[SOUT_BUFFER_SIZE];
static size_t ms_outputBufferPointer = 0U;

void sout::_putchar(char character)
{
    ms_outputBuffer[ms_outputBufferPointer++] = character;
};

void ResetOutputBuffer()
{
    ms_outputBufferPointer = 0U;
    memset(ms_outputBuffer, 0x0, SOUT_BUFFER_SIZE);
};

void Log_ShouldLogDebug_WithoutModule()
{
    char expectedValue[] = "1900-01-01 00:00:00 DEBUG Test message.\n";
    char testMsg[] = "Test message.";

    ResetOutputBuffer();
    FakeDateTimeProvider provider;
    FakeLogPersister persister;
    Log logger(&persister, &provider);

    logger.debug(testMsg);

    TEST_ASSERT_EQUAL_STRING(expectedValue, ms_outputBuffer);
}

void Log_ShouldLogDebug_WithModule()
{
    char expectedValue[] = "1900-01-01 00:00:00 DEBUG [Unit Test Module] Test message.\n";
    char testMsg[] = "Test message.";

    ResetOutputBuffer();
    FakeDateTimeProvider provider;
    FakeLogPersister persister;
    Log logger(&persister, &provider);

    logger.debug(module, testMsg);

    TEST_ASSERT_EQUAL_STRING(expectedValue, ms_outputBuffer);
}

void Log_ShouldLogInfo_WithoutModule()
{
    char expectedValue[] = "1900-01-01 00:00:00 INFO Test message.\n";
    char testMsg[] = "Test message.";

    ResetOutputBuffer();
    FakeDateTimeProvider provider;
    FakeLogPersister persister;
    Log logger(&persister, &provider);

    logger.info(testMsg);

    TEST_ASSERT_EQUAL_STRING(expectedValue, ms_outputBuffer);
}

void Log_ShouldLogInfo_WithModule()
{
    char expectedValue[] = "1900-01-01 00:00:00 INFO [Unit Test Module] Test message.\n";
    char testMsg[] = "Test message.";

    ResetOutputBuffer();
    FakeDateTimeProvider provider;
    FakeLogPersister persister;
    Log logger(&persister, &provider);

    logger.info(module, testMsg);

    TEST_ASSERT_EQUAL_STRING(expectedValue, ms_outputBuffer);
}

void Log_ShouldLogWarning_WithoutModule()
{
    char expectedValue[] = "1900-01-01 00:00:00 WARN Test message.\n";
    char testMsg[] = "Test message.";

    ResetOutputBuffer();
    FakeDateTimeProvider provider;
    FakeLogPersister persister;
    Log logger(&persister, &provider);

    logger.warn(testMsg);

    TEST_ASSERT_EQUAL_STRING(expectedValue, ms_outputBuffer);
}

void Log_ShouldLogWarning_WithModule()
{
    char expectedValue[] = "1900-01-01 00:00:00 WARN [Unit Test Module] Test message.\n";
    char testMsg[] = "Test message.";

    ResetOutputBuffer();
    FakeDateTimeProvider provider;
    FakeLogPersister persister;
    Log logger(&persister, &provider);

    logger.warn(module, testMsg);

    TEST_ASSERT_EQUAL_STRING(expectedValue, ms_outputBuffer);
}

void Log_ShouldLogError_WithoutModule()
{
    char expectedValue[] = "1900-01-01 00:00:00 ERROR Test message.\n";
    char testMsg[] = "Test message.";

    ResetOutputBuffer();
    FakeDateTimeProvider provider;
    FakeLogPersister persister;
    Log logger(&persister, &provider);

    logger.error(testMsg);

    TEST_ASSERT_EQUAL_STRING(expectedValue, ms_outputBuffer);
}

void Log_ShouldLogError_WithModule()
{
    char expectedValue[] = "1900-01-01 00:00:00 ERROR [Unit Test Module] Test message.\n";
    char testMsg[] = "Test message.";

    ResetOutputBuffer();
    FakeDateTimeProvider provider;
    FakeLogPersister persister;
    Log logger(&persister, &provider);

    logger.error(module, testMsg);

    TEST_ASSERT_EQUAL_STRING(expectedValue, ms_outputBuffer);
}

void Log_ShouldLogFatal_WithoutModule()
{
    char expectedValue[] = "1900-01-01 00:00:00 FATAL Test message.\n";
    char testMsg[] = "Test message.";

    ResetOutputBuffer();
    FakeDateTimeProvider provider;
    FakeLogPersister persister;
    Log logger(&persister, &provider);

    logger.fatal(testMsg);

    TEST_ASSERT_EQUAL_STRING(expectedValue, ms_outputBuffer);
}

void Log_ShouldLogFatal_WithModule()
{
    char expectedValue[] = "1900-01-01 00:00:00 FATAL [Unit Test Module] Test message.\n";
    char testMsg[] = "Test message.";

    ResetOutputBuffer();
    FakeDateTimeProvider provider;
    FakeLogPersister persister;
    Log logger(&persister, &provider);

    logger.fatal(module, testMsg);

    TEST_ASSERT_EQUAL_STRING(expectedValue, ms_outputBuffer);
}

void Log_ShouldNotLog_WhenPersisterIsNull()
{
    ResetOutputBuffer();
    FakeDateTimeProvider provider;
    Log logger(nullptr, &provider);

    logger.info("Test message.");

    TEST_ASSERT_EQUAL_STRING("", ms_outputBuffer);
}

void Log_ShouldLog_WithoutDateTime()
{
    char expectedValue[] = "INFO Test message.\n";

    ResetOutputBuffer();
    FakeLogPersister persister;
    Log logger(&persister, nullptr);

    logger.info("Test message.");

    TEST_ASSERT_EQUAL_STRING(expectedValue, ms_outputBuffer);
}

void Log_ShouldLog_WithFormatArguments()
{
    char expectedValue[] = "1900-01-01 00:00:00 INFO Value 42.\n";

    ResetOutputBuffer();
    FakeDateTimeProvider provider;
    FakeLogPersister persister;
    Log logger(&persister, &provider);

    logger.info("Value %d.", 42);

    TEST_ASSERT_EQUAL_STRING(expectedValue, ms_outputBuffer);
}

#endif
#endif
