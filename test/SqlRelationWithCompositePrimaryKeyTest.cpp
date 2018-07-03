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

TEST_CASE(" SqlRelation with composite primary key ") {
    SqliteDriver drv;
    drv.open(":memory:");

    SUBCASE(" relation columns are in order in both tables ") {

        drv.execute("create table user (id int NOT NULL, name varchar(20), PRIMARY KEY(id, name))");
        drv.execute("create table session (id int NOT NULL PRIMARY KEY, time varchar(20), user_id int, user_name varchar(20), FOREIGN KEY (user_id) REFERENCES USER(id) ON DELETE CASCADE ON UPDATE CASCADE, FOREIGN KEY (user_name) REFERENCES USER(name) ON DELETE CASCADE ON UPDATE CASCADE)");
        drv.execute("create table user_session (user_id int NOT NULL, user_name varchar(20), session_id int NOT NULL, exp varchar(99), PRIMARY KEY(user_id, user_name, session_id), FOREIGN KEY (user_id) REFERENCES USER(id) ON DELETE CASCADE ON UPDATE CASCADE, FOREIGN KEY (user_name) REFERENCES USER(name) ON DELETE CASCADE ON UPDATE CASCADE, FOREIGN KEY (session_id) REFERENCES SESSION(id) ON DELETE CASCADE ON UPDATE CASCADE)");

        SqlEntityConfigurer<UserMock> userConfig;
        userConfig.setDriver(&drv);
        userConfig.setTableName("user");
        userConfig.setPrimaryField("id", UserMock::getId, UserMock::setId);
        userConfig.setPrimaryField("name", UserMock::getName, UserMock::setName);

        SqlEntityConfigurer<SessionMock> sessionConfig;
        sessionConfig.setDriver(&drv);
        sessionConfig.setTableName("session");
        sessionConfig.setPrimaryField("id", SessionMock::getId, SessionMock::setId);
        sessionConfig.setField("time", SessionMock::getTime, SessionMock::setTime);

        SUBCASE(" fetching entities ") {
            drv.execute("INSERT INTO user(id, name) values(1, 'Ali')");
            drv.execute("INSERT INTO user(id, name) values(1, 'Sami')");
            drv.execute("INSERT INTO user(id, name) values(1, 'Omar')");

            drv.execute("INSERT INTO session(id, time, user_id, user_name) values(1, '2018-01-23T08:54:22', 1, 'Ali')");
            drv.execute("INSERT INTO session(id, time, user_id, user_name) values(2, '2018-02-10T05:12:06', 1, 'Sami')");
            drv.execute("INSERT INTO session(id, time, user_id, user_name) values(3, '2018-03-11T17:46:52', 1, 'Ali')");

            drv.execute("INSERT INTO user_session(user_id, user_name, session_id) values(1, 'Ali', 1)");
            drv.execute("INSERT INTO user_session(user_id, user_name, session_id) values(1, 'Sami', 2)");
            drv.execute("INSERT INTO user_session(user_id, user_name, session_id) values(1, 'Ali', 3)");

            SUBCASE(" without foreign fields ") {
                sessionConfig.setOneToOnePersistentField("user",{
                    {"user_id", "id"},
                    {"user_name", "name"}
                }, SessionMock::getUser, SessionMock::setUser);

                UserMock* user = SqlRepository<UserMock>::get({1, "Ali"});
                REQUIRE(user != nullptr);
                CHECK(user->getId() == 1);
                CHECK(user->getName() == "Ali");

                delete user;
            }

            SUBCASE(" one-to-one relation ") {
                sessionConfig.setOneToOnePersistentField("user",{
                    {"user_id", "id"},
                    {"user_name", "name"}
                }, SessionMock::getUser, SessionMock::setUser);

                SessionMock* session1 = SqlRepository<SessionMock>::get(1);

                REQUIRE(session1 != nullptr);
                CHECK(session1->getId() == 1);
                CHECK(session1->getTime() == "2018-01-23T08:54:22");
                REQUIRE(session1->getUser() != nullptr);
                CHECK(session1->getUser()->getId() == 1);
                CHECK(session1->getUser()->getName() == "Ali");

                SessionMock* session2 = SqlRepository<SessionMock>::get(2);

                REQUIRE(session2 != nullptr);
                CHECK(session2->getId() == 2);
                CHECK(session2->getTime() == "2018-02-10T05:12:06");
                REQUIRE(session2->getUser() != nullptr);
                CHECK(session2->getUser()->getId() == 1);
                CHECK(session2->getUser()->getName() == "Sami");

                delete session1->getUser();
                delete session1;

                delete session2->getUser();
                delete session2;
            }

            SUBCASE(" many-to-one relation ") {
                userConfig.setOneToOneTransientField("session",{
                    {"id", "user_id"},
                    {"name", "user_name"}
                }, UserMock::getSession, UserMock::setSession);

                UserMock* user = SqlRepository<UserMock>::get({1, "Sami"});

                REQUIRE(user != nullptr);
                CHECK(user->getId() == 1);
                CHECK(user->getName() == "Sami");
                REQUIRE(user->getSession() != nullptr);
                CHECK(user->getSession()->getId() == 2);
                CHECK(user->getSession()->getTime() == "2018-02-10T05:12:06");

                delete user->getSession();
                delete user;
            }

            SUBCASE(" one-to-many relation ") {
                userConfig.setOneToManyField("session",{
                    {"name", "user_name"},
                    {"id", "user_id"}
                }, UserMock::getSessions, UserMock::setSessions);

                UserMock* user = SqlRepository<UserMock>::get({1, "Ali"});

                REQUIRE(user != nullptr);
                CHECK(user->getId() == 1);
                CHECK(user->getName() == "Ali");
                REQUIRE(user->getSessions().size() == 2);
                CHECK(user->getSessions().at(0)->getId() == 1);
                CHECK(user->getSessions().at(0)->getTime() == "2018-01-23T08:54:22");
                CHECK(user->getSessions().at(1)->getId() == 3);
                CHECK(user->getSessions().at(1)->getTime() == "2018-03-11T17:46:52");
                delete user->getSession();
                delete user;
            }

            SUBCASE(" many-to-many relation ") {
                SqlManyToManyMapping mapping("user", "user_session", "session");
                mapping.setLeftMapping("user_id", "id");
                mapping.setLeftMapping("user_name", "name");
                mapping.setRightMapping("session_id", "id");

                userConfig.setManyToManyField(mapping, UserMock::getSessions, UserMock::setSessions);

                UserMock* user = SqlRepository<UserMock>::get({1, "Ali"});

                REQUIRE(user != nullptr);
                CHECK(user->getId() == 1);
                CHECK(user->getName() == "Ali");
                REQUIRE(user->getSessions().size() == 2);
                CHECK(user->getSessions().at(0)->getId() == 1);
                CHECK(user->getSessions().at(0)->getTime() == "2018-01-23T08:54:22");
                CHECK(user->getSessions().at(1)->getId() == 3);
                CHECK(user->getSessions().at(1)->getTime() == "2018-03-11T17:46:52");

                delete user->getSessions()[0];
                delete user->getSessions()[1];
                delete user;
            }
        }

        SUBCASE(" persisting entities ") {

            SUBCASE(" without foreign fields ") {
                UserMock* user = new UserMock;
                user->setId(1);
                user->setName("Ali");

                SqlRepository<UserMock>::save(user);

                drv.execute("select * from user");
                REQUIRE(drv.nextRow());
                CHECK(drv.getInt(0) == 1);
                CHECK(drv.getStdString(1) == "Ali");

                delete user;
            }

            SUBCASE(" one-to-one relation ") {
                sessionConfig.setOneToOnePersistentField("user",{
                    {"user_id", "id"},
                    {"user_name", "name"}
                }, SessionMock::getUser, SessionMock::setUser);

                UserMock* user = new UserMock;
                user->setId(1);
                user->setName("Sarah");

                SessionMock* session = new SessionMock;
                session->setId(5);
                session->setTime("2018-01-23T08:54:22");
                session->setUser(user);

                SqlRepository<UserMock>::save(user);
                SqlRepository<SessionMock>::save(session);

                drv.execute("select * from user");
                REQUIRE(drv.nextRow());
                CHECK(drv.getInt(0) == 1);
                CHECK(drv.getStdString(1) == "Sarah");

                drv.execute("select * from session");
                REQUIRE(drv.nextRow());
                CHECK(drv.getInt(0) == 5);
                CHECK(drv.getStdString(1) == "2018-01-23T08:54:22");

                delete user;
                delete session;
            }

            SUBCASE(" many-to-one relation ") {
                sessionConfig.setManyToOneField("user",{
                    {"user_id", "id"},
                    {"user_name", "name"}
                }, SessionMock::getUser, SessionMock::setUser);

                UserMock* user = new UserMock;
                user->setId(1);
                user->setName("Sarah");

                SessionMock* session1 = new SessionMock;
                session1->setId(5);
                session1->setTime("2018-01-23T08:54:22");
                session1->setUser(user);

                SessionMock* session2 = new SessionMock;
                session2->setId(6);
                session2->setTime("2018-07-02T12:47:13");
                session2->setUser(user);

                SqlRepository<UserMock>::save(user);

                SqlRepository<SessionMock>::save(session1);
                SqlRepository<SessionMock>::save(session2);

                drv.execute("select * from user");
                REQUIRE(drv.nextRow());
                CHECK(drv.getInt(0) == 1);
                CHECK(drv.getStdString(1) == "Sarah");

                drv.execute("select * from session");
                REQUIRE(drv.nextRow());
                CHECK(drv.getInt(0) == 5);
                CHECK(drv.getStdString(1) == "2018-01-23T08:54:22");
                REQUIRE(drv.nextRow());
                CHECK(drv.getInt(0) == 6);
                CHECK(drv.getStdString(1) == "2018-07-02T12:47:13");

                delete user;
                delete session1;
                delete session2;
            }

            SUBCASE(" one-to-many relation ") {
                userConfig.setOneToManyField("session",{
                    {"name", "user_name"},
                    {"id", "user_id"}
                }, UserMock::getSessions, UserMock::setSessions);

                SessionMock* session1 = new SessionMock;
                session1->setId(7);
                session1->setTime("2018-01-23T08:54:22");

                SessionMock* session2 = new SessionMock;
                session2->setId(8);
                session2->setTime("2018-03-11T17:46:52");

                UserMock* user = new UserMock;
                user->setId(1);
                user->setName("Sarah");
                user->setSessions({session1, session2});

                SqlRepository<SessionMock>::save(session1);
                SqlRepository<SessionMock>::save(session2);
                SqlRepository<UserMock>::save(user);

                drv.execute("select * from user");
                REQUIRE(drv.nextRow());
                CHECK(drv.getInt(0) == 1);
                CHECK(drv.getStdString(1) == "Sarah");

                drv.execute("select * from session");
                REQUIRE(drv.nextRow());
                CHECK(drv.getInt(0) == 7);
                CHECK(drv.getStdString(1) == "2018-01-23T08:54:22");
                REQUIRE(drv.nextRow());
                CHECK(drv.getInt(0) == 8);
                CHECK(drv.getStdString(1) == "2018-03-11T17:46:52");

                delete session1;
                delete session2;
                delete user;
            }

            SUBCASE(" many-to-many relation ") {
                SqlManyToManyMapping mapping("user", "user_session", "session");
                mapping.setLeftMapping("user_id", "id");
                mapping.setLeftMapping("user_name", "name");
                mapping.setRightMapping("session_id", "id");

                userConfig.setManyToManyField(mapping, UserMock::getSessions, UserMock::setSessions);

                SessionMock* session1 = new SessionMock;
                session1->setId(7);
                session1->setTime("2018-01-23T08:54:22");

                SessionMock* session2 = new SessionMock;
                session2->setId(8);
                session2->setTime("2018-03-11T17:46:52");

                UserMock* user = new UserMock;
                user->setId(1);
                user->setName("Sarah");
                user->setSessions({session1, session2});

                SqlRepository<SessionMock>::save(session1);
                SqlRepository<SessionMock>::save(session2);
                SqlRepository<UserMock>::save(user);

                drv.execute("select * from user");
                REQUIRE(drv.nextRow());
                CHECK(drv.getInt(0) == 1);
                CHECK(drv.getStdString(1) == "Sarah");

                drv.execute("select * from session");
                REQUIRE(drv.nextRow());
                CHECK(drv.getInt(0) == 7);
                CHECK(drv.getStdString(1) == "2018-01-23T08:54:22");
                REQUIRE(drv.nextRow());
                CHECK(drv.getInt(0) == 8);
                CHECK(drv.getStdString(1) == "2018-03-11T17:46:52");

                drv.execute("select * from user_session");
                REQUIRE(drv.nextRow());
                CHECK(drv.getInt(0) == 1);
                CHECK(drv.getStdString(1) == "Sarah");
                CHECK(drv.getInt(2) == 7);

                REQUIRE(drv.nextRow());
                CHECK(drv.getInt(0) == 1);
                CHECK(drv.getStdString(1) == "Sarah");
                CHECK(drv.getInt(2) == 8);

                delete session1;
                delete session2;
                delete user;
            }
        }
    }

    SUBCASE(" relation columns are NOT in order in both tables ") {

        drv.execute("create table user (id int NOT NULL, name varchar(20), PRIMARY KEY(id, name))");
        drv.execute("create table session (id int NOT NULL PRIMARY KEY, time varchar(20), user_name varchar(20), user_id int, FOREIGN KEY (user_id) REFERENCES USER(id) ON DELETE CASCADE ON UPDATE CASCADE, FOREIGN KEY (user_name) REFERENCES USER(name) ON DELETE CASCADE ON UPDATE CASCADE)");
        drv.execute("create table user_session (session_id int NOT NULL, user_name varchar(20), user_id int NOT NULL, exp varchar(99), PRIMARY KEY(session_id, user_name, user_id), FOREIGN KEY (session_id) REFERENCES SESSION(id) ON DELETE CASCADE ON UPDATE CASCADE, FOREIGN KEY (user_name) REFERENCES USER(name) ON DELETE CASCADE ON UPDATE CASCADE, FOREIGN KEY (user_id) REFERENCES USER(id) ON DELETE CASCADE ON UPDATE CASCADE)");

        SqlEntityConfigurer<UserMock> userConfig;
        userConfig.setDriver(&drv);
        userConfig.setTableName("user");
        userConfig.setPrimaryField("name", UserMock::getName, UserMock::setName);
        userConfig.setPrimaryField("id", UserMock::getId, UserMock::setId);

        SqlEntityConfigurer<SessionMock> sessionConfig;
        sessionConfig.setDriver(&drv);
        sessionConfig.setTableName("session");
        sessionConfig.setPrimaryField("id", SessionMock::getId, SessionMock::setId);
        sessionConfig.setField("time", SessionMock::getTime, SessionMock::setTime);

        SUBCASE(" fetching entities ") {
            drv.execute("INSERT INTO user(id, name) values(1, 'Ali')");
            drv.execute("INSERT INTO user(id, name) values(1, 'Sami')");
            drv.execute("INSERT INTO user(id, name) values(1, 'Omar')");

            drv.execute("INSERT INTO session(id, time, user_name, user_id) values(1, '2018-01-23T08:54:22', 'Ali', 1)");
            drv.execute("INSERT INTO session(id, time, user_name, user_id) values(2, '2018-02-10T05:12:06', 'Sami', 1)");
            drv.execute("INSERT INTO session(id, time, user_name, user_id) values(3, '2018-03-11T17:46:52', 'Ali', 1)");

            drv.execute("INSERT INTO user_session(session_id, user_name, user_id) values(1, 'Ali', 1)");
            drv.execute("INSERT INTO user_session(session_id, user_name, user_id) values(2, 'Sami', 1)");
            drv.execute("INSERT INTO user_session(session_id, user_name, user_id) values(3, 'Ali', 1)");

            SUBCASE(" one-to-one relation ") {
                sessionConfig.setOneToOnePersistentField("user",{
                    {"user_name", "name"},
                    {"user_id", "id"}
                }, SessionMock::getUser, SessionMock::setUser);

                SessionMock* session = SqlRepository<SessionMock>::get(1);

                REQUIRE(session != nullptr);
                CHECK(session->getId() == 1);
                CHECK(session->getTime() == "2018-01-23T08:54:22");
                REQUIRE(session->getUser() != nullptr);
                CHECK(session->getUser()->getId() == 1);
                CHECK(session->getUser()->getName() == "Ali");

                delete session->getUser();
                delete session;
            }

            SUBCASE(" many-to-one relation ") {
                userConfig.setOneToOneTransientField("session",{
                    {"id", "user_id"},
                    {"name", "user_name"}
                }, UserMock::getSession, UserMock::setSession);

                UserMock* user = SqlRepository<UserMock>::get({"Sami", 1});

                REQUIRE(user != nullptr);
                CHECK(user->getId() == 1);
                CHECK(user->getName() == "Sami");
                REQUIRE(user->getSession() != nullptr);
                CHECK(user->getSession()->getId() == 2);
                CHECK(user->getSession()->getTime() == "2018-02-10T05:12:06");

                delete user->getSession();
                delete user;
            }

            SUBCASE(" one-to-many relation ") {
                userConfig.setOneToManyField("session",{
                    {"id", "user_id"},
                    {"name", "user_name"}
                }, UserMock::getSessions, UserMock::setSessions);

                UserMock* user = SqlRepository<UserMock>::get({"Ali", 1});

                REQUIRE(user != nullptr);
                CHECK(user->getId() == 1);
                CHECK(user->getName() == "Ali");
                REQUIRE(user->getSessions().size() == 2);
                CHECK(user->getSessions().at(0)->getId() == 1);
                CHECK(user->getSessions().at(0)->getTime() == "2018-01-23T08:54:22");
                CHECK(user->getSessions().at(1)->getId() == 3);
                CHECK(user->getSessions().at(1)->getTime() == "2018-03-11T17:46:52");
                delete user->getSession();
                delete user;
            }

            SUBCASE(" many-to-many relation ") {
                SqlManyToManyMapping mapping("user", "user_session", "session");
                mapping.setLeftMapping("user_id", "id");
                mapping.setLeftMapping("user_name", "name");
                mapping.setRightMapping("session_id", "id");

                userConfig.setManyToManyField(mapping, UserMock::getSessions, UserMock::setSessions);

                UserMock* user = SqlRepository<UserMock>::get({"Ali", 1});

                REQUIRE(user != nullptr);
                CHECK(user->getId() == 1);
                CHECK(user->getName() == "Ali");
                REQUIRE(user->getSessions().size() == 2);
                CHECK(user->getSessions().at(0)->getId() == 1);
                CHECK(user->getSessions().at(0)->getTime() == "2018-01-23T08:54:22");
                CHECK(user->getSessions().at(1)->getId() == 3);
                CHECK(user->getSessions().at(1)->getTime() == "2018-03-11T17:46:52");

                delete user->getSessions()[0];
                delete user->getSessions()[1];
                delete user;
            }
        }

        SUBCASE(" persisting entities ") {

            SUBCASE(" without foreign fields ") {
                UserMock* user = new UserMock;
                user->setId(1);
                user->setName("Ali");

                SqlRepository<UserMock>::save(user);

                drv.execute("select * from user");
                REQUIRE(drv.nextRow());
                CHECK(drv.getInt(0) == 1);
                CHECK(drv.getStdString(1) == "Ali");

                delete user;
            }

            SUBCASE(" one-to-one relation ") {
                sessionConfig.setOneToOnePersistentField("user",{
                    {"user_id", "id"},
                    {"user_name", "name"}
                }, SessionMock::getUser, SessionMock::setUser);

                UserMock* user = new UserMock;
                user->setId(1);
                user->setName("Sarah");

                SessionMock* session = new SessionMock;
                session->setId(5);
                session->setTime("2018-01-23T08:54:22");
                session->setUser(user);

                SqlRepository<UserMock>::save(user);
                SqlRepository<SessionMock>::save(session);

                drv.execute("select * from user");
                REQUIRE(drv.nextRow());
                CHECK(drv.getInt(0) == 1);
                CHECK(drv.getStdString(1) == "Sarah");

                drv.execute("select * from session");
                REQUIRE(drv.nextRow());
                CHECK(drv.getInt(0) == 5);
                CHECK(drv.getStdString(1) == "2018-01-23T08:54:22");
                CHECK(drv.getStdString(2) == "Sarah");
                CHECK(drv.getInt(3) == 1);

                delete user;
                delete session;
            }

            SUBCASE(" many-to-one relation ") {
                sessionConfig.setManyToOneField("user",{
                    {"user_id", "id"},
                    {"user_name", "name"}
                }, SessionMock::getUser, SessionMock::setUser);

                UserMock* user = new UserMock;
                user->setId(1);
                user->setName("Sarah");

                SessionMock* session1 = new SessionMock;
                session1->setId(5);
                session1->setTime("2018-01-23T08:54:22");
                session1->setUser(user);

                SessionMock* session2 = new SessionMock;
                session2->setId(6);
                session2->setTime("2018-07-02T12:47:13");
                session2->setUser(user);

                SqlRepository<UserMock>::save(user);

                SqlRepository<SessionMock>::save(session1);
                SqlRepository<SessionMock>::save(session2);

                drv.execute("select * from user");
                REQUIRE(drv.nextRow());
                CHECK(drv.getInt(0) == 1);
                CHECK(drv.getStdString(1) == "Sarah");

                drv.execute("select * from session");
                REQUIRE(drv.nextRow());
                CHECK(drv.getInt(0) == 5);
                CHECK(drv.getStdString(1) == "2018-01-23T08:54:22");
                CHECK(drv.getStdString(2) == "Sarah");
                CHECK(drv.getInt(3) == 1);
                REQUIRE(drv.nextRow());
                CHECK(drv.getInt(0) == 6);
                CHECK(drv.getStdString(1) == "2018-07-02T12:47:13");
                CHECK(drv.getStdString(2) == "Sarah");
                CHECK(drv.getInt(3) == 1);

                delete user;
                delete session1;
                delete session2;
            }

            SUBCASE(" one-to-many relation ") {
                userConfig.setOneToManyField("session",{
                    {"name", "user_name"},
                    {"id", "user_id"}
                }, UserMock::getSessions, UserMock::setSessions);

                SessionMock* session1 = new SessionMock;
                session1->setId(7);
                session1->setTime("2018-01-23T08:54:22");

                SessionMock* session2 = new SessionMock;
                session2->setId(8);
                session2->setTime("2018-03-11T17:46:52");

                UserMock* user = new UserMock;
                user->setId(1);
                user->setName("Sarah");
                user->setSessions({session1, session2});

                SqlRepository<SessionMock>::save(session1);
                SqlRepository<SessionMock>::save(session2);
                SqlRepository<UserMock>::save(user);

                drv.execute("select * from user");
                REQUIRE(drv.nextRow());
                CHECK(drv.getInt(0) == 1);
                CHECK(drv.getStdString(1) == "Sarah");

                drv.execute("select * from session");
                REQUIRE(drv.nextRow());
                CHECK(drv.getInt(0) == 7);
                CHECK(drv.getStdString(1) == "2018-01-23T08:54:22");
                REQUIRE(drv.nextRow());
                CHECK(drv.getInt(0) == 8);
                CHECK(drv.getStdString(1) == "2018-03-11T17:46:52");

                delete session1;
                delete session2;
                delete user;
            }

            SUBCASE(" many-to-many relation ") {
                SqlManyToManyMapping mapping("user", "user_session", "session");
                mapping.setLeftMapping("user_id", "id");
                mapping.setLeftMapping("user_name", "name");
                mapping.setRightMapping("session_id", "id");

                userConfig.setManyToManyField(mapping, UserMock::getSessions, UserMock::setSessions);

                SessionMock* session1 = new SessionMock;
                session1->setId(7);
                session1->setTime("2018-01-23T08:54:22");

                SessionMock* session2 = new SessionMock;
                session2->setId(8);
                session2->setTime("2018-03-11T17:46:52");

                UserMock* user = new UserMock;
                user->setId(1);
                user->setName("Sarah");
                user->setSessions({session1, session2});

                SqlRepository<SessionMock>::save(session1);
                SqlRepository<SessionMock>::save(session2);
                SqlRepository<UserMock>::save(user);

                drv.execute("select * from user");
                REQUIRE(drv.nextRow());
                CHECK(drv.getInt(0) == 1);
                CHECK(drv.getStdString(1) == "Sarah");
                REQUIRE_FALSE(drv.nextRow());

                drv.execute("select * from session");
                REQUIRE(drv.nextRow());
                CHECK(drv.getInt(0) == 7);
                CHECK(drv.getStdString(1) == "2018-01-23T08:54:22");
                REQUIRE(drv.nextRow());
                CHECK(drv.getInt(0) == 8);
                CHECK(drv.getStdString(1) == "2018-03-11T17:46:52");

                drv.execute("select * from user_session");
                REQUIRE(drv.nextRow());
                CHECK(drv.getInt(0) == 7);
                CHECK(drv.getStdString(1) == "Sarah");
                CHECK(drv.getInt(2) == 1);


                REQUIRE(drv.nextRow());
                CHECK(drv.getInt(0) == 8);
                CHECK(drv.getStdString(1) == "Sarah");
                CHECK(drv.getInt(2) == 1);

                delete session1;
                delete session2;
                delete user;
            }
        }
    }
}