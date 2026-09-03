// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.
//
// C++ Templates
// https://isocpp.org/wiki/faq/templates#templates-defn-vs-decl

/// @file Log.h
/// @brief Formatted logger with level filtering and optional timestamps.
#ifndef _HC_LIB_LOG_H_
#define _HC_LIB_LOG_H_

#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include "persisters/ILogPersister.h"
#include "providers/IDateTimeProvider.h"

#if defined(ARDUINO)
#include <Arduino.h>
#else
/// @brief Host stand-in so F() / flash overloads compile in native tests.
class __FlashStringHelper;
#ifndef F
#define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(string_literal))
#endif
#endif

#define LOG_LEVEL_OFF 0 // Logging is disabled
#define LOG_LEVEL_CRITICAL 1 // Indicates the system is unusable, or an error that is unrecoverable
#define LOG_LEVEL_ERROR 2 // Indicates an error condition
#define LOG_LEVEL_WARNING 3 // Indicates a warning condition
#define LOG_LEVEL_INFO 4 // Informational messages
#define LOG_LEVEL_DEBUG 5 // Debug-level messages

#define LOG_LEVEL_CRITICAL_PREFIX "FATAL"
#define LOG_LEVEL_ERROR_PREFIX "ERROR"
#define LOG_LEVEL_WARNING_PREFIX "WARN"
#define LOG_LEVEL_INFO_PREFIX "INFO"
#define LOG_LEVEL_DEBUG_PREFIX "DEBUG"

#ifndef LOG_LEVEL_NAMES
#define LOG_LEVEL_NAMES                                                             \
    {                                                                               \
        "O", LOG_LEVEL_CRITICAL_PREFIX, LOG_LEVEL_ERROR_PREFIX, LOG_LEVEL_WARNING_PREFIX, \
            LOG_LEVEL_INFO_PREFIX, LOG_LEVEL_DEBUG_PREFIX,                          \
    }
#endif

/// @brief Stack buffer for a format string copied from PROGMEM before fctprintf.
#ifndef LOG_FLASH_FMT_MAX
#define LOG_FLASH_FMT_MAX 64
#endif

/// @brief Severity of a log message. Higher values are more verbose.
enum class LogLevel : uint8_t
{
    Off = LOG_LEVEL_OFF,
    Critical = LOG_LEVEL_CRITICAL,
    Error = LOG_LEVEL_ERROR,
    Warn = LOG_LEVEL_WARNING,
    Info = LOG_LEVEL_INFO,
    Debug = LOG_LEVEL_DEBUG
};

/// @brief Optional source name written as [name] in the log line.
/// @note Use LogModule(F("name")) so the name stays in flash on AVR.
///       On AVR, F()/PSTR() is a statement-expression: construct the module
///       inside a function (setup, or a function-local static), not at global scope.
struct LogModule
{
    const char *name;
    bool inFlash;

    constexpr LogModule()
        : name(nullptr), inFlash(false) {};

    constexpr LogModule(const char *t_name)
        : name(t_name), inFlash(false) {};

    LogModule(const __FlashStringHelper *t_name)
        : name(reinterpret_cast<const char *>(t_name)), inFlash(true) {};
};

#define LOG_LEVEL_REQUESTED_DEFAULT LogLevel::Debug

namespace sout {
    #include "printf.h"
}

/// @brief Application-provided minimum log level. Read once in the Log constructor.
LogLevel gLogLevel();

/// @brief Formats and persists log lines through an ILogPersister.
/// @note Log does not own the persister or the datetime provider.
/// @note Prefer F("...") for format strings and LogModule(F("...")). On AVR those
///       stay in flash; tiny printf cannot read PROGMEM, so formats are copied
///       to a LOG_FLASH_FMT_MAX stack buffer.
class Log
{
    private:
        static constexpr LogModule EMPTY_LOG_MODULE {};

        ILogPersister *m_logPersister;
        IDateTimeProvider *m_dateTimeProvider;
        const LogLevel m_logLevelRequested;
        bool m_enabled;

        /// @brief Copies a PROGMEM (or RAM on host) string into _t_dest_.
        void copyFromFlash(char *t_dest, const char *t_src, const size_t t_destSize) const;

        /// @brief Writes a PROGMEM (or RAM on host) string without formatting.
        void writeFlash(const char *t_src);

        /// @brief Writes the timestamp prefix from _t_datetime_.
        void writeDateTime(const DateTime& t_datetime);

        /// @brief Writes the level name and a trailing space.
        void writeLevelPrefix(const LogLevel t_level);

        /// @brief Writes [name] and a trailing space.
        void writeModule(const LogModule& t_module);
    protected:
        template<typename... Args>
        void write(const LogLevel t_level, const LogModule t_module,
            const char* t_format, const bool t_formatInFlash, const Args&... args) noexcept
        {
            if(!m_enabled || !t_format)
            {
                return;
            }
            if(static_cast<uint8_t>(t_level) > static_cast<uint8_t>(m_logLevelRequested))
            {
                return;
            }

            if(m_dateTimeProvider)
            {
                writeDateTime(m_dateTimeProvider->getLocalDatetime());
            }

            writeLevelPrefix(t_level);

            if(t_module.name)
            {
                writeModule(t_module);
            }

            if(t_formatInFlash)
            {
                char fmt[LOG_FLASH_FMT_MAX];
                copyFromFlash(fmt, t_format, sizeof(fmt));
                sout::fctprintf(&Log::writeBounce, this, fmt, args...);
            }
            else
            {
                sout::fctprintf(&Log::writeBounce, this, t_format, args...);
            }
            write('\n');
        };

