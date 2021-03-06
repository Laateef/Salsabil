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

#ifndef SALSABIL_SQLVALUE_HPP
#define SALSABIL_SQLVALUE_HPP

#include <string>
#include "StringHelper.hpp"

namespace Salsabil {

    class SqlValue {
        std::string mValue;
    public:

        SqlValue(int value) : mValue(Utility::toSqlString(value)) {
        }

        SqlValue(float value) : mValue(Utility::toSqlString(value)) {
        }

        SqlValue(const char* value) : mValue(Utility::toSqlString(value)) {
        }

        SqlValue(std::string value) : mValue(Utility::toSqlString(value)) {
        }

        std::string toString() const {
            return mValue;
        }
    };

}

#endif // SALSABIL_SQLVALUE_HPP 
