// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "FakeLogPersister.h"

#ifdef __cplusplus
extern "C" {
#endif
namespace sout {
    #include "printf.h"
}
#ifdef __cplusplus
}
#endif

void FakeLogPersister::write(const char t_character)
{
    sout::_putchar(t_character);
};