        void write(const char t_character)
        {
            if(m_logPersister)
            {
                m_logPersister->write(t_character);
            }
        };

        static void writeBounce(char t_character, void* t_thisPtr)
        {
            reinterpret_cast<Log*>(t_thisPtr)->write(t_character);
        };
    public:
        Log() = delete;

        /// @brief Initializes the logger. Does not take ownership of the pointers.
        /// @param t_logPersister Output sink. Logging is disabled when null.
        /// @param t_datetimeProvider Optional timestamp source. Null omits the date prefix.
        Log(ILogPersister *t_logPersister, IDateTimeProvider *t_datetimeProvider)
            : m_logPersister(t_logPersister),
              m_dateTimeProvider(t_datetimeProvider),
              m_logLevelRequested(gLogLevel()),
              m_enabled((t_logPersister != nullptr) && (m_logLevelRequested != LogLevel::Off))
        {
        };

        ~Log() = default;

        template<typename... Args>
        void debug(const char* t_format, const Args&... args) noexcept
        {
            debug(EMPTY_LOG_MODULE, t_format, args...);
        };

        template<typename... Args>
        void debug(const __FlashStringHelper* t_format, const Args&... args) noexcept
        {
            debug(EMPTY_LOG_MODULE, t_format, args...);
        };

        template<typename... Args>
        void debug(const LogModule t_module, const char* t_format, const Args&... args) noexcept
        {
            write(LogLevel::Debug, t_module, t_format, false, args...);
        };

        template<typename... Args>
        void debug(const LogModule t_module, const __FlashStringHelper* t_format, const Args&... args) noexcept
        {
            write(LogLevel::Debug, t_module, reinterpret_cast<const char*>(t_format), true, args...);
        };

        template<typename... Args>
        void info(const char* t_format, const Args&... args) noexcept
        {
            info(EMPTY_LOG_MODULE, t_format, args...);
        };

        template<typename... Args>
        void info(const __FlashStringHelper* t_format, const Args&... args) noexcept
        {
            info(EMPTY_LOG_MODULE, t_format, args...);
        };

        template<typename... Args>
        void info(const LogModule t_module, const char* t_format, const Args&... args) noexcept
        {
            write(LogLevel::Info, t_module, t_format, false, args...);
        };

        template<typename... Args>
        void info(const LogModule t_module, const __FlashStringHelper* t_format, const Args&... args) noexcept
        {
            write(LogLevel::Info, t_module, reinterpret_cast<const char*>(t_format), true, args...);
        };

        template<typename... Args>
        void warn(const char* t_format, const Args&... args) noexcept
        {
            warn(EMPTY_LOG_MODULE, t_format, args...);
        };

        template<typename... Args>
        void warn(const __FlashStringHelper* t_format, const Args&... args) noexcept
        {
            warn(EMPTY_LOG_MODULE, t_format, args...);
        };

        template<typename... Args>
        void warn(const LogModule t_module, const char* t_format, const Args&... args) noexcept
        {
            write(LogLevel::Warn, t_module, t_format, false, args...);
        };

        template<typename... Args>
        void warn(const LogModule t_module, const __FlashStringHelper* t_format, const Args&... args) noexcept
        {
            write(LogLevel::Warn, t_module, reinterpret_cast<const char*>(t_format), true, args...);
        };

        template<typename... Args>
        void error(const char* t_format, const Args&... args) noexcept
        {
            error(EMPTY_LOG_MODULE, t_format, args...);
        };

        template<typename... Args>
        void error(const __FlashStringHelper* t_format, const Args&... args) noexcept
        {
            error(EMPTY_LOG_MODULE, t_format, args...);
        };

        template<typename... Args>
        void error(const LogModule t_module, const char* t_format, const Args&... args) noexcept
        {
            write(LogLevel::Error, t_module, t_format, false, args...);
        };

        template<typename... Args>
        void error(const LogModule t_module, const __FlashStringHelper* t_format, const Args&... args) noexcept
        {
            write(LogLevel::Error, t_module, reinterpret_cast<const char*>(t_format), true, args...);
        };

        template<typename... Args>
        void fatal(const char* t_format, const Args&... args) noexcept
        {
            fatal(EMPTY_LOG_MODULE, t_format, args...);
        };

        template<typename... Args>
        void fatal(const __FlashStringHelper* t_format, const Args&... args) noexcept
        {
            fatal(EMPTY_LOG_MODULE, t_format, args...);
        };

        template<typename... Args>
        void fatal(const LogModule t_module, const char* t_format, const Args&... args) noexcept
        {
            write(LogLevel::Critical, t_module, t_format, false, args...);
        };

        template<typename... Args>
        void fatal(const LogModule t_module, const __FlashStringHelper* t_format, const Args&... args) noexcept
        {
            write(LogLevel::Critical, t_module, reinterpret_cast<const char*>(t_format), true, args...);
        };
};

#endif
