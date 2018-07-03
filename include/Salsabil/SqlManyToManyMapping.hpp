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

#ifndef SALSABIL_SQLMANYTOMANYMAPPING_HPP
#define SALSABIL_SQLMANYTOMANYMAPPING_HPP

#include <string>
#include <map>
#include "internal/StringHelper.hpp"

namespace Salsabil {

    class SqlManyToManyMapping {
        std::string mLeftTableName;
        std::string mIntersectionTableName;
        std::string mRightTableName;
        std::map<std::string, std::string> mLeftMapping;
        std::map<std::string, std::string> mRightMapping;
        std::map<std::string, std::string> mRelationMapping;

    public:

        SqlManyToManyMapping(std::string leftTableName, std::string intersectionTableName, std::string rightTableName) : mLeftTableName(leftTableName), mIntersectionTableName(intersectionTableName), mRightTableName(rightTableName) {
        }

        void setLeftMapping(std::string intersectionColumnName, std::string targetColumnName) {
            mLeftMapping.insert({intersectionColumnName, targetColumnName});
        }

        void setRightMapping(std::string intersectionColumnName, std::string targetColumnName) {
            mRightMapping.insert({intersectionColumnName, targetColumnName});
        }

        std::string leftTableName() const {
            return mLeftTableName;
        }

        std::string rightTableName() const {
            return mLeftTableName;
        }

        std::string intersectionTableName() const {
            return mIntersectionTableName;
        }

        std::string forwardMapping(std::string intersectionColumnName) const {
            std::map<std::string, std::string>::const_iterator it;
            it = mLeftMapping.find(intersectionColumnName);
            if (it != mLeftMapping.end())
                return it->first;
            it = mRightMapping.find(intersectionColumnName);
            if (it != mRightMapping.end())
                return it->first;
        }

        std::string backwardMapping(std::string tableName, std::string targetColumnName) const {
            std::map<std::string, std::string>::const_iterator it;
            if (tableName == mLeftTableName) {
                it = mLeftMapping.begin();
                while (it != mLeftMapping.end()) {
                    if (it->second == targetColumnName)
                        return it->first;
                    ++it;
                }
            } else {
                it = mRightMapping.begin();
                while (it != mRightMapping.end()) {
                    if (it->second == targetColumnName)
                        return it->first;
                    ++it;
                }
            }
            return "";
        }
    };
}

#endif // SALSABIL_SQLMANYTOMANYMAPPING_HPP 
