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
#include "Exception.hpp"
#include "core/SqlDriverMock.hpp"
#include "SqlDriverFactory.hpp"
#include "SqlTableConfigurer.hpp"
#include "SqlRepository.hpp"

using namespace ::testing;
using namespace Salsabil;

TEST(SqlRepository, ThrowsIfPrimaryFieldNotConfigured) {
    SqlDriverMock drv;
    EXPECT_CALL(drv, isOpen()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(drv, tableList()).Times(1).WillOnce(Return(std::vector<std::string>{"user"}));
    EXPECT_CALL(drv, columnList("user")).Times(2).WillRepeatedly(Return(std::vector<std::string>{"id", "name"}));

    SqlTableConfigurer<ClassMock> conf;
    conf.setDriver(&drv);
    conf.setTableName("user");
    conf.setField("id", ClassMock::getId, ClassMock::setId);
    conf.setField("name", ClassMock::getName, ClassMock::setName);

    ASSERT_THROW(SqlRepository<ClassMock>::get("1"), Exception);
}

TEST(SqlRepository, ThrowsIfRowWithThatPrimaryKeyNotFound) {
    SqlDriverMock drv;
    EXPECT_CALL(drv, isOpen()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(drv, tableList()).Times(1).WillOnce(Return(std::vector<std::string>{"user"}));
    EXPECT_CALL(drv, columnList("user")).Times(2).WillRepeatedly(Return(std::vector<std::string>{"id", "name"}));
    EXPECT_CALL(drv, execute(_)).Times(1);
    EXPECT_CALL(drv, nextRow()).Times(1).WillOnce(Return(false));

    SqlTableConfigurer<ClassMock> conf;
    conf.setDriver(&drv);
    conf.setTableName("user");
    conf.setPrimaryField("id", ClassMock::getId, ClassMock::setId);
    conf.setField("name", ClassMock::getName, ClassMock::setName);

    ASSERT_THROW(SqlRepository<ClassMock>::get("1"), Exception);
}

TEST(SqlRepository, TestsGetObjectFromDatabaseThroughAttributePointers) {
    SqlDriverMock drv;
    EXPECT_CALL(drv, isOpen()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(drv, tableList()).Times(1).WillOnce(Return(std::vector<std::string>{"user"}));
    EXPECT_CALL(drv, columnList("user")).Times(3).WillRepeatedly(Return(std::vector<std::string>{"id", "name", "weight"}));
    EXPECT_CALL(drv, execute(_)).Times(1);
    EXPECT_CALL(drv, nextRow()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(drv, getInt(0)).Times(1).WillOnce(Return(3));
    EXPECT_CALL(drv, getStdString(1)).Times(1).WillOnce(Return("some name"));
    EXPECT_CALL(drv, getFloat(2)).Times(1).WillOnce(Return(54.5));


    SqlTableConfigurer<ClassMock> conf;
    conf.setDriver(&drv);
    conf.setTableName("user");
    conf.setPrimaryField("id", &ClassMock::id);
    conf.setField("name", &ClassMock::name);
    conf.setField("weight", &ClassMock::weight);

    ClassMock *obj = SqlRepository<ClassMock>::get("3");
    ASSERT_THAT(obj->id, Eq(3u));
    ASSERT_THAT(obj->name, StrEq("some name"));
    ASSERT_THAT(obj->weight, Eq(54.5f));

    delete obj;
}

TEST(SqlRepository, TestsGetObjectFromDatabase) {
    SqlDriverMock drv;
    EXPECT_CALL(drv, isOpen()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(drv, tableList()).Times(1).WillOnce(Return(std::vector<std::string>{"user"}));
    EXPECT_CALL(drv, columnList("user")).Times(2).WillRepeatedly(Return(std::vector<std::string>{"id", "name"}));
    EXPECT_CALL(drv, execute(_)).Times(1);
    EXPECT_CALL(drv, nextRow()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(drv, getInt(0)).Times(1).WillOnce(Return(3));
    EXPECT_CALL(drv, getStdString(1)).Times(1).WillOnce(Return("some name"));


    SqlTableConfigurer<ClassMock> conf;
    conf.setDriver(&drv);
    conf.setTableName("user");
    conf.setPrimaryField("id", ClassMock::getId, ClassMock::setId);
    conf.setField("name", ClassMock::getName, ClassMock::setName);

    ClassMock *obj = SqlRepository<ClassMock>::get("3");
    ASSERT_THAT(obj->getId(), Eq(3u));
    ASSERT_THAT(obj->getName(), StrEq("some name"));

    delete obj;
}

TEST(SqlRepository, TestsGetAllObjectFromDatabase) {
    SqlDriverMock drv;
    EXPECT_CALL(drv, isOpen()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(drv, tableList()).Times(1).WillOnce(Return(std::vector<std::string>{"user"}));
    EXPECT_CALL(drv, columnList("user")).Times(2).WillRepeatedly(Return(std::vector<std::string>{"id", "name"}));
    EXPECT_CALL(drv, execute(_)).Times(1);
    EXPECT_CALL(drv, nextRow()).Times(3).WillOnce(Return(true)).WillOnce(Return(true)).WillOnce(Return(false));
    EXPECT_CALL(drv, getInt(0)).Times(2).WillOnce(Return(2)).WillOnce(Return(3));
    EXPECT_CALL(drv, getStdString(1)).Times(2).WillOnce(Return("name2")).WillOnce(Return("name3"));


    SqlTableConfigurer<ClassMock> conf;
    conf.setDriver(&drv);
    conf.setTableName("user");
    conf.setPrimaryField("id", ClassMock::getId, ClassMock::setId);
    conf.setField("name", ClassMock::getName, ClassMock::setName);

    std::vector<ClassMock*> objList = SqlRepository<ClassMock>::getAll();
    ASSERT_THAT(objList.size(), Eq(2u));
    ASSERT_THAT(objList.at(0)->getId(), Eq(2u));
    ASSERT_THAT(objList.at(0)->getName(), StrEq("name2"));
    ASSERT_THAT(objList.at(1)->getId(), Eq(3u));
    ASSERT_THAT(objList.at(1)->getName(), StrEq("name3"));

    delete objList.at(0);
    delete objList.at(1);
}

TEST(SqlRepository, TestsSaveObjectToDatabase) {
    SqlDriverMock drv;
    EXPECT_CALL(drv, isOpen()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(drv, tableList()).Times(1).WillOnce(Return(std::vector<std::string>{"user"}));
    EXPECT_CALL(drv, columnList("user")).Times(2).WillRepeatedly(Return(std::vector<std::string>{"id", "name"}));
    EXPECT_CALL(drv, prepare("INSERT INTO user (id, name) VALUES (?, ?)")).Times(1);
    EXPECT_CALL(drv, bindInt(0, 34)).Times(1);
    EXPECT_CALL(drv, bindStdString(1, "something")).Times(1);
    EXPECT_CALL(drv, execute()).Times(1);


    SqlTableConfigurer<ClassMock> conf;
    conf.setDriver(&drv);
    conf.setTableName("user");
    conf.setPrimaryField("id", ClassMock::getId, ClassMock::setId);
    conf.setField("name", ClassMock::getName, ClassMock::setName);

    ClassMock obj;
    obj.setId(34);
    obj.setName("something");

    SqlRepository<ClassMock>::save(&obj);
}