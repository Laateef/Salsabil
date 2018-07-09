/*
 * Copyright (C) 2017-2018, Abdullatif Kalla. All rights reserved.
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

#ifndef SALSABIL_LOGGING_HPP
#define SALSABIL_LOGGING_HPP

#include <iostream>

#if defined(_MSC_VER)  // Visual C++
#define CURRENT_FUNCTION_SIGNATURE __FUNCSIG__
#elif defined(__GNUC__) || defined(__MINGW32__) || defined(__MINGW64__) || defined(__INTEL_COMPILER) || defined(__clang__)
#define CURRENT_FUNCTION_SIGNATURE __PRETTY_FUNCTION__
#else // Unknown
#if defined(__func__)
#define CURRENT_FUNCTION_SIGNATURE __func__
#else
#define CURRENT_FUNCTION_SIGNATURE ""
#endif
#endif

#define SALSABIL_ENABLE_LOG_DEBUG 0
#define SALSABIL_ENABLE_LOG_INFO  1

#if SALSABIL_ENABLE_LOG_DEBUG
#define SALSABIL_LOG_DEBUG(ARG) std::cout << "\nSalsabil [debug] file: '" << __FILE__ << "', line: '"<< __LINE__ << "', function: \n" << CURRENT_FUNCTION_SIGNATURE << "\n"<< ARG << "\n";
#else
#define SALSABIL_LOG_DEBUG(ARG)
#endif

#if SALSABIL_ENABLE_LOG_INFO
#define SALSABIL_LOG_INFO(ARG) std::cout << "\nSalsabil [info]: " << ARG << "\n";
#else
#define SALSABIL_LOG_INFO(ARG)
#endif

#endif // SALSABIL_LOGGING_HPP

