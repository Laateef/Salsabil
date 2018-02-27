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

#ifndef SALSABIL_SQLRELATIONONETOMANYATTRIBUTEIMPL_HPP
#define SALSABIL_SQLRELATIONONETOMANYATTRIBUTEIMPL_HPP

#include "SqlRelation.hpp"
#include "TypeHelper.hpp"
#include "TypeResolver.hpp"
#include "SqlEntityConfigurer.hpp"
#include "SqlGenerator.hpp"

namespace Salsabil {
    class SqlDriver;

    template<class ClassType, typename AttributeType>
    class SqlRelationOneToManyAttributeImpl : public SqlRelation<ClassType> {
        using FieldType = typename Utility::Traits<AttributeType>::AttributeType;
        using FieldItemType = typename FieldType::value_type;
        using FieldItemPureType = typename Utility::Traits<FieldItemType>::UnqualifiedType;

    public:

        SqlRelationOneToManyAttributeImpl(const std::string& targetTableName, const std::string& targetColumnName, RelationType type, AttributeType attribute) :
        SqlRelation<ClassType>(targetTableName, targetColumnName, type),
        mAttribute(attribute) {
        }

        virtual void readFromDriver(SqlDriver* driver, ClassType* classInstance) {
            const std::string& sqlStatement = SqlGenerator::fetchById(SqlRelation<ClassType>::tableName(), SqlRelation<ClassType>::fieldName(), "?");
            driver->prepare(sqlStatement);
            SALSABIL_LOG_INFO(sqlStatement);
            for (const auto& f : SqlEntityConfigurer<ClassType>::primaryFieldList()) {
                f->writeToDriver(classInstance, 1);
            }
            driver->execute();

            FieldType fieldInstanceContainer;

            while (driver->nextRow()) {
                FieldItemType fieldInstance;
                FieldItemPureType* pFieldInstance = Utility::initializeInstance(&fieldInstance);
                for (const auto& f : SqlEntityConfigurer<FieldItemPureType>::primaryFieldList())
                    f->readFromDriver(pFieldInstance, f->column());
                for (const auto& f : SqlEntityConfigurer<FieldItemPureType>::persistentFieldList())
                    f->readFromDriver(pFieldInstance, f->column());

                fieldInstanceContainer.push_back(pFieldInstance);
            }

            classInstance->*getAttribute() = fieldInstanceContainer;

        }

        virtual void writeToDriver(SqlDriver* driver, const ClassType* classInstance) {
            std::string sqlStatement = SqlGenerator::update(SqlEntityConfigurer<FieldItemPureType>::tableName(),{std::make_pair(SqlRelation<ClassType>::fieldName(), "?")},
            SqlEntityConfigurer<FieldItemPureType>::primaryFieldList().at(0)->name(), "?");
            SALSABIL_LOG_INFO(sqlStatement);

            FieldType fieldInstanceContinaer = classInstance->*getAttribute();

            for (auto fieldInstanceItem : fieldInstanceContinaer) {
                FieldItemPureType* pFieldInstanceItem = Utility::pointerizeInstance(&fieldInstanceItem);
                driver->prepare(sqlStatement);
                for (const auto& field : SqlEntityConfigurer<ClassType>::primaryFieldList()) {
                    field->writeToDriver(classInstance, 1);
                }
                for (const auto& field : SqlEntityConfigurer<FieldItemPureType>::primaryFieldList()) {
                    field->writeToDriver(pFieldInstanceItem, 2);
                }
                driver->execute();
            }
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
#endif // SALSABIL_SQLRELATIONONETOMANYATTRIBUTEIMPL_HPP
