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

#include "gmock/gmock.h"
#include "internal/StringHelper.hpp"

using namespace ::testing;
using namespace Salsabil;

TEST(JoinVectorOfStringIntoStringFunction, returnsEmptyStringForEmptyVector) {
    std::vector<std::string> vs;

    ASSERT_THAT(Utility::join(vs, " "), Eq(""));
}

TEST(JoinVectorOfStringIntoStringFunction, returnsTheSameStringForOneStringVector) {
    std::vector<std::string> vs;

    vs.push_back("A String");

    ASSERT_THAT(Utility::join(vs, " "), Eq("A String"));
}

TEST(JoinVectorOfStringIntoStringFunction, returnsDelimiterConcatenatedStringForMultipleStringVector) {
    std::vector<std::string> vs;

    vs.push_back("A String");
    vs.push_back("Another String");
    vs.push_back("One More String");

    ASSERT_THAT(Utility::join(vs, "-"), Eq("A String-Another String-One More String"));
}