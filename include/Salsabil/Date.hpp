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

#ifndef SALSABIL_DATE_HPP
#define SALSABIL_DATE_HPP

#include <chrono>

#include "internal/StringHelper.hpp"

namespace Salsabil {

    /**
     * @class Date
     * @brief Date is an immutable class representing a date without a time-zone in the ISO-8601 calendar system, such as 2017-12-15.
     * 
     * The ISO-8601 calendar system is the modern civil calendar system used today in most of the world. It is equivalent to the proleptic Gregorian calendar system. 
     * Date describes the date as the number of days since year 1 CE. 
     * 
     * A default-constructed Date objects is initialized to the current date from the system clock. 
     * Date objects can be created by giving the number of years, months and days, explicitly. Also, it can be created from the number of days since year 1 CE.
     * 
     * The year, month and day of the date can be accessed though the functions year(), month() and day(), respectively.
     * Other date fields, such as day-of-year, day-of-week and week-of-year, can also be accessed through dayOfYear(), dayOfWeek() and weekOfYear(), respectively.
     * 
     * Date provides functions for manipulating dates. Years, months, and days can be added to a date (through addYears(), addMonths(), and addDays(), respectively) or subtracted from it (through subtractYears(), subtractMonths(), and subtractDays(), respectively).
     * It also provides functions for comparing dates. Date A is considered earlier than Date B if A is smaller than B, and so on.
     * the function daysBetween() returns how many days between two dates.
     * 
     * The toString() function can be used to get a textual representation of the date formatted according to a given formatter string.
     */

    class Date {
    public:
        /** @name Aliases */
        //@{
        using Days = std::chrono::duration
                <long, std::ratio_multiply<std::ratio<24>, std::chrono::hours::period>>;
        //@}

        /** @name Enumerations */
        //@{

        enum class Weekday {
            Monday = 1,
            Tuesday = 2,
            Wednesday = 3,
            Thursday = 4,
            Friday = 5,
            Saturday = 6,
            Sunday = 7
        };

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

        /** @name Constructors and Destructors */
        //@{

        /** @brief Constructs a Date object from the given <i>year</i>, <i>month</i> and <i>day</i>. */
        explicit Date(int year, int month, int day);

        /** @brief Constructs a Date object from the given number of days <i>days</i>. */
        explicit Date(const Days& days);

        /** @brief Copy-constructs a Date object from the given <i>date</i> object. */
        Date(const Date& date) = default;

        /** @brief Move-constructs a Date object from the given <i>date</i> object. */
        Date(Date&& date) = default;

        /** @brief Default constructor. Constructs a Date object and initializes it to the current date obtained from the system clock. */
        Date();

        /** @brief Default destructor. */
        virtual ~Date() {
        }
        //@}

        /** @name Manipulation Functions */
        //@{

        /** @brief Returns a new copy of this Date object with the days <i>days</i> added to it. */
        Date addDays(int days) const;

        /** @brief Returns a new copy of this Date object with the days <i>days</i> subtracted from it. */
        Date subtractDays(int days) const;

        /** 
         * @brief Returns a new copy of this Date object with the months <i>months</i> added to it. 
         *          
         * When the ending day/month combination does not exist in the resulting month/year, it returns the latest valid date. For example:
         * {@code
         *    Date d = Date(2013, 1, 31).addMonths(1); //d = Date(2013, 2, 28)
         * }
         */
        Date addMonths(int months) const;

        /** 
         * @brief Returns a new copy of this Date object with the months <i>months</i> subtracted from it. 
         *
         * When the ending day/month combination does not exist in the resulting month/year, it returns the latest valid date. For example:
         * {@code
         *    Date d = Date(2012, 3, 31).subtractMonths(1); //d = Date(2012, 2, 29)
         * }
         */
        Date subtractMonths(int months) const;

        /** @brief Returns a new copy of this Date object with the years <i>years</i> added to it. */
        Date addYears(int years) const;

        /** @brief Returns a new copy of this Date object with the years <i>years</i> subtracted from it. */
        Date subtractYears(int years) const;
        //@}

        /** @name Querying Functions */
        //@{

