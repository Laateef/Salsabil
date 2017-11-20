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

#ifndef SALSABIL_SQLREPOSITORY_HPP
#define SALSABIL_SQLREPOSITORY_HPP

#include "SqlTableConfigurer.hpp"
#include "SqlStatementBuilder.hpp"


namespace Salsabil {

    template<typename C>
    class SqlRepository {
    public:

        explicit SqlRepository() {

        }

        static C* get(std::string id) {
            SqlDriver* driver = SqlTableConfigurer<C>::driver();

            for (const auto& field : SqlTableConfigurer<C>::fieldList()) {
                if (field->isPrimary()) {
                    driver->execute(SqlStatementBuilder().SELECT_ALL_FROM(SqlTableConfigurer<C>::tableName()).WHERE(field->name()).equalTo(id).asString());

                    if (!driver->nextRow())
                        throw Exception("no row with id " + id + " was found");

                    C *instance = new C;

                    for (const auto& field : SqlTableConfigurer<C>::fieldList())
                        field->readFromDriver(driver, instance);

                    return instance;
                }
            }

            throw Exception("Could not fetch data, no primary field is configured.");
        }

        static std::vector<C*> getAll() {
            SqlDriver* driver = SqlTableConfigurer<C>::driver();

            for (const auto& field : SqlTableConfigurer<C>::fieldList()) {
                if (field->isPrimary()) {
                    driver->execute(SqlStatementBuilder().SELECT_ALL_FROM(SqlTableConfigurer<C>::tableName()).asString());

                    std::vector<C*> instanceList;

                    while (driver->nextRow()) {
                        C *instance = new C;

                        for (const auto& field : SqlTableConfigurer<C>::fieldList())
                            field->readFromDriver(driver, instance);

                        instanceList.push_back(instance);
                    }

                    return instanceList;
                }
            }

            throw Exception("Could not fetch data, no primary field is configured.");
        }

        static void save(const C* instance) {
            SqlDriver* driver = SqlTableConfigurer<C>::driver();
            driver->prepare(SqlStatementBuilder().INSERT_INTO(SqlTableConfigurer<C>::tableName(), SqlTableConfigurer<C>::fieldNameList()).parameterizeValues().asString());
            for (const auto& field : SqlTableConfigurer<C>::fieldList()) {
                field->writeToDriver(instance, driver);
            }
            driver->execute();
        }
    };
}

#endif // SALSABIL_SQLREPOSITORY_HPP

