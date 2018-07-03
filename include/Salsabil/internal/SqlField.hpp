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

#ifndef SALSABIL_SQLFIELD_HPP
#define SALSABIL_SQLFIELD_HPP

#include <string>

#include "SqlValue.hpp"

namespace Salsabil {

    template<typename ClassType>
    class SqlField {
        std::string mName;
        int mColumn;
    public:

        SqlField(const std::string& name, int column) : mName(name), mColumn(column) {
        }

        virtual ~SqlField() {
        }

        virtual SqlValue fetchFromInstance(const ClassType* instance) = 0;

        /* Reads the data at the corresponding column from <i>driver</i> and inject it in <i>instance</i> using its setter method. */
        virtual void readFromDriver(ClassType* instance, int column) = 0;

        /* Gets the data from <i>instance</i> via its getter method and writes it to <i>driver</i> at the corresponding column. */
        virtual void writeToDriver(const ClassType* instance, int column) = 0;

        std::string name() const {
            return mName;
        }

        int column() const {
            return mColumn;
        }
    };

}
#endif // SALSABIL_SQLFIELD_HPP 

