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

#ifndef SALSABIL_SQLRELATIONONETOMANYIMPL_HPP
#define SALSABIL_SQLRELATIONONETOMANYIMPL_HPP

#include "SqlRelation.hpp"
#include "TypeHelper.hpp"
#include "TypeResolver.hpp"
#include "SqlEntityConfigurer.hpp"
#include "SqlGenerator.hpp"
#include "AccessWrapper.hpp"

namespace Salsabil {
    class SqlDriver;

    template<typename ClassType, typename FieldType>
    class SqlRelationOneToManyImpl : public SqlRelation<ClassType> {
        using FieldItemType = typename FieldType::value_type;
        using FieldItemPureType = typename Utility::Traits<FieldItemType>::UnqualifiedType;
        std::map<std::string, std::string> mColumnNameMap;
        std::unique_ptr<AccessWrapper<ClassType, FieldType>> mAccessWrapper;

    public:

        SqlRelationOneToManyImpl(const std::string& targetTableName, const std::map<std::string, std::string>& columnNameMap, RelationType type, AccessWrapper<ClassType, FieldType>* accessWrapper) :
        SqlRelation<ClassType>(targetTableName, type),
        mColumnNameMap(columnNameMap),
        mAccessWrapper(accessWrapper) {
        }

        virtual void readFromDriver(SqlDriver* driver, ClassType* classInstance) {
            std::map<std::string, std::string> columnNameValueMap;
            auto primaryFieldList = SqlEntityConfigurer<ClassType>::primaryFieldList();
            for (std::size_t idx = 0; idx < primaryFieldList.size(); ++idx) {
                columnNameValueMap.insert({mColumnNameMap.at(primaryFieldList.at(idx)->name()), primaryFieldList.at(idx)->fetchFromInstance(classInstance).toString()});
            }
            std::string sqlStatement = SqlGenerator::fetchById(SqlRelation<ClassType>::tableName(), columnNameValueMap);

            SALSABIL_LOG_INFO(sqlStatement);
            driver->execute(sqlStatement);

            FieldType fieldInstanceContainer;

            while (driver->nextRow()) {
                FieldItemType fieldInstance;
                FieldItemPureType* pFieldInstance = Utility::initializeInstance(&fieldInstance);
                for (const auto& f : SqlEntityConfigurer<FieldItemPureType>::primaryFieldList())
                    f->readFromDriver(pFieldInstance, f->column());
                for (const auto& f : SqlEntityConfigurer<FieldItemPureType>::fieldList())
                    f->readFromDriver(pFieldInstance, f->column());

                fieldInstanceContainer.push_back(pFieldInstance);
            }

            mAccessWrapper->set(classInstance, &fieldInstanceContainer);
        }

        virtual void writeToDriver(SqlDriver* driver, const ClassType* classInstance) {

            std::map<std::string, std::string> columnNameValueMap;
            auto primaryFieldList = SqlEntityConfigurer<ClassType>::primaryFieldList();
            for (std::size_t idx = 0; idx < primaryFieldList.size(); ++idx) {
                columnNameValueMap.insert({mColumnNameMap.at(primaryFieldList.at(idx)->name()), primaryFieldList.at(idx)->fetchFromInstance(classInstance).toString()});
            }

            FieldType fieldInstanceContainer;
            mAccessWrapper->get(classInstance, &fieldInstanceContainer);

            for (auto fieldInstanceItem : fieldInstanceContainer) {
                std::map<std::string, std::string> whereConditionMap;

                FieldItemPureType* pFieldInstanceItem = Utility::pointerizeInstance(&fieldInstanceItem);
                for (const auto& field : SqlEntityConfigurer<FieldItemPureType>::primaryFieldList()) {
                    whereConditionMap.insert({field->name(), field->fetchFromInstance(pFieldInstanceItem).toString()});
                }
                std::string sqlStatement = SqlGenerator::update(SqlRelation<ClassType>::tableName(), columnNameValueMap, whereConditionMap);
                SALSABIL_LOG_INFO(sqlStatement);
                driver->execute(sqlStatement);
            }
        }
    };
}
#endif // SALSABIL_SQLRELATIONONETOMANYIMPL_HPP

