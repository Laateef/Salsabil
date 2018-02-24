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

#ifndef SALSABIL_SQLWHERECLAUSE_HPP
#define SALSABIL_SQLWHERECLAUSE_HPP

#include "internal/StringHelper.hpp"

namespace Salsabil {

    class SqlFromClause;

    class SqlWhereClause {
        friend class SqlFromClause;

    public:

        template <typename T> SqlWhereClause& equalTo(T rightOperand) {
            append(" = " + Utility::toString(rightOperand));
            return *this;
        }

        template <typename T> SqlWhereClause& notEqualTo(T rightOperand) {
            append(" <> " + Utility::toString(rightOperand));
            return *this;
        }

        SqlWhereClause& equalToString(const std::string& rightOperand) {
            append(" = " + Utility::toSqlString(rightOperand));
            return *this;
        }

        SqlWhereClause& notEqualToString(const std::string& rightOperand) {
            append(" <> " + Utility::toSqlString(rightOperand));
            return *this;
        }

        template <typename T> SqlWhereClause& greaterThan(T rightOperand) {
            append(" > " + Utility::toString(rightOperand));
            return *this;
        }

        template <typename T> SqlWhereClause& greaterThanOrEqualTo(T rightOperand) {
            append(" >= " + Utility::toString(rightOperand));
            return *this;
        }

        template <typename T> SqlWhereClause& lessThan(T rightOperand) {
            append(" < " + Utility::toString(rightOperand));
            return *this;
        }

        template <typename T> SqlWhereClause& lessThanOrEqualTo(T rightOperand) {
            append(" <= " + Utility::toString(rightOperand));
            return *this;
        }

        SqlWhereClause& AND(const std::string& leftOperand);

        SqlWhereClause& OR(const std::string& leftOperand);

        std::string asString() const;

    private:
        SqlWhereClause(std::string& sql, const std::string& leftOperand);

        void append(const std::string& sqlFragment);

        std::string& mSqlString;

    };
}

#endif // SALSABIL_SQLWHERECLAUSE_HPP