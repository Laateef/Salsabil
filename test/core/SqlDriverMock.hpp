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

#ifndef SALSABIL_SQLDRIVERMOCK_HPP
#define SALSABIL_SQLDRIVERMOCK_HPP

#include <stdint.h>

#include "gmock/gmock.h"
#include "SqlDriver.hpp"

using namespace ::testing;
using namespace Salsabil;

class SqlDriverMock : public SqlDriver {
public:
    MOCK_CONST_METHOD0(driverName, std::string(void));
    MOCK_CONST_METHOD0(create, SqlDriver*(void));
    MOCK_METHOD1(open, void(const std::string&));
    MOCK_CONST_METHOD0(isOpen, bool(void));
    MOCK_METHOD0(close, void(void));
    MOCK_METHOD1(prepare, void(const std::string&));
    MOCK_METHOD0(execute, void(void));
    MOCK_METHOD1(execute, void(const std::string&));
    MOCK_METHOD0(nextRow, bool(void));
    MOCK_CONST_METHOD1(isNull, bool(int));
    MOCK_CONST_METHOD1(getInt, int(int));
    MOCK_CONST_METHOD1(getInt64, int64_t(int));
    MOCK_CONST_METHOD1(getFloat, float(int));
    MOCK_CONST_METHOD1(getDouble, double(int));
    MOCK_CONST_METHOD1(getRawString, const unsigned char*(int));
    MOCK_CONST_METHOD1(getCString, const char*(int));
    MOCK_CONST_METHOD1(getStdString, std::string(int));
    MOCK_CONST_METHOD1(getSize, std::size_t(int));
    MOCK_CONST_METHOD1(getBlob, const void*(int));
    MOCK_CONST_METHOD1(bindNull, void(int));
    MOCK_CONST_METHOD2(bindInt, void(int, int));
    MOCK_CONST_METHOD2(bindInt64, void(int, int64_t));
    MOCK_CONST_METHOD2(bindDouble, void(int, double));
    MOCK_CONST_METHOD2(bindCString, void(int, const char*));
    MOCK_CONST_METHOD2(bindStdString, void(int, const std::string&));
    MOCK_CONST_METHOD3(bindBlob, void(int, const void*, std::size_t));
    MOCK_METHOD0(tableList, std::vector<std::string>(void));
    MOCK_METHOD1(columnList, std::vector<std::string>(const std::string&));
};

#endif // SALSABIL_SQLDRIVERMOCK_HPP