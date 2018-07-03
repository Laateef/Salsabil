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

TEST_CASE("SqlRelationManyToManyMethodImpl") {
    SqliteDriver drv;
    drv.open(":memory:");
    drv.execute("create table user (id int NOT NULL PRIMARY KEY, name varchar(20))");
    drv.execute("create table session (id int NOT NULL PRIMARY KEY, time varchar(20))");
    drv.execute("create table user_session (user_id int NOT NULL, session_id int NOT NULL, exp varchar(99), PRIMARY KEY(user_id, session_id), FOREIGN KEY (user_id) REFERENCES USER(id) ON DELETE CASCADE ON UPDATE CASCADE, FOREIGN KEY (session_id) REFERENCES SESSION(id) ON DELETE CASCADE ON UPDATE CASCADE)");

    SqlEntityConfigurer<UserMock> userConfig;
    userConfig.setDriver(&drv);
    userConfig.setTableName("user");
    userConfig.setPrimaryField("id", UserMock::getId, UserMock::setId);
    userConfig.setField("name", UserMock::getName, UserMock::setName);
    SqlManyToManyMapping mapping("user", "user_session", "session");
    mapping.setLeftMapping("user_id", "id");
    mapping.setRightMapping("session_id", "id");
    userConfig.setManyToManyField(mapping, UserMock::getSessions, UserMock::setSessions);

    SqlEntityConfigurer<SessionMock> sessionConfig;
    sessionConfig.setDriver(&drv);
    sessionConfig.setTableName("session");
    sessionConfig.setPrimaryField("id", SessionMock::getId, SessionMock::setId);
    sessionConfig.setField("time", SessionMock::getTime, SessionMock::setTime);

    SUBCASE("fetching entities") {
        drv.execute("INSERT INTO user(id, name) values(1, 'Ali')");
        drv.execute("INSERT INTO session(id, time) values(1, '2018-01-23T08:54:22')");
        drv.execute("INSERT INTO session(id, time) values(2, '2018-02-23T19:19:05')");
        drv.execute("INSERT INTO user_session(user_id, session_id, exp) values(1, 1, 'abc')");
        drv.execute("INSERT INTO user_session(user_id, session_id, exp) values(1, 2, 'xyz')");

        UserMock* user = SqlRepository<UserMock>::get(1);

        REQUIRE(user != nullptr);
        CHECK(user->getId() == 1);
        CHECK(user->getName() == "Ali");
        REQUIRE(user->getSessions().size() == 2);
        CHECK(user->getSessions().at(0)->getId() == 1);
        CHECK(user->getSessions().at(0)->getTime() == "2018-01-23T08:54:22");
        CHECK(user->getSessions().at(1)->getId() == 2);
        CHECK(user->getSessions().at(1)->getTime() == "2018-02-23T19:19:05");

        delete user->getSessions().at(0);
        delete user->getSessions().at(1);
        delete user;
    }

    SUBCASE("persisting entities") {
        UserMock user;
        user.setId(1);
        user.setName("Ali");

        SessionMock session1;
        session1.setId(1);
        session1.setTime("2018-01-23T08:54:22");

        SessionMock session2;
        session2.setId(2);
        session2.setTime("2018-02-18T01:48:44");

        std::vector<SessionMock*> sessionList;
        sessionList.push_back(&session1);
        sessionList.push_back(&session2);
        user.setSessions(sessionList);

        SqlRepository<SessionMock>::save(&session1);
        SqlRepository<SessionMock>::save(&session2);

        SqlRepository<UserMock>::save(&user);

        drv.execute("select * from user");
        REQUIRE(drv.nextRow() == true);
        CHECK(drv.getInt(0) == 1);
        CHECK(drv.getStdString(1) == "Ali");
        REQUIRE(drv.nextRow() == false);

        drv.execute("select * from session");
        REQUIRE(drv.nextRow() == true);
        CHECK(drv.getInt(0) == 1);
        CHECK(drv.getStdString(1) == "2018-01-23T08:54:22");
        REQUIRE(drv.nextRow() == true);
        CHECK(drv.getInt(0) == 2);
        CHECK(drv.getStdString(1) == "2018-02-18T01:48:44");
        REQUIRE(drv.nextRow() == false);

        drv.execute("select * from user_session");
        REQUIRE(drv.nextRow() == true);
        CHECK(drv.getInt(0) == 1);
        CHECK(drv.getInt(1) == 1);
        REQUIRE(drv.nextRow() == true);
        CHECK(drv.getInt(0) == 1);
        CHECK(drv.getInt(1) == 2);
        REQUIRE(drv.nextRow() == false);
    }
}