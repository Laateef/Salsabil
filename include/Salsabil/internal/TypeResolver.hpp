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

#ifndef SALSABIL_TYPERESOLVER_HPP
#define SALSABIL_TYPERESOLVER_HPP

#include <string>

#include "SqlDriver.hpp"
#include "internal/Logging.hpp"
#include "SqlField.hpp"


namespace Salsabil {
    template<typename> class SqlEntityConfigurer;

    namespace Utility {

        template<typename T>
        inline T* initializeInstance(T* obj) {
            return obj;
        }

        template<typename T>
        inline T* initializeInstance(T** obj) {
            *obj = new T;
            return *obj;
        }

        template<typename T>
        inline T* pointerizeInstance(T* obj) {
            return obj;
        }

        template<typename T>
        inline T* pointerizeInstance(T** obj) {
            return *obj;
        }

        //        template<class T>
        //        void driverToVariable(const SqlDriver*, int column, T* to) {
        //            SALSABIL_LOG_DEBUG("Fetching entity's primary key at column '" + std::to_string(column) + "' ");
        //            for (const auto& field : SqlEntityConfigurer<T>::primaryFieldList())
        //                field->readFromDriver(to, column);
        //        }
        //
        //        template<class T>
        //        void variableToDriver(SqlDriver*, int column, const T* from) {
        //            SALSABIL_LOG_DEBUG("Binding entity's primary key at column '" + std::to_string(column) + "' ");
        //            for (const auto& field : SqlEntityConfigurer<T>::primaryFieldList())
        //                field->writeToDriver(from, column);
        //        }
        //
        //        template<class T>
        //        void driverToVariable(const SqlDriver* driver, int column, T** to) {
        //            driverToVariable(driver, column, *to);
        //        }
        //
        //        template<class T>
        //        void variableToDriver(SqlDriver* driver, int column, const T** from) {
        //            variableToDriver(driver, column, *from);
        //        }

        inline void driverToVariable(const SqlDriver* driver, int column, int* to) {
            *to = driver->getInt(column);
            SALSABIL_LOG_DEBUG("Fetching int value '" + std::to_string(*to) + "' from driver at column '" + std::to_string(column) + "' ");
        }

        inline void driverToVariable(const SqlDriver* driver, int column, std::string* to) {
            *to = driver->getStdString(column);
            SALSABIL_LOG_DEBUG("Fetching string value '" + *to + "' from driver at column '" + std::to_string(column) + "' ");
        }

        inline void driverToVariable(const SqlDriver* driver, int column, float *to) {
            *to = driver->getFloat(column);
            SALSABIL_LOG_DEBUG("Fetching float value '" + std::to_string(*to) + "' from driver at column '" + std::to_string(column) + "' ");
        }

        inline void driverToVariable(const SqlDriver* driver, int column, double *to) {
            *to = driver->getDouble(column);
            SALSABIL_LOG_DEBUG("Fetching double value '" + std::to_string(*to) + "' from driver at column '" + std::to_string(column) + "' ");
        }

        inline void variableToDriver(SqlDriver* driver, int column, int* from) {
            SALSABIL_LOG_DEBUG("Binding int variable '" + std::to_string(*from) + "' to driver at column '" + std::to_string(column) + "' ");
            driver->bindInt(column, *from);
        }

        inline void variableToDriver(SqlDriver* driver, int column, std::string* from) {
            SALSABIL_LOG_DEBUG("Binding string variable '" + *from + "' to driver at column '" + std::to_string(column) + "' ");
            driver->bindStdString(column, *from);
        }

        inline void variableToDriver(SqlDriver* driver, int column, float* from) {
            SALSABIL_LOG_DEBUG("Binding float variable '" + std::to_string(*from) + "' to driver at column '" + std::to_string(column) + "' ");
            driver->bindFloat(column, *from);
        }

        inline void variableToDriver(SqlDriver* driver, int column, double* from) {
            SALSABIL_LOG_DEBUG("Binding double variable '" + std::to_string(*from) + "' to driver at column '" + std::to_string(column) + "' ");
            driver->bindDouble(column, *from);
        }
    }
}

#endif // SALSABIL_TYPERESOLVER_HPP

