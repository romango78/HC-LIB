// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _LOG_PERSISTER_H_
#define _LOG_PERSISTER_H_

#include <iostream>
#include "log/ILogPersister.h"

class LogPersister : ILogPersister
{
    public:
        LogPersister() {};
        void write(const char t_character) override
        {
          std::cout << t_character;
        };
};

#endif