        /** 
         * @brief Set the year, month and day of this date in the parameters year, month and day, respectively. 
         *
         * Calling this function is faster than calling the three equivalents year(), month(), and day(). 
         */
        void getYearMonthDay(int* year, int* month, int* day) const;

        /** @brief Returns the year of this date as a number. Negative numbers indicate years before 1 CE, for example, year -125 is 125 BCE. */
        int year() const;

        /** @brief Returns the month of this date as a number between 1 and 12, which corresponds to the enumeration #Month. */
        int month() const;

        /** @brief Returns the day of the month of this date as a number between 1 and 31. */
        int day() const;

        /** @brief Returns the weekday of this date as a number between 1 and 7, which corresponds to the enumeration #Weekday. */
        int dayOfWeek() const;

        /** @brief Returns the day of the year of this date as a number between 1 and 365 or 366 on leap years. */
        int dayOfYear() const;

        /** @brief Returns the number of days in the month of this date. It ranges between 28 and 31. */
        int daysInMonth() const;

        /** @brief Returns the number of days in the year of this date. It is either 365 or 366. */
        int daysInYear() const;

        /** 
         * @brief Returns whether the year of this date is a leap year.
         * 
         * For more information see isLeapYear(int).
         */
        bool isLeapYear() const;

        /** 
         * @brief Returns the week of the year of this date, which ranges between 1 and 53, and stores the year in weekYear unless it is not specified (will be defaulted to nullptr).
         * 
         * According to ISO-8601, weeks start on Monday and the first Thursday of a year is always in week 1 of that year. Most years have 52 weeks, but some have 53.
         * yearWeek is not always the same as year(). For example, the date of '1 January 2000' falls in the week 52 of the year 1999, the date of '31 December 2002' falls in the week 1 of the year 2003, and the date of '1 January 2010' falls in the week 53 of the year 2009.
         */
        int weekOfYear(int* weekYear = nullptr) const;

        /** 
         * @brief Returns the name of the weekday of this date. 
         * 
         * @param useShortName If true, returns the short name of the weekday such as "Sat". If false (the default), returns the long name such as "Saturday". 
         * The short and long names of the weekdays are named according to following convention:
         * <table>
         * <tr><th>Weekday No.<th>Short Name<th>Long Name
         * <tr><td>1<td>Mon<td>Monday
         * <tr><td>2<td>Tue<td>Tuesday
         * <tr><td>3<td>Wed<td>Wednesday
         * <tr><td>4<td>Thu<td>Thursday
         * <tr><td>5<td>Fri<td>Friday
         * <tr><td>6<td>Sat<td>Saturday
         * <tr><td>7<td>Sun<td>Sunday
         * </table>
         */
        std::string dayOfWeekName(bool useShortName = false) const;

        /** 
         * @brief Returns the name of the month of this date. 
         * 
         * @param useShortName If true, returns the short name of the month such as "Jan". If false(the default), returns the long name such as "January". 
         * The short and long names of the months are named according to following convention:
         * <table>
         * <tr><th>Month No.<th>Short Name<th>Long Name
         * <tr><td>1<td>Jan<td>January
         * <tr><td>2<td>Feb<td>February
         * <tr><td>3<td>Mar<td>March
         * <tr><td>4<td>Apr<td>April
         * <tr><td>5<td>May<td>May
         * <tr><td>6<td>Jun<td>June
         * <tr><td>7<td>Jul<td>July
         * <tr><td>8<td>Aug<td>August
         * <tr><td>9<td>Sep<td>September
         * <tr><td>10<td>Oct<td>October
         * <tr><td>11<td>Nov<td>November
         * <tr><td>12<td>Dec<td>December
         * </table>
         */
        std::string monthName(bool useShortName = false) const;
        //@}

        /** @name Comparison Operators */
        //@{

        /** @brief Returns whether this date is earlier than the date of <i>other</i>. */
        bool operator<(const Date& other) const;

        /** @brief Returns whether this date is earlier than the date of <i>other</i> or equal to it. */
        bool operator<=(const Date& other) const;

        /** @brief Returns whether this date is later than the date of <i>other</i>. */
        bool operator>(const Date& other) const;

        /** @brief Returns whether this date is later than the date of <i>other</i> or equal to it. */
        bool operator>=(const Date& other) const;

