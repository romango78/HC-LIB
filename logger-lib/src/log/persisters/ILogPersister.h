// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

/// @file ILogPersister.h
/// @brief Character sink for formatted log output.
#ifndef _HC_LIB_I_LOG_PERSISTER_H_
#define _HC_LIB_I_LOG_PERSISTER_H_

/// @brief Writes log characters to an output target.
class ILogPersister
{
    public:
        /// @brief Destructor.
        virtual ~ILogPersister() = default;

        /// @brief Writes one character to the output target.
        /// @param t_character The character to persist.
        virtual void write(const char t_character) = 0;
};

#endif
