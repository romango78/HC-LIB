// Copyright (c) 2021 Roman Gorielov. All Rights Reserved.
// 
// This software is the confidential and proprietary information of Roman Gorielov.
// It is furnished under license and may only be used or copied in accordance
// with the terms of such license.
// This software is subject to change without notice and no information
// contained in it should be construed as commitment by Roman Gorielov.

#ifndef _KEY_VALUE_PAIR_H_
#define _KEY_VALUE_PAIR_H_

template<class TKey, class TValue>
class KeyValuePair
{
    private:
        TKey m_key;
        TValue m_value;
    public:        
        KeyValuePair(const TKey t_key, const TValue t_value)
        {
            m_key = t_key;
            m_value = t_value;
        };

        ~KeyValuePair()
        {

        };

        TKey& getKey()
        {
            return m_key;
        };

        TValue& getValue()
        {
            return m_value;
        };
};

#endif