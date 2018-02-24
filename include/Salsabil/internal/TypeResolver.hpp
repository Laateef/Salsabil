#ifndef SALSABIL_TYPERESOLVER_HPP
#define SALSABIL_TYPERESOLVER_HPP

#include <string>

#include "SqlDriver.hpp"
#include "internal/Logging.hpp"


namespace Salsabil {
    template<typename> class SqlTableConfigurer;

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

        template<class T>
        void driverToVariable(const SqlDriver* driver, int column, T* to) {
            SALSABIL_LOG_DEBUG("Fetching entity's primary key at column '" + std::to_string(column) + "' ");
            for (const auto& field : SqlTableConfigurer<T>::primaryFieldList())
                field->readFromDriver(to, column);
        }

        template<class T>
        void variableToDriver(SqlDriver* driver, int column, const T* from) {
            SALSABIL_LOG_DEBUG("Binding entity's primary key at column '" + std::to_string(column) + "'");
            for (const auto& field : SqlTableConfigurer<T>::primaryFieldList())
                field->writeToDriver(from, column);
        }

        template<class T>
        void driverToVariable(const SqlDriver* driver, int column, T** to) {
            driverToVariable(driver, column, *to);
        }

        template<class T>
        void variableToDriver(SqlDriver* driver, int column, const T** from) {
            variableToDriver(driver, column, *from);
        }

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

