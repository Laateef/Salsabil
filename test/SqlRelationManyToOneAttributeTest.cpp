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
#include "mocks/UserMock.hpp"
#include "mocks/SessionMock.hpp"
#include "Exception.hpp"
#include "SqlDriverFactory.hpp"
#include "SqlEntityConfigurer.hpp"
#include "SqlRepository.hpp"
#include "SqliteDriver.hpp"

using namespace Salsabil;

TEST_CASE("SqlRelationManyToOneAttributeImpl") {
    SqliteDriver drv;
    drv.open(":memory:");
    drv.execute("create table user (id int NOT NULL PRIMARY KEY, name varchar(20))");
    drv.execute("create table session (id int NOT NULL PRIMARY KEY, time varchar(20), user_id int, FOREIGN KEY (user_id) REFERENCES USER(id) ON DELETE CASCADE ON UPDATE CASCADE)");

    SqlEntityConfigurer<UserMock> userConfig;
    userConfig.setDriver(&drv);
    userConfig.setTableName("user");
    userConfig.setPrimaryField("id", &UserMock::id);
    userConfig.setField("name", &UserMock::name);

    SqlEntityConfigurer<SessionMock> sessionConfig;
    sessionConfig.setDriver(&drv);
    sessionConfig.setTableName("session");
    sessionConfig.setPrimaryField("id", &SessionMock::id);
    sessionConfig.setField("time", &SessionMock::time);
    sessionConfig.setManyToOneField(&SessionMock::user, "user", "user_id", "id");

    SUBCASE("fetching entities") {
        drv.execute("INSERT INTO user(id, name) values(1, 'Ali')");
        drv.execute("INSERT INTO session(id, time, user_id) values(1, '2018-01-23T08:54:22', 1)");
        drv.execute("INSERT INTO session(id, time, user_id) values(2, '2018-01-27T01:48:44', 1)");

        SessionMock* session1 = SqlRepository<SessionMock>::fetch(1);

        REQUIRE(session1 != nullptr);
        CHECK(session1->id == 1);
        CHECK(session1->time == "2018-01-23T08:54:22");
        REQUIRE(session1->user != nullptr);
        CHECK(session1->user->id == 1);
        CHECK(session1->user->name == "Ali");

        SessionMock* session2 = SqlRepository<SessionMock>::fetch(2);

        REQUIRE(session2 != nullptr);
        CHECK(session2->id == 2);
        CHECK(session2->time == "2018-01-27T01:48:44");
        REQUIRE(session2->user != nullptr);
        CHECK(session2->user->id == 1);
        CHECK(session2->user->name == "Ali");

        delete session1->user;
        delete session1;
        delete session2->user;
        delete session2;
    }

    SUBCASE("persisting entities") {
        UserMock user;
        user.setId(1);
        user.setName("Ali");

        SessionMock session1;
        session1.setId(1);
        session1.setTime("2018-01-23T08:54:22");
        session1.setUser(&user);

        SessionMock session2;
        session2.setId(2);
        session2.setTime("2018-02-18T01:48:44");
        session2.setUser(&user);

        SqlRepository<UserMock>::persist(&user);

        SUBCASE(" save entity into database ") {
            sessionConfig.setManyToOneField(&SessionMock::user, "user", "user_id", "id");

            SqlRepository<SessionMock>::persist(&session1);

            drv.execute("select * from session");
            REQUIRE(drv.nextRow());
            CHECK(drv.getInt(0) == session1.id);
            CHECK(drv.getStdString(1) == session1.time);
            CHECK(drv.getInt(2) == session1.user->id);
            REQUIRE_FALSE(drv.nextRow());
        }

        SUBCASE(" update entity in database ") {
            drv.execute("INSERT INTO user(id, name) values(2, 'Latif')");
            drv.execute("INSERT INTO session(id, time, user_id) values(1, '2018-01-23T08:54:22', 1)");
            drv.execute("INSERT INTO session(id, time, user_id) values(2, '2018-07-23T10:19:02', 2)");

            sessionConfig.setManyToOneField(&SessionMock::user, "user", "user_id", "id");

            session2.setUser(&user);
            SqlRepository<SessionMock>::update(&session2);

            drv.execute("select * from session");
            REQUIRE(drv.nextRow());
            CHECK(drv.getInt(0) == session1.id);
            CHECK(drv.getStdString(1) == session1.time);
            CHECK(drv.getInt(2) == session1.user->id);
            REQUIRE(drv.nextRow());
            CHECK(drv.getInt(0) == session2.id);
            CHECK(drv.getStdString(1) == session2.time);
            CHECK(drv.getInt(2) == session2.user->id);
            REQUIRE_FALSE(drv.nextRow());
        }
    }
}