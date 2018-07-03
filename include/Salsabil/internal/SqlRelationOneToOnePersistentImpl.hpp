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
#include "SqlRepository.hpp"

namespace Salsabil {
    class SqlDriver;

    template<typename ClassType, typename FieldType>
    class SqlRelationOneToOnePersistentImpl : public SqlRelation<ClassType> {
        using FieldPureType = typename Utility::Traits<FieldType>::UnqualifiedType;
        std::map<std::string, std::string> mColumnNameMap;
        std::unique_ptr<AccessWrapper<ClassType, FieldType>> mAccessWrapper;

    public:

        SqlRelationOneToOnePersistentImpl(const std::string& targetTableName, const std::map<std::string, std::string>& columnNameMap, RelationType type, AccessWrapper<ClassType, FieldType>* accessWrapper) :
        SqlRelation<ClassType>(targetTableName, type),
        mAccessWrapper(accessWrapper) {
            for (auto columnNamePair : columnNameMap) {
                mColumnNameMap.insert({columnNamePair.second, columnNamePair.first});
            }
        }

        virtual void readFromDriver(SqlDriver* driver, ClassType* classInstance) {
            SALSABIL_LOG_DEBUG("SqlRelationOneToOnePersistentImpl, readFromDriver");

            FieldType fieldInstance;
            mAccessWrapper->get(classInstance, &fieldInstance);

            FieldPureType* pFieldInstance = Utility::pointerizeInstance(&fieldInstance);

            std::map<std::string, std::string> columnNameValueMap;
            auto primaryFieldList = SqlEntityConfigurer<FieldPureType>::primaryFieldList();
            for (std::size_t idx = 0; idx < primaryFieldList.size(); ++idx) {
                columnNameValueMap.insert({primaryFieldList.at(idx)->name(), primaryFieldList.at(idx)->fetchFromInstance(pFieldInstance).toString()});
            }
            std::string sqlStatement = SqlGenerator::fetchById(SqlRelation<ClassType>::tableName(), columnNameValueMap);

            SALSABIL_LOG_INFO(sqlStatement);
            driver->execute(sqlStatement);

            if (!driver->nextRow())
                throw Exception("SqlRelationOneToOnePersistentImpl, readFromDriver, no rows to fetch");

            for (const auto& f : SqlEntityConfigurer<FieldPureType>::fieldList())
                f->readFromDriver(pFieldInstance, f->column());
            for (const auto& r : SqlEntityConfigurer<FieldPureType>::transientFieldList())
                r->readFromDriver(driver, pFieldInstance);

            mAccessWrapper->set(classInstance, &fieldInstance);
        }

        virtual void writeToDriver(SqlDriver*, const ClassType*) {
        }
    };
}
#endif // SALSABIL_SQLRELATIONONETOONEPERSISTENTIMPL_HPP

