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

#include "internal/StringHelper.hpp"

using namespace Salsabil;

std::string Utility::toUpper(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
            [](unsigned char c) {
                return std::toupper(c);
            }
    );
    return s;
}

std::string Utility::toString(int value) {
    return std::to_string(value);
}

std::string Utility::toString(long value) {
    return std::to_string(value);
}

std::string Utility::toString(long long value) {
    return std::to_string(value);
}

std::string Utility::toString(unsigned value) {
    return std::to_string(value);
}

std::string Utility::toString(unsigned long value) {
    return std::to_string(value);
}

std::string Utility::toString(unsigned long long value) {
    return std::to_string(value);
}

inline static std::string& trailingZerosTrimmed(std::string str) {
    return str.erase(str.find_last_not_of('0') + 1, std::string::npos);

}

std::string Utility::toString(float value) {
    return trailingZerosTrimmed(std::to_string(value));
}

std::string Utility::toString(double value) {
    return trailingZerosTrimmed(std::to_string(value));
}

std::string Utility::toString(long double value) {
    return trailingZerosTrimmed(std::to_string(value));
}

std::string Utility::toString(const char* value) {
    return "'" + std::string(value) + "'";
}

std::string Utility::toString(std::string value) {
    return value;
}

int Utility::countIdenticalCharsFrom(std::size_t pos, const std::string& str) {
    int idx = pos + 1;

    while (idx < str.size() && str[idx] == str[pos])
        ++idx;

    return idx - pos;
}

int Utility::readIntAndAdvancePos(const std::string& str, int& pos, int maxDigitCount) {
    std::string intStr;

    while (intStr.size() < maxDigitCount && pos < str.size() && std::isdigit(str[pos])) {
        intStr += str[pos];
        ++pos;
    }

    return std::stoi(intStr);
}