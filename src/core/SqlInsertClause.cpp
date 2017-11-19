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

#include "internal/SqlInsertClause.hpp"
#include "internal/StringHelper.hpp"
#include "Exception.hpp"

using namespace Salsabil;

SqlInsertClause::SqlInsertClause(std::string& sql, const std::string& table, std::vector<std::string> columnList)
: mSqlString(sql)
, mColumnList(columnList) {
    append("INSERT INTO " + table);
    if (columnList.size() > 0) {
        append(" (");
        append(Utility::join(columnList.begin(), columnList.end(), ", "));
        append(")");

    }
}

SqlInsertClause& SqlInsertClause::parameterizeValues() {
    std::vector<std::string> questionMarkList(mColumnList.size(), "?");
    if (questionMarkList.empty())
        throw Exception("could not parameterize, the column list is empty!");

    append(" VALUES (" + Utility::join(questionMarkList.begin(), questionMarkList.end(), ", ") + ")");

    return *this;
}

void SqlInsertClause::append(const std::string& sqlFragment) {

    mSqlString += sqlFragment;
}

std::string SqlInsertClause::asString() const {
    return mSqlString;
}