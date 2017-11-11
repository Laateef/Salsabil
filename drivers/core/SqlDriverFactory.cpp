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


#include "SqlDriverFactory.hpp"
#include "SqlDriver.hpp"
#include "SqlDriverRegistrar.hpp"

using namespace std;
using namespace Salsabil;

std::map<std::string, SqlDriver*> SqlDriverFactory::mDriverMap;

SqlDriverFactory::~SqlDriverFactory() {
    for (auto iter = mDriverMap.begin(); iter != mDriverMap.end(); ++iter) {
        delete iter->second;
        mDriverMap.erase(iter);
    }
}

SqlDriver* SqlDriverFactory::getDriver(const std::string& driverName) {
    static SqlDriverRegistrar instance;
    
    decltype(mDriverMap)::const_iterator iter = mDriverMap.find(driverName);
    if (iter == mDriverMap.end())
        throw Exception("Driver not found!");
    return (*iter).second->create();
}