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

#include "doctest.h"
#include "test/mocks/SqlDriverMock.hpp"
#include "Exception.hpp"
#include "SqlDriverFactory.hpp"

using namespace Salsabil;

TEST_CASE("SqlDriverFactory") {

    SUBCASE("ThrowsWhenTryingToRegisterExistentDriver") {
        SqlDriverFactory::registerDriver<SqlDriverMock>("MockDriver");

        CHECK_THROWS_AS(SqlDriverFactory::registerDriver<SqlDriverMock>("MockDriver"), Exception);
    }

    SUBCASE("ThrowsWhenTryingToGetNonExistentDriver") {
        CHECK_THROWS_AS(SqlDriverFactory::getDriver("aDriver"), Exception);
    }
}