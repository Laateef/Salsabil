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
#include "internal/Logging.hpp"

namespace Salsabil {
    template<typename ClassType> class SqlEntityConfigurer;

    template<typename ClassType>
    class SqlRepository {
    public:

        SqlRepository() {
        }

        static ClassType* get(std::string id) {
            SALSABIL_LOG_DEBUG("Getting object from repository with id '" + id + "' ");

            if (SqlEntityConfigurer<ClassType>::primaryFieldList().size() == 0)
                throw Exception("Could not fetch data, no primary field is configured.");

            SqlDriver* driver = SqlEntityConfigurer<ClassType>::driver();

            const std::string& sqlStatement = SqlGenerator::fetchById(SqlEntityConfigurer<ClassType>::tableName(), SqlEntityConfigurer<ClassType>::primaryFieldList().at(0)->name(), id);

            SALSABIL_LOG_INFO(sqlStatement);

            driver->execute(sqlStatement);

            if (!driver->nextRow())
                throw Exception("no row with id(s) was found");

            ClassType* instance;
            ClassType* pInstance = Utility::initializeInstance(&instance);

            for (const auto& f : SqlEntityConfigurer<ClassType>::primaryFieldList())
                f->readFromDriver(pInstance, f->column());
            for (const auto& f : SqlEntityConfigurer<ClassType>::persistentFieldList())
                f->readFromDriver(pInstance, f->column());
            for (const auto& r : SqlEntityConfigurer<ClassType>::transientFieldList())
                r->readFromDriver(driver, pInstance);
            return instance;
        }

        static std::vector<ClassType*> getAll() {
            if (SqlEntityConfigurer<ClassType>::primaryFieldList().size() == 0)
                throw Exception("Could not fetch data, no primary field is configured.");

            SqlDriver* driver = SqlEntityConfigurer<ClassType>::driver();

            //            assert(SqlTableConfigurer<C>::fieldList().size() != 0);

            const std::string& sqlStatement = SqlGenerator::fetchAll(SqlEntityConfigurer<ClassType>::tableName());
            SALSABIL_LOG_INFO(sqlStatement);

            driver->execute(sqlStatement);

            std::vector<ClassType*> instanceList;

            while (driver->nextRow()) {
                ClassType* instance;
                ClassType* pInstance = Utility::initializeInstance(&instance);

                for (const auto& f : SqlEntityConfigurer<ClassType>::primaryFieldList())
                    f->readFromDriver(pInstance, f->column());
                for (const auto& f : SqlEntityConfigurer<ClassType>::persistentFieldList())
                    f->readFromDriver(pInstance, f->column());
                for (const auto& r : SqlEntityConfigurer<ClassType>::transientFieldList())
                    r->readFromDriver(driver, pInstance);

                instanceList.push_back(instance);
            }

            return instanceList;
        }

        static void save(const ClassType* instance) {
            SqlDriver* driver = SqlEntityConfigurer<ClassType>::driver();

            for (const auto& relation : SqlEntityConfigurer<ClassType>::transientFieldList()) {
                relation->writeToDriver(driver, instance);
            }

            const std::string& sqlStatement = SqlGenerator::insert(SqlEntityConfigurer<ClassType>::tableName(), driver->columnList(SqlEntityConfigurer<ClassType>::tableName()));
            SALSABIL_LOG_INFO(sqlStatement);

            driver->prepare(sqlStatement);

            for (const auto& field : SqlEntityConfigurer<ClassType>::primaryFieldList()) {
                field->writeToDriver(instance, field->column() + 1);
            }
            for (const auto& field : SqlEntityConfigurer<ClassType>::persistentFieldList()) {
                field->writeToDriver(instance, field->column() + 1);
            }
            driver->execute();

        }
    };
}

#endif // SALSABIL_SQLREPOSITORY_HPP

