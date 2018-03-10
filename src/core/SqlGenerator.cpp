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

#include <vector>
#include <map>
#include <cassert>

#include "internal/SqlGenerator.hpp"
#include "internal/StringHelper.hpp"
#include "Exception.hpp"

using namespace Salsabil;

std::string SqlGenerator::fetchAll(const std::string &table) {
    return "SELECT * FROM " + table;
}

std::string SqlGenerator::fetchById(const std::string &table, const std::string& column, const std::string& id) {
    return "SELECT * FROM " + table + " WHERE " + column + " = " + id;
}

std::string SqlGenerator::fetchById(const std::string& table, const std::map<std::string, std::string>& columnValueMap) {
    std::string statement = "SELECT * FROM " + table + " WHERE ";

    for (const auto& columnValuePair : columnValueMap)
        statement.append(columnValuePair.first + " = " + columnValuePair.second + " AND ");

    statement.erase(statement.end() - 5, statement.end());
    return statement;
}

std::string SqlGenerator::fetchByJoin(SqlGenerator::JoinMode mode, const std::string& table, const std::string& intersectionTable, const std::string& onCondition, const std::string& whereCondition) {
    return "SELECT " + table + ".* FROM " + table + " " +
            std::string(mode == SqlGenerator::JoinMode::Left ? "LEFT" : (mode == SqlGenerator::JoinMode::Right ? "RIGHT" : (mode == SqlGenerator::JoinMode::Inner ? "INNER" : "FULL OUTER")))
            + " JOIN " + intersectionTable + " ON " + onCondition + " WHERE " + whereCondition;
}

std::string SqlGenerator::insert(const std::string& table, const std::vector<std::string>& columnList) {
    std::vector<std::string> questionMarkList(columnList.size(), "?");
    if (questionMarkList.empty())
        throw Exception("could not parameterize the insert statement, the column list is empty!");

    return "INSERT INTO " + table + std::string(columnList.size() == 0 ? "" : "(" + Utility::join(columnList.begin(), columnList.end(), ", ") + ")")
            + " VALUES(" + Utility::join(questionMarkList.begin(), questionMarkList.end(), ", ") + ")";
}

std::string SqlGenerator::update(
        const std::string& table,
        const std::map<std::string, std::string>& columnValueMap,
        const std::string& columnName,
        const std::string& id
        ) {
    std::string statement = "UPDATE " + table + " SET ";

    for (const auto& columnValuePair : columnValueMap)
        statement.append(columnValuePair.first + " = " + columnValuePair.second + ", ");

    assert(columnValueMap.size() >= 1);
    statement.erase(statement.end() - 2, statement.end());
    statement.append(" WHERE " + columnName + " = " + id);
    return statement;
}