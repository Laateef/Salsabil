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

#ifndef SALSABIL_SQLREPOSITORY_HPP
#define SALSABIL_SQLREPOSITORY_HPP

#include "SqlDriver.hpp"
#include "internal/SqlGenerator.hpp"
#include "internal/TypeResolver.hpp"
#include "internal/SqlValue.hpp"
#include "internal/Logging.hpp"
#include "internal/SqlField.hpp"
#include "SqlEntityConfigurer.hpp"

#include <cassert>

namespace Salsabil {
    template<typename ClassType> class SqlEntityConfigurer;

    template<typename ClassType>
    class SqlRepository {
    public:

        SqlRepository() {
        }

        static ClassType* get(std::initializer_list<SqlValue> idList) {
            if (SqlEntityConfigurer<ClassType>::primaryFieldList().size() == 0)
                throw Exception("Could not fetch data, no primary field is configured.");

            assert(SqlEntityConfigurer<ClassType>::primaryFieldList().size() == idList.size());

            std::map<std::string, std::string> columnValueMap;
            std::initializer_list<SqlValue>::const_iterator idIter = idList.begin();
            for (auto field : SqlEntityConfigurer<ClassType>::primaryFieldList()) {
                columnValueMap.insert({field->name(), idIter->toString()});
                idIter++;
            }

            const std::string& sqlStatement = SqlGenerator::fetchById(SqlEntityConfigurer<ClassType>::tableName(), columnValueMap);

            SALSABIL_LOG_INFO(sqlStatement);

            SqlDriver* driver = SqlEntityConfigurer<ClassType>::driver();

            driver->execute(sqlStatement);

            if (!driver->nextRow())
                throw Exception("no row with id(s) was found");

            ClassType* instance;
            ClassType* pInstance = Utility::initializeInstance(&instance);

            for (auto f : SqlEntityConfigurer<ClassType>::primaryFieldList())
                f->readFromDriver(pInstance, f->column());
            for (const auto& f : SqlEntityConfigurer<ClassType>::fieldList())
                f->readFromDriver(pInstance, f->column());
            for (auto f : SqlEntityConfigurer<ClassType>::relationalPersistentFieldList())
                f->injectInto(pInstance);
            for (auto r : SqlEntityConfigurer<ClassType>::transientFieldList())
                r->readFromDriver(driver, pInstance);
            return instance;
        }

        static ClassType* get(SqlValue id) {
            return get({id});
        }

        static std::vector<ClassType*> getAll() {
            if (SqlEntityConfigurer<ClassType>::primaryFieldList().size() == 0)
                throw Exception("Could not fetch data, no primary field is configured.");

            SqlDriver* driver = SqlEntityConfigurer<ClassType>::driver();

            const std::string& sqlStatement = SqlGenerator::fetchAll(SqlEntityConfigurer<ClassType>::tableName());
            SALSABIL_LOG_INFO(sqlStatement);

            driver->execute(sqlStatement);

            std::vector<ClassType*> instanceList;

            while (driver->nextRow()) {
                ClassType* instance;
                ClassType* pInstance = Utility::initializeInstance(&instance);

                for (const auto& f : SqlEntityConfigurer<ClassType>::primaryFieldList())
                    f->readFromDriver(pInstance, f->column());
                for (const auto& f : SqlEntityConfigurer<ClassType>::fieldList())
                    f->readFromDriver(pInstance, f->column());
                for (const auto& f : SqlEntityConfigurer<ClassType>::relationalPersistentFieldList())
                    f->injectInto(pInstance);
                for (const auto& r : SqlEntityConfigurer<ClassType>::transientFieldList())
                    r->readFromDriver(driver, pInstance);

                instanceList.push_back(instance);
            }

            return instanceList;
        }

        static void save(const ClassType * instance) {
            SqlDriver* driver = SqlEntityConfigurer<ClassType>::driver();

            for (auto relation : SqlEntityConfigurer<ClassType>::transientFieldList()) {
                relation->writeToDriver(driver, instance);
            }

            std::map<std::string, std::string> columnValueMap;
            for (auto field : SqlEntityConfigurer<ClassType>::primaryFieldList())
                columnValueMap.insert({field->name(), field->fetchFromInstance(instance).toString()});
            for (auto field : SqlEntityConfigurer<ClassType>::fieldList())
                columnValueMap.insert({field->name(), field->fetchFromInstance(instance).toString()});

            for (auto field : SqlEntityConfigurer<ClassType>::relationalPersistentFieldList()) {
                auto m = field->parseFrom(instance);
                columnValueMap.insert(m.begin(), m.end());
            }
            const std::string& sqlStatement = SqlGenerator::insert(SqlEntityConfigurer<ClassType>::tableName(), columnValueMap);

            SALSABIL_LOG_INFO(sqlStatement);

            driver->execute(sqlStatement);
        }

        static void remove(const ClassType * instance) {
            SqlDriver* driver = SqlEntityConfigurer<ClassType>::driver();

            std::map<std::string, std::string> primaryColumnValueMap;
            for (auto field : SqlEntityConfigurer<ClassType>::primaryFieldList())
                primaryColumnValueMap.insert({field->name(), field->fetchFromInstance(instance).toString()});

            const std::string& sqlStatement = SqlGenerator::remove(SqlEntityConfigurer<ClassType>::tableName(), primaryColumnValueMap);

            SALSABIL_LOG_INFO(sqlStatement);

            driver->execute(sqlStatement);
        }

        static void update(const ClassType * instance) {
            SqlDriver* driver = SqlEntityConfigurer<ClassType>::driver();

            std::map<std::string, std::string> primaryColumnValueMap;
            for (auto field : SqlEntityConfigurer<ClassType>::primaryFieldList())
                primaryColumnValueMap.insert({field->name(), field->fetchFromInstance(instance).toString()});

            std::map<std::string, std::string> columnValueMap;
            for (auto field : SqlEntityConfigurer<ClassType>::fieldList())
                columnValueMap.insert({field->name(), field->fetchFromInstance(instance).toString()});

            const std::string& sqlStatement = SqlGenerator::update(SqlEntityConfigurer<ClassType>::tableName(), columnValueMap, primaryColumnValueMap);

            SALSABIL_LOG_INFO(sqlStatement);

            driver->execute(sqlStatement);
        }
    };
}

#endif // SALSABIL_SQLREPOSITORY_HPP

