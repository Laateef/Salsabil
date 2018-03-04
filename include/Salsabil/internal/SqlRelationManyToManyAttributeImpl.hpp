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

#ifndef SALSABIL_SQLRELATIONMANYTOMANYATTRIBUTEIMPL_HPP
#define SALSABIL_SQLRELATIONMANYTOMANYATTRIBUTEIMPL_HPP

#include "SqlRelation.hpp"
#include "TypeHelper.hpp"
#include "TypeResolver.hpp"
#include "SqlEntityConfigurer.hpp"
#include "SqlGenerator.hpp"
#include "Logging.hpp"

namespace Salsabil {
    class SqlDriver;

    template<class ClassType, typename AttributeType>
    class SqlRelationManyToManyAttributeImpl : public SqlRelation<ClassType> {
        using FieldType = typename Utility::Traits<AttributeType>::AttributeType;
        using FieldItemType = typename FieldType::value_type;
        using FieldItemPureType = typename Utility::Traits<FieldItemType>::UnqualifiedType;

    public:

        SqlRelationManyToManyAttributeImpl(const std::string& targetTableName, const std::string& intersectionTableName, const std::string& intersectionTargetColumnName, const std::string& intersectionColumnName, RelationType type, AttributeType attribute) :
        SqlRelation<ClassType>(targetTableName, type),
        mIntersectionTableName(intersectionTableName),
        mIntersectionColumnName(intersectionColumnName),
        mIntersectionTargetColumnName(intersectionTargetColumnName),
        mAttribute(attribute) {
        }

        virtual void readFromDriver(SqlDriver* driver, ClassType* classInstance) {
            const std::string& sqlStatement = SqlGenerator::fetchByJoin(SqlGenerator::JoinMode::Inner,
                    SqlRelation<ClassType>::tableName(),
                    mIntersectionTableName,
                    mIntersectionTableName + "." + mIntersectionTargetColumnName + " = " + SqlRelation<ClassType>::tableName() + "." + SqlEntityConfigurer<FieldItemPureType>::primaryFieldList().at(0)->name(),
                    mIntersectionTableName + "." + mIntersectionColumnName + " = ?");

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

                fieldInstanceContainer.push_back(fieldInstance);
            }
            classInstance->*getAttribute() = fieldInstanceContainer;
        }

        virtual void writeToDriver(SqlDriver* driver, const ClassType* classInstance) {
            const std::string& sqlStatement = SqlGenerator::insert(mIntersectionTableName,{mIntersectionColumnName, mIntersectionTargetColumnName});
            FieldType fieldInstanceContainer = classInstance->*getAttribute();

            typename FieldType::const_iterator iter = fieldInstanceContainer.begin();
            while (iter < fieldInstanceContainer.end()) {

                driver->prepare(sqlStatement);
                SALSABIL_LOG_INFO(sqlStatement);

                for (const auto& f : SqlEntityConfigurer<ClassType>::primaryFieldList()) {
                    f->writeToDriver(classInstance, 1);
                }

                FieldItemType fieldInstance = *iter++;
                FieldItemPureType* pFieldInstance = Utility::pointerizeInstance(&fieldInstance);
                for (const auto& f : SqlEntityConfigurer<FieldItemPureType>::primaryFieldList()) {
                    f->writeToDriver(pFieldInstance, 2);
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
        std::string mIntersectionTableName;
        std::string mIntersectionColumnName;
        std::string mIntersectionTargetColumnName;

        AttributeType mAttribute;
    };
}
#endif // SALSABIL_SQLRELATIONMANYTOMANYATTRIBUTEIMPL_HPP

