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

#ifndef SALSABIL_SQLONETOONERELATIONTRANSIENTIMPL_HPP
#define SALSABIL_SQLONETOONERELATIONTRANSIENTIMPL_HPP

#include "SqlRelation.hpp"
#include "TypeHelper.hpp"
#include "TypeResolver.hpp"
#include "SqlEntityConfigurer.hpp"
#include "SqlGenerator.hpp"
#include "AccessWrapper.hpp"
#include "Logging.hpp"
#include "Declarations.hpp"

namespace Salsabil {
    class SqlDriver;

    template<typename ClassType, typename FieldType>
    class SqlRelationOneToOneTransientImpl : public SqlRelation<ClassType> {
        using FieldPureType = typename Utility::Traits<FieldType>::UnqualifiedType;
        std::map<std::string, std::string> mColumnNameMap;
        std::unique_ptr<AccessWrapper<ClassType, FieldType>> mAccessWrapper;
        int mCascade;

    public:

        SqlRelationOneToOneTransientImpl(const std::string& targetTableName, const std::map<std::string, std::string>& columnNameMap, RelationType type, AccessWrapper<ClassType, FieldType>* accessWrapper, int cascade) :
        SqlRelation<ClassType>(targetTableName, type),
        mColumnNameMap(columnNameMap),
        mAccessWrapper(accessWrapper),
        mCascade(cascade) {
        }

        virtual void readFromDriver(SqlDriver* driver, ClassType* classInstance) override {
            fetch(driver, classInstance);
        }

        virtual void writeToDriver(SqlDriver*, const ClassType* classInstance) override {
            persist(classInstance);
        }

        virtual void fetch(SqlDriver* driver, ClassType* classInstance) override {
            std::map<std::string, std::string> columnNameValueMap;
            auto primaryFieldList = SqlEntityConfigurer<ClassType>::primaryFieldList();
            for (std::size_t idx = 0; idx < primaryFieldList.size(); ++idx) {
                columnNameValueMap.insert({mColumnNameMap.at(primaryFieldList.at(idx)->name()), primaryFieldList.at(idx)->fetchFromInstance(classInstance).toString()});
            }
            std::string sqlStatement = SqlGenerator::fetchById(SqlRelation<ClassType>::tableName(), columnNameValueMap);

            SALSABIL_LOG_INFO(sqlStatement);
            driver->execute(sqlStatement);

            if (!driver->nextRow())
                throw Exception("no rows found");

            FieldType fieldInstance;
            FieldPureType* pfieldInstance = Utility::initializeInstance(&fieldInstance);

            for (const auto& f : SqlEntityConfigurer<FieldPureType>::primaryFieldList())
                f->readFromDriver(pfieldInstance, f->column());
            for (const auto& f : SqlEntityConfigurer<FieldPureType>::fieldList())
                f->readFromDriver(pfieldInstance, f->column());
            for (const auto& r : SqlEntityConfigurer<FieldPureType>::transientFieldList())
                r->fetch(driver, pfieldInstance);

            mAccessWrapper->set(classInstance, &fieldInstance);
        }

        virtual void persist(const ClassType* classInstance) override {
            if (mCascade & CascadeType::Persist)
                SqlRepository<FieldPureType>::persist(pointerizedFieldInstance(classInstance));
        }

        virtual void update(const ClassType* classInstance) override {
            if (mCascade & CascadeType::Update)
                SqlRepository<FieldPureType>::update(pointerizedFieldInstance(classInstance));
        }

        virtual void remove(const ClassType* classInstance) override {
            if (mCascade & CascadeType::Remove)
                SqlRepository<FieldPureType>::remove(pointerizedFieldInstance(classInstance));
        }

        FieldPureType* pointerizedFieldInstance(const ClassType* classInstance) {
            FieldType fieldInstance;
            mAccessWrapper->get(classInstance, &fieldInstance);
            return Utility::pointerizeInstance(&fieldInstance);
        }
    };
}
#endif // SALSABIL_SQLONETOONERELATIONTRANSIENTIMPL_HPP

