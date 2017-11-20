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

#include "gmock/gmock.h"
#include "ClassMock.hpp"
#include "SqlDriver.hpp"
#include "Exception.hpp"
#include "SqlDriverFactory.hpp"
#include "SqlTableConfigurer.hpp"
#include "core/SqlDriverMock.hpp"
#include "ClassMock.hpp"

using namespace ::testing;
using namespace Salsabil;

TEST(SqlTableConfigurer, ThrowsIfDriverIsNull) {
    SqlTableConfigurer<ClassMock> conf;

    ASSERT_THROW(conf.setDriver(nullptr), Exception);
}

TEST(SqlTableConfigurer, ThrowsIfDriverIsNotOpen) {
    SqlDriverMock drv;
    EXPECT_CALL(drv, isOpen()).Times(1).WillOnce(Return(false));

    SqlTableConfigurer<ClassMock> conf;

    ASSERT_THROW(conf.setDriver(&drv), Exception);
}

TEST(SqlTableConfigurer, ThrowsIfTableDoesNotExist) {
    SqlDriverMock drv;
    EXPECT_CALL(drv, isOpen()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(drv, tableList()).Times(1).WillOnce(Return(std::vector<std::string>{}));

    SqlTableConfigurer<ClassMock> conf;
    conf.setDriver(&drv);

    ASSERT_THROW(conf.setTableName("person"), Exception);
}

TEST(SqlTableConfigurer, RegisterFieldThroughAttributePointers) {
    SqlDriverMock drv;
    EXPECT_CALL(drv, isOpen()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(drv, tableList()).Times(1).WillOnce(Return(std::vector<std::string>{"person"}));
    EXPECT_CALL(drv, columnList("person")).Times(3).WillRepeatedly(Return(std::vector<std::string>{"id", "name", "weight"}));

    SqlTableConfigurer<ClassMock> conf;
    conf.setDriver(&drv);
    conf.setTableName("person");
    conf.setField("id", &ClassMock::id);
    conf.setField("name", &ClassMock::name);
    conf.setField("weight", &ClassMock::weight);

    ASSERT_THAT(conf.fieldList().size(), Eq(3u));
    ASSERT_THAT(conf.fieldList().at(0)->name(), StrEq("id"));
    ASSERT_THAT(conf.fieldList().at(0)->column(), Eq(0));
    ASSERT_THAT(conf.fieldList().at(1)->name(), StrEq("name"));
    ASSERT_THAT(conf.fieldList().at(1)->column(), Eq(1));
    ASSERT_THAT(conf.fieldList().at(2)->name(), StrEq("weight"));
    ASSERT_THAT(conf.fieldList().at(2)->column(), Eq(2));
}

TEST(SqlTableConfigurer, RegisterFieldThroughMethodPointers) {
    SqlDriverMock drv;
    EXPECT_CALL(drv, isOpen()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(drv, tableList()).Times(1).WillOnce(Return(std::vector<std::string>{"person"}));
    EXPECT_CALL(drv, columnList("person")).Times(2).WillRepeatedly(Return(std::vector<std::string>{"id", "name"}));

    SqlTableConfigurer<ClassMock> conf;
    conf.setDriver(&drv);
    conf.setTableName("person");

    conf.setField("id", ClassMock::getId, ClassMock::setId);
    conf.setField("name", ClassMock::getName, ClassMock::setName);

    ASSERT_THAT(conf.fieldList().size(), Eq(2u));
    ASSERT_THAT(conf.fieldList().at(0)->name(), StrEq("id"));
    ASSERT_THAT(conf.fieldList().at(0)->column(), Eq(0));
    ASSERT_THAT(conf.fieldList().at(1)->name(), StrEq("name"));
    ASSERT_THAT(conf.fieldList().at(1)->column(), Eq(1));
}

TEST(SqlTableConfigurer, ThrowsIfFieldDoesNotExist) {

    SqlDriverMock drv;
    EXPECT_CALL(drv, isOpen()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(drv, tableList()).Times(1).WillOnce(Return(std::vector<std::string>{"person"}));
    EXPECT_CALL(drv, columnList("person")).Times(2).WillRepeatedly(Return(std::vector<std::string>{"name"}));

    SqlTableConfigurer<ClassMock> conf;
    conf.setDriver(&drv);
    conf.setTableName("person");

    ASSERT_THROW(conf.setField("id", ClassMock::getId, ClassMock::setId), Exception);
    ASSERT_NO_THROW(conf.setField("name", ClassMock::getName, ClassMock::setName));
}

TEST(SqlTableConfigurer, CheckWhetherFieldIsPrimary) {

    SqlDriverMock drv;
    EXPECT_CALL(drv, isOpen()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(drv, tableList()).Times(1).WillOnce(Return(std::vector<std::string>{"person"}));
    EXPECT_CALL(drv, columnList("person")).Times(2).WillRepeatedly(Return(std::vector<std::string>{"id", "name"}));

    SqlTableConfigurer<ClassMock> conf;
    conf.setDriver(&drv);
    conf.setTableName("person");
    conf.setPrimaryField("id", ClassMock::getId, ClassMock::setId);
    conf.setField("name", ClassMock::getName, ClassMock::setName);

    ASSERT_THAT(conf.fieldList().size(), Eq(2u));
    ASSERT_TRUE(conf.fieldList().at(0)->isPrimary());
    ASSERT_FALSE(conf.fieldList().at(1)->isPrimary());
}

TEST(SqlTableConfigurer, ResetStaticConfigurationAfterConstruction) {
    SqlDriverMock drv;
    EXPECT_CALL(drv, isOpen()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(drv, tableList()).Times(1).WillOnce(Return(std::vector<std::string>{"person"}));
    EXPECT_CALL(drv, columnList("person")).Times(2).WillRepeatedly(Return(std::vector<std::string>{"id", "name"}));

    SqlTableConfigurer<ClassMock> conf1;
    conf1.setDriver(&drv);
    conf1.setTableName("person");
    conf1.setPrimaryField("id", ClassMock::getId, ClassMock::setId);
    conf1.setField("name", ClassMock::getName, ClassMock::setName);

    SqlTableConfigurer<ClassMock> conf2;
    ASSERT_TRUE(conf2.tableName().empty());
    ASSERT_THAT(conf2.fieldList().size(), Eq(0));
}