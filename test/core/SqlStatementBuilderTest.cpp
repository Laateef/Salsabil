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

#include "doctest.h"
#include "SqlStatementBuilder.hpp"

using namespace Salsabil;

TEST_CASE("SqlStatementBuilder") {

    SUBCASE("IsEmptyWhenCreated") {
        SqlStatementBuilder ssb;
        CHECK(ssb.asString() == "");
    }

    SUBCASE("GeneratesSelectColumnFromTable") {
        CHECK(SqlStatementBuilder().SELECT("name").FROM("user").asString() == "SELECT name FROM user");
    }

    SUBCASE("GeneratesSelectStarFromTable") {
        CHECK(SqlStatementBuilder().SELECT_ALL_FROM("user").asString() == "SELECT * FROM user");
    }

    SUBCASE("GeneratesSelectColumnFromTableWhereColumnEqualToParameter") {
        CHECK(SqlStatementBuilder().SELECT("name").FROM("user").WHERE("id").equalTo(345).asString() == "SELECT name FROM user WHERE id = 345");
    }

    SUBCASE("GeneratesSelectColumnFromTableWhereColumnNotEqualToParameter") {
        CHECK(SqlStatementBuilder().SELECT("name").FROM("user").WHERE("id").notEqualTo(345).asString() == "SELECT name FROM user WHERE id <> 345");
    }

    SUBCASE("GeneratesSelectColumnFromTableWhereColumnGreaterThanParameter") {
        CHECK(SqlStatementBuilder().SELECT("name").FROM("product").WHERE("price").greaterThan(12.5).asString() == "SELECT name FROM product WHERE price > 12.5");
    }

    SUBCASE("GeneratesSelectColumnFromTableWhereColumnGreaterThanOrEqualToParameter") {
        CHECK(SqlStatementBuilder().SELECT("id").FROM("measurement").WHERE("temp").greaterThanOrEqualTo(-85.5).asString() == "SELECT id FROM measurement WHERE temp >= -85.5");
    }

    SUBCASE("GeneratesSelectColumnFromTableWhereColumnLessThanParameter") {
        CHECK(SqlStatementBuilder().SELECT("name").FROM("item").WHERE("id").lessThan(12).asString() == "SELECT name FROM item WHERE id < 12");
    }

    SUBCASE("GeneratesSelectColumnFromTableWhereColumnLessThanOrEqualToParameter") {
        CHECK(SqlStatementBuilder().SELECT("name").FROM("item").WHERE("id").lessThanOrEqualTo(32).asString() == "SELECT name FROM item WHERE id <= 32");
    }

    SUBCASE("GeneratesSelectGivenAndWhereCondition") {
        CHECK(SqlStatementBuilder().SELECT("name").FROM("user").WHERE("id").equalTo(45).AND("session_count").greaterThan(3).asString() == "SELECT name FROM user WHERE id = 45 AND session_count > 3");
    }

    SUBCASE("GeneratesSelectGivenOrWhereCondition") {
        CHECK(SqlStatementBuilder().SELECT("name").FROM("user").WHERE("id").equalTo(45).OR("user_group").notEqualToString("admin").asString() == "SELECT name FROM user WHERE id = 45 OR user_group <> 'admin'");
    }

    SUBCASE("GeneratesInsertIntoTableValues") {
        CHECK(SqlStatementBuilder().INSERT_INTO("user").VALUES(12, "Ali", 37.4).asString() == "INSERT INTO user VALUES (12, 'Ali', 37.4)");
    }
}