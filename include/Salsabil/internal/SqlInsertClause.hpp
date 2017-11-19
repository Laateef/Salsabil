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

#ifndef SALSABIL_SQLINSERTCLAUSE_HPP
#define SALSABIL_SQLINSERTCLAUSE_HPP

#include "internal/StringHelper.hpp"

namespace Salsabil {

    class SqlStatementBuilder;

    class SqlInsertClause {
        friend class SqlStatementBuilder;

    public:

        template <typename Type>
        SqlInsertClause& VALUES(Type value) {
            mValueList.push_back(Utility::toString(value));
            append(" VALUES (" + Utility::join(mValueList.begin(), mValueList.end(), ", ") + ")");
            return *this;
        }

        template<typename Type, typename... Args>
        SqlInsertClause& VALUES(Type value, Args... args) {
            mValueList.push_back(Utility::toString(value));
            return VALUES(args...);
        }

        SqlInsertClause& parameterizeValues();

        std::string asString() const;

    private:
        SqlInsertClause(std::string& sql, const std::string& table, std::vector<std::string> columnList = {});

        void append(const std::string& sqlFragment);

        std::vector<std::string> mColumnList;

        std::vector<std::string> mValueList;

        std::string& mSqlString;

    };
}

#endif // SALSABIL_SQLINSERTCLAUSE_HPP