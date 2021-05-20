// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _FAKE_LOG_PERSISTER_H_
#define _FAKE_LOG_PERSISTER_H_

#include "ILogPersister.h"

class FakeLogPersister : ILogPersister
{
    public:
        FakeLogPersister() {};
        void write(const char t_character) override;
};

#endif