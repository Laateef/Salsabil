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

#ifndef SALSABIL_SQLRELATIONMANYTOMANYIMPL_HPP
#define SALSABIL_SQLRELATIONMANYTOMANYIMPL_HPP

#include "SqlRelation.hpp"
#include "TypeHelper.hpp"
#include "TypeResolver.hpp"
#include "SqlEntityConfigurer.hpp"
#include "SqlGenerator.hpp"
#include "AccessWrapper.hpp"
#include "SqlManyToManyMapping.hpp"
#include "Logging.hpp"

namespace Salsabil {
    class SqlDriver;

    template<typename ClassType, typename FieldType>
    class SqlRelationManyToManyImpl : public SqlRelation<ClassType> {
        SqlManyToManyMapping mRelationMapping;
        bool mIsRelationLeftSided;
        std::unique_ptr<AccessWrapper<ClassType, FieldType>> mAccessWrapper;

        using FieldItemType = typename FieldType::value_type;
        using FieldItemPureType = typename Utility::Traits<FieldItemType>::UnqualifiedType;

    public:

        SqlRelationManyToManyImpl(const SqlManyToManyMapping& mapping, AccessWrapper<ClassType, FieldType>* accessWrapper) :
        SqlRelation<ClassType>(mapping.leftTableName() == SqlEntityConfigurer<ClassType>::tableName() ? mapping.rightTableName() : mapping.leftTableName(), RelationType::ManyToMany),
        mRelationMapping(mapping),
        mIsRelationLeftSided(mapping.leftTableName() == SqlEntityConfigurer<ClassType>::tableName()),
        mAccessWrapper(accessWrapper) {
        }

        virtual void readFromDriver(SqlDriver* driver, ClassType* classInstance) {
            std::vector<std::string> onConditionList;
            for (auto field : SqlEntityConfigurer<FieldItemPureType>::primaryFieldList()) {
                onConditionList.push_back(SqlEntityConfigurer<FieldItemPureType>::tableName() + "." + field->name() + " = " + mRelationMapping.intersectionTableName() + "." + mRelationMapping.backwardMapping(SqlEntityConfigurer<FieldItemPureType>::tableName(), field->name()));
            }
            std::vector<std::string> whereConditionList;
            for (auto field : SqlEntityConfigurer<ClassType>::primaryFieldList()) {
                whereConditionList.push_back(mRelationMapping.intersectionTableName() + "." + mRelationMapping.backwardMapping(SqlEntityConfigurer<ClassType>::tableName(), field->name()) + " = " + field->fetchFromInstance(classInstance).toString());
            }
            const std::string& sqlStatement = "SELECT " + SqlEntityConfigurer<FieldItemPureType>::tableName() + ".* FROM " + SqlEntityConfigurer<FieldItemPureType>::tableName() + " INNER JOIN " + mRelationMapping.intersectionTableName() + " ON " +
                    Utility::join(onConditionList.begin(), onConditionList.end(), " AND ") + " WHERE " + Utility::join(whereConditionList.begin(), whereConditionList.end(), " AND ");

            driver->execute(sqlStatement);
            SALSABIL_LOG_INFO(sqlStatement);

            FieldType fieldInstanceContainer;

            while (driver->nextRow()) {
                FieldItemType fieldInstance;
                FieldItemPureType* pFieldInstance = Utility::initializeInstance(&fieldInstance);
                for (const auto& f : SqlEntityConfigurer<FieldItemPureType>::primaryFieldList())
                    f->readFromDriver(pFieldInstance, f->column());
                for (const auto& f : SqlEntityConfigurer<FieldItemPureType>::fieldList())
                    f->readFromDriver(pFieldInstance, f->column());

                fieldInstanceContainer.push_back(fieldInstance);
            }
            mAccessWrapper->set(classInstance, &fieldInstanceContainer);
        }

        virtual void writeToDriver(SqlDriver* driver, const ClassType* classInstance) {

            FieldType fieldInstanceContainer;
            mAccessWrapper->get(classInstance, &fieldInstanceContainer);

            typename FieldType::const_iterator iter = fieldInstanceContainer.begin();
            while (iter != fieldInstanceContainer.end()) {
                std::map<std::string, std::string> columnValueMap;

                for (const auto& f : SqlEntityConfigurer<ClassType>::primaryFieldList()) {
                    columnValueMap.insert({mRelationMapping.backwardMapping(SqlEntityConfigurer<ClassType>::tableName(), f->name()), f->fetchFromInstance(classInstance).toString()});
                }

                FieldItemType fieldInstance = *iter;
                FieldItemPureType* pFieldInstance = Utility::pointerizeInstance(&fieldInstance);
                for (const auto& f : SqlEntityConfigurer<FieldItemPureType>::primaryFieldList()) {
                    columnValueMap.insert({mRelationMapping.backwardMapping(SqlEntityConfigurer<FieldItemPureType>::tableName(), f->name()), f->fetchFromInstance(pFieldInstance).toString()});
                }

                const std::string& sqlStatement = SqlGenerator::insert(mRelationMapping.intersectionTableName(), columnValueMap);

                SALSABIL_LOG_INFO(sqlStatement);
                driver->execute(sqlStatement);

                ++iter;
            }
        }
    };
}
#endif // SALSABIL_SQLRELATIONMANYTOMANYIMPL_HPP

