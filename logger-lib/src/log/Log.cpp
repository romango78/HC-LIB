// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.
//
// This file is part of the Logger Library v1.0.1
//
// C++ Templates
// https://isocpp.org/wiki/faq/templates#templates-defn-vs-decl

#include "Log.h"

#ifdef __cplusplus
extern "C" {
#endif
namespace sout {
    #include "printf.h"
}

#ifdef __cplusplus
}
#endif

constexpr const char* Log::getLogLevelName(const LogLevelEnum t_level)
{
        constexpr const char* level_short_names[] = LOG_LEVEL_NAMES;
        return level_short_names[t_level];
};

template<typename... Args>
void Log::write(const LogLevelEnum t_level, const LogModule t_module, 
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

void Log::write(const char t_character)
{
    m_logPersister->write(t_character);    
};

static void Log::writeBounce(char t_character, void* t_thisPtr)
{   
    reinterpret_cast<Log*>(t_thisPtr)->write(t_character);
};         

template<typename... Args>
void Log::debug(const char* t_format, const Args&... args) noexcept
{
    debug(EMPTY_LOG_MODULE, t_format, args...);
};

template<typename... Args>
void Log::debug(const LogModule t_module, const char* t_format, const Args&... args) noexcept
{
    write(LogLevelEnum::debug, t_module, t_format, args...);
};

template<typename... Args>
void Log::info(const char* t_format, const Args&... args) noexcept
{
    info(EMPTY_LOG_MODULE, t_format, args...);
};

template<typename... Args>
void Log::info(const LogModule t_module, const char* t_format, const Args&... args) noexcept
{
    write(LogLevelEnum::info, t_module, t_format, args...);
};

template<typename... Args>
void Log::warn(const char* t_format, const Args&... args) noexcept
{
    warn(EMPTY_LOG_MODULE, t_format, args...);
};

template<typename... Args>
void Log::warn(const LogModule t_module, const char* t_format, const Args&... args) noexcept
{
    write(LogLevelEnum::warn, t_module, t_format, args...);
};

template<typename... Args>
void Log::error(const char* t_format, const Args&... args) noexcept
{
    error(EMPTY_LOG_MODULE, t_format, args...);
};

template<typename... Args>
void Log::error(const LogModule t_module, const char* t_format, const Args&... args) noexcept
{
    write(LogLevelEnum::error, t_module, t_format, args...);
};

template<typename... Args>
void Log::fatal(const char* t_format, const Args&... args) noexcept
{
    fatal(EMPTY_LOG_MODULE, t_format, args...);
};

template<typename... Args>
void Log::fatal(const LogModule t_module, const char* t_format, const Args&... args) noexcept
{
    write(LogLevelEnum::critical, t_module, t_format, args...);
};