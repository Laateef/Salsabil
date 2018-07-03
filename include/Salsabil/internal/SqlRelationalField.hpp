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

#ifndef SALSABIL_SQLRELATIONALFIELD_HPP
#define SALSABIL_SQLRELATIONALFIELD_HPP

#include <string>
#include <map>

#include "SqlValue.hpp"

namespace Salsabil {

    template<typename ClassType>
    class SqlRelationalField {
        std::map<std::string, std::string> mColumnNameMap;
        std::map<std::string, int> mColumnNameIndexMap;

    public:

        SqlRelationalField(std::map<std::string, std::string> columnNameMap) {
            for (auto columnNamePair : columnNameMap) {
                mColumnNameMap.insert({columnNamePair.second, columnNamePair.first});
            }
            for (auto columnNamePair : mColumnNameMap)
                mColumnNameIndexMap.insert({columnNamePair.first, SqlEntityConfigurer<ClassType>::fieldColumnIndex(columnNamePair.second)});
        }

        virtual ~SqlRelationalField() {
        }

        virtual void injectInto(ClassType* instance) = 0;

        virtual std::map<std::string, std::string> parseFrom(const ClassType* instance) = 0;

        std::map<std::string, std::string> columnNameMap() const {
            return mColumnNameMap;
        }

        std::map<std::string, int> columnNameIndexMap() const {
            return mColumnNameIndexMap;
        }
    };

}
#endif // SALSABIL_SQLRELATIONALFIELD_HPP 

