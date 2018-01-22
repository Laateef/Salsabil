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

#ifndef SALSABIL_DATE_HPP
#define SALSABIL_DATE_HPP

#include <chrono>

#include "internal/StringHelper.hpp"

namespace Salsabil {

    /**
     * @class Date
     * @brief Date is an immutable class representing a date without a time-zone in the ISO-8601 calendar system, such as "2017-12-15".
     * 
     * The ISO-8601 calendar system is the modern civil calendar system used today in most of the world. It is equivalent to the proleptic Gregorian calendar system. 
     * 
     * Date describes the date as a triple (year, month, day). There is no year 0, dates with year 0 are considered invalid. 
     * Negative years indicate years before the common era (BCE). So, year -1 represents year 1 BCE, year -2 represents year 2 BCE, and so on.
     * 
     * A Default-constructed Date object is invalid (calling isValid() on it returns false). 
     * Date objects can be created by giving the number of years, months and days, explicitly. Also, it can be created from the number of days since the epoch "1970-01-01".
     * 
     * The year, month and day of the date can be accessed though the methods year(), month() and day(), respectively.
     * Other date fields, such as day-of-year, day-of-week and week-of-year, can also be accessed through dayOfYear(), dayOfWeek() and weekOfYear(), respectively.
     * 
     * Date defines two durations; #Days and #Weeks, and two enumerations #Weekday and #Month.
     * 
     * Date provides methods for manipulating dates. Years, months, and days can be added to a date (through addYears(), addMonths(), and addDays(), respectively) or subtracted from it (through subtractYears(), subtractMonths(), and subtractDays(), respectively).
     * 
     * It also provides operators for comparing dates. Date A is considered earlier than Date B if A is smaller than B, and so on.
     * The methods daysBetween() and weeksBetween() returns how many days and weeks between two dates, respectively.
     * 
     * Also, Date provides the methods fromJulianDay() and toJulianDay() to convert a date from and to a Julian Day Number (JDN).
     * The toString() method can be used to get a textual representation of the date formatted according to a given formatter string.
     */

    class Date {
    public:
        /// @name Durations
        //@{
        /// Day duration.
        using Days = std::chrono::duration
                <long, std::ratio_multiply<std::ratio<24>, std::chrono::hours::period>>;
        /// Week duration.
        using Weeks = std::chrono::duration
                <long, std::ratio_multiply<std::ratio<7>, Days::period>>;
        //@}

        /// @name Enumerations
        //@{
        /// Weekday enumeration.

        enum class Weekday {
            Monday = 1,
            Tuesday = 2,
            Wednesday = 3,
            Thursday = 4,
            Friday = 5,
            Saturday = 6,
            Sunday = 7
        };

        /// Month enumeration.

        enum class Month {
            January = 1,
            February = 2,
            March = 3,
            April = 4,
            May = 5,
            June = 6,
            July = 7,
            August = 8,
            September = 9,
            October = 10,
            November = 11,
            December = 12
        };
        //@}

        /// @name Constructors and Destructors
        //@{
        /// Default constructor. Constructs an invalid Date object with every field is set to zero (calling isValid() on it returns false). @see isValid()
        Date();

        /// Copy-constructs a Date object from ***other***.
        Date(const Date& other);

        /// Move-constructs a Date object from ***other***.
        Date(Date&& other);

        /// Constructs a Date object from ***days*** elapsed since the epoch "1970-01-01".
        explicit Date(const Days& days);

        /// Constructs a Date object from the given ***year***, ***month*** and ***day***.
        explicit Date(int year, int month, int day);

        /// @brief Default destructor.

        virtual ~Date() {
        }
        //@}

        /// @name Assignment Operators
        //@{
        /// Copy assignment operator.
        Date& operator=(const Date& other);

        /// Move assignment operator.
        Date& operator=(Date&& other);
        //@}

        /// @name Comparison Operators
        //@{
        /// Returns whether this date is earlier than ***other***.
        bool operator<(const Date& other) const;

        /// Returns whether this date is earlier than ***other*** or equal to it.
        bool operator<=(const Date& other) const;

        /// Returns whether this date is later than ***other***.
        bool operator>(const Date& other) const;

        /// Returns whether this date is later than ***other*** or equal to it.
        bool operator>=(const Date& other) const;

        /// Returns whether this date is equal to ***other***.
        bool operator==(const Date& other) const;

        /// Returns whether this date is different from ***other***.
        bool operator!=(const Date& other) const;
        //@}

        /// @name Querying Methods
        //@{
        /** 
         * @brief Returns whether this date object represents a valid date. 
         * 
         * A valid date contains a non-zero year, a valid month (between 1 and 12) and a valid day of month (between 1 and 31).
         * {@code
         *    Date d; // same as Date(0, 0, 0)
         *    bool state1 = d.isValid(); // returns false
         *    bool state2 = Date(1999, -1, 1).isValid(); // returns false
         *    bool state3 = Date(1999, 1, 0).isValid(); // returns false
         *    d = Date(1970, 1, 1);
         *    bool state4 = d.isValid(); // returns true
         * }
         */
        bool isValid() const;

        /// Set the year, month and day of this date in the parameters ***year***, ***month*** and ***day***, respectively.
        void getYearMonthDay(int* year, int* month, int* day) const;

        /// Returns the day of month of this date as a number between 1 and 31.
        int day() const;

        /// Returns the month of year of this date as a number between 1 and 12, which corresponds to the enumeration #Month.
        int month() const;

        /// Returns the year of this date as a number. The is no year 0. Negative numbers indicate years before 1 BCE, for example, year -1 is year 1 BCE, and so on.
        int year() const;

        /// Returns the weekday of this date as a number between 1 and 7, which corresponds to the enumeration #Weekday.
        int dayOfWeek() const;

        /// Returns the day of year of this date as a number between 1 and 365 (1 to 366 on leap years).
        int dayOfYear() const;

        /// Returns the number of days in the month of this date. It ranges between 28 and 31.
        int daysInMonth() const;

        /// Returns the number of days in the year of this date. It is either 365 or 366.
        int daysInYear() const;

        /// Returns whether the year of this date is a leap year. @see isLeapYear(int)
        bool isLeapYear() const;

        /** 
         * @brief Returns the week of the year of this date, which ranges between 1 and 53, and stores the year in weekYear unless it is not specified (defaulted).
         * 
         * According to ISO-8601, weeks start on Monday and the first Thursday of a year is always in week 1 of that year. Most years have 52 weeks, but some have 53.
         * yearWeek is not always the same as year(). For example, the date of "1 January 2000" falls in the week 52 of the year 1999, the date of "31 December 2002" falls in the week 1 of the year 2003, and the date of "1 January 2010" falls in the week 53 of the year 2009.
         */
        int weekOfYear(int* weekYear = nullptr) const;

        /** 
         * @brief Returns the name of the weekday of this date. 
         * 
         * @param useShortName If true, returns the short name of the weekday such as "Sat". If false (by default), returns the long name such as "Saturday". 
         * 
         * The short and long names of the weekdays are named according to following convention:
         * 
         *  Weekday No. | Short Name | Long Name
         *  ----------- | ---------- | ---------
         *  1           | Mon        | Monday
         *  2           | Tue        | Tuesday
         *  3           | Wed        | Wednesday
         *  4           | Thu        | Thursday
         *  5           | Fri        | Friday
         *  6           | Sat        | Saturday
         *  7           | Sun        | Sunday
         */
        std::string dayOfWeekName(bool useShortName = false) const;

        /** 
         * @brief Returns the name of the month of this date. 
         * 
         * @param useShortName If true, returns the short name of the month such as "Jan". If false (by default), returns the long name such as "January". 
         * 
         * The short and long names of the months are named according to following convention:
         * 
         *  Month No. | Short Name | Long Name
         *  --------- | ---------- | ---------
         *  1         | Jan        | January
         *  2         | Feb        | February
         *  3         | Mar        | March
         *  4         | Apr        | April
         *  5         | May        | May
         *  6         | Jun        | June
         *  7         | Jul        | July
         *  8         | Aug        | August
         *  9         | Sep        | September
         *  10        | Oct        | October
         *  11        | Nov        | November
         *  12        | Dec        | December
         */
        std::string monthName(bool useShortName = false) const;
        //@}

        /// @name Addition/Subtraction Methods
        //@{
        /// Returns the result of adding ***days*** to this date as a new Date object.
        Date addDays(int days) const;

        /// Returns the result of subtracting ***days*** from this date as a new Date object.
        Date subtractDays(int days) const;

        /** 
         * @brief Returns the result of adding ***months*** to this date as a new Date object.  
         *          
         * When the ending day/month combination does not exist in the resulting month/year, it returns the latest valid date. For example:
         * {@code
         *    Date d = Date(2013, 1, 31).addMonths(1); //d = Date(2013, 2, 28)
         * }
         */
        Date addMonths(int months) const;

        /** 
         * @brief Returns the result of subtracting ***months*** from this date as a new Date object. 
         *
         * When the ending day/month combination does not exist in the resulting month/year, it returns the latest valid date. For example:
         * {@code
         *    Date d = Date(2012, 3, 31).subtractMonths(1); //d = Date(2012, 2, 29)
         * }
         */
        Date subtractMonths(int months) const;

        /// Returns the result of adding ***years*** to this date as a new Date object.
        Date addYears(int years) const;

        /// Returns the result of subtracting ***years*** from this date as a new Date object.
        Date subtractYears(int years) const;
        //@}

        /// @name Conversion Methods
        //@{
        /// Returns the number of elapsed days since the epoch "1970-01-01".
        long toDaysSinceEpoch() const;

        /// Returns the elapsed time since the epoch "1970-01-01" as a #Days duration.
        Days toStdDurationSinceEpoch() const;

        /** 
         * @brief Returns the corresponding Julian Day Number (JDN) of this date. 
         * 
         * JDN is the consecutive numbering of days since the beginning of the Julian Period on 1 January 4713 BCE in the proleptic Julian calendar, which occurs on 24 November 4714 BCE in the proleptic Gregorian Calender.
         * 
         * Note that The date to be converted is considered Gregorian. Also, the current Gregorian rules are extended backwards and forwards. 
         * 
         * There is no year 0. The first year before the common era (i.e. year 1 BCE) is year -1, year -2 is year 2 BCE, and so on.  
         */
        long toJulianDay() const;

        /** 
         * @brief Returns this date as a string, formatted according to the formatter string ***format***. 
         * 
         * The formatter string may contain the following patterns:
         * 
         *    Pattern   |                        Meaning                    
         *  ----------- | --------------------------------------------------------- 
         *  #           | era of year as a positive sign(+) or negative sign(-)                       
         *  E           | era of year as CE or BCE                      
         *  y           | year as one digit or more (1, 9999)                      
         *  yy          | year of era as two digits (00, 99)                  
         *  yyyy        | year as four digits (0000, 9999)                  
         *  M           | month of year as one digit or more (1, 12)                     
         *  MM          | month of year as two digits (01, 12)                
         *  MMM         | month of year as short name (e.g. "Feb")               
         *  MMMM        | month of year as long name (e.g. "February")              
         *  d           | day of month as one digit or more (1, 31)                
         *  dd          | day of month as two digits (00, 31)          
         *  ddd         | day of week as short name (e.g. "Fri")         
         *  dddd        | day of week as long name (e.g. "Friday")       
         *
         * Any character in the formatter string not listed above will be inserted as is into the output string. 
         * If this date is invalid, an empty string will be returned.
         * 
         * @see dayOfWeekName(), monthName()
         */
        std::string toString(const std::string& format) const;
        //@}

        /** 
         * @brief Returns a Date object set to the current date obtained from the system clock.
         * 
         * **Note** that the returned date is not the current local date, rather it is the current system date; the current date in Coordinated Universal %Time (UTC). 
         */
        static Date current();

        /// Returns a Date object set to the epoch "1970-01-01".
        static Date epoch();

        /** 
         * @brief Returns a Date object from the date string ***date*** according to the formatter string ***format***.
         * 
         * The formatter patterns are the same patterns used in the method toString(). @see toString() 
         */
        static Date fromString(const std::string& date, const std::string& format);

        /// Returns a Date object corresponding to the Julian day ***julianDay***. @see toJulianDay()
        static Date fromJulianDay(long julianDay);

        /**  
         * @name Calculation Methods
         * @brief The following methods return the time difference between two dates; ***from*** and ***to***. If ***to*** is earlier than (smaller than) ***from***, then the difference is negative.
         */
        //@{

        /** 
         * @brief Returns the number of days between ***from*** and ***to***. 
         *
         * For example:
         * {@code
         *    int num = Date::daysBetween(Date(1999, 1, 1), Date(1999, 1, 3));  // num = 2
         * } 
         */
        static long daysBetween(const Date& from, const Date& to);

        /** 
         * @brief Returns the number of weeks between ***from*** and ***to***. 
         *
         * For example:
         * {@code
         *    int num = Date::weeksBetween(Date(1970, 1, 1), Date(1970, 1, 8));  // num = 1
         * } 
         */
        static long weeksBetween(const Date& from, const Date& to);
        //@}

        /** 
         * @brief Returns whether ***year*** is a leap year.
         * 
         * According to the ISO proleptic calendar system rules, a year is a leap year if it is divisible by four without remainder. However, years divisible by 100, are not leap years, with the exception of years divisible by 400 which are.
         * For example, 1904 is a leap year it is divisible by 4. 1900 was not a leap year as it is divisible by 100, however 2000 was a leap year as it is divisible by 400.
         */
        static bool isLeapYear(int year);

        /// Returns the number of days in ***month*** of ***year***. It ranges between 28 and 31.
        static int daysInMonthOfYear(int year, int month);

    private:
        int mYear;
        int mMonth;
        int mDay;
    };

    /** 
     * @relates Date
     * @name Input/Output Operators 
     */
    //@{

    /// Writes date ***d*** to stream ***os*** in ISO-8601 date format "yyyy-MM-dd". See toString() for information about the formatter patterns.
    std::ostream& operator<<(std::ostream& os, const Date& d);

    /// Reads a date in ISO-8601 date format "yyyy-MM-dd" from stream ***is*** and stores it in date ***d***. See toString() for information about the formatter patterns.
    std::istream& operator>>(std::istream& is, Date& d);
    //@}

}
#endif // SALSABIL_DATE_HPP 

