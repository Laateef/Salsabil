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

#ifndef SALSABIL_SQLENTITYCONFIGURER_HPP
#define SALSABIL_SQLENTITYCONFIGURER_HPP

#include "SqlDriver.hpp"
#include "Exception.hpp"
#include "internal/AccessWrapper.hpp"
#include "internal/SqlFieldImpl.hpp"
#include "internal/SqlRelationOneToOnePersistentImpl.hpp"
#include "internal/SqlRelationOneToOneTransientImpl.hpp"
#include "internal/SqlRelationOneToManyImpl.hpp"
#include "internal/SqlRelationManyToManyImpl.hpp"
#include "internal/Logging.hpp"
#include <vector>
#include <string>
#include <algorithm>


namespace Salsabil {

    template<typename ClassType>
    class SqlEntityConfigurer {
    public:

        SqlEntityConfigurer() {
            SALSABIL_LOG_DEBUG("Configuring class '" + std::string(typeid (ClassType).name()) + "'");
            reset();
        }

        static void setDriver(SqlDriver* driver) {
            if (driver == nullptr)
                throw Exception("driver is NULL");

            SALSABIL_LOG_DEBUG("Setting SQL driver '" + driver->driverName() + "'");

            if (!driver->isOpen())
                throw Exception("driver not connected to database");

            mSqlDriver = driver;
        }

        static SqlDriver* driver() {
            return mSqlDriver;
        }

        static void setTableName(const std::string& tableName) {
            SALSABIL_LOG_DEBUG("Setting SQL table '" + tableName + "'");

            for (auto& currentTableName : mSqlDriver->tableList()) {
                if (currentTableName == tableName) {
                    mTableName = tableName;
                    return;
                }
            }

            throw Exception("table not found in database!");
        }

        static const std::string& tableName() {
            return mTableName;
        }

        template<typename AttributeType>
        static void setPrimaryField(std::string columnName, AttributeType attribute) {
            SALSABIL_LOG_DEBUG("Setting primary field[attribute]: " + columnName);
            using FieldType = typename Utility::Traits<AttributeType>::AttributeType;
            mPrimaryFieldList.push_back(new SqlFieldImpl<ClassType, FieldType>(columnName, fieldColumnIndex(columnName), new AccessWrapperAttributeImpl<ClassType, FieldType, AttributeType>(attribute)));
        }

        template<typename GetMethodType, typename SetMethodType>
        static void setPrimaryField(std::string columnName, GetMethodType getter, SetMethodType setter) {
            SALSABIL_LOG_DEBUG("Setting primary field[method]: " + columnName);
            using FieldType = typename Utility::Traits<GetMethodType>::ReturnType;
            mPrimaryFieldList.push_back(new SqlFieldImpl<ClassType, FieldType>(columnName, fieldColumnIndex(columnName), new AccessWrapperMethodImpl<ClassType, FieldType, GetMethodType, SetMethodType>(getter, setter)));
        }

        template<typename AttributeType>
        static void setField(std::string columnName, AttributeType attribute) {
            SALSABIL_LOG_DEBUG("Setting field[attribute]: " + columnName);
            using FieldType = typename Utility::Traits<AttributeType>::AttributeType;
            mPersistentFieldList.push_back(new SqlFieldImpl<ClassType, FieldType>(columnName, fieldColumnIndex(columnName), new AccessWrapperAttributeImpl<ClassType, FieldType, AttributeType>(attribute)));
        }

        template<typename GetMethodType, typename SetMethodType>
        static void setField(std::string columnName, GetMethodType getter, SetMethodType setter) {
            SALSABIL_LOG_DEBUG("Setting field[method]: " + columnName);
            using FieldType = typename Utility::Traits<GetMethodType>::ReturnType;
            mPersistentFieldList.push_back(new SqlFieldImpl<ClassType, FieldType>(columnName, fieldColumnIndex(columnName), new AccessWrapperMethodImpl<ClassType, FieldType, GetMethodType, SetMethodType>(getter, setter)));
        }

        template<typename AttributeType>
        static void setOneToOnePersistentField(const std::string& columnName, const std::string& targetTableName, const std::string& targetColumnName, AttributeType attribute) {
            SALSABIL_LOG_DEBUG("Setting one-to-one persistent relational field[attribute]: " + tableName() + "(" + columnName + ") -> " + targetTableName + "(" + targetColumnName + ")");
            using FieldType = typename Utility::Traits<AttributeType>::AttributeType;
            mPersistentFieldList.push_back(new SqlFieldImpl<ClassType, FieldType>(columnName, fieldColumnIndex(columnName), new AccessWrapperAttributeImpl<ClassType, FieldType, AttributeType>(attribute)));
            mTransientFieldList.push_back(new SqlRelationOneToOnePersistentImpl<ClassType, FieldType>(targetTableName, targetColumnName, RelationType::OneToOne, new AccessWrapperAttributeImpl<ClassType, FieldType, AttributeType>(attribute)));
        }