        /** @brief Returns whether this date is equal to the date of <i>other</i>. */
        bool operator==(const Date& other) const;

        /** @brief Returns whether this date is different from the date of <i>other</i>. */
        bool operator!=(const Date& other) const;
        //@}

        /** @name Assignment Operators */
        //@{
        /** @brief Copy assignment operator. */
        Date& operator=(const Date& date) = default;

        /** @brief Move assignment operator. */
        Date& operator=(Date&& date) = default;
        //@}

        /** @name Conversion Functions */
        //@{

        /** @brief Returns the number of elapsed days since 1 CE. */
        long toDays() const;

        /** @brief Returns the corresponding Julian Day Number (JDN) of this date. JDN is the consecutive numbering of days since the beginning of the Julian Period (1 January 4713 BCE).  */
        long toJulianDay() const;

        /** 
         * @brief Returns the date as a string formatted according to the formatter string <i>format</i>. 
         * 
         * The formatter string may contain the following patterns:
         * <table>
         * <tr><th>Pattern<th>Meaning
         * <tr><td>#<td>the era of year as a positive sign(+) or negative sign(-)
         * <tr><td>E<td>the era of year as CE or BCE
         * <tr><td>y<td>the year as one digit or more (1, 9999)
         * <tr><td>yy<td>the year of era as two digits (00, 99)
         * <tr><td>yyyy<td>the year as four digits (0000, 9999)
         * <tr><td>M<td>the month of year as one digit or more (1, 12)
         * <tr><td>MM<td>the month of year as two digits (01, 12)
         * <tr><td>MMM<td>the month of year as short name (e.g. "Feb")
         * <tr><td>MMMM<td>the month of year as long name (e.g. "February")
         * <tr><td>d<td>the day of month as one digit or more (1, 31)
         * <tr><td>dd<td>the day of month as two digits (00, 31)
         * <tr><td>ddd<td>the day of week as short (e.g. "Fri")
         * <tr><td>dddd<td>the day of week as long name (e.g. "Friday")
         * </table>
         * Any character in the formatter string not listed above will be inserted as is into the output string. 
         * 
         * See also dayOfWeekName() and monthName()
         */
        std::string toString(const std::string& format) const;
        //@}

        /** 
         * @brief Returns a Date object that represents the date parsed from the string <i>dateString</i> according to the format <i>format</i>.
         * 
         * For further information about the <i>format</i> parameter, see toString(). 
         */
        static Date fromString(const std::string& dateString, const std::string& format);

        /** @brief Returns a Date object corresponding to the Julian day <i>julianDay</i>. */
        static Date fromJulianDay(long julianDay);

        /** 
         * @brief Returns the number of the days between the dates <i>from</i> and <i>to</i>, excluding the last day. 
         *
         * For example:
         * {@code
         *    int num = Date::daysBetween(Date(1999, 1, 1), Date(1999, 1, 3));  // num = 2
         * } 
         */
        static int daysBetween(const Date& from, const Date& to);

        /** 
         * @brief Returns whether the year <i>year</i> is a leap year.
         * 
         * According to the ISO proleptic calendar system rules, a year is a leap year if it is divisible by four without remainder. However, years divisible by 100, are not leap years, with the exception of years divisible by 400 which are.
         * For example, 1904 is a leap year it is divisible by 4. 1900 was not a leap year as it is divisible by 100, however 2000 was a leap year as it is divisible by 400.
         */
        static bool isLeapYear(int year);

        /** @brief Returns the number of days in the month <i>month</i> of year <i>year</i>. It ranges between 28 and 31. */
        static int daysInMonthOfYear(int year, int month);

    private:
        Days mDateDuration;
    };

    /** 
     * @relates Date
     * @name Serialization/Deserialization Functions 
     */
    //@{

    /** @brief Writes date <i>d</i> to stream <i>os</i> in ISO-8601 date format "yyyy-MM-dd". */
    std::ostream& operator<<(std::ostream& os, const Date& d);

    /** @brief Reads a date in ISO-8601 date format "yyyy-MM-dd" from stream <i>is</i> and stores it in date <i>d</i>. */
    std::istream& operator>>(std::istream& is, Date& d);
    //@}

}
#endif // SALSABIL_DATE_HPP 

