// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#include "log/LogManager.h"

LogModule module = { "TestModule" };

int main()
{
    /********************************
     * Setup Dependencies
     ********************************/

    /********************************
     * MAIN
     ********************************/    
    char t[] = "Test";

    Log *logger = LogManager::getLogger();
    logger->info("The '%s' message is logged.", t);
    logger->info(t);
    logger->info(module, "The '%s' message is logged.", t);
    logger->info(module, t);
};