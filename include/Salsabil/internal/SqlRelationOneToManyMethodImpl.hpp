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

#ifndef SALSABIL_SQLRELATIONONETOMANYMETHODIMPL_HPP
#define SALSABIL_SQLRELATIONONETOMANYMETHODIMPL_HPP

#include "SqlRelation.hpp"
#include "TypeHelper.hpp"
#include "TypeResolver.hpp"
#include "SqlEntityConfigurer.hpp"

namespace Salsabil {
    class SqlDriver;

    template<class ClassType, typename GetMethodType, typename SetMethodType>
    class SqlRelationOneToManyMethodImpl : public SqlRelation<ClassType> {
        using FieldType = typename Utility::Traits<GetMethodType>::ReturnType;
        using FieldItemType = typename FieldType::value_type;
        using FieldItemPureType = typename std::remove_pointer<FieldItemType>::type;

    public:

        SqlRelationOneToManyMethodImpl(const std::string& targetTableName, const std::string& targetColumnName, RelationType type, GetMethodType getter, SetMethodType setter) :
        SqlRelation<ClassType>(targetTableName, type),
        mTargetColumnName(targetColumnName),
        mGetter(getter),
        mSetter(setter) {
        }

        virtual void readFromDriver(SqlDriver* driver, ClassType* classInstance) {
            const std::string& sqlStatement = SqlGenerator::fetchById(SqlEntityConfigurer<FieldItemPureType>::tableName(), mTargetColumnName, "?");
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
            (classInstance->*setter())(fieldInstanceContainer);

        }

        virtual void writeToDriver(SqlDriver* driver, const ClassType* classInstance) {
            std::string sqlStatement = SqlGenerator::update(SqlEntityConfigurer<FieldItemPureType>::tableName(),{std::make_pair(mTargetColumnName, "?")},
            SqlEntityConfigurer<FieldItemPureType>::primaryFieldList().at(0)->name(), "?");
            SALSABIL_LOG_INFO(sqlStatement);

            FieldType fieldInstanceContinaer = (classInstance->*getter())();

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

        void getter(GetMethodType getter) {
            mGetter = getter;
        }

        GetMethodType getter() {
            return mGetter;
        }

        void setter(SetMethodType setter) {
            mSetter = setter;
        }

        SetMethodType setter() {
            return mSetter;
        }

    private:
        std::string mTargetColumnName;

        GetMethodType mGetter;
        SetMethodType mSetter;
    };
}
#endif // SALSABIL_SQLRELATIONONETOMANYMETHODIMPL_HPP

