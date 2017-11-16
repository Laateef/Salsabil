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
#include "SqliteDriver.hpp"
#include "Exception.hpp"

using namespace ::testing;
using namespace Salsabil;

TEST(SqliteDriver, ThrowsIfDatabaseNotFoundWhileOpening) {
    SqliteDriver ssd;

    ASSERT_THROW(ssd.open("some_strange_database"), Exception);
}

TEST(SqliteDriver, ClosesDatabaseAutomaticallyWhenDestroyed) {
    SqliteDriver ssd;
    ssd.open(":memory:");
    EXPECT_TRUE(ssd.isOpen());

    ssd.~SqliteDriver();

    ASSERT_FALSE(ssd.isOpen());
}

TEST(SqliteDriver, ThrowsIfStatementCannotBePrepared) {
    SqliteDriver ssd;
    ssd.open(":memory:");

    ASSERT_THROW(ssd.prepare("CREATE TABLE (id INT PRIMARY KEY, name TEXT)"), Exception);
}

TEST(SqliteDriver, ThrowsIfStatementCannotBeExecuted) {
    SqliteDriver ssd;
    ssd.open(":memory:");
    ssd.prepare("CREATE TABLE tbl(id INT PRIMARY KEY NOT NULL, name TEXT)");
    ssd.execute();
    ssd.prepare("INSERT INTO tbl VALUES(NULL, 'abc')");

    ASSERT_THROW(ssd.execute(), Exception);
}

TEST(SqliteDriver, ThrowsIfAskedToFetchRowForNonQuerySqlStatements) {
    SqliteDriver ssd;
    ssd.open(":memory:");
    ssd.prepare("CREATE TABLE tbl(id INT PRIMARY KEY, name TEXT)");
    ssd.execute();

    ASSERT_FALSE(ssd.nextRow());
}

TEST(SqliteDriver, ThrowsIfAskedToFetchRowAfterFetchingAllRowsForQuerySqlStatements) {
    SqliteDriver ssd;
    ssd.open(":memory:");
    ssd.prepare("CREATE TABLE tbl(id INT PRIMARY KEY, name TEXT)");
    ssd.execute();
    ssd.prepare("INSERT INTO tbl VALUES(1, 'abc')");
    ssd.execute();
    ssd.prepare("INSERT INTO tbl VALUES(2, 'cde')");
    ssd.execute();
    ssd.prepare("SELECT * FROM tbl");
    ssd.execute();
    
    ASSERT_TRUE(ssd.nextRow());
    ASSERT_TRUE(ssd.nextRow());
    ASSERT_FALSE(ssd.nextRow());
}

TEST(SqliteDriver, FetchNextReturnsFalseIfQueryResultSetHasNoRows) {
    SqliteDriver ssd;
    ssd.open(":memory:");
    ssd.prepare("CREATE TABLE tbl(id INT PRIMARY KEY, name TEXT)");
    ssd.execute();
    ssd.prepare("SELECT * FROM tbl");
    ssd.execute();

    ASSERT_FALSE(ssd.nextRow());
}

TEST(SqliteDriver, TestsNullResult) {
    SqliteDriver ssd;
    ssd.open(":memory:");
    ssd.prepare("CREATE TABLE tbl(id INT PRIMARY KEY, name TEXT, balance REAL, picture BLOB)");
    ssd.execute();
    ssd.prepare("INSERT INTO tbl VALUES(1, NULL, NULL, NULL)");
    ssd.execute();
    ssd.prepare("SELECT * FROM tbl");
    ssd.execute();

    ASSERT_FALSE(ssd.isNull(0));
    ASSERT_TRUE(ssd.isNull(1));
    ASSERT_TRUE(ssd.isNull(2));
    ASSERT_TRUE(ssd.isNull(3));
}

TEST(SqliteDriver, FetchIntegralResult) {
    SqliteDriver ssd;
    ssd.open(":memory:");
    ssd.prepare("CREATE TABLE tbl(id INT PRIMARY KEY, num1 INT, num2 INT, num3 INT)");
    ssd.execute();
    ssd.prepare("INSERT INTO tbl VALUES(32767, -32767, 9223372036854775807, -9223372036854775807)");
    ssd.execute();
    ssd.prepare("SELECT * FROM tbl");
    ssd.execute();

    ASSERT_THAT(ssd.getInt(0), Eq(32767));
    ASSERT_THAT(ssd.getInt(1), Eq(-32767));
    ASSERT_THAT(ssd.getInt64(2), Eq(9223372036854775807LL));
    ASSERT_THAT(ssd.getInt64(3), Eq(-9223372036854775807LL));
}

TEST(SqliteDriver, FetchFloatingPointResult) {
    SqliteDriver ssd;
    ssd.open(":memory:");
    ssd.prepare("CREATE TABLE tbl(id INT PRIMARY KEY, num1 REAL, num2 REAL)");
    ssd.execute();
    ssd.prepare("INSERT INTO tbl VALUES(1, -1.175494351, 3.402823466)");
    ssd.execute();
    ssd.prepare("SELECT * FROM tbl");
    ssd.execute();

    ASSERT_THAT(ssd.getDouble(2), DoubleEq(3.402823466));
}

TEST(SqliteDriver, FetchLiteralResult) {
    SqliteDriver ssd;
    ssd.open(":memory:");
    ssd.prepare("CREATE TABLE tbl(id INT PRIMARY KEY, str1 TEXT, str2 TEXT)");
    ssd.execute();
    ssd.prepare("INSERT INTO tbl VALUES(1, 'Hi, everyone!', 'Here is another string!')");
    ssd.execute();
    ssd.prepare("SELECT * FROM tbl");
    ssd.execute();

    ASSERT_THAT(ssd.getCString(1), StrEq("Hi, everyone!"));
    ASSERT_THAT(ssd.getStdString(2), StrEq("Here is another string!"));
}

TEST(SqliteDriver, FetchBlobResult) {
    SqliteDriver ssd;
    ssd.open(":memory:");
    ssd.prepare("CREATE TABLE tbl(id INT PRIMARY KEY, b1 BLOB, b2 BLOB)");
    ssd.execute();
    ssd.prepare("INSERT INTO tbl VALUES(1, NULL, X'53514C697465')");
    ssd.execute();
    ssd.prepare("SELECT * FROM tbl");
    ssd.execute();

    ASSERT_THAT(ssd.getSize(2), Eq(6u));
    const char expect[] = {0x53, 0x51, 0x4C, 0x69, 0x74, 0x65};
    ASSERT_THAT(memcmp(ssd.getBlob(2), expect, sizeof (expect)), Eq(0));
}

TEST(SqliteDriver, ThrowsIfValueIsBoundToOutOfRangeIndexedParameterInPreparedStatement) {
    SqliteDriver ssd;
    ssd.open(":memory:");
    ssd.prepare("CREATE TABLE tbl(id INT PRIMARY KEY, num1 INT, num2 INT)");
    ssd.execute();
    ssd.prepare("INSERT INTO tbl VALUES(1, ?, ?)");

    ASSERT_THROW(ssd.bindNull(3), Exception);
    ASSERT_THROW(ssd.bindInt(4, 32767), Exception);
    ASSERT_THROW(ssd.bindInt64(5, 9223372036854775807LL), Exception);
    ASSERT_THROW(ssd.bindDouble(6, 3.402823466), Exception);
    ASSERT_THROW(ssd.bindCString(7, "some_thing"), Exception);
    ASSERT_THROW(ssd.bindStdString(8, std::string("some_thing")), Exception);
    ASSERT_THROW(ssd.bindBlob(9, nullptr, 0), Exception);
}

TEST(SqliteDriver, TestsTableExistence) {
    SqliteDriver ssd;
    ssd.open(":memory:");
    ssd.prepare("CREATE TABLE tb(id INT PRIMARY KEY, num1 INT, num2 INT)");
    ssd.execute();
    auto tables = ssd.tableList();
    
    EXPECT_THAT(tables.size(), Eq(1u));
    ASSERT_TRUE(std::find(tables.begin(), tables.end(), "tb") != tables.end());
}
