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

#include "SqliteDriver.hpp"
#include "Exception.hpp"

#include "sqlite3.h"
#include <iostream>

using namespace Salsabil;

SqliteDriver::SqliteDriver()
: mHandle(nullptr)
, mStatement(nullptr) {
}

void SqliteDriver::open(const std::string& databaseFileName) {
    int errorCode = sqlite3_open_v2(databaseFileName.c_str(), &mHandle, SQLITE_OPEN_READWRITE, nullptr);
    if (errorCode != SQLITE_OK) {
        throw Exception("Error occured while opening " + databaseFileName + " with error code " +
                std::to_string(errorCode) + " " + sqlite3_errmsg(mHandle));
    }
}

bool SqliteDriver::isOpen() {
    if (mHandle)
        return true;

    return false;
}

void SqliteDriver::close() {
    int errorCode = sqlite3_close_v2(mHandle);
    if (errorCode != SQLITE_OK) {
        const char* fileName = sqlite3_db_filename(mHandle, "main");

        throw Exception("Error occured while closing " + std::string(fileName ? fileName : "") +
                " with error code " + std::to_string(errorCode) + " " + sqlite3_errmsg(mHandle));
    }
    mHandle = nullptr;
}

void SqliteDriver::prepare(const std::string& sqlStatement) {
    int errorCode = sqlite3_prepare_v2(mHandle, sqlStatement.c_str(), -1, &mStatement, nullptr);
    if (errorCode != SQLITE_OK) {
        throw Exception("Error occured while preparing " + sqlStatement + " with error code " +
                std::to_string(errorCode) + " " + sqlite3_errmsg(mHandle));
    }
}

void SqliteDriver::execute() {
    int errorCode = sqlite3_step(mStatement);
    switch (errorCode) {
        case SQLITE_DONE:
            finalize();
        case SQLITE_ROW:
            return;
        default:
            throw Exception("Error occured while executing with error code " + std::to_string(errorCode) + " " +
                    sqlite3_errmsg(mHandle));
    }
}

bool SqliteDriver::nextRow() {
    if (mStatement) {
        switch (int errorCode = sqlite3_step(mStatement)) {
            case SQLITE_ROW:
                return true;
            case SQLITE_DONE:
                finalize();
                return false;
            default:
                throw Exception("Error occured while fetching the next row with error message " +
                        std::to_string(errorCode) + " " + sqlite3_errmsg(mHandle));
        }
    } else {
        throw Exception("No statement executed to fetch from!");
    }
}

bool SqliteDriver::isNull(int columnIndex) {
    return sqlite3_column_type(mStatement, columnIndex) == SQLITE_NULL;
}

int SqliteDriver::getInt(int columnIndex) {
    return sqlite3_column_int(mStatement, columnIndex);
}

int64_t SqliteDriver::getInt64(int columnIndex) {
    return sqlite3_column_int64(mStatement, columnIndex);
}

double SqliteDriver::getDouble(int columnIndex) {
    return sqlite3_column_double(mStatement, columnIndex);
}

const unsigned char* SqliteDriver::getRawString(int columnIndex) {
    return sqlite3_column_text(mStatement, columnIndex);
}

const char* SqliteDriver::getCString(int columnIndex) {
    return reinterpret_cast<const char*> (getRawString(columnIndex));
}

std::string SqliteDriver::getStdString(int columnIndex) {
    return std::string(getCString(columnIndex));
}

std::size_t SqliteDriver::getSize(int columnIndex) {
    return sqlite3_column_bytes(mStatement, columnIndex);
}

const void* SqliteDriver::getBlob(int columnIndex) {
    return sqlite3_column_blob(mStatement, columnIndex);
}

void SqliteDriver::bindNull(int position) {
    int errorCode = sqlite3_bind_null(mStatement, position);
    if (errorCode != SQLITE_OK) {
        throw Exception("Error occured while binding parameter at position " + std::to_string(position) +
                " with error code " + std::to_string(errorCode) + " " + sqlite3_errmsg(mHandle));
    }
}

void SqliteDriver::bindInt(int position, int value) {
    int errorCode = sqlite3_bind_int(mStatement, position, value);
    if (errorCode != SQLITE_OK) {
        throw Exception("Error occured while binding parameter at position " + std::to_string(position) +
                " with error code " + std::to_string(errorCode) + " " + sqlite3_errmsg(mHandle));
    }
}

void SqliteDriver::bindInt64(int position, int64_t value) {
    int errorCode = sqlite3_bind_int64(mStatement, position, value);
    if (errorCode != SQLITE_OK) {
        throw Exception("Error occured while binding parameter at position " + std::to_string(position) +
                " with error code " + std::to_string(errorCode) + " " + sqlite3_errmsg(mHandle));
    }
}

void SqliteDriver::bindDouble(int position, double value) {
    int errorCode = sqlite3_bind_double(mStatement, position, value);
    if (errorCode != SQLITE_OK) {
        throw Exception("Error occured while binding parameter at position " + std::to_string(position) +
                " with error code " + std::to_string(errorCode) + " " + sqlite3_errmsg(mHandle));
    }
}

void SqliteDriver::bindCString(int position, const char* str) {
    int errorCode = sqlite3_bind_text(mStatement, position, str, -1, SQLITE_TRANSIENT);
    if (errorCode != SQLITE_OK) {
        throw Exception("Error occured while binding parameter " + std::string(str) + " at position " +
                std::to_string(position) + " with error code " + std::to_string(errorCode) + " " +
                sqlite3_errmsg(mHandle));
    }
}

void SqliteDriver::bindStdString(int position, const std::string& str) {
    int errorCode = sqlite3_bind_text(mStatement, position, str.c_str(), -1, SQLITE_TRANSIENT);
    if (errorCode != SQLITE_OK) {
        throw Exception("Error occured while binding parameter " + str + " at position " +
                std::to_string(position) + " with error code " + std::to_string(errorCode) + " " +
                sqlite3_errmsg(mHandle));
    }
}

void SqliteDriver::bindBlob(int position, void* blob, std::size_t size) {
    int errorCode = sqlite3_bind_blob(mStatement, position, blob, size, SQLITE_TRANSIENT);
    if (errorCode != SQLITE_OK) {
        throw Exception("Error occured while binding parameter at position " + std::to_string(position) +
                " with error code " + std::to_string(errorCode) + " " + sqlite3_errmsg(mHandle));
    }
}

void SqliteDriver::finalize() {
    if (mHandle && mStatement) {
        int errorCode = sqlite3_finalize(mStatement);
        if (errorCode != SQLITE_OK) {
            throw Exception("Error occured while deleting last statement with error code " +
                    std::to_string(errorCode) + " " + sqlite3_errmsg(mHandle));
        }
        mStatement = nullptr;
    }
}

SqliteDriver::~SqliteDriver() {
    if (isOpen()) {
        try {
            finalize();
            close();
        } catch (Exception& exp) {
            std::cerr << exp.what() << std::endl;
        }
    }
}
