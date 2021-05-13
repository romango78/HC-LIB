// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _KEY_VALUE_PAIR_TESTS_H_
#define _KEY_VALUE_PAIR_TESTS_H_

#include "generic/KeyValuePair.h"

void KeyValuePair_ShouldCreate_RefKey_RefValue()
{
    char key[] = "Key";
    char value[] = "Value";
    KeyValuePair<char*, char*> *sut = new KeyValuePair<char*, char*>(key, value);


}


#endif