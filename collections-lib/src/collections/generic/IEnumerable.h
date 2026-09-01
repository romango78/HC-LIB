// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
//
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.
#ifndef _HC_LIB_I_ENUMERABLE_H_
#define _HC_LIB_I_ENUMERABLE_H_

#include "IEnumerator.h"

// Exposes an enumerator, which supports a simple iteration over a generic collection.
template <typename T>
class IEnumerable
{
public:
    virtual IEnumerator<T> *getEnumerator() = 0;
};

#endif