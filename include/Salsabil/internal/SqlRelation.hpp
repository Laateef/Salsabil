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

#ifndef SALSABIL_SQLRELATION_HPP
#define SALSABIL_SQLRELATION_HPP

namespace Salsabil {

    enum class RelationType {
        None, OneToOne, OneToMany, ManyToOne, ManyToMany
    };

    template<class ClassType>
    class SqlRelation {
    public:

        explicit SqlRelation(const std::string& targetTableName, RelationType type) : mTableName(targetTableName), mType(type) {
        }

        virtual ~SqlRelation() {
        }

        virtual void readFromDriver(SqlDriver* driver, ClassType* instance) = 0;

        virtual void writeToDriver(SqlDriver* driver, const ClassType* instance) = 0;

        virtual void fetch(SqlDriver* driver, ClassType* classInstance) {
        }

        virtual void persist(const ClassType* classInstance) {
        }

        virtual void update(const ClassType* classInstance) {
        }

        virtual void remove(const ClassType* classInstance) {
        }

        std::string tableName() const {
            return mTableName;
        }

        void setTableName(std::string name) {
            mTableName = name;
        }

        RelationType type() const {
            return mType;
        }

        void setType(RelationType type) {
            mType = type;
        }

    private:
        std::string mTableName;
        RelationType mType;
    };
}
#endif // SALSABIL_SQLRELATION_HPP

