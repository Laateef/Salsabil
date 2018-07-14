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
#include "mocks/ClassMock.hpp"
#include "mocks/SessionMock.hpp"
#include "Exception.hpp"
#include "SqliteDriver.hpp"
#include "SqlDriverFactory.hpp"
#include "SqlEntityConfigurer.hpp"
#include "SqlRepository.hpp"

using namespace Salsabil;

TEST_CASE("SqlRepository") {
    SqliteDriver drv;
    drv.open(":memory:");

    SUBCASE("Single Primary Key") {
        drv.execute("create table person (id int NOT NULL PRIMARY KEY, name varchar(20), weight float)");

        SqlEntityConfigurer<ClassMock> conf;
        conf.setDriver(&drv);
        conf.setTableName("person");

        SUBCASE("ThrowsIfPrimaryFieldNotConfigured") {

            SUBCASE("through attributes") {
                conf.setField("id", &ClassMock::id);
                REQUIRE_THROWS_AS(SqlRepository<ClassMock>::get(1), Exception);
            }

            SUBCASE("through methods") {
                conf.setField("id", ClassMock::getId, ClassMock::setId);
                REQUIRE_THROWS_AS(SqlRepository<ClassMock>::get(1), Exception);
            }
        }

        SUBCASE("ThrowsIfRowWithThatPrimaryKeyNotFound") {

            SUBCASE("through attributes") {
                conf.setPrimaryField("id", &ClassMock::id);
                REQUIRE_THROWS_AS(SqlRepository<ClassMock>::get(1), Exception);
            }

            SUBCASE("through methods") {
                conf.setPrimaryField("id", ClassMock::getId, ClassMock::setId);
                REQUIRE_THROWS_AS(SqlRepository<ClassMock>::get(1), Exception);
            }
        }

        SUBCASE("TestsGetObjectFromDatabase") {
            drv.execute("INSERT INTO person(id, name, weight) values(1, 'Ali', 80.5)");
            drv.execute("INSERT INTO person(id, name, weight) values(2, 'Ruby', 53.8)");

            SUBCASE("through methods") {
                conf.setPrimaryField("id", ClassMock::getId, ClassMock::setId);
                conf.setField("name", ClassMock::getName, ClassMock::setName);
                conf.setField("weight", &ClassMock::getWeight, &ClassMock::setWeight);

                ClassMock *obj = SqlRepository<ClassMock>::get(1);

                CHECK(obj->getId() == 1);
                CHECK(obj->getName() == "Ali");
                CHECK(obj->getWeight() == 80.5f);

                delete obj;
            }

            SUBCASE("through attributes") {
                conf.setPrimaryField("id", &ClassMock::id);
                conf.setField("name", &ClassMock::name);
                conf.setField("weight", &ClassMock::weight);

                ClassMock *obj = SqlRepository<ClassMock>::get(1);

                CHECK(obj->id == 1);
                CHECK(obj->name == "Ali");
                CHECK(obj->weight == 80.5f);

                delete obj;
            }

            SUBCASE("TestsGetAllObjectFromDatabase") {

                SUBCASE("through methods") {
                    conf.setPrimaryField("id", ClassMock::getId, ClassMock::setId);
                    conf.setField("name", ClassMock::getName, ClassMock::setName);
                    conf.setField("weight", &ClassMock::getWeight, &ClassMock::setWeight);

                    std::vector<ClassMock*> objList = SqlRepository<ClassMock>::getAll();

                    REQUIRE(objList.size() == 2);
                    CHECK(objList.at(0)->getId() == 1);
                    CHECK(objList.at(0)->getName() == "Ali");
                    CHECK(objList.at(0)->getWeight() == 80.5f);
                    CHECK(objList.at(1)->getId() == 2);
                    CHECK(objList.at(1)->getName() == "Ruby");
                    CHECK(objList.at(1)->getWeight() == 53.8f);

                    delete objList.at(0);
                    delete objList.at(1);
                }

                SUBCASE("through attributes") {
                    conf.setPrimaryField("id", &ClassMock::id);
                    conf.setField("name", &ClassMock::name);
                    conf.setField("weight", &ClassMock::weight);

                    std::vector<ClassMock*> objList = SqlRepository<ClassMock>::getAll();

                    REQUIRE(objList.size() == 2);
                    CHECK(objList.at(0)->id == 1);
                    CHECK(objList.at(0)->name == "Ali");
                    CHECK(objList.at(0)->weight == 80.5f);
                    CHECK(objList.at(1)->id == 2);
                    CHECK(objList.at(1)->name == "Ruby");
                    CHECK(objList.at(1)->weight == 53.8f);

                    delete objList.at(0);
                    delete objList.at(1);
                }
            }
        }

        SUBCASE("TestsSaveObjectToDatabase") {
            ClassMock obj;
            obj.setId(1);
            obj.setName("Ali");
            obj.setWeight(80.5f);

            SUBCASE("through methods") {
                conf.setPrimaryField("id", ClassMock::getId, ClassMock::setId);
                conf.setField("name", ClassMock::getName, ClassMock::setName);
                conf.setField("weight", &ClassMock::getWeight, &ClassMock::setWeight);

                SqlRepository<ClassMock>::save(&obj);

                drv.execute("select * from person");
                REQUIRE(drv.nextRow() == true);
                CHECK(drv.getInt(0) == 1);
                CHECK(drv.getStdString(1) == "Ali");
                CHECK(drv.getFloat(2) == 80.5f);
                REQUIRE(drv.nextRow() == false);
            }

            SUBCASE("through attributes") {
                conf.setPrimaryField("id", &ClassMock::id);
                conf.setField("name", &ClassMock::name);
                conf.setField("weight", &ClassMock::weight);

                SqlRepository<ClassMock>::save(&obj);

                drv.execute("select * from person");
                REQUIRE(drv.nextRow() == true);
                CHECK(drv.getInt(0) == 1);
                CHECK(drv.getStdString(1) == "Ali");
                CHECK(drv.getFloat(2) == 80.5f);
                REQUIRE(drv.nextRow() == false);
            }
        }
    }

    SUBCASE("Composite Primary Key") {
        drv.execute("create table person (id int NOT NULL, name varchar(20) NOT NULL, weight float, PRIMARY KEY(id, name))");
        drv.execute("INSERT INTO person(id, name, weight) values(1, 'Ali', 80.5)");
        drv.execute("INSERT INTO person(id, name, weight) values(2, 'John', 53.8)");
        drv.execute("INSERT INTO person(id, name, weight) values(2, 'Tom', 12.1)");

        SqlEntityConfigurer<ClassMock> conf;
        conf.setDriver(&drv);
        conf.setTableName("person");
        conf.setPrimaryField("id", &ClassMock::id);
        conf.setPrimaryField("name", &ClassMock::name);
        conf.setField("weight", &ClassMock::weight);

        ClassMock *obj = SqlRepository<ClassMock>::get({2, "Tom"});

        REQUIRE(obj != nullptr);
        CHECK(obj->id == 2);
        CHECK(obj->name == "Tom");
        CHECK(obj->weight == 12.1f);

        delete obj;
    }

    SUBCASE(" remove object from database ") {
        drv.execute("create table person (id int NOT NULL PRIMARY KEY, name varchar(20), weight float)");
        drv.execute("INSERT INTO person(id, name, weight) values(1, 'Ali', 80.5)");
        drv.execute("INSERT INTO person(id, name, weight) values(2, 'Ruby', 53.8)");

        SqlEntityConfigurer<ClassMock> conf;
        conf.setDriver(&drv);
        conf.setTableName("person");
        conf.setPrimaryField("id", ClassMock::getId, ClassMock::setId);
        conf.setField("name", ClassMock::getName, ClassMock::setName);
        conf.setField("weight", &ClassMock::getWeight, &ClassMock::setWeight);

        ClassMock obj;
        obj.setId(1);
        obj.setName("Ali");
        obj.setWeight(80.5f);
        SqlRepository<ClassMock>::remove(&obj);

        drv.execute("select * from person");
        REQUIRE(drv.nextRow() == true);
        CHECK(drv.getInt(0) == 2);
        CHECK(drv.getStdString(1) == "Ruby");
        CHECK(drv.getFloat(2) == 53.8f);
        REQUIRE(drv.nextRow() == false);
    }

    SUBCASE(" update object in database ") {
        drv.execute("create table person (id int NOT NULL PRIMARY KEY, name varchar(20), weight float)");
        drv.execute("INSERT INTO person(id, name, weight) values(1, 'Ali', 80.5)");

        SqlEntityConfigurer<ClassMock> conf;
        conf.setDriver(&drv);
        conf.setTableName("person");
        conf.setPrimaryField("id", ClassMock::getId, ClassMock::setId);
        conf.setField("name", ClassMock::getName, ClassMock::setName);
        conf.setField("weight", &ClassMock::getWeight, &ClassMock::setWeight);

        ClassMock obj;
        obj.setId(1);
        obj.setName("Sami");
        obj.setWeight(105.5f);
        SqlRepository<ClassMock>::update(&obj);

        drv.execute("select * from person");
        REQUIRE(drv.nextRow() == true);
        CHECK(drv.getInt(0) == 1);
        CHECK(drv.getStdString(1) == "Sami");
        CHECK(drv.getFloat(2) == 105.5f);
        REQUIRE(drv.nextRow() == false);
    }
}