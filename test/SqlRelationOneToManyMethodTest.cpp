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

TEST_CASE("SqlRelationOneToManyMethodImpl") {

    SqliteDriver drv;
    drv.open(":memory:");
    drv.execute("create table user (id int NOT NULL PRIMARY KEY, name varchar(20))");
    drv.execute("create table session (id int NOT NULL PRIMARY KEY, time varchar(20), user_id int, FOREIGN KEY (user_id) REFERENCES USER(id) ON DELETE CASCADE ON UPDATE CASCADE)");

    SqlEntityConfigurer<SessionMock> sessionConfig;
    sessionConfig.setDriver(&drv);
    sessionConfig.setTableName("session");
    sessionConfig.setPrimaryField("id", &SessionMock::getId, &SessionMock::setId);
    sessionConfig.setField("time", &SessionMock::getTime, &SessionMock::setTime);

    SqlEntityConfigurer<UserMock> userConfig;
    userConfig.setDriver(&drv);
    userConfig.setTableName("user");
    userConfig.setPrimaryField("id", &UserMock::getId, &UserMock::setId);
    userConfig.setField("name", &UserMock::getName, &UserMock::setName);

    SUBCASE("fetching entities") {
        userConfig.setOneToManyField(&UserMock::getSessions, &UserMock::setSessions, "session", "user_id");

        drv.execute("INSERT INTO user(id, name) values(1, 'Ali')");
        drv.execute("INSERT INTO session(id, time, user_id) values(1, '2018-01-23T08:54:22', 1)");
        drv.execute("INSERT INTO session(id, time, user_id) values(2, '2018-01-27T01:48:44', 1)");

        UserMock* user = SqlRepository<UserMock>::fetch(1);

        REQUIRE(user != nullptr);
        CHECK(user->getId() == 1);
        CHECK(user->getName() == "Ali");
        REQUIRE(user->getSessions().size() == 2);
        CHECK(user->getSessions().at(0)->getId() == 1);
        CHECK(user->getSessions().at(0)->getTime() == "2018-01-23T08:54:22");
        CHECK(user->getSessions().at(1)->getId() == 2);
        CHECK(user->getSessions().at(1)->getTime() == "2018-01-27T01:48:44");

        for (auto session : user->getSessions())
            delete session;

        delete user;
    }

    SUBCASE("persisting entities") {
        UserMock user;
        user.setId(1);
        user.setName("Ali");

        std::vector<SessionMock*> sessionList;
        SessionMock session1;
        session1.setId(1);
        session1.setTime("2018-07-09T08:09:44");
        sessionList.push_back(&session1);
        SessionMock session2;
        session2.setId(2);
        session2.setTime("2018-07-23T10:19:02");
        sessionList.push_back(&session2);

        user.setSessions(sessionList);

        SUBCASE(" save entity into database without cascading ") {
            userConfig.setOneToManyField(&UserMock::getSessions, &UserMock::setSessions, "session", "user_id", CascadeType::None);

            SqlRepository<UserMock>::persist(&user);

            drv.execute("select * from user");
            REQUIRE(drv.nextRow());
            CHECK(drv.getInt(0) == user.getId());
            CHECK(drv.getStdString(1) == user.getName());
            REQUIRE_FALSE(drv.nextRow());

            drv.execute("select * from session");
            REQUIRE_FALSE(drv.nextRow());
        }

        SUBCASE(" save entity into database with cascading ") {
            userConfig.setOneToManyField(&UserMock::getSessions, &UserMock::setSessions, "session", "user_id", CascadeType::Persist);

            SqlRepository<UserMock>::persist(&user);

            drv.execute("select * from user");
            REQUIRE(drv.nextRow());
            CHECK(drv.getInt(0) == user.getId());
            CHECK(drv.getStdString(1) == user.getName());
            REQUIRE_FALSE(drv.nextRow());

            drv.execute("select * from session");
            REQUIRE(drv.nextRow());
            CHECK(drv.getInt(0) == session1.getId());
            CHECK(drv.getStdString(1) == session1.getTime());
            CHECK(drv.getInt(2) == user.getId());
            REQUIRE(drv.nextRow());
            CHECK(drv.getInt(0) == session2.getId());
            CHECK(drv.getStdString(1) == session2.getTime());
            CHECK(drv.getInt(2) == user.getId());
            REQUIRE_FALSE(drv.nextRow());
        }

        SUBCASE(" update entity in database ") {
            drv.execute("INSERT INTO user(id, name) values(1, 'Ali')");
            drv.execute("INSERT INTO session(id, time, user_id) values(1, '2017-01-23T08:54:22', 1)");
            drv.execute("INSERT INTO session(id, time, user_id) values(2, '2018-07-23T10:19:02', 1)");

            userConfig.setOneToManyField(&UserMock::getSessions, &UserMock::setSessions, "session", "user_id", CascadeType::Update);

            user.getSessions().at(0)->setTime("2018-09-05T21:44:31");

            SqlRepository<UserMock>::update(&user);

            drv.execute("select * from user");
            REQUIRE(drv.nextRow());
            CHECK(drv.getInt(0) == user.getId());
            CHECK(drv.getStdString(1) == user.getName());
            REQUIRE_FALSE(drv.nextRow());

            drv.execute("select * from session");
            REQUIRE(drv.nextRow());
            CHECK(drv.getInt(0) == session1.getId());
            CHECK(drv.getStdString(1) == session1.getTime());
            CHECK(drv.getInt(2) == user.getId());
            REQUIRE(drv.nextRow());
            CHECK(drv.getInt(0) == session2.getId());
            CHECK(drv.getStdString(1) == session2.getTime());
            CHECK(drv.getInt(2) == user.getId());
            REQUIRE_FALSE(drv.nextRow());
        }

        SUBCASE(" remove entity from database ") {
            drv.execute("INSERT INTO user(id, name) values(1, 'Ali')");
            drv.execute("INSERT INTO session(id, time, user_id) values(1, '2017-01-23T08:54:22', 1)");
            drv.execute("INSERT INTO session(id, time, user_id) values(2, '2018-07-23T10:19:02', 1)");

            userConfig.setOneToManyField(&UserMock::getSessions, &UserMock::setSessions, "session", "user_id", CascadeType::Remove);

            SqlRepository<UserMock>::remove(&user);

            drv.execute("select * from user");
            REQUIRE_FALSE(drv.nextRow());

            drv.execute("select * from session");
            REQUIRE_FALSE(drv.nextRow());
        }
    }
}