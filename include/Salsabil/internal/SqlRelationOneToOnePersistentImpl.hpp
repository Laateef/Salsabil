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

#ifndef SALSABIL_SQLRELATIONONETOONEPERSISTENTIMPL_HPP
#define SALSABIL_SQLRELATIONONETOONEPERSISTENTIMPL_HPP

#include "SqlRelation.hpp"
#include "TypeHelper.hpp"
#include "TypeResolver.hpp"
#include "Logging.hpp"
#include "SqlEntityConfigurer.hpp"
#include "SqlGenerator.hpp"
#include "AccessWrapper.hpp"

namespace Salsabil {
    class SqlDriver;

    template<typename ClassType, typename FieldType>
    class SqlRelationOneToOnePersistentImpl : public SqlRelation<ClassType> {
        using FieldPureType = typename Utility::Traits<FieldType>::UnqualifiedType;

    public:

        SqlRelationOneToOnePersistentImpl(const std::string& targetTableName, const std::string& columnName, RelationType type, AccessWrapper<ClassType, FieldType>* accessWrapper) :
        SqlRelation<ClassType>(targetTableName, type),
        mColumnName(columnName),
        mAccessWrapper(accessWrapper) {
        }

        virtual void readFromDriver(SqlDriver* driver, ClassType* classInstance) {
            std::string sqlStatement = SqlGenerator::fetchById(SqlRelation<ClassType>::tableName(), mColumnName, "?");
            driver->prepare(sqlStatement);
            SALSABIL_LOG_INFO(sqlStatement);

            FieldType fieldInstance;
            mAccessWrapper->get(classInstance, &fieldInstance);

            FieldPureType* pFieldInstance = Utility::pointerizeInstance(&fieldInstance);

            for (std::size_t idx = 0; idx < SqlEntityConfigurer<FieldPureType>::primaryFieldList().size(); ++idx) {
                SqlEntityConfigurer<FieldPureType>::primaryFieldList()[idx]->writeToDriver(pFieldInstance, idx + 1);
            }

            driver->execute();

            if (!driver->nextRow())
                throw Exception("No rows to fetch");

            for (const auto& f : SqlEntityConfigurer<FieldPureType>::persistentFieldList())
                f->readFromDriver(pFieldInstance, f->column());
            for (const auto& r : SqlEntityConfigurer<FieldPureType>::transientFieldList())
                r->readFromDriver(driver, pFieldInstance);

            mAccessWrapper->set(classInstance, &fieldInstance);
        }

        virtual void writeToDriver(SqlDriver*, const ClassType*) {
        }

    private:
        std::string mColumnName;

        std::unique_ptr<AccessWrapper<ClassType, FieldType>> mAccessWrapper;
    };
}
#endif // SALSABIL_SQLRELATIONONETOONEPERSISTENTIMPL_HPP

