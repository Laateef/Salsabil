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

#ifndef SALSABIL_SQLONETOONERELATIONTRANSIENTATTRIBUTEIMPL_HPP
#define SALSABIL_SQLONETOONERELATIONTRANSIENTATTRIBUTEIMPL_HPP

#include "SqlRelation.hpp"
#include "TypeHelper.hpp"
#include "TypeResolver.hpp"
#include "SqlEntityConfigurer.hpp"
#include "SqlGenerator.hpp"
#include "Logging.hpp"

namespace Salsabil {
    class SqlDriver;

    template<class ClassType, typename AttributeType>
    class SqlRelationOneToOneTransientAttributeImpl : public SqlRelation<ClassType> {
        using FieldType = typename Utility::Traits<AttributeType>::AttributeType;
        using FieldPureType = typename Utility::Traits<FieldType>::UnqualifiedType;

    public:

        SqlRelationOneToOneTransientAttributeImpl(const std::string& foreignTableName, const std::string& foreignFieldName, RelationType type, AttributeType attribute) :
        SqlRelation<ClassType>(foreignTableName, foreignFieldName, type),
        mAttribute(attribute) {
        }

        virtual void readFromDriver(SqlDriver* driver, ClassType* classInstance) {
            SALSABIL_LOG_DEBUG("SqlRelationOneToOneTransientAttributeImpl, reading field '" + SqlRelation<ClassType>::fieldName() + "' from driver ");

            std::string sqlStatement = SqlGenerator::fetchById(SqlRelation<ClassType>::tableName(), SqlRelation<ClassType>::fieldName(), "?");
            driver->prepare(sqlStatement);
            SALSABIL_LOG_INFO(sqlStatement);
            for (const auto& f : SqlEntityConfigurer<ClassType>::primaryFieldList()) {
                f->writeToDriver(classInstance, 1);
            }
            driver->execute();

            if (!driver->nextRow())
                throw Exception("no row with id(s) was found");

            FieldType fieldInstance;
            FieldPureType* pfInstance = Utility::initializeInstance(&fieldInstance);

            for (const auto& f : SqlEntityConfigurer<FieldPureType>::primaryFieldList())
                f->readFromDriver(pfInstance, f->column());
            for (const auto& f : SqlEntityConfigurer<FieldPureType>::persistentFieldList())
                f->readFromDriver(pfInstance, f->column());
            for (const auto& r : SqlEntityConfigurer<FieldPureType>::transientFieldList())
                r->readFromDriver(driver, pfInstance);

            classInstance->*getAttribute() = fieldInstance;
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
#endif // SALSABIL_SQLONETOONERELATIONTRANSIENTATTRIBUTEIMPL_HPP

