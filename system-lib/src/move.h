// Copyright (C) 2007-2018 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

#ifndef MOVE_H
#define MOVE_H

#ifndef ARDUINO
#include <bits/move.h>
#else

#include "type_traits.h"

namespace std 
{

    /**
     *  @brief  Convert a value to an rvalue.
     *  @param  __t  A thing of arbitrary type.
     *  @return The parameter cast to an rvalue-reference to allow moving it.
     */
    template<typename _Tp>
        constexpr typename std::remove_reference<_Tp>::type&&
        move(_Tp&& __t) noexcept
        { return static_cast<typename std::remove_reference<_Tp>::type&&>(__t); }

}
#endif

#endif