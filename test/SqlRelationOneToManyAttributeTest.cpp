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

TEST_CASE("SqlRelationOneToManyAttributeImpl") {

    SqliteDriver drv;
    drv.open(":memory:");
    drv.execute("create table user (id int NOT NULL PRIMARY KEY, name varchar(20))");
    drv.execute("create table session (id int NOT NULL PRIMARY KEY, time varchar(20), user_id int, FOREIGN KEY (user_id) REFERENCES USER(id) ON DELETE CASCADE ON UPDATE CASCADE)");

    SqlEntityConfigurer<SessionMock> sessionConfig;
    sessionConfig.setDriver(&drv);
    sessionConfig.setTableName("session");
    sessionConfig.setPrimaryField("id", &SessionMock::id);
    sessionConfig.setField("time", &SessionMock::time);

    SqlEntityConfigurer<UserMock> userConfig;
    userConfig.setDriver(&drv);
    userConfig.setTableName("user");
    userConfig.setPrimaryField("id", &UserMock::id);
    userConfig.setField("name", &UserMock::name);
    userConfig.setOneToManyField("session", "user_id", &UserMock::sessions);

    SUBCASE("fetching entities") {
        drv.execute("INSERT INTO user(id, name) values(1, 'Ali')");
        drv.execute("INSERT INTO session(id, time, user_id) values(1, '2018-01-23T08:54:22', 1)");
        drv.execute("INSERT INTO session(id, time, user_id) values(2, '2018-01-27T01:48:44', 1)");

        UserMock* obj = SqlRepository<UserMock>::fetch(1);

        REQUIRE(obj != nullptr);
        CHECK(obj->getId() == 1);
        CHECK(obj->getName() == "Ali");
        REQUIRE(obj->sessions.size() == 2);
        CHECK(obj->sessions.at(0)->id == 1);
        CHECK(obj->sessions.at(0)->time == "2018-01-23T08:54:22");
        CHECK(obj->sessions.at(1)->id == 2);
        CHECK(obj->sessions.at(1)->time == "2018-01-27T01:48:44");

        for (auto obj : obj->sessions)
            delete obj;

        delete obj;
    }

    SUBCASE("persisting entities") {
        UserMock user;
        user.id = 1;
        user.name = "Ali";

        SessionMock session1;
        session1.id = 1;
        session1.time = "2018-01-23T08:54:22";

        SessionMock session2;
        session2.id = 2;
        session2.time = "2018-01-27T01:48:44";

        std::vector<SessionMock*> vec;
        vec.push_back(&session1);
        vec.push_back(&session2);
        user.sessions = vec;

        SqlRepository<SessionMock>::persist(&session1);
        SqlRepository<SessionMock>::persist(&session2);
        SqlRepository<UserMock>::persist(&user);

        drv.execute("select * from user");
        REQUIRE(drv.nextRow() == true);
        CHECK(drv.getInt(0) == 1);
        CHECK(drv.getStdString(1) == "Ali");
        CHECK(drv.nextRow() == false);
        
        drv.execute("select * from session");
        REQUIRE(drv.nextRow() == true);
        CHECK(drv.getInt(0) == 1);
        CHECK(drv.getStdString(1) == "2018-01-23T08:54:22");
        CHECK(drv.getInt(2) == 1);
        REQUIRE(drv.nextRow() == true);
        CHECK(drv.getInt(0) == 2);
        CHECK(drv.getStdString(1) == "2018-01-27T01:48:44");
        CHECK(drv.getInt(2) == 1);
        REQUIRE(drv.nextRow() == false);
    }
}