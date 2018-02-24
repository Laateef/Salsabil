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

#ifndef SALSABIL_SQLDRIVERMOCK_HPP
#define SALSABIL_SQLDRIVERMOCK_HPP

#include "SqlDriver.hpp"
#include <map>
#include <algorithm>
#include "Exception.hpp"

using namespace Salsabil;

class SqlDriverMock : public SqlDriver {
public:

    SqlDriverMock() {
    }

    virtual std::string driverName() const {
        return "MockDriver";
    }

    virtual SqlDriver* create() const {
        return new SqlDriverMock;
    }

    virtual void open(const std::string& databasePath) {
    }

    virtual bool isOpen() const {
        return false;
    }

    virtual void close() {
    }

    virtual void prepare(const std::string& statement) {
    }

    virtual void execute() {
    }

    virtual void execute(const std::string& statement) {
    }

    virtual bool nextRow() {
        return false;
    }

    virtual bool isNull(int column) const {
        return false;
    }

    virtual int getInt(int column) const {
        return 0;
    }

    virtual int64_t getInt64(int column) const {
        return 0;
    }

    virtual float getFloat(int column) const {
        return 0;
    }

    virtual double getDouble(int column) const {
        return 0;
    }

    virtual const unsigned char* getRawString(int column) const {
        return nullptr;
    }

    virtual const char* getCString(int column) const {
        return "";
    }

    virtual std::string getStdString(int column) const {
        return std::string();
    }

    virtual std::size_t getSize(int column) const {
        return 0;
    }

    virtual const void* getBlob(int column) const {
        return nullptr;
    }

    virtual void bindNull(int column) const {
    }

    virtual void bindInt(int column, int value) const {
    }

    virtual void bindInt64(int column, int64_t value) const {
    }

    virtual void bindFloat(int column, float value) const {
    }

    virtual void bindDouble(int column, double value) const {
    }

    virtual void bindCString(int column, const char* value) const {
    }

    virtual void bindStdString(int column, const std::string& value) const {
    }

    virtual void bindBlob(int column, const void* date, std::size_t size) const {
    }

    virtual std::vector<std::string> tableList() {
        return std::vector<std::string>();
    }

    virtual std::vector<std::string> columnList(const std::string& table) {
        return std::vector<std::string>();
    }
};

#endif // SALSABIL_SQLDRIVERMOCK_HPP