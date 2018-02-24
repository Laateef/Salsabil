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

#ifndef SALSABIL_SQLGENERATOR_HPP
#define SALSABIL_SQLGENERATOR_HPP

#include <string>
#include <vector>

namespace Salsabil {

    class SqlGenerator {
    public:
        static std::string fetchAll(const std::string& table);
        static std::string fetchById(const std::string& table, const std::string& column, const std::string& id);
        static std::string insert(const std::string& table, const std::vector<std::string>& columnList = std::vector<std::string>());
    };
}

#endif // SALSABIL_SQLGENERATOR_HPP