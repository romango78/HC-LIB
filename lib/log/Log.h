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
#ifndef _LOG_H_
#define _LOG_H_

#include "ILogPersister.h"
#include "IDatetimeProvider.h"

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
    {                                                                                     \
        "O", LOG_LEVEL_CRITICAL_PREFIX, LOG_LEVEL_ERROR_PREFIX, LOG_LEVEL_WARNING_PREFIX, \
            LOG_LEVEL_INFO_PREFIX, LOG_LEVEL_DEBUG_PREFIX,                                \
    }
#endif

enum LogLevelEnum
{
    off = LOG_LEVEL_OFF,
    critical = LOG_LEVEL_CRITICAL,
    error = LOG_LEVEL_ERROR,
    warn = LOG_LEVEL_WARNING,
    info = LOG_LEVEL_INFO,
    debug = LOG_LEVEL_DEBUG,
};

struct LogModule
{
    const char *name;
};

/// Whether the logging module is enabled automatically on boot.
#define LOG_ENABLED_DEFAULT true
#define LOG_LEVEL_REQUESTED_DEFAULT LogLevelEnum::debug

#ifdef __cplusplus
extern "C" {
#endif
namespace sout {
    #include "printf.h"
}

LogLevelEnum gLogLevel();

#ifdef __cplusplus
}
#endif

class Log 
{
    private:
        const LogModule EMPTY_LOG_MODULE = { nullptr };
        
        ILogPersister *m_logPersister;
        IDateTimeProvider *m_dateTimeProvider;

        bool m_enabled = LOG_ENABLED_DEFAULT;
        const LogLevelEnum m_logLevelRequested = LOG_LEVEL_REQUESTED_DEFAULT;       

        constexpr const char* getLogLevelName(const LogLevelEnum t_level)
        {
             constexpr const char* level_short_names[] = LOG_LEVEL_NAMES;
             return level_short_names[t_level];
        };
    protected:
        template<typename... Args>
        void write(const LogLevelEnum t_level, const LogModule t_module, 
            const char* t_format, const Args&... args) noexcept
        {
            if(m_enabled && t_level <= m_logLevelRequested)
            {
                if(m_dateTimeProvider)
                {
                    DateTime dt = *m_dateTimeProvider->getLocalDatetime();
                    sout::fctprintf(&Log::writeBounce, this, 
                        "%i-%02i-%02i %02i:%02i:%02i ", 
                        dt.year,
                        dt.month,
                        dt.day,
                        dt.hours,
                        dt.minutes,
                        dt.seconds);
                }

                const char* prefix = this->getLogLevelName(t_level);    
                sout::fctprintf(&Log::writeBounce, this, "%s ", prefix);

                if(t_module.name)
                {
                    sout::fctprintf(&Log::writeBounce, this, "[%s] ", t_module.name);
                }

                sout::fctprintf(&Log::writeBounce, this, t_format, args...);
                // Add new line control symbol
                sout::fctprintf(&Log::writeBounce, this, "\n");
            }
        };
        void write(const char t_character)
        {
            m_logPersister->write(t_character);    
        };

        static void writeBounce(char t_character, void* t_thisPtr)
        {   
            reinterpret_cast<Log*>(t_thisPtr)->write(t_character);
        };         
    public:
        Log(ILogPersister *t_logPersister, IDateTimeProvider *t_datetimeProvider)
            : m_logLevelRequested(gLogLevel())
        {
            m_logPersister = t_logPersister;
            m_dateTimeProvider = t_datetimeProvider;         
            m_enabled = (m_logPersister != nullptr) 
                && (m_logLevelRequested != LogLevelEnum::off);
        };

        template<typename... Args>
        void debug(const char* t_format, const Args&... args) noexcept
        {
            debug(EMPTY_LOG_MODULE, t_format, args...);
        };

        template<typename... Args>
        void debug(const LogModule t_module, const char* t_format, const Args&... args) noexcept
        {
            write(LogLevelEnum::debug, t_module, t_format, args...);
        };

        template<typename... Args>
        void info(const char* t_format, const Args&... args) noexcept
        {
            info(EMPTY_LOG_MODULE, t_format, args...);
        };

        template<typename... Args>
        void info(const LogModule t_module, const char* t_format, const Args&... args) noexcept
        {
            write(LogLevelEnum::info, t_module, t_format, args...);
        };

        template<typename... Args>
        void warn(const char* t_format, const Args&... args) noexcept
        {
            warn(EMPTY_LOG_MODULE, t_format, args...);
        };

        template<typename... Args>
        void warn(const LogModule t_module, const char* t_format, const Args&... args) noexcept
        {
            write(LogLevelEnum::warn, t_module, t_format, args...);
        };

        template<typename... Args>
        void error(const char* t_format, const Args&... args) noexcept
        {
            error(EMPTY_LOG_MODULE, t_format, args...);
        };

        template<typename... Args>
        void error(const LogModule t_module, const char* t_format, const Args&... args) noexcept
        {
            write(LogLevelEnum::error, t_module, t_format, args...);
        };

        template<typename... Args>
        void fatal(const char* t_format, const Args&... args) noexcept
        {
            fatal(EMPTY_LOG_MODULE, t_format, args...);
        };

        template<typename... Args>
        void fatal(const LogModule t_module, const char* t_format, const Args&... args) noexcept
        {
            write(LogLevelEnum::critical, t_module, t_format, args...);
        };
};

#endif