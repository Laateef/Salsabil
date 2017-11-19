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
#include "SqlStatementBuilder.hpp"

using namespace ::testing;
using namespace Salsabil;

TEST(SqlStatementBuilder, IsEmptyWhenCreated) {
    SqlStatementBuilder ssb;
    ASSERT_THAT(ssb.asString(), Eq(""));
}

TEST(SqlStatementBuilder, GeneratesSelectColumnFromTable) {
    ASSERT_THAT(SqlStatementBuilder().SELECT("name").FROM("user").asString(), Eq("SELECT name FROM user"));
}

TEST(SqlStatementBuilder, GeneratesSelectStarFromTable) {
    ASSERT_THAT(SqlStatementBuilder().SELECT_ALL_FROM("user").asString(), Eq("SELECT * FROM user"));
}

TEST(SqlStatementBuilder, GeneratesSelectColumnFromTableWhereColumnEqualToParameter) {
    ASSERT_THAT(SqlStatementBuilder().SELECT("name").FROM("user").WHERE("id").equalTo(345).asString(),
            Eq("SELECT name FROM user WHERE id = 345"));
}

TEST(SqlStatementBuilder, GeneratesSelectColumnFromTableWhereColumnNotEqualToParameter) {
    ASSERT_THAT(SqlStatementBuilder().SELECT("name").FROM("user").WHERE("id").notEqualTo(345).asString(),
            Eq("SELECT name FROM user WHERE id <> 345"));
}

TEST(SqlStatementBuilder, GeneratesSelectColumnFromTableWhereColumnGreaterThanParameter) {
    ASSERT_THAT(SqlStatementBuilder().SELECT("name").FROM("product").WHERE("price").greaterThan(12.5).asString(),
            Eq("SELECT name FROM product WHERE price > 12.5"));
}

TEST(SqlStatementBuilder, GeneratesSelectColumnFromTableWhereColumnGreaterThanOrEqualToParameter) {
    ASSERT_THAT(SqlStatementBuilder().SELECT("id").FROM("measurement").WHERE("temp").greaterThanOrEqualTo(-85.5).asString(),
            Eq("SELECT id FROM measurement WHERE temp >= -85.5"));
}

TEST(SqlStatementBuilder, GeneratesSelectColumnFromTableWhereColumnLessThanParameter) {
    ASSERT_THAT(SqlStatementBuilder().SELECT("name").FROM("item").WHERE("id").lessThan(12).asString(),
            Eq("SELECT name FROM item WHERE id < 12"));
}

TEST(SqlStatementBuilder, GeneratesSelectColumnFromTableWhereColumnLessThanOrEqualToParameter) {
    ASSERT_THAT(SqlStatementBuilder().SELECT("name").FROM("item").WHERE("id").lessThanOrEqualTo(32).asString(),
            Eq("SELECT name FROM item WHERE id <= 32"));
}

TEST(SqlStatementBuilder, GeneratesSelectGivenAndWhereCondition) {
    ASSERT_THAT(SqlStatementBuilder().SELECT("name")
            .FROM("user")
            .WHERE("id")
            .equalTo(45)
            .AND("session_count")
            .greaterThan(3)
            .asString(),
            Eq("SELECT name FROM user WHERE id = 45 AND session_count > 3"));
}

TEST(SqlStatementBuilder, GeneratesSelectGivenOrWhereCondition) {
    ASSERT_THAT(SqlStatementBuilder().SELECT("name")
            .FROM("user")
            .WHERE("id")
            .equalTo(45)
            .OR("user_group")
            .notEqualTo("admin")
            .asString(),
            Eq("SELECT name FROM user WHERE id = 45 OR user_group <> 'admin'"));
}

TEST(SqlStatementBuilder, GeneratesInsertIntoTableValues) {
    ASSERT_THAT(SqlStatementBuilder().INSERT_INTO("user").VALUES(12, "Ali", 37.4).asString(), Eq("INSERT INTO user VALUES (12, 'Ali', 37.4)"));
}

//TEST(SqlStatementBuilder, GeneratesInsertIntoTableGivenColumnsAndValues) {
//    ASSERT_THAT(SqlStatementBuilder().SELECT("name")
//            .FROM("user")
//            .WHERE("id")
//            .equalTo(45)
//            .AND("session_count")
//            .greaterThan(3)
//            .OR("user_group")
//            .notEqualTo("'admin'")
//            .asString(),
//            Eq("SELECT name FROM user WHERE id = 45 AND session_count > 3 OR user_group <> 'admin'"));
//}