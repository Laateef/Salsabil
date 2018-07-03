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
#include "Exception.hpp"
#include "SqlEntityConfigurer.hpp"
#include "SqliteDriver.hpp"

using namespace Salsabil;

TEST_CASE("SqlEntityConfigurer") {
    SqlEntityConfigurer<ClassMock> conf;

    SUBCASE("ThrowsIfDriverIsNull") {
        REQUIRE_THROWS_AS(conf.setDriver(nullptr), Exception);
    }

    SqliteDriver drv;

    SUBCASE("ThrowsIfDriverIsNotOpen") {
        REQUIRE_THROWS_AS(conf.setDriver(&drv), Exception);
    }

    drv.open(":memory:");
    conf.setDriver(&drv);

    SUBCASE("ThrowsIfTableDoesNotExist") {
        REQUIRE_THROWS_AS(conf.setTableName("person"), Exception);
    }

    drv.execute("create table person (id int NOT NULL PRIMARY KEY, name varchar(20), weight float)");

    conf.setTableName("person");

    SUBCASE("RegisterFieldThroughAttributePointers") {
        conf.setField("id", &ClassMock::id);
        conf.setField("name", &ClassMock::name);
        conf.setField("weight", &ClassMock::weight);

        REQUIRE(conf.fieldList().size() == 3u);
        REQUIRE(conf.fieldList().at(0)->name() == "id");
        REQUIRE(conf.fieldList().at(0)->column() == 0);
        REQUIRE(conf.fieldList().at(1)->name() == "name");
        REQUIRE(conf.fieldList().at(1)->column() == 1);
        REQUIRE(conf.fieldList().at(2)->name() == "weight");
        REQUIRE(conf.fieldList().at(2)->column() == 2);
    }

    SUBCASE("RegisterFieldThroughMethodPointers") {
        conf.setField("id", ClassMock::getId, ClassMock::setId);
        conf.setField("name", ClassMock::getName, ClassMock::setName);

        REQUIRE(conf.fieldList().size() == 2u);
        REQUIRE(conf.fieldList().at(0)->name() == "id");
        REQUIRE(conf.fieldList().at(0)->column() == 0);
        REQUIRE(conf.fieldList().at(1)->name() == "name");
        REQUIRE(conf.fieldList().at(1)->column() == 1);
    }

    SUBCASE("ThrowsIfFieldDoesNotExist") {
        REQUIRE_THROWS_AS(conf.setField("idfield", ClassMock::getId, ClassMock::setId), Exception);
        REQUIRE_THROWS_AS(conf.setField("namefield", &ClassMock::name), Exception);
    }

    SUBCASE("CheckWhetherFieldIsPrimary") {
        conf.setPrimaryField("id", ClassMock::getId, ClassMock::setId);
        conf.setField("name", ClassMock::getName, ClassMock::setName);

        REQUIRE(conf.fieldList().size() == 1u);
        REQUIRE(conf.primaryFieldList().size() == 1);
    }

    SUBCASE("ResetStaticConfigurationAfterConstruction") {
        conf.setPrimaryField("id", ClassMock::getId, ClassMock::setId);
        conf.setField("name", ClassMock::getName, ClassMock::setName);

        SqlEntityConfigurer<ClassMock> conf2;
        REQUIRE(conf2.tableName().empty());
        REQUIRE(conf2.fieldList().size() == 0);
    }
}