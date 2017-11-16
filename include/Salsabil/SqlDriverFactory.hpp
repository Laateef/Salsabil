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


#ifndef SALSABIL_SQLDRIVERFACTORY_HPP
#define SALSABIL_SQLDRIVERFACTORY_HPP

#include <map>

#include "Exception.hpp"

namespace Salsabil {
    class SqlDriver;

    class SqlDriverFactory {
    public:

        explicit SqlDriverFactory() {
        }

        ~SqlDriverFactory();

        template<typename DriverType>
        static void registerDriver(const std::string& driverName) {
            if (mDriverMap.find(driverName) != mDriverMap.end())
                throw Exception("A driver with the same name exists!");

            mDriverMap[driverName] = new DriverType;
        }

        static SqlDriver* getDriver(const std::string& driverName);

    private:
        static std::map<std::string, SqlDriver*> mDriverMap;
    };
}

#endif // SALSABIL_SQLDRIVERFACTORY_HPP 