        template<typename GetMethodType, typename SetMethodType>
        static void setOneToOnePersistentField(const std::string& columnName, const std::string& targetTableName, const std::string& targetColumnName, GetMethodType getter, SetMethodType setter) {
            SALSABIL_LOG_DEBUG("Setting one-to-one persistent relational field[method]: " + tableName() + "(" + columnName + ") -> " + targetTableName + "(" + targetColumnName + ")");
            using FieldType = typename Utility::Traits<GetMethodType>::ReturnType;
            mPersistentFieldList.push_back(new SqlFieldImpl<ClassType, FieldType>(columnName, fieldColumnIndex(columnName), new AccessWrapperMethodImpl<ClassType, FieldType, GetMethodType, SetMethodType>(getter, setter)));
            mTransientFieldList.push_back(new SqlRelationOneToOnePersistentImpl<ClassType, FieldType>(targetTableName, targetColumnName, RelationType::OneToOne, new AccessWrapperMethodImpl<ClassType, FieldType, GetMethodType, SetMethodType>(getter, setter)));
        }

        template<typename AttributeType>
        static void setOneToOneTransientField(const std::string& targetTableName, const std::string& targetColumnName, AttributeType attribute) {
            SALSABIL_LOG_DEBUG("Setting one-to-one transient relational field[attribute]: " + tableName() + " -> " + targetTableName + "(" + targetColumnName + ")");
            using FieldType = typename Utility::Traits<AttributeType>::AttributeType;
            mTransientFieldList.push_back(new SqlRelationOneToOneTransientImpl<ClassType, FieldType>(targetTableName, targetColumnName, RelationType::OneToOne, new AccessWrapperAttributeImpl<ClassType, FieldType, AttributeType>(attribute)));
        }

        template<typename GetMethodType, typename SetMethodType>
        static void setOneToOneTransientField(const std::string& targetTableName, const std::string& targetColumnName, GetMethodType getter, SetMethodType setter) {
            SALSABIL_LOG_DEBUG("Setting one-to-one transient relational field[method]: " + tableName() + " -> " + targetTableName + "(" + targetColumnName + ")");
            using FieldType = typename Utility::Traits<GetMethodType>::ReturnType;
            mTransientFieldList.push_back(new SqlRelationOneToOneTransientImpl<ClassType, FieldType>(targetTableName, targetColumnName, RelationType::OneToOne, new AccessWrapperMethodImpl<ClassType, FieldType, GetMethodType, SetMethodType>(getter, setter)));
        }

        template<typename AttributeType>
        static void setOneToManyTransientField(const std::string& targetTableName, const std::string& targetColumnName, AttributeType attribute) {
            SALSABIL_LOG_DEBUG("Setting one-to-many transient relational field[attribute]: " + tableName() + " -> " + targetTableName + "(" + targetColumnName + ")");
            using FieldType = typename Utility::Traits<AttributeType>::AttributeType;
            mTransientFieldList.push_back(new SqlRelationOneToManyImpl<ClassType, FieldType>(targetTableName, targetColumnName, RelationType::OneToMany, new AccessWrapperAttributeImpl<ClassType, FieldType, AttributeType>(attribute)));
        }

        template<typename GetMethodType, typename SetMethodType>
        static void setOneToManyTransientField(const std::string& targetTableName, const std::string& targetColumnName, GetMethodType getter, SetMethodType setter) {
            SALSABIL_LOG_DEBUG("Setting one-to-many transient relational field[method]: " + tableName() + " -> " + targetTableName + "(" + targetColumnName + ")");
            using FieldType = typename Utility::Traits<GetMethodType>::ReturnType;
            mTransientFieldList.push_back(new SqlRelationOneToManyImpl<ClassType, FieldType>(targetTableName, targetColumnName, RelationType::OneToMany, new AccessWrapperMethodImpl<ClassType, FieldType, GetMethodType, SetMethodType>(getter, setter)));
        }

        template<typename AttributeType>
        static void setManyToOnePersistentField(const std::string& columnName, const std::string& targetTableName, const std::string& targetColumnName, AttributeType attribute) {
            SALSABIL_LOG_DEBUG("Setting many-to-one persistent relational field[attribute]: " + tableName() + "(" + columnName + ") -> " + targetTableName + "(" + targetColumnName + ")");
            using FieldType = typename Utility::Traits<AttributeType>::AttributeType;
            mPersistentFieldList.push_back(new SqlFieldImpl<ClassType, FieldType>(columnName, fieldColumnIndex(columnName), new AccessWrapperAttributeImpl<ClassType, FieldType, AttributeType> (attribute)));
            mTransientFieldList.push_back(new SqlRelationOneToOnePersistentImpl<ClassType, FieldType>(targetTableName, targetColumnName, RelationType::ManyToOne, new AccessWrapperAttributeImpl<ClassType, FieldType, AttributeType>(attribute)));
        }

