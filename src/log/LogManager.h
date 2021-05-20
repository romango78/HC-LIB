// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _LOG_MANAGER_H_
#define _LOG_MANAGER_H_

#include "LogPersister.h"
#include "../providers/DateTimeProvider.h"
#include "Log.h"

class LogManager
{
    private:
        static Log *m_log;
    public:
        static Log* getLogger();
};

Log* LogManager::m_log = nullptr;

Log* LogManager::getLogger()
{
    if(m_log == nullptr)
    {
        m_log = new Log(
                    reinterpret_cast<ILogPersister*>(new LogPersister()), 
                    reinterpret_cast<IDateTimeProvider*>(new DateTimeProvider()));
    }
    return m_log;
}

#endif