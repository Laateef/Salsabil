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

#ifndef SALSABIL_SQLITEDRIVER_HPP
#define SALSABIL_SQLITEDRIVER_HPP

#include "SqlDriver.hpp"

struct sqlite3;
struct sqlite3_stmt;

namespace Salsabil {

    /** 
     * @class SqliteDriver
     * @brief SqliteDriver is a driver for SQLite3 databases which deals with SQL statements in a functional style.
     * 
     * SqliteDriver encapsulates the functionality to prepare and execute SQL statements 
     * in addition to retrieving the results from executed SQL queries.
     * 
     * It can be used to execute all kinds of statements that the underlying engine 
     * (e.g., SQLite in this case) can understand, such as DML (data manipulation language)
     * statements, such as SELECT, INSERT, UPDATE and DELETE, as well as DDL (data definition language) 
     * SQL statements, such as CREATE TABLE or even any database-specific commands, such as ATTACH DATABASE.
     * 
     * Executing statements is easy and intuitive, we begin by constructing an instance of SqliteDriver. Then
     * we open the connection to the database by specifying its name. SQLite gives 
     * the opportunity to create temporary in-memory database to work on by specifying the 
     * database file name as ":memory:". Also, you have the option to create temporary on-disk 
     * database to work on by specifying empty file name parameter to the #open() function. For example:
     * {@code 
     * using namespace Salsabil;
     * 
     * SqliteDriver sd;
     * try {
     *  sd.open(":memory:");
     *  sd.prepare("CREATE TABLE tbl(id INT PRIMARY KEY)");
     *  sd.execute();
     *  sd.close();
     * } catch (const Exception& e) {
     *  std::err << e.what() << std::endl;
     * }
     * }
     * After executing SQL queries, you can iterate over the result set using #nextRow() 
     * and access the individual field values of the current row by using the functions 
     * #isNull(), #getInt(), #getInt64(), #getDouble(), #getRawString(), #getCString(), 
     * #getStdString(), #getSize(), #getBlob()
     * 
     * Each field is accessed by passing the field's position in the statement, starting from 0. 
     * The fields are numbered from left to right in the same order as the corresponding columns 
     * in the text of the SELECT statement, e.g. in {@code SELECT count(*), id, name FROM department} 
     * the columnIndex 0 is for count(*), 1 is for name and so on. For example
     * {@code 
     * try {
     *  sd.prepare("SELECT id, name FROM user");
     *  sd.execute();
     *  do {
     *      std::cout << sd.getInt(0) << " " << sd.getCString(1) << std::endl;
     *  } while(sd.nextRow());
     * } catch (const Exception& e) {
     * ...
     * }
     * }
     * SqliteDriver supports binding parameter values to placeholders while preparing 
     * SQL statements by means of the functions #bindNull(), #bindInt()
     * #bindInt64(), #bindDouble(), #bindCString(), #bindStdString(), #bindBlob()
     * 
     * Contrary to the reult retrieving functions, binding functions bind parameter values 
     * to positional placeholders from index 1 in order if no indices are specified along the ? 
     * placeholders (e.g., ?1, ?3, ?5, etc). For example:
     * {@code 
     * try {
     *  sd.prepare("INSERT INTO player VALUES(?, ?, ?)");
     *  sd.bind(1, 6);
     *  sd.bind(2, "Muhammad Ali Clay");
     *  sd.bind(3, 93);
     *  sd.execute();
     * } catch (const Exception& e) {
     * ...
     * }
     * }
     */
    class SqliteDriver : public SqlDriver {
    public:
        /** @brief Constructs a SqliteDriver. */
        SqliteDriver();

        virtual ~SqliteDriver();

        /** @brief Returns the name of this SQL driver. */
        virtual std::string driverName() const;

        /** @brief Returns an instance of this driver. */
        virtual SqliteDriver* create() const;

        /** 
         * @brief Opens a connection to database databaseFileName.
         * @param databaseFileName location of the database file.
         * @throw Exception if database file not found. 
         */
        virtual void open(const std::string& databaseFileName);

        /** 
         * @brief Returns the state of connectivity with the database.
         * @retval true if the connection to database is still open.
         * @retval false otherwise.
         */
        virtual bool isOpen() const;

        /** 
         * @brief Closes the current connection to database.
         * @throw Exception if an error occurred while trying to close the database connection. 
         */
        virtual void close();

        /** 
         * @brief Prepares the SQL statement sqlStatement for execution.
         * The statement may contain placeholders for binding values. Currently, only 
         * the ODBC style (? placeholders) is supported.
         * The statement sqlStatement may not contain more than one SQL statement. 
         * @throw Exception if an error occurred while trying to prepare the statement. 
         */
        virtual void prepare(const std::string& sqlStatement);

        /** 
         * @brief Executes the latest SQL statement prepared.
         * @throw Exception if an error occurred while trying to execute the statement. 
         */
        virtual void execute();

        /** 
         * @brief This convenience function prepares and executes the SQL statement <i>sqlStatement</i> in one call.
         * @throw Exception if an error occurred. 
         */
        virtual void execute(const std::string& sqlStatement);

        /**  
         * @brief Fetches the next row from the result set if available.
         * @retval true if a row is fetched.
         * @retval false if there is no further row to be fetched.
         * @throw Exception if called to fetch a row after executing a non-query 
         * statement or while trying to fetch beyond the last row in the result 
         * set (i.e., after #nextRow() has been called and returned false). 
         */
        virtual bool nextRow();

        /**  
         * @name Result Retrieving Functions
         * @brief These functions retrieve the field values from the current row of the result set of the executed query.
         * @param columnIndex the index of the field column in the current row that needs to be retrieved.
         */
        //@{
        /**  
         * @brief Checks whether the value of the field <i>columnIndex</i> in the current row is NULL.
         * @retval true if the value is NULL.
         * @retval false otherwise.
         */
        virtual bool isNull(int columnIndex) const;

        /** @brief Returns the integer value of the field <i>columnIndex</i> in the current row. */
        virtual int getInt(int columnIndex) const;

        /** @brief Returns the 64bit-integer value of the field <i>columnIndex</i> in the current row. */
        virtual int64_t getInt64(int columnIndex) const;

        /** @brief Returns the float value of the field <i>columnIndex</i> in the current row. */
        virtual float getFloat(int columnIndex) const;

        /** @brief Returns the double value of the field <i>columnIndex</i> in the current row. */
        virtual double getDouble(int columnIndex) const;

        /** @brief Returns the string value of the field <i>columnIndex</i> in the current row as raw data pointer. */
        virtual const unsigned char* getRawString(int columnIndex) const;

        /** @brief Returns the string value of the field <i>columnIndex</i> in the current row as a C-string. */
        virtual const char* getCString(int columnIndex) const;

        /** @brief Returns the string value of the field <i>columnIndex</i> in the current row as a STL-string. */
        virtual std::string getStdString(int columnIndex) const;

        /** @brief Returns the value size of the field <i>columnIndex</i> in the current row in bytes. */
        virtual std::size_t getSize(int columnIndex) const;

        /** @brief Returns the blob value of the field <i>columnIndex</i> in the current row as a void pointer. */
        virtual const void* getBlob(int columnIndex) const;
        //@}

        /**  
         * @name Binding Functions
         * @brief These functions bind parameter values to positional placeholders in the SQL statement 
         * text that are of the form ?NNN where NNN is an integer between 1 and 999 specifies the position of the placeholder.
         * @param position the position of the placeholder that needs to be replaced.
         * @throw Exception if the parameter couldn't be bound or <i>position</i> is out of range. 
         * @see SqliteDriver 
         */
        //@{
        /** Binds a NULL to the placeholder at <i>position</i>. */
        virtual void bindNull(int position) const;

        /** Binds an integer to the placeholder at <i>position</i>. */
        virtual void bindInt(int position, int value) const;

        /** Binds a 64bit integer to the placeholder at <i>position</i>. */
        virtual void bindInt64(int position, int64_t value) const;

        /** Binds a double to the placeholder at <i>position</i>. */
        virtual void bindDouble(int position, double value) const;

        /** Binds a C literal string to the placeholder at <i>position</i>. */
        virtual void bindCString(int position, const char* str) const;

        /** Binds a standard literal string to the placeholder at <i>position</i>. */
        virtual void bindStdString(int position, const std::string& str) const;

        /** Binds a non-typed array <i>blob</i> of length <i>size</i> to the placeholder at <i>position</i>. */
        virtual void bindBlob(int position, const void* blob, std::size_t size) const;
        //@}

        /** Returns a list(as a vector of strings) containing the existing database tables. */
        virtual std::vector<std::string> tableList();

        /** Returns a list(as a vector of strings) containing the columns of the table <i>table</i> in order. */
        virtual std::vector<std::string> columnList(const std::string& table);

    private:
        void finalize();

        sqlite3* mHandle;
        sqlite3_stmt* mStatement;
        bool mNextFetchFlag;
        bool mDelayCycleFlag;
    };
}
#endif // SALSABIL_SQLITEDRIVER_HPP
