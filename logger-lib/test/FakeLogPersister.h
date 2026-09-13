// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _HC_LIB_FAKE_LOG_PERSISTER_H_
#define _HC_LIB_FAKE_LOG_PERSISTER_H_

#include "log/persisters/ILogPersister.h"

#include <stdarg.h>
#include <stddef.h>
namespace sout {
    #include "printf.h"
}

class FakeLogPersister : public ILogPersister
{
    public:
        FakeLogPersister() = default;

        void write(const char t_character) override
        {
            sout::_putchar(t_character);
        };
};

#endif