        template<typename GetMethodType, typename SetMethodType>
        static void setManyToOnePersistentField(const std::string& columnName, const std::string& targetTableName, const std::string& targetColumnName, GetMethodType getter, SetMethodType setter) {
            SALSABIL_LOG_DEBUG("Setting many-to-one persistent relational field[method]: " + tableName() + "(" + columnName + ") -> " + targetTableName + "(" + targetColumnName + ")");
            using FieldType = typename Utility::Traits<GetMethodType>::ReturnType;
            mPersistentFieldList.push_back(new SqlFieldImpl<ClassType, FieldType>(columnName, fieldColumnIndex(columnName), new AccessWrapperMethodImpl<ClassType, FieldType, GetMethodType, SetMethodType > (getter, setter)));
            mTransientFieldList.push_back(new SqlRelationOneToOnePersistentImpl<ClassType, FieldType>(targetTableName, targetColumnName, RelationType::ManyToOne, new AccessWrapperMethodImpl<ClassType, FieldType, GetMethodType, SetMethodType>(getter, setter)));
        }

        template<typename AttributeType>
        static void setManyToManyTransientField(const std::string& targetTableName, const std::string& intersectionTableName, const std::string& intersectionTargetColumnName, const std::string& intersectionColumnName, AttributeType attribute) {
            SALSABIL_LOG_DEBUG("Setting many-to-many transient relational field[attribute]: " + tableName() + " -> " + intersectionTableName + "(" + intersectionColumnName + ", " + intersectionTargetColumnName + ")" + " <- " + targetTableName);
            using FieldType = typename Utility::Traits<AttributeType>::AttributeType;
            mTransientFieldList.push_back(new SqlRelationManyToManyImpl<ClassType, FieldType>(targetTableName, intersectionTableName, intersectionTargetColumnName, intersectionColumnName, RelationType::ManyToMany, new AccessWrapperAttributeImpl<ClassType, FieldType, AttributeType>(attribute)));
        }

        template<typename GetMethodType, typename SetMethodType>
        static void setManyToManyTransientField(const std::string& targetTableName, const std::string& intersectionTableName, const std::string& intersectionTargetColumnName, const std::string& intersectionColumnName, GetMethodType getter, SetMethodType setter) {
            SALSABIL_LOG_DEBUG("Setting many-to-many transient relational field[method]: " + tableName() + " -> " + intersectionTableName + "(" + intersectionColumnName + ", " + intersectionTargetColumnName + ")" + " <- " + targetTableName);
            using FieldType = typename Utility::Traits<GetMethodType>::ReturnType;
            mTransientFieldList.push_back(new SqlRelationManyToManyImpl<ClassType, FieldType>(targetTableName, intersectionTableName, intersectionTargetColumnName, intersectionColumnName, RelationType::ManyToMany, new AccessWrapperMethodImpl<ClassType, FieldType, GetMethodType, SetMethodType>(getter, setter)));
        }

        static std::vector< std::string > columnNameList() {
            std::vector< std::string > nameList;
            for (const auto& field : mPrimaryFieldList)
                nameList.push_back(field->name());
            for (const auto& field : mPersistentFieldList)
                nameList.push_back(field->name());

            return nameList;
        }

        static const std::vector< SqlField<ClassType>* >& primaryFieldList() {

            return mPrimaryFieldList;
        }

        static const std::vector< SqlField<ClassType>* >& persistentFieldList() {

            return mPersistentFieldList;
        }

        static const std::vector< SqlRelation<ClassType>* >& transientFieldList() {

            return mTransientFieldList;
        }

        static void reset() {
            mTableName.clear();
            std::for_each(mPrimaryFieldList.begin(), mPrimaryFieldList.end(), [](SqlField<ClassType>* ptr) {
                delete ptr; });
            mPrimaryFieldList.clear();
            std::for_each(mPersistentFieldList.begin(), mPersistentFieldList.end(), [](SqlField<ClassType>* ptr) {
                delete ptr; });
            mPersistentFieldList.clear();
            std::for_each(mTransientFieldList.begin(), mTransientFieldList.end(), [](SqlRelation<ClassType>* ptr) {

                delete ptr; });
            mTransientFieldList.clear();
        }

        static int fieldColumnIndex(const std::string& fieldName) {
            auto columns = mSqlDriver->columnList(mTableName);
            decltype(columns)::iterator iter = std::find(columns.begin(), columns.end(), fieldName);
            if (iter == columns.end())
                throw Exception("the field " + fieldName + " does not exist in the table " + mTableName);

            return iter - columns.begin();
        }

    private:

        static SqlDriver* mSqlDriver;
        static std::string mTableName;
        static std::vector< SqlField<ClassType>* > mPrimaryFieldList;
        static std::vector< SqlField<ClassType>* > mPersistentFieldList;
        static std::vector< SqlRelation<ClassType>* > mTransientFieldList;
    };

    template<typename C> SqlDriver* SqlEntityConfigurer<C>::mSqlDriver = nullptr;
    template<typename C> std::string SqlEntityConfigurer<C>::mTableName;
    template<typename C> std::vector< SqlField<C>* > SqlEntityConfigurer<C>::mPrimaryFieldList;
    template<typename C> std::vector< SqlField<C>* > SqlEntityConfigurer<C>::mPersistentFieldList;
    template<typename C> std::vector< SqlRelation<C>* > SqlEntityConfigurer<C>::mTransientFieldList;
}

#endif // SALSABIL_SQLENTITYCONFIGURER_HPP 
