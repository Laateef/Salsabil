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

#ifndef SALSABIL_SQLRELATIONONETOONEPERSISTENTMETHODIMPL_HPP
#define SALSABIL_SQLRELATIONONETOONEPERSISTENTMETHODIMPL_HPP

#include "SqlRelation.hpp"
#include "TypeHelper.hpp"
#include "TypeResolver.hpp"
#include "Logging.hpp"
#include "SqlEntityConfigurer.hpp"
#include "SqlGenerator.hpp"

namespace Salsabil {
    class SqlDriver;

    template<class ClassType, typename GetMethodType, typename SetMethodType>
    class SqlRelationOneToOnePersistentMethodImpl : public SqlRelation<ClassType> {
        using FieldType = typename Utility::Traits<GetMethodType>::ReturnType;
        using FieldPureType = typename Utility::Traits<FieldType>::UnqualifiedType;
    public:

        SqlRelationOneToOnePersistentMethodImpl(const std::string& targetTableName, const std::string& targetColumnName, RelationType type, GetMethodType getter, SetMethodType setter) :
        SqlRelation<ClassType>(targetTableName, targetColumnName, type),
        mGetter(getter),
        mSetter(setter) {
        }

        virtual void readFromDriver(SqlDriver* driver, ClassType* classInstance) {
            SALSABIL_LOG_DEBUG("Reading field '" + SqlRelation<ClassType>::fieldName() + "' from driver via pointer ");

            std::string sqlStatement = SqlGenerator::fetchById(SqlRelation<ClassType>::tableName(), SqlRelation<ClassType>::fieldName(), "?");
            driver->prepare(sqlStatement);
            SALSABIL_LOG_INFO(sqlStatement);

            FieldType ins = (classInstance->*getter())();
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

            (classInstance->*setter())(ins);
        }

        virtual void writeToDriver(SqlDriver*, const ClassType*) {

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
        GetMethodType mGetter;
        SetMethodType mSetter;
    };
}
#endif // SALSABIL_SQLRELATIONONETOONEPERSISTENTMETHODIMPL_HPP

