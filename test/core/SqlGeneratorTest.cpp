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
#include "internal/SqlGenerator.hpp"

using namespace Salsabil;

TEST_CASE("SqlGenerator") {

    SUBCASE(" fetch all rows from a table ") {
        CHECK(SqlGenerator::fetchAll("user") == "SELECT * FROM user");
    }

    SUBCASE(" fetch a specific row from a table ") {
        CHECK(SqlGenerator::fetchById("user", "id", "7") == "SELECT * FROM user WHERE id = 7");
    }

    SUBCASE(" fetch a specific row from a table with a composite primary key ") {
        CHECK(SqlGenerator::fetchById("user",{
            {"no", "7"},
            {"name", "'Ali'"}
        }) == "SELECT * FROM user WHERE name = 'Ali' AND no = 7");
    }

    //    SUBCASE(" fetch common rows by joining ") {
    //        CHECK(SqlGenerator::fetchByJoin(SqlGenerator::JoinMode::Inner, "session", "user_session", "user_session.session_id = session.id", "user_session.user_id = 1")
    //                == "SELECT session.* FROM session INNER JOIN user_session ON user_session.session_id = session.id WHERE user_session.user_id = 1");
    //    }

    SUBCASE(" insert a row into a table with values ") {
        CHECK(SqlGenerator::insert("user",{
            {"id", "1"},
            {"name", "'Joly'"}
        }) == "INSERT INTO user(id, name) VALUES(1, 'Joly')");
    }

    SUBCASE(" update a row in a table with parameterized values ") {
        CHECK(SqlGenerator::update("user",{
            {"name", "'Osama'"}
        }, "id", "1") == "UPDATE user SET name = 'Osama' WHERE id = 1");
    }

    SUBCASE(" update a row in a table with parameterized values and multiple where conditions ") {
        CHECK(SqlGenerator::update("user",{
            {"name", "'Osama'"}
        },
        {
            {
                "id", "1"
            },
            {
                "name", "'Omar'"
            }
        }) == "UPDATE user SET name = 'Osama' WHERE id = 1 AND name = 'Omar'");
    }
}