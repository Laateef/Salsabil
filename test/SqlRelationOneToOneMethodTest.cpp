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

TEST_CASE("SqlOneToOneRelationMethodImpl") {
    SqliteDriver drv;
    drv.open(":memory:");
    drv.execute("create table user (id int NOT NULL PRIMARY KEY, name varchar(20))");
    drv.execute("create table session (id int NOT NULL PRIMARY KEY, time varchar(20), user_id int, FOREIGN KEY (user_id) REFERENCES USER(id) ON DELETE CASCADE ON UPDATE CASCADE)");

    SqlEntityConfigurer<UserMock> userConfig;
    userConfig.setDriver(&drv);
    userConfig.setTableName("user");
    userConfig.setPrimaryField("id", UserMock::getId, UserMock::setId);
    userConfig.setField("name", UserMock::getName, UserMock::setName);

    SqlEntityConfigurer<SessionMock> sessionConfig;
    sessionConfig.setDriver(&drv);
    sessionConfig.setTableName("session");
    sessionConfig.setPrimaryField("id", SessionMock::getId, SessionMock::setId);
    sessionConfig.setField("time", SessionMock::getTime, SessionMock::setTime);

    SUBCASE("fetching entities") {
        drv.execute("INSERT INTO user(id, name) values(1, 'Ali')");
        drv.execute("INSERT INTO session(id, time, user_id) values(1, '2018-01-23T08:54:22', 1)");

        SUBCASE("with a pointer foreign field ") {
            sessionConfig.setOneToOnePersistentField("user", "user_id", "id", SessionMock::getUser, SessionMock::setUser);

            SessionMock* session = SqlRepository<SessionMock>::fetch(1);

            REQUIRE(session != nullptr);
            CHECK(session->getId() == 1);
            CHECK(session->getTime() == "2018-01-23T08:54:22");
            REQUIRE(session->getUser() != nullptr);
            CHECK(session->getUser()->getId() == 1);
            CHECK(session->getUser()->getName() == "Ali");

            delete session->getUser();
            delete session;
        }

        SUBCASE("with a reference foreign field ") {
            sessionConfig.setOneToOnePersistentField("user", "user_id", "id", SessionMock::getStackUser, SessionMock::setStackUser);

            SessionMock* session = SqlRepository<SessionMock>::fetch(1);

            REQUIRE(session != nullptr);
            CHECK(session->getId() == 1);
            CHECK(session->getTime() == "2018-01-23T08:54:22");
            CHECK(session->getStackUser().getId() == 1);
            CHECK(session->getStackUser().getName() == "Ali");

            delete session;
        }

        SUBCASE("with a pointer referential field ") {
            userConfig.setOneToOneTransientField("session", "user_id", UserMock::getSession, UserMock::setSession);

            UserMock* user = SqlRepository<UserMock>::fetch(1);

            REQUIRE(user != nullptr);
            CHECK(user->getId() == 1);
            CHECK(user->getName() == "Ali");
            REQUIRE(user->getSession() != nullptr);
            CHECK(user->getSession()->getId() == 1);
            CHECK(user->getSession()->getTime() == "2018-01-23T08:54:22");

            delete user->getSession();
            delete user;
        }

        SUBCASE("with a reference referential field ") {
            sessionConfig.setOneToOneTransientField("user", "id", SessionMock::getStackUser, SessionMock::setStackUser);

            SessionMock* session = SqlRepository<SessionMock>::fetch(1);

            REQUIRE(session != nullptr);
            CHECK(session->getId() == 1);
            CHECK(session->getTime() == "2018-01-23T08:54:22");
            CHECK(session->getStackUser().getId() == 1);
            CHECK(session->getStackUser().getName() == "Ali");

            delete session;
        }
    }

    SUBCASE("persisting entities") {
        UserMock user;
        user.setId(1);
        user.setName("Ali");
        SessionMock session;
        session.setId(1);
        session.setTime("2018-01-23T08:54:22");
        SqlRepository<UserMock>::persist(&user);

        SUBCASE("with a pointer foreign field") {
            sessionConfig.setOneToOnePersistentField("user", "user_id", "id", SessionMock::getUser, SessionMock::setUser);
            session.setUser(&user);

            SqlRepository<SessionMock>::persist(&session);

            drv.execute("select * from session");
            REQUIRE(drv.nextRow() == true);
            CHECK(drv.getInt(0) == 1);
            CHECK(drv.getStdString(1) == "2018-01-23T08:54:22");
            CHECK(drv.getInt(2) == 1);
            REQUIRE(drv.nextRow() == false);

            drv.execute("select * from user");
            REQUIRE(drv.nextRow() == true);
            CHECK(drv.getInt(0) == 1);
            CHECK(drv.getStdString(1) == "Ali");
            REQUIRE(drv.nextRow() == false);
        }

        SUBCASE("with a reference foreign field") {
            sessionConfig.setOneToOnePersistentField("user", "user_id", "id", SessionMock::getStackUser, SessionMock::setStackUser);
            session.setStackUser(user);

            SqlRepository<SessionMock>::persist(&session);

            drv.execute("select * from session");
            REQUIRE(drv.nextRow() == true);
            CHECK(drv.getInt(0) == 1);
            CHECK(drv.getStdString(1) == "2018-01-23T08:54:22");
            CHECK(drv.getInt(2) == 1);
            REQUIRE(drv.nextRow() == false);

            drv.execute("select * from user");
            REQUIRE(drv.nextRow() == true);
            CHECK(drv.getInt(0) == 1);
            CHECK(drv.getStdString(1) == "Ali");
            REQUIRE(drv.nextRow() == false);
        }

        SUBCASE("with a pointer referential field") {
            sessionConfig.setOneToOneTransientField("user", "id", SessionMock::getUser, SessionMock::setUser);
            session.setUser(&user);

            SqlRepository<SessionMock>::persist(&session);

            drv.execute("select * from user");
            REQUIRE(drv.nextRow() == true);
            CHECK(drv.getInt(0) == 1);
            CHECK(drv.getStdString(1) == "Ali");
            REQUIRE(drv.nextRow() == false);

            drv.execute("select * from session");
            REQUIRE(drv.nextRow() == true);
            CHECK(drv.getInt(0) == 1);
            CHECK(drv.getStdString(1) == "2018-01-23T08:54:22");
            REQUIRE(drv.nextRow() == false);
        }

        SUBCASE("with a reference referential field") {
            sessionConfig.setOneToOneTransientField("user", "id", SessionMock::getStackUser, SessionMock::setStackUser);
            session.setStackUser(user);

            SqlRepository<SessionMock>::persist(&session);

            drv.execute("select * from session");
            REQUIRE(drv.nextRow() == true);
            CHECK(drv.getInt(0) == 1);
            CHECK(drv.getStdString(1) == "2018-01-23T08:54:22");
            REQUIRE(drv.nextRow() == false);

            drv.execute("select * from user");
            REQUIRE(drv.nextRow() == true);
            CHECK(drv.getInt(0) == 1);
            CHECK(drv.getStdString(1) == "Ali");
            REQUIRE(drv.nextRow() == false);
        }
    }

    SUBCASE(" cascading ") {
        UserMock user;
        user.setId(1);
        user.setName("Ali");
        SessionMock session;
        session.setId(1);
        session.setTime("2018-07-09T08:09:44");
        user.setSession(&session);

        SUBCASE(" persist object into database ") {
            userConfig.setOneToOneTransientField("session", "id", UserMock::getSession, UserMock::setSession, CascadeType::Persist);

            SqlRepository<UserMock>::persist(&user);

            drv.execute("select * from user");
            REQUIRE(drv.nextRow() == true);
            CHECK(drv.getInt(0) == 1);
            CHECK(drv.getStdString(1) == "Ali");
            REQUIRE(drv.nextRow() == false);

            drv.execute("select * from session");
            REQUIRE(drv.nextRow() == true);
            CHECK(drv.getInt(0) == 1);
            CHECK(drv.getStdString(1) == "2018-07-09T08:09:44");
            REQUIRE(drv.nextRow() == false);
        }

        SUBCASE(" update object in database ") {
            drv.execute("INSERT INTO user(id, name) values(1, 'Ali')");
            drv.execute("INSERT INTO session(id, time) values(1, '2018-01-23T08:54:22')");

            userConfig.setOneToOneTransientField("session", "id", UserMock::getSession, UserMock::setSession, CascadeType::Update);
            user.getSession()->setTime("2018-07-14T19:24:01");

            SqlRepository<UserMock>::update(&user);

            drv.execute("select * from user");
            REQUIRE(drv.nextRow() == true);
            CHECK(drv.getInt(0) == 1);
            CHECK(drv.getStdString(1) == "Ali");
            REQUIRE(drv.nextRow() == false);

            drv.execute("select * from session");
            REQUIRE(drv.nextRow() == true);
            CHECK(drv.getInt(0) == 1);
            CHECK(drv.getStdString(1) == "2018-07-14T19:24:01");
            REQUIRE(drv.nextRow() == false);
        }

        SUBCASE(" remove object from database ") {
            drv.execute("INSERT INTO user(id, name) values(1, 'Ali')");
            drv.execute("INSERT INTO session(id, time) values(1, '2018-01-23T08:54:22')");

            userConfig.setOneToOneTransientField("session", "id", UserMock::getSession, UserMock::setSession, CascadeType::Remove);
            SqlRepository<UserMock>::remove(&user);

            drv.execute("select * from user");
            REQUIRE(drv.nextRow() == false);

            drv.execute("select * from session");
            REQUIRE(drv.nextRow() == false);
        }

        SUBCASE(" persist, update and remove object ") {
            userConfig.setOneToOneTransientField("session", "id", UserMock::getSession, UserMock::setSession, CascadeType::Persist | CascadeType::Update | CascadeType::Remove);
            std::cout << "persist, update and remove object" << std::endl;
            SqlRepository<UserMock>::persist(&user);

            drv.execute("select * from user");
            REQUIRE(drv.nextRow() == true);
            CHECK(drv.getInt(0) == 1);
            CHECK(drv.getStdString(1) == "Ali");
            REQUIRE(drv.nextRow() == false);

            drv.execute("select * from session");
            REQUIRE(drv.nextRow() == true);
            CHECK(drv.getInt(0) == 1);
            CHECK(drv.getStdString(1) == "2018-07-09T08:09:44");
            REQUIRE(drv.nextRow() == false);

            user.setName("Mary");
            user.getSession()->setTime("2018-07-14T20:36:26");
            SqlRepository<UserMock>::update(&user);

            drv.execute("select * from user");
            REQUIRE(drv.nextRow() == true);
            CHECK(drv.getInt(0) == 1);
            CHECK(drv.getStdString(1) == "Mary");
            REQUIRE(drv.nextRow() == false);

            drv.execute("select * from session");
            REQUIRE(drv.nextRow() == true);
            CHECK(drv.getInt(0) == 1);
            CHECK(drv.getStdString(1) == "2018-07-14T20:36:26");
            REQUIRE(drv.nextRow() == false);

            SqlRepository<UserMock>::remove(&user);

            drv.execute("select * from user");
            REQUIRE(drv.nextRow() == false);

            drv.execute("select * from session");
            REQUIRE(drv.nextRow() == false);
        }
    }
}