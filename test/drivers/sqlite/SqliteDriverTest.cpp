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
#include "SqliteDriver.hpp"
#include "Exception.hpp"
#include <cstring>
#include <algorithm>

using namespace Salsabil;

TEST_CASE("SqliteDriver") {
    SqliteDriver drv;

    SUBCASE("ThrowsIfDatabaseNotFoundWhileOpening") {
        REQUIRE_THROWS_AS(drv.open("some_strange_database"), Exception);
    }

    drv.open(":memory:");

    SUBCASE("ClosesDatabaseAutomaticallyWhenDestroyed") {
        REQUIRE(drv.isOpen());

        drv.~SqliteDriver();

        REQUIRE_FALSE(drv.isOpen());
    }

    SUBCASE("ThrowsIfStatementCannotBePrepared") {
        REQUIRE_THROWS_AS(drv.prepare("CREATE TABLE (id INT PRIMARY KEY, name TEXT)"), Exception);
    }

    SUBCASE("ThrowsIfStatementCannotBeExecuted") {
        drv.prepare("CREATE TABLE tbl(id INT PRIMARY KEY NOT NULL, name TEXT)");
        drv.execute();
        drv.prepare("INSERT INTO tbl VALUES(NULL, 'abc')");

        REQUIRE_THROWS_AS(drv.execute(), Exception);
    }

    SUBCASE("ThrowsIfAskedToFetchRowForNonQuerySqlStatements") {
        drv.prepare("CREATE TABLE tbl(id INT PRIMARY KEY, name TEXT)");
        drv.execute();

        REQUIRE_FALSE(drv.nextRow());
    }

    SUBCASE("ThrowsIfAskedToFetchRowAfterFetchingAllRowsForQuerySqlStatements") {
        drv.execute("CREATE TABLE tbl(id INT PRIMARY KEY, name TEXT)");
        drv.execute("INSERT INTO tbl VALUES(1, 'abc')");
        drv.execute("INSERT INTO tbl VALUES(2, 'cde')");
        drv.execute("SELECT * FROM tbl");

        REQUIRE(drv.nextRow());
        REQUIRE(drv.nextRow());
        REQUIRE_FALSE(drv.nextRow());
    }

    SUBCASE("FetchNextReturnsFalseIfQueryResultSetHasNoRows") {
        drv.execute("CREATE TABLE tbl(id INT PRIMARY KEY, name TEXT)");
        drv.execute("SELECT * FROM tbl");

        REQUIRE_FALSE(drv.nextRow());
    }

    SUBCASE("TestsNullResult") {
        drv.execute("CREATE TABLE tbl(id INT PRIMARY KEY, name TEXT, balance REAL, picture BLOB)");
        drv.execute("INSERT INTO tbl VALUES(1, NULL, NULL, NULL)");
        drv.execute("SELECT * FROM tbl");

        REQUIRE_FALSE(drv.isNull(0));
        REQUIRE(drv.isNull(1));
        REQUIRE(drv.isNull(2));
        REQUIRE(drv.isNull(3));
    }

    SUBCASE("FetchIntegralResult") {
        drv.execute("CREATE TABLE tbl(id INT PRIMARY KEY, num1 INT, num2 INT, num3 INT)");
        drv.execute("INSERT INTO tbl VALUES(32767, -32767, 9223372036854775807, -9223372036854775807)");
        drv.execute("SELECT * FROM tbl");

        REQUIRE(drv.getInt(0) == 32767);
        REQUIRE(drv.getInt(1) == -32767);
        REQUIRE(drv.getInt64(2) == 9223372036854775807LL);
        REQUIRE(drv.getInt64(3) == -9223372036854775807LL);
    }

    SUBCASE("FetchFloatingPointResult") {
        drv.execute("CREATE TABLE tbl(id INT PRIMARY KEY, num1 REAL, num2 REAL)");
        drv.execute("INSERT INTO tbl VALUES(1, -1.175494351, 3.402823466)");
        drv.execute("SELECT * FROM tbl");

        REQUIRE(drv.getDouble(2) == 3.402823466);
    }

    SUBCASE("FetchLiteralResult") {
        drv.execute("CREATE TABLE tbl(id INT PRIMARY KEY, str1 TEXT, str2 TEXT)");
        drv.execute("INSERT INTO tbl VALUES(1, 'Hi, everyone!', 'Here is another string!')");
        drv.execute("SELECT * FROM tbl");

        REQUIRE(strcmp(drv.getCString(1), "Hi, everyone!") == 0);
        REQUIRE(drv.getStdString(2) == "Here is another string!");
    }

    SUBCASE("FetchBlobResult") {
        drv.execute("CREATE TABLE tbl(id INT PRIMARY KEY, b1 BLOB, b2 BLOB)");
        drv.execute("INSERT INTO tbl VALUES(1, NULL, X'53514C697465')");
        drv.execute("SELECT * FROM tbl");

        REQUIRE(drv.getSize(2) == 6u);
        const char expect[] = {0x53, 0x51, 0x4C, 0x69, 0x74, 0x65};
        REQUIRE(memcmp(drv.getBlob(2), expect, sizeof (expect)) == 0);
    }

    SUBCASE("ThrowsIfValueIsBoundToOutOfRangeIndexedParameterInPreparedStatement") {
        drv.execute("CREATE TABLE tbl(id INT PRIMARY KEY, num1 INT, num2 INT)");
        drv.prepare("INSERT INTO tbl VALUES(1, ?, ?)");

        REQUIRE_THROWS_AS(drv.bindNull(3), Exception);
        REQUIRE_THROWS_AS(drv.bindInt(4, 32767), Exception);
        REQUIRE_THROWS_AS(drv.bindInt64(5, 9223372036854775807LL), Exception);
        REQUIRE_THROWS_AS(drv.bindDouble(6, 3.402823466), Exception);
        REQUIRE_THROWS_AS(drv.bindCString(7, "some_thing"), Exception);
        REQUIRE_THROWS_AS(drv.bindStdString(8, std::string("some_thing")), Exception);
        REQUIRE_THROWS_AS(drv.bindBlob(9, nullptr, 0), Exception);
    }

    SUBCASE("TestsTableExistence") {
        drv.execute("CREATE TABLE tb(id INT PRIMARY KEY, num1 INT, num2 INT)");
        auto tables = drv.tableList();

        REQUIRE(tables.size() == 1u);
        REQUIRE(std::find(tables.begin(), tables.end(), "tb") != tables.end());
    }    
}