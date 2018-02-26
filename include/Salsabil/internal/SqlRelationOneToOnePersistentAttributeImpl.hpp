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

#ifndef SALSABIL_SQLRELATIONONETOONEPERSISTENTATTRIBUTEIMPL_HPP
#define SALSABIL_SQLRELATIONONETOONEPERSISTENTATTRIBUTEIMPL_HPP

#include "SqlRelation.hpp"
#include "TypeHelper.hpp"
#include "TypeResolver.hpp"
#include "Logging.hpp"
#include "SqlEntityConfigurer.hpp"
#include "SqlGenerator.hpp"

namespace Salsabil {
    class SqlDriver;

    template<class ClassType, typename AttributeType>
    class SqlRelationOneToOnePersistentAttributeImpl : public SqlRelation<ClassType> {
        using FieldType = typename Utility::Traits<AttributeType>::AttributeType;
        using FieldPureType = typename Utility::Traits<FieldType>::UnqualifiedType;
    public:

        SqlRelationOneToOnePersistentAttributeImpl(const std::string& targetTableName, const std::string& targetColumnName, RelationType type, AttributeType attribute) :
        SqlRelation<ClassType>(targetTableName, targetColumnName, type),
        mAttribute(attribute) {
        }

        virtual void readFromDriver(SqlDriver* driver, ClassType* classInstance) {
            SALSABIL_LOG_DEBUG("SqlRelationOneToOnePersistentAttributeImpl, reading field '" + SqlRelation<ClassType>::fieldName() + "' from driver ");

            std::string sqlStatement = SqlGenerator::fetchById(SqlRelation<ClassType>::tableName(), SqlRelation<ClassType>::fieldName(), "?");
            driver->prepare(sqlStatement);
            SALSABIL_LOG_INFO(sqlStatement);

            FieldType ins = classInstance->*getAttribute();
            FieldPureType* fieldInstance = Utility::pointerizeInstance(&ins);

            for (std::size_t idx = 0; idx < SqlEntityConfigurer<FieldPureType>::primaryFieldList().size(); ++idx) {
                SqlEntityConfigurer<FieldPureType>::primaryFieldList()[idx]->writeToDriver(fieldInstance, idx + 1);
            }

            driver->execute();

            if (!driver->nextRow())
                throw Exception("No rows to fetch");

            for (const auto& f : SqlEntityConfigurer<FieldPureType>::persistentFieldList())
                f->readFromDriver(fieldInstance, f->column());
            for (const auto& r : SqlEntityConfigurer<FieldPureType>::transientFieldList())
                r->readFromDriver(driver, fieldInstance);

            classInstance->*getAttribute() = ins;
        }

        virtual void writeToDriver(SqlDriver*, const ClassType*) {
        }

        void setAttribute(AttributeType attribute) {
            mAttribute = attribute;
        }

        AttributeType getAttribute() const {
            return mAttribute;
        }

    private:
        AttributeType mAttribute;
    };
}
#endif // SALSABIL_SQLRELATIONONETOONEPERSISTENTATTRIBUTEIMPL_HPP

