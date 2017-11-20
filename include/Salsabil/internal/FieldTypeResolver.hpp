#ifndef SALSABIL_TYPERESOLVER_HPP
#define SALSABIL_TYPERESOLVER_HPP

#include <string>
#include <iostream>
#include <typeinfo>

#include "SqlDriver.hpp"

namespace Salsabil {

    namespace Utility {

        template <typename T> struct is_basic_type : std::false_type {
        };

        template <> struct is_basic_type<int> : std::true_type {
        };

        template <> struct is_basic_type<float> : std::true_type {
        };

        template <> struct is_basic_type<double> : std::true_type {
        };

        template <> struct is_basic_type<std::string> : std::true_type {
        };

        class FieldTypeResolver {
        public:

            static void driverToVariable(const SqlDriver* driver, int column, int *to) {
                *to = driver->getInt(column);
            }

            static void driverToVariable(const SqlDriver* driver, int column, std::string *to) {
                *to = driver->getStdString(column);
            }

            static void driverToVariable(const SqlDriver* driver, int column, float *to) {
                *to = driver->getFloat(column);
            }

            static void driverToVariable(const SqlDriver* driver, int column, double *to) {
                *to = driver->getDouble(column);
            }

            static void variableToDriver(SqlDriver* driver, int column, int from) {
                driver->bindInt(column, from);
            }

            static void variableToDriver(SqlDriver* driver, int column, std::string from) {
                driver->bindStdString(column, from);
            }

            static void variableToDriver(SqlDriver* driver, int column, float from) {
                driver->bindFloat(column, from);
            }

            static void variableToDriver(SqlDriver* driver, int column, double from) {
                driver->bindDouble(column, from);
            }
        };
    }
}

#endif // SALSABIL_TYPERESOLVER_HPP

