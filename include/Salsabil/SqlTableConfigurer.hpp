/*
 * Copyright (C) 2017, Abdullatif Kalla. All rights reserved.
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

#ifndef SALSABIL_SQLTABLECONFIGURER_HPP
#define SALSABIL_SQLTABLECONFIGURER_HPP


#include "SqlDriver.hpp"
#include "Exception.hpp"
#include "SqlDriverFactory.hpp"
#include "internal/SqlFieldAttributePtrImpl.hpp"
#include "internal/SqlFieldMethodPtrImpl.hpp"

namespace Salsabil {

    template<typename ClassType>
    class SqlTableConfigurer {
    public:

        explicit SqlTableConfigurer() {
            reset();
        }

        static void setDriver(SqlDriver* driver) {
            if (driver == nullptr)
                throw Exception("driver is NULL");

            if (!driver->isOpen())
                throw Exception("driver not connected to database");

            mSqlDriver = driver;
        }

        static SqlDriver* driver() {
            return mSqlDriver;
        }

        static void setTableName(const std::string& tableName) {
            for (auto& currentTableName : mSqlDriver->tableList()) {
                if (currentTableName == tableName) {
                    mTableName = tableName;
                    return;
                }
            }

            throw Exception("table not found in database!");
        }

        static std::string& tableName() {
            return mTableName;
        }

        template<typename AttributeType>
        static void setPrimaryField(std::string name, AttributeType attribute) {
            mFieldList.push_back(new SqlFieldAttributePtrImpl<ClassType, AttributeType>(name, fieldColumnIndex(name), attribute, true));
        }

        template<typename AttributeType>
        static void setField(std::string name, AttributeType attribute) {
            mFieldList.push_back(new SqlFieldAttributePtrImpl<ClassType, AttributeType>(name, fieldColumnIndex(name), attribute, false));
        }

        template<typename GetMethodType, typename SetMethodType>
        static void setPrimaryField(std::string name, GetMethodType getter, SetMethodType setter) {
            mFieldList.push_back(new SqlFieldMethodPtrImpl<ClassType, GetMethodType, SetMethodType>(name, fieldColumnIndex(name), getter, setter, true));
        }

        template<typename GetMethodType, typename SetMethodType>
        static void setField(std::string name, GetMethodType getter, SetMethodType setter) {
            mFieldList.push_back(new SqlFieldMethodPtrImpl<ClassType, GetMethodType, SetMethodType>(name, fieldColumnIndex(name), getter, setter, false));
        }

        static std::vector< std::string > fieldNameList() {
            std::vector< std::string > nameList;
            for (const auto& field : mFieldList)
                nameList.push_back(field->name());
            return nameList;
        }

        static std::vector< SqlField<ClassType>* >& fieldList() {
            return mFieldList;
        }

        static void reset() {
            mTableName.clear();
            std::for_each(mFieldList.begin(), mFieldList.end(), [](SqlField<ClassType>* ptr) {
                delete ptr; });
            mFieldList.clear();
        }

    private:

        static int fieldColumnIndex(const std::string& fieldName) {
            auto columns = mSqlDriver->columnList(mTableName);
            decltype(columns)::iterator iter = std::find(columns.begin(), columns.end(), fieldName);
            if (iter == columns.end())
                throw Exception("the field " + fieldName + " does not exist");

            return iter - columns.begin();
        }

        static SqlDriver* mSqlDriver;
        static std::string mTableName;
        static std::vector< SqlField<ClassType>* > mFieldList;
    };

    template<typename C> SqlDriver* SqlTableConfigurer<C>::mSqlDriver = nullptr;
    template<typename C> std::string SqlTableConfigurer<C>::mTableName;
    template<typename C> std::vector< SqlField<C>* > SqlTableConfigurer<C>::mFieldList;
}

#endif /* SALSABIL_SQLTABLECONFIGURER_HPP */
