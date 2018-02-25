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
    drv.execute("create table person (id int NOT NULL PRIMARY KEY, name varchar(20), weight float)");

    SqlEntityConfigurer<ClassMock> conf;
    conf.setDriver(&drv);
    conf.setTableName("person");

    SUBCASE("ThrowsIfPrimaryFieldNotConfigured") {

        SUBCASE("through attributes") {
            conf.setField("id", &ClassMock::id);
            REQUIRE_THROWS_AS(SqlRepository<ClassMock>::get("1"), Exception);
        }

        SUBCASE("through methods") {
            conf.setField("id", ClassMock::getId, ClassMock::setId);
            REQUIRE_THROWS_AS(SqlRepository<ClassMock>::get("1"), Exception);
        }
    }

    SUBCASE("ThrowsIfRowWithThatPrimaryKeyNotFound") {

        SUBCASE("through attributes") {
            conf.setPrimaryField("id", &ClassMock::id);
            REQUIRE_THROWS_AS(SqlRepository<ClassMock>::get("1"), Exception);
        }

        SUBCASE("through methods") {
            conf.setPrimaryField("id", ClassMock::getId, ClassMock::setId);
            REQUIRE_THROWS_AS(SqlRepository<ClassMock>::get("1"), Exception);
        }
    }

    SUBCASE("TestsGetObjectFromDatabase") {

        drv.execute("INSERT INTO person(id, name, weight) values(1, 'Ali', 80.5)");
        drv.execute("INSERT INTO person(id, name, weight) values(2, 'Ruby', 53.8)");

        SUBCASE("through methods") {
            conf.setPrimaryField("id", ClassMock::getId, ClassMock::setId);
            conf.setField("name", ClassMock::getName, ClassMock::setName);
            conf.setField("weight", &ClassMock::getWeight, &ClassMock::setWeight);

            ClassMock *obj = SqlRepository<ClassMock>::get("1");
            REQUIRE(obj->getId() == 1);
            REQUIRE(obj->getName() == "Ali");
            REQUIRE(obj->getWeight() == 80.5f);

            delete obj;
        }

        SUBCASE("through attributes") {
            conf.setPrimaryField("id", &ClassMock::id);
            conf.setField("name", &ClassMock::name);
            conf.setField("weight", &ClassMock::weight);

            ClassMock *obj = SqlRepository<ClassMock>::get("1");
            REQUIRE(obj->id == 1);
            REQUIRE(obj->name == "Ali");
            REQUIRE(obj->weight == 80.5f);

            delete obj;
        }

        SUBCASE("TestsGetAllObjectFromDatabase") {

            SUBCASE("through methods") {
                conf.setPrimaryField("id", ClassMock::getId, ClassMock::setId);
                conf.setField("name", ClassMock::getName, ClassMock::setName);
                conf.setField("weight", &ClassMock::getWeight, &ClassMock::setWeight);

                std::vector<ClassMock*> objList = SqlRepository<ClassMock>::getAll();
                REQUIRE(objList.size() == 2);
                REQUIRE(objList.at(0)->getId() == 1);
                REQUIRE(objList.at(0)->getName() == "Ali");
                REQUIRE(objList.at(0)->getWeight() == 80.5f);
                REQUIRE(objList.at(1)->getId() == 2);
                REQUIRE(objList.at(1)->getName() == "Ruby");
                REQUIRE(objList.at(1)->getWeight() == 53.8f);

                delete objList.at(0);
                delete objList.at(1);
            }

            SUBCASE("through attributes") {
                conf.setPrimaryField("id", &ClassMock::id);
                conf.setField("name", &ClassMock::name);
                conf.setField("weight", &ClassMock::weight);

                std::vector<ClassMock*> objList = SqlRepository<ClassMock>::getAll();
                REQUIRE(objList.size() == 2);
                REQUIRE(objList.at(0)->id == 1);
                REQUIRE(objList.at(0)->name == "Ali");
                REQUIRE(objList.at(0)->weight == 80.5f);
                REQUIRE(objList.at(1)->id == 2);
                REQUIRE(objList.at(1)->name == "Ruby");
                REQUIRE(objList.at(1)->weight == 53.8f);

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
            REQUIRE(drv.getInt(0) == 1);
            REQUIRE(drv.getStdString(1) == "Ali");
            REQUIRE(drv.getFloat(2) == 80.5f);
            REQUIRE(drv.nextRow() == false);
        }

        SUBCASE("through attributes") {
            conf.setPrimaryField("id", &ClassMock::id);
            conf.setField("name", &ClassMock::name);
            conf.setField("weight", &ClassMock::weight);

            SqlRepository<ClassMock>::save(&obj);
            drv.execute("select * from person");
            REQUIRE(drv.nextRow() == true);
            REQUIRE(drv.getInt(0) == 1);
            REQUIRE(drv.getStdString(1) == "Ali");
            REQUIRE(drv.getFloat(2) == 80.5f);
            REQUIRE(drv.nextRow() == false);
        }
    }
}