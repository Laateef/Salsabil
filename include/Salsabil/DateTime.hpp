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

#ifndef SALSABIL_DATETIME_HPP
#define SALSABIL_DATETIME_HPP

#include "Date.hpp"
#include "Time.hpp"

namespace Salsabil {

    /**
     * @class DateTime
     * @brief DateTime is an immutable class representing a datetime without a time-zone in the ISO-8601 calendar system, such as "2017-12-31, 22:34:55 UTC".
     * 
     * The ISO-8601 calendar system is the modern civil calendar system used today in most of the world. It is equivalent to the proleptic Gregorian calendar system. 
     * DateTime describes the datetime as a date part (year, month, day) and a Time part (hour, minute, second, subsecond). 
     * 
     * Internally, DateTime describes a datetime as a combination of the number of days since the epoch "1970-01-01 00:00:00.000 UTC" plus the number of nanoseconds since midnight. 
     * 
     * Default-constructed DateTime objects are invalid(calling isValid() on them returns false) and are set to "0000-00-00 00:00:00". 
     * DateTime objects can be created by giving a Date object and a Time object. It can also be created by giving only a Date object, in this case, the time part is considered to be at midnight.
     * Also, a DateTime object can be created from a formatted string through fromString() or from a Julian day through fromJulianDay().
     * The method current() returns the current datetime obtained from the system clock.
     * 
     * The datetime fields can be accessed though the methods year(), month(), day(), hour(), minute(), second(), millisecond(), microsecond() and nanosecond().
     * Other fields, such as day-of-year, day-of-week and week-of-year, can also be accessed through dayOfYear(), dayOfWeek() and weekOfYear(), respectively.
     * 
     * DateTime provides methods for manipulating datetimes. Years, months, days, hours, minutes, seconds, milliseconds, microseconds, and nanoseconds can be added to a datetime (through addYears(), addMonths(), addDays(), addHours(), addMinutes(), addSeconds(), addMilliseconds(), addMicroseconds() and addNanoseconds(), respectively) 
     * or subtracted from it (through subtractYears(), subtractMonths(), subtractDays(), subtractHours(), subtractMinutes(), subtractSeconds(), subtractMilliseconds(), subtractMicroseconds() and subtractNanoseconds(), respectively).
     * 
     * It also has operators for comparison. DateTime A is considered earlier than DateTime B if A is smaller than B, and so on.
     * 
     * The methods weeksBetween(), daysBetween(), hoursBetween(), minutesBetween(), secondsBetween(), millisecondsBetween(), and microsecondsBetween() returns how many weeks, days, hours, minutes, seconds, milliseconds and microseconds respectively, between two DateTime objects.
     * 
     * The toString() method can be used to get a textual representation of the datetime formatted according to a given formatter string.
     */
    class DateTime {
        using Duration = std::chrono::nanoseconds;

    public:
        /// @name Durations
        //@{
        /// Nanosecond duration.
        using Nanoseconds = Time::Nanoseconds;
        /// Microsecond duration.
        using Microseconds = Time::Microseconds;
        /// Millisecond duration.
        using Milliseconds = Time::Milliseconds;
        /// Second duration.
        using Seconds = Time::Seconds;
        /// Minute duration.
        using Minutes = Time::Minutes;
        /// Hour duration.
        using Hours = Time::Hours;
        /// Day duration.
        using Days = Date::Days;
        /// Week duration.
        using Weeks = Date::Weeks;
        //@}

        /// @name Enumerations
        //@{
        /// Weekday enumeration.
        using Weekday = Date::Weekday;
        /// Month enumeration.
        using Month = Date::Month;
        //@}

        /// @name Constructors and Destructors
        //@{

        /// Default constructor. Constructs an invalid DateTime object with every field set to zero (calling isValid() on it returns false).
        DateTime();

        /// Copy-constructs a DateTime object from ***other***.
        DateTime(const DateTime& other);

        /// Move-constructs a DateTime object from ***other***.
        DateTime(DateTime&& other);

        /** 
         * @brief Constructs a DateTime object from ***duration*** since the epoch "1970-01-01 00:00:00 UTC". 
         * 
         * The constructed datetime has what ever precision it is given, down to nanoseconds. 
         */
        explicit DateTime(const Duration& duration);

        /// Constructs a DateTime object from the standard library's chrono time point ***timePoint***.
        explicit DateTime(const std::chrono::system_clock::time_point& timePoint);

        /// Constructs a DateTime object from ***date***, leaving the time part at midnight "00:00:00".
        explicit DateTime(const Date& date);

        /// Constructs a DateTime object from ***date*** and ***time***.
        explicit DateTime(const Date& date, const Time& time);

        /// Default destructor.

        virtual ~DateTime() {
        }
        //@}

        /// @name Assignment Operators
        //@{
        /// Copy assignment operator.
        DateTime& operator=(const DateTime& other);

        /// Move assignment operator.
        DateTime& operator=(DateTime&& other);
        //@}

        /// @name Comparison Operators
        //@{
        /// Returns whether this datetime is earlier than ***other***.
        bool operator<(const DateTime& other) const;

        /// Returns whether this datetime is earlier than ***other*** or equal to it.
        bool operator<=(const DateTime& other) const;

        /// Returns whether this datetime is later than ***other***.
        bool operator>(const DateTime& other) const;

        /// Returns whether this datetime is later than ***other*** or equal to it.
        bool operator>=(const DateTime& other) const;

        /// Returns whether this datetime is equal to ***other***.
        bool operator==(const DateTime& other) const;

        /// Returns whether this datetime is different from ***other***.
        bool operator!=(const DateTime& other) const;
        //@}

        /// Addition/Subtraction Operators
        //@{
        /// Returns the result of subtracting ***other*** from this datetime as #Nanoseconds duration.
        Nanoseconds operator-(const DateTime& other) const;

        /// Returns the result of subtracting ***duration*** from this datetime as a new DateTime object.
        DateTime operator-(const Duration& duration) const;

        /// Returns the result of adding ***duration*** to this datetime as a new DateTime object.
        DateTime operator+(const Duration& duration) const;
        //@}

        /// @name Querying Methods
        //@{
        /// Returns whether this datetime object represents a valid datetime. A DateTime object is valid if both the date and time parts are valid. For more information, see Date#isValid() and Time#isValid().
        bool isValid() const;

        /// Returns the date part of this datetime.
        Date date() const;

        /// Returns the time part of this datetime.
        Time time() const;

        /// Returns the nanosecond of second (0, 999999999).
        long nanosecond() const;

        /// Returns the microsecond of second (0, 999999).
        long microsecond() const;

        /// Returns the millisecond of second (0, 999).
        int millisecond() const;

        /// Returns the second of minute (0, 59).
        int second() const;

        /// Returns the minute of hour (0, 59).
        int minute() const;

        /// Returns the hour of day (0, 23).
        int hour() const;

        /// Returns the day of month (1, 31).
        int day() const;

        /// Returns the month of year (1, 12), which corresponds to the enumeration #Month.
        int month() const;

        /// Returns the year as a number. There is no year 0. Negative numbers indicate years before 1 CE, that is, year -1 is year 1 BCE, year -2 is year 2 BCE, and so on.
        int year() const;

        /// Set the year, month and day in the parameters ***year***, ***month*** and ***day***, respectively.
        void getYearMonthDay(int* year, int* month, int* day) const;

        /// Returns the weekday as a number between 1 and 7, which corresponds to the enumeration #Weekday.
        int dayOfWeek() const;

        /// Returns the day of the year as a number between 1 and 365 (1 to 366 on leap years).
        int dayOfYear() const;

        /// Returns the number of days in the current month. It ranges between 28 and 31.
        int daysInMonth() const;

        /// Returns the number of days in the current year. It is either 365 or 366.
        int daysInYear() const;

        /// Returns whether the year of this datetime is a leap year. For more information, see Date#isLeapYear(int).
        bool isLeapYear() const;

        /** 
         * @brief Returns the week number of the year of this datetime, which ranges between 1 and 53, and stores the year in ***weekYear*** unless it is not specified (defaulted to nullptr).
         * 
         * According to ISO-8601, weeks start on Monday and the first Thursday of a year is always in week 1 of that year. Most years have 52 weeks, but some have 53.
         * ***yearWeek*** is not always the same as year(). For example, the date of "1 January 2000" falls in the week 52 of the year 1999, the date of "31 December 2002" falls in the week 1 of the year 2003, and the date of "1 January 2010" falls in the week 53 of the year 2009.
         */
        int weekOfYear(int* weekYear = nullptr) const;

        /** 
         * @brief Returns the name of the weekday of this datetime. 
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
         * 
         */
        std::string dayOfWeekName(bool useShortName = false) const;

        /** 
         * @brief Returns the name of the month of this datetime. 
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
         * 
         */
        std::string monthName(bool useShortName = false) const;
        //@}

        /// @name Addition/Subtraction Methods
        //@{
        /// Returns a new DateTime object representing this datetime with ***nanoseconds*** added to it.
        DateTime addNanoseconds(int nanoseconds) const;

        /// Returns a new DateTime object representing this datetime with ***nanoseconds*** subtracted from it.
        DateTime subtractNanoseconds(int nanoseconds) const;

        /// Returns a new DateTime object representing this datetime with ***microseconds*** added to it.
        DateTime addMicroseconds(int microseconds) const;

        /// Returns a new DateTime object representing this datetime with ***microseconds*** subtracted from it.
        DateTime subtractMicroseconds(int microseconds) const;

        /// Returns a new DateTime object representing this datetime with ***milliseconds*** added to it.
        DateTime addMilliseconds(int milliseconds) const;

        /// Returns a new DateTime object representing this datetime with ***milliseconds*** subtracted from it.
        DateTime subtractMilliseconds(int milliseconds) const;

        /// Returns a new DateTime object representing this datetime with ***seconds*** added to it.
        DateTime addSeconds(int seconds) const;

        /// Returns a new DateTime object representing this datetime with ***seconds*** subtracted from it.
        DateTime subtractSeconds(int seconds) const;

        /// Returns a new DateTime object representing this datetime with ***minutes*** added to it.
        DateTime addMinutes(int minutes) const;

        /// Returns a new DateTime object representing this datetime with ***minutes*** subtracted from it.
        DateTime subtractMinutes(int minutes) const;

        /// Returns a new DateTime object representing this datetime with ***hours*** added to it. 
        DateTime addHours(int hours) const;

        /// Returns a new DateTime object representing this datetime with ***hours*** subtracted from it.
        DateTime subtractHours(int hours) const;

        /// Returns a new DateTime object representing this datetime with ***days*** added to it.
        DateTime addDays(int days) const;

        /// Returns a new DateTime object representing this datetime with ***days*** subtracted from it.
        DateTime subtractDays(int days) const;

        /// Returns a new DateTime object representing this datetime with ***months*** added to it. See Date#addMonths() for more information about how the operation is done.       
        DateTime addMonths(int months) const;

        /// Returns a new DateTime object representing this datetime with ***months*** subtracted from it. See Date#subtractMonths() for more information about how the operation is done.         
        DateTime subtractMonths(int months) const;

        /// Returns a new DateTime object representing this datetime with ***years*** added to it.
        DateTime addYears(int years) const;

        /// Returns a new DateTime object representing this datetime with ***years*** subtracted from it.
        DateTime subtractYears(int years) const;

        /// Returns a new DateTime object representing this datetime with ***duration*** added to it.
        DateTime addDuration(const Duration& duration) const;

        /// Returns a new DateTime object representing this datetime with ***duration*** subtracted from it.
        DateTime subtractDuration(const Duration& duration) const;
        //@}

        /// @name Conversion Methods 
        //@{
        /// Returns the number of elapsed nanoseconds since "1970-01-01 00:00:00.000 UTC", not counting leap seconds.
        long long toNanosecondsSinceEpoch() const;

        /// Returns the number of elapsed microseconds since "1970-01-01 00:00:00.000 UTC", not counting leap seconds.
        long long toMicrosecondsSinceEpoch() const;

        /// Returns the number of elapsed milliseconds since "1970-01-01 00:00:00.000 UTC", not counting leap seconds.
        long long toMillisecondsSinceEpoch() const;

        /// Returns the number of elapsed seconds since "1970-01-01 00:00:00.000 UTC, not counting leap seconds.
        long long toSecondsSinceEpoch() const;

        /// Returns the number of elapsed minutes since "1970-01-01 00:00:00.000 UTC, not counting leap seconds.
        long toMinutesSinceEpoch() const;

        /// Returns the number of elapsed hours since "1970-01-01 00:00:00.000 UTC, not counting leap seconds.
        long toHoursSinceEpoch() const;

        /// Returns the number of elapsed days since "1970-01-01 00:00:00.000 UTC", not counting leap seconds.
        long toDaysSinceEpoch() const;

        /// Returns a **std::chrono::microseconds** duration since "1970-01-01 00:00:00.000 UTC", not counting leap seconds.
        Microseconds toStdDurationSinceEpoch() const;

        /// Returns a **std::chrono::system_clock::time_point** representation of this datetime.
        std::chrono::system_clock::time_point toStdTimePoint() const;

        /// Returns a **std::tm** representation of this datetime.
        std::tm toBrokenStdTime() const;

        /// Returns a **std::time_t** representation of this datetime.
        std::time_t toScalarStdTime() const;

        /** 
         * @brief Returns the corresponding Julian Day Number (JDN) of this datetime as a double where the integral part represents the day count and the fractional part represents the time since midday Universal %Time (UT).
         * 
         * The JDN is the consecutive numbering of days and fractions since noon Universal %Time (UT) on 1 January  4713 BCE in the proleptic Julian calendar, which occurs on 24 November 4714 BCE in the proleptic Gregorian Calender.
         * That is, the JDN 0 corresponds to "12:00:00 UTC, 24 November 4714 BCE", the JDN 1.5 represents "00:00:00 UTC, 26 November 4714 BCE", and the JDN of today at "00:09:35 UTC, 31 December 2017 CE" is 2458118.506655093. 
         * 
         * **Note** that The date to be converted is considered Gregorian. Also, the current Gregorian rules are extended backwards and forwards. There is no year 0. The first year before the common era (i.e. year 1 BCE) is year -1, year -2 is year 2 BCE, and so on.
         */
        double toJulianDay() const;

        /** 
         * @brief Returns the datetime as a string formatted according to the formatter string ***format***. 
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
         *  h           | one-digit hour (0, 23)                        
         *  hh          | two-digit hour (00, 23)                       
         *  H           | one-digit hour (1, 12)                        
         *  HH          | two-digit hour (01, 12)                       
         *  m           | one-digit minute (0, 59)                      
         *  mm          | two-digit minute (00, 59)                     
         *  s           | one-digit second (0, 59)                      
         *  ss          | two-digit second (00, 59)                     
         *  f           | one-digit subsecond (0, 9)                    
         *  ff          | two-digit subsecond (00, 99)                  
         *  fff         | three-digit subsecond (000, 999)                
         *  ffff        | four-digit subsecond (0000, 9999)             
         *  fffff       | five-digit subsecond (00000, 99999)           
         *  ffffff      | six-digit subsecond (000000, 999999)           
         *  fffffff     | seven-digit subsecond (0000000, 9999999)      
         *  ffffffff    | eight-digit subsecond (00000000, 99999999)    
         *  fffffffff   | nine-digit subsecond (000000000, 999999999)  
         *  a           | before/after noon indicator(i.e. am or pm)    
         *  A           | before/after noon indicator(i.e. AM or PM)    
         * 
         * Any character in the formatter string not listed above will be inserted as is into the output string. 
         * If the datetime is invalid, an empty string will be returned.
         * 
         * @see dayOfWeekName(), monthName()
         */
        std::string toString(const std::string& format) const;
        //@}

        /** 
         * @brief Returns a DateTime object set to the current datetime obtained from the system clock.
         * 
         * **Note** that the returned datetime is not the current local datetime, rather it is the current system datetime; the current datetime in Coordinated Universal %Time (UTC). 
         */
        static DateTime current();

        /// Returns a DateTime object set to the epoch "1970-1-1T00:00:00".
        static DateTime epoch();

        /** 
         * @brief Returns a DateTime object from the string ***datetime*** formatted according to the formatter string ***format***.
         * 
         * The formatter patterns are the same patterns used in the method toString(). @see toString() 
         */
        static DateTime fromString(const std::string& datetime, const std::string& format);

        /// Returns a DateTime object corresponding to the Julian day ***julianDay***. See toJulianDay() for information about Julian Days.
        static DateTime fromJulianDay(double julianDay);

        /**  
         * @name Calculation Methods
         * @brief The following methods return the time difference between two datetimes; ***from*** and ***to***. If ***to*** is earlier than (smaller than) ***from***, then the difference is negative.
         */
        //@{
        /// Returns the number of nanoseconds between ***from*** and ***to***.
        static long long nanosecondsBetween(const DateTime& from, const DateTime& to);

        /// Returns the number of microseconds between ***from*** and ***to***.
        static long long microsecondsBetween(const DateTime& from, const DateTime& to);

        /// Returns the number of milliseconds between ***from*** and ***to***.
        static long long millisecondsBetween(const DateTime& from, const DateTime& to);

        /// Returns the number of seconds between ***from*** and ***to***.
        static long long secondsBetween(const DateTime& from, const DateTime& to);

        /// Returns the number of minutes between ***from*** and ***to***.
        static long minutesBetween(const DateTime& from, const DateTime& to);

        /// Returns the number of hours between ***from*** and ***to***.
        static long hoursBetween(const DateTime& from, const DateTime& to);

        /// Returns the number of days between ***from*** and ***to***.
        static long daysBetween(const DateTime& from, const DateTime& to);

        /// Returns the number of weeks between ***from*** and ***to***.
        static long weeksBetween(const DateTime& from, const DateTime& to);
        //@}

    private:
        Date mDate;
        Time mTime;
    };

    /** 
     * @relates DateTime
     * @name Input/Output Operators  
     */
    //@{
    /// Writes ***dt*** to stream ***os*** in ISO-8601 date and time format "yyyy-MM-ddThh:mm:ss.fff". See toString() for information about the formatter patterns.
    std::ostream& operator<<(std::ostream& os, const DateTime& dt);

    /// Reads a datetime in ISO-8601 date and time format "yyyy-MM-ddThh:mm:ss.fff" from stream ***is*** and stores it in ***dt***. See toString() for information about the formatter patterns.
    std::istream& operator>>(std::istream& is, DateTime& dt);
    //@}
}

#endif // SALSABIL_DATETIME_HPP

