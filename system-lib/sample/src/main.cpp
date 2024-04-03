// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.
#if defined(ARDUINO)

#include <Arduino.h>

void setup() 
{
    // put your setup code here, to run once:
}

void loop() 
{
    // put your main code here, to run repeatedly:
    delay(100);
}

#endif

#if !defined(ARDUINO)

int main( int argc, char **argv) 
{    
  return 0;
};

#endif