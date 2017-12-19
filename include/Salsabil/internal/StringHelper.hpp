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

#ifndef SALSABIL_STRINGUTILS_HPP
#define SALSABIL_STRINGUTILS_HPP

#include <string>
#include <vector>
#include <algorithm>

namespace Salsabil {
    namespace Utility {

        // returns a delimiter-separated concatenation of the strings in stringList.

        //        std::string join(const std::vector<std::string>& stringList, std::string delimiter);

        template<class Iter>
        std::string join(Iter first, Iter last, std::string delimiter) {
            std::string result;
            auto it = first;

            if (it != last)
                result.append(*it++);

            for (; it != last; ++it)
                result.append(delimiter).append(*it);
            return result;
        }

        std::string toUpper(std::string s);

        std::string toString(int value);

        std::string toString(long value);

        std::string toString(long long value);

        std::string toString(unsigned value);

        std::string toString(unsigned long value);

        std::string toString(unsigned long long value);

        std::string toString(float value);

        std::string toString(double value);

        std::string toString(long double value);

        std::string toString(const char* value);

        std::string toString(std::string value);

        int countIdenticalCharsFrom(std::size_t pos, const std::string& str);

        int readIntAndAdvancePos(const std::string& str, int& pos, int maxDigitCount = std::numeric_limits<int>::max());
    }
}
#endif // SALSABIL_STRINGUTILS_HPP
