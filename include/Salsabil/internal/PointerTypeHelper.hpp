/*
 * Copyright (C) 2017, Abdullatif Kalla. All rights reserved.
 * E-mail: laateef@outlook.com
 * Github: https://github.com/Laateef/Salsabil
 *
 * This file is part of the Salsabil project.
 * 
 * Salsabil is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation, either version 3 of the License, or 
 * (at your option) any later version.
 * 
 * Salsabil is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Salsabil. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SALSABIL_POINTERTYPEHELPER_HPP
#define SALSABIL_POINTERTYPEHELPER_HPP

#include <tuple>

namespace Salsabil {
    namespace Utility {

        template<class F>
        struct Traits;

        // function pointer

        template<class R, class... Args>
        struct Traits<R(*)(Args...)> : public Traits<R(Args...)> {
        };

        template<class R, class... Args>
        struct Traits<R(Args...)> {
            using ReturnType = R;

            static constexpr std::size_t arity = sizeof...(Args);

            template <std::size_t N>
            struct argument {
                static_assert(N < arity, "error: invalid parameter index.");
                using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
            };
        };

        // pointer to member function

        template<class C, class R, class... Args>
        struct Traits<R(C::*)(Args...)> : public Traits<R(C&, Args...)> {
        };

        // pointer to const member function

        template<class C, class R, class... Args>
        struct Traits<R(C::*)(Args...) const> : public Traits<R(C&, Args...)> {
        };

        // pointer to member object

        template<class C, class R>
        struct Traits<R(C::*)> : public Traits<R(C&)> {
            using AttributeType = R;
        };


    }
}

#endif // SALSABIL_POINTERTYPEHELPER_HPP 
