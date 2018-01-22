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

#ifndef SALSABIL_LOCALDATETIME_HPP
#define SALSABIL_LOCALDATETIME_HPP

#include "DateTime.hpp"
#include "TimeZone.hpp"

namespace Salsabil {

    /** 
     * @class LocalDateTime
     * @brief LocalDateTime is an immutable class representing a local datetime (a datetime with a time zone) in the ISO-8601 calendar system, such as "2017-12-31, 22:34:55+03:00[Europe/Istanbul]".
     * 
     * The ISO-8601 calendar system is the modern civil calendar system used today in most of the world. It is equivalent to the proleptic Gregorian calendar system. 
     * This class stores all date and time fields, to a precision of nanoseconds, and a time zone. It's a composition of a DateTime object and a TimeZone object. 
     * 
     * Default-constructed LocalDateTime objects are invalid(calling isValid() on them returns false). LocalDateTime objects can be created by giving a DateTime object and a TimeZone object. 
     * 
     * Also, a LocalDateTime object can be created from a formatted string through fromString(), they can only be parsed if the IANA time zone ID exists in the passed string. Otherwise, the time zone is ambiguous and thus an invalid time zone object is set in the returned object.
     * The method current() returns the current local datetime obtained from the system clock.
     * 
     * The datetime fields can be accessed though the methods year(), month(), day(), hour(), minute(), second(), millisecond(), microsecond() and nanosecond().
     * Other fields, such as day-of-year, day-of-week and week-of-year, can also be accessed through dayOfYear(), dayOfWeek() and weekOfYear(), respectively.
     * 
     * LocalDateTime provides methods for adding or subtracting a time duration. Years, months, days, hours, minutes, seconds, milliseconds, microseconds, and nanoseconds can be added to a local datetime (through addYears(), addMonths(), addDays(), addHours(), addMinutes(), addSeconds(), addMilliseconds(), addMicroseconds() and addNanoseconds(), respectively) 
     * or subtracted from it (through subtractYears(), subtractMonths(), subtractDays(), subtractHours(), subtractMinutes(), subtractSeconds(), subtractMilliseconds(), subtractMicroseconds() and subtractNanoseconds(), respectively).
     * 
     * To know the offset from UTC for a LocalDateTime object, offsetFromUtc() returns the offset in seconds.

     * It also has operators for comparison. For example, LocalDateTime A is considered earlier than LocalDateTime B if A is smaller than B in the same time zone. Hence, two objects to be compared are firstly converted to UTC and then compared in that time zone.
     * 
     * The methods weeksBetween(), daysBetween(), hoursBetween(), minutesBetween(), secondsBetween(), millisecondsBetween(), and microsecondsBetween() returns how many weeks, days, hours, minutes, seconds, milliseconds and microseconds respectively, between two LocalDateTime objects.
     * 
     * The toString() method can be used to get a textual representation of a LocalDateTime object formatted according to a given formatter string.
     */
    class LocalDateTime {
        using Duration = std::chrono::nanoseconds;

    public:
        /// @name Durations
        //@{
        /// Nanosecond duration.
        using Nanoseconds = DateTime::Nanoseconds;
        /// Microsecond duration.
        using Microseconds = DateTime::Microseconds;
        /// Millisecond duration.
        using Milliseconds = DateTime::Milliseconds;
        /// Second duration.
        using Seconds = DateTime::Seconds;
        /// Minute duration.
        using Minutes = DateTime::Minutes;
        /// Hour duration.
        using Hours = DateTime::Hours;
        /// Day duration.
        using Days = DateTime::Days;
        /// Week duration.
        using Weeks = DateTime::Weeks;
        //@}

        /// @name Enumerations
        //@{
        /// Weekday enumeration.
        using Weekday = DateTime::Weekday;
        /// Month enumeration.
        using Month = DateTime::Month;
        //@}

        /// @name Constructors and Destructors
        //@{

        /// Default constructor. Constructs an invalid LocalDateTime object with every field set to zero (calling isValid() on it returns false).
        LocalDateTime();

        /// Copy-constructs a LocalDateTime object from ***other***.
        LocalDateTime(const LocalDateTime& other);

        /// Move-constructs a LocalDateTime object from ***other***.
        LocalDateTime(LocalDateTime&& other);

        /// Constructs a LocalDateTime object from ***dateTime*** in the time zone ***timeZone***.
        LocalDateTime(const DateTime& dateTime, const TimeZone& timeZone);

        /// Default destructor.

        virtual ~LocalDateTime() {
        }
        //@}

        /// @name Assignment Operators
        //@{
        /// Copy assignment operator.
        LocalDateTime& operator=(const LocalDateTime& other);

        /// Move assignment operator.
        LocalDateTime& operator=(LocalDateTime&& other);
        //@}


        /// @name Comparison Operators
        //@{
        /** 
         * @brief Returns whether this local datetime is earlier than ***other***. 
         * 
         * The comparison is done in the Coordinated Universal %Time (UTC) zone, so the objects to be compared are first converted to UTC and then compared. For example
         * {@code
         *     DateTime dt1(Date(2018, 1, 13), Time(9, 6, 21));
         *     DateTime dt2(Date(2018, 1, 13), Time(12, 6, 21));
         *    
         *     bool isEarlier1 = LocalDateTime(dt1, TimeZone("Etc/GMT+2")) < LocalDateTime(dt2, TimeZone("Etc/GMT+2")); // isEarlier1 is true.
         *     bool isEarlier2 = LocalDateTime(dt1, TimeZone("Etc/GMT-6")) < LocalDateTime(dt1, TimeZone("Etc/GMT-3")); // isEarlier2 is true.
         *     bool isEarlier3 = LocalDateTime(dt1, TimeZone("Etc/GMT-6")) < LocalDateTime(dt2, TimeZone("Etc/GMT-3")); // isEarlier3 is true.
         * }
         */
        bool operator<(const LocalDateTime& other) const;

        /// Returns whether this local datetime is earlier than ***other*** or equal to it. See operator<() for more details about how the comparison is done. 
        bool operator<=(const LocalDateTime& other) const;

        /// Returns whether this local datetime is later than ***other***. See operator<() for more details about how the comparison is done.
        bool operator>(const LocalDateTime& other) const;

        /// Returns whether this local datetime is later than ***other*** or equal to it. See operator<() for more details about how the comparison is done.
        bool operator>=(const LocalDateTime& other) const;

        /// Returns whether this local datetime is equal to ***other***. See operator<() for more details about how the comparison is done.
        bool operator==(const LocalDateTime& other) const;

        /// Returns whether this local datetime is different from ***other***. See operator<() for more details about how the comparison is done.
        bool operator!=(const LocalDateTime& other) const;
        //@}

        /// @name Addition/Subtraction Operators
        //@{
        /// Returns the result of subtracting ***other*** from this object as #Nanoseconds duration.
        Nanoseconds operator-(const LocalDateTime& other) const;

        /// Returns the result of subtracting ***duration*** from this object as a new LocalDateTime object.
        LocalDateTime operator-(const Duration& duration) const;

        /// Returns the result of adding ***duration*** to this object as a new LocalDateTime object.
        LocalDateTime operator+(const Duration& duration) const;
        //@}

        /// @name Querying Methods
        //@{
        /// Returns whether this LocalDateTime object represents a valid local datetime. A LocalDateTime object is valid if both the datetime and time zone parts are valid. For more information, see DateTime#isValid() and TimeZone#isValid().
        bool isValid() const;

        /** 
         * @brief Returns the datetime of this object. 
         * 
         * The returned datetime is the one that was passed to the constructor. For example:
         * {@code
         *     LocalDateTime ldt(DateTime(Date(1998, 3, 1), Time(23, 4, 19)), TimeZone("Etc/GMT+3"));
         *     DateTime dt = ldt.datetime(); // returns DateTime(Date(1998, 3, 1), Time(23, 4, 19)).
         * }
         */
        DateTime dateTime() const;

        /// Returns the time zone of this object.
        TimeZone timeZone() const;

        /// Returns the date part of this object.
        Date date() const;

        /// Returns the time part of this object.
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

        /** 
         * @brief Returns the datetime offset from UTC as a #Seconds duration. 
         * 
         * {@code
         *     LocalDateTime ldt(DateTime(Date(1998, 3, 1), Time(23, 4, 19)), TimeZone("Etc/GMT+3"));
         *     LocalDateTime::Seconds offset = ldt.offsetFromUtc(); // LocalDateTime::Seconds(-10800)
         * }
         */
        Seconds offsetFromUtc() const;

        /// Returns the weekday as a number between 1 and 7, which corresponds to the enumeration #Weekday.
        int dayOfWeek() const;

        /// Returns the day of the year as a number between 1 and 365 (1 to 366 on leap years).
        int dayOfYear() const;

        /// Returns the number of days in the current month. It ranges between 28 and 31.
        int daysInMonth() const;

        /// Returns the number of days in the current year. It is either 365 or 366.
        int daysInYear() const;

        /// Returns whether the year of this LocaDateTime is a leap year. For more information, see Date#isLeapYear(int).
        bool isLeapYear() const;

        /// Returns the week number of the year of this datetime. For more information, see Date#weekOfYear().
        int weekOfYear(int* weekYear = nullptr) const;

        /// Returns the name of the weekday of this datetime. For more information, see Date#dayOfWeekName().
        std::string dayOfWeekName(bool useShortName = false) const;

        /// Returns the name of the month of this datetime. For more information, see Date#monthName().
        std::string monthName(bool useShortName = false) const;
        //@}

        /// @name Addition/Subtraction Methods
        //@{
        /// Returns a new LocalDateTime object representing this local datetime with ***nanoseconds*** added to it. 
        LocalDateTime addNanoseconds(int nanoseconds) const;

        /// Returns a new LocalDateTime object representing this datetime with ***nanoseconds*** subtracted from it.
        LocalDateTime subtractNanoseconds(int nanoseconds) const;

        /// Returns a new LocalDateTime object representing this datetime with ***microseconds*** added to it.
        LocalDateTime addMicroseconds(int microseconds) const;

        /// Returns a new LocalDateTime object representing this local datetime with ***microseconds*** subtracted from it.
        LocalDateTime subtractMicroseconds(int microseconds) const;

        /// Returns a new LocalDateTime object representing this local datetime with ***milliseconds*** added to it.
        LocalDateTime addMilliseconds(int milliseconds) const;

        /// Returns a new LocalDateTime object representing this local datetime with ***milliseconds*** subtracted from it. 
        LocalDateTime subtractMilliseconds(int milliseconds) const;

        /// Returns a new LocalDateTime object representing this local datetime with ***seconds*** added to it.
        LocalDateTime addSeconds(int seconds) const;

        /// Returns a new LocalDateTime object representing this local datetime with ***seconds*** subtracted from it.
        LocalDateTime subtractSeconds(int seconds) const;

        /// Returns a new LocalDateTime object representing this local datetime with ***minutes*** added to it.
        LocalDateTime addMinutes(int minutes) const;

        /// Returns a new LocalDateTime object representing this local datetime with ***minutes*** subtracted from it.
        LocalDateTime subtractMinutes(int minutes) const;

        /// Returns a new LocalDateTime object representing this local datetime with ***hours*** added to it.
        LocalDateTime addHours(int hours) const;

        /// Returns a new LocalDateTime object representing this local datetime with ***hours*** subtracted from it.
        LocalDateTime subtractHours(int hours) const;

        /// Returns a new LocalDateTime object representing this local datetime with ***days*** added to it.
        LocalDateTime addDays(int days) const;

        /// Returns a new LocalDateTime object representing this local datetime with ***days*** subtracted from it.
        LocalDateTime subtractDays(int days) const;

        /// Returns a new LocalDateTime object representing this local datetime with ***months*** added to it. See Date#addMonths() for more details about how the operation is done.     
        LocalDateTime addMonths(int months) const;

        /// Returns a new LocalDateTime object representing this local datetime with ***months*** subtracted from it. See Date#subtractMonths() for more details about how the operation is done.     
        LocalDateTime subtractMonths(int months) const;

        /// Returns a new LocalDateTime object representing this local datetime with ***years*** added to it.
        LocalDateTime addYears(int years) const;

        /// Returns a new LocalDateTime object representing this local datetime with ***years*** subtracted from it.
        LocalDateTime subtractYears(int years) const;

        /// Returns a new LocalDateTime object representing this local datetime with ***duration*** added to it.
        LocalDateTime addDuration(const Duration& duration) const;

        /// Returns a new LocalDateTime object representing this local datetime with ***duration*** subtracted from it.
        LocalDateTime subtractDuration(const Duration& duration) const;

        //@}

        /// @name Conversion Methods
        //@{
        /** 
         * @brief Returns this LocalDateTime in the universal time zone (Etc/UTC). 
         * 
         * For example:
         * {@code
         *     LocalDateTime ldt(DateTime(Date(1998, 3, 1), Time(23, 4, 19)), TimeZone("Etc/GMT-3"));
         *     LocalDateTime inUTC = ldt.toUTC(); // returns LocalDateTime(DateTime(Date(1998, 3, 1), Time(20, 4, 19)), TimeZone("Etc/UTC")).
         * }
         */
        LocalDateTime toUtc() const;

        /** 
         * @brief Returns this LocalDateTime in the time zone ***timeZone***. 
         * 
         * For example:
         * {@code
         *     LocalDateTime ldt(DateTime(Date(2018, 1, 14), Time(20, 30, 11)), TimeZone("Etc/GMT-3"));
         *     LocalDateTime other = ldt.toTimeZone("Etc/GMT+3"); // returns LocalDateTime(DateTime(Date(2018, 1, 14), Time(14, 30, 11)), TimeZone("Etc/GMT+3")).
         * }
         */
        LocalDateTime toTimeZone(const TimeZone &timeZone) const;

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

        /// Returns a **std::chrono::system_clock::time_point** representation of this local datetime.
        std::chrono::system_clock::time_point toStdTimePoint() const;

        /// Returns a **std::tm** representation of this local datetime.
        std::tm toBrokenStdTime() const;

        /// Returns a **std::time_t** representation of this local datetime.
        std::time_t toScalarStdTime() const;

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
         *  z           |  time-zone offset without a colon, such as +0100                        
         *  zz          |  time-zone offset with a colon, such as +01:00                     
         *  zzz         |  time-zone abbreviation, such as "CEST" or "+03"                      
         *  zzzz        |  time-zone ID, such as "Europe/Istanbul"         
         * 
         * Any character in the formatter string not listed above will be inserted as is into the output string. 
         * If this object is invalid, an empty string will be returned.
         * 
         * @see dayOfWeekName(), monthName()
         */
        std::string toString(const std::string& format) const;
        //@}

        /// Returns the current system local datetime.
        static LocalDateTime current();

        /** 
         * @brief Returns a LocalDateTime object that represents the datetime parsed from the string ***datetime*** formatted according to ***format***.
         * 
         * The formatter patterns are the same patterns used in the method toString(). However, the time zone patterns "z", "zz" and "zzz" are not supported because they are mostly ambiguous (cannot be resolved to a certain IANA time zone). So this method can only parse a complete IANA time zone ID in a date-time string, such as "2018-01-21 15:25:06 Europe/Istanbul". 
         * For example:
         * {@code
         *     LocalDateTime ldt = LocalDateTime::fromString("21/1/2018, 14:18:34.762[Europe/Istanbul]", "d/M/yyyy, hh:mm:ss.fff[zzzz]"); 
         *     if (ldt == LocalDateTime(DateTime(Date(2018, 1, 21), Time(14, 18, 34, 762)), TimeZone("Europe/Istanbul"))) { // returns true.
         *         std::cout << ldt << std::endl; 
         *     }
         * }
         * @see toString()
         */
        static LocalDateTime fromString(const std::string& datetime, const std::string& format);

        /// @name Calculation Methods
        //@{
        /// Returns the number of the nanoseconds between ***from*** and ***to***.
        static long long nanosecondsBetween(const LocalDateTime& from, const LocalDateTime& to);

        /// Returns the number of the microseconds between ***from*** and ***to***.
        static long long microsecondsBetween(const LocalDateTime& from, const LocalDateTime& to);

        /// Returns the number of the milliseconds between ***from*** and ***to***.
        static long long millisecondsBetween(const LocalDateTime& from, const LocalDateTime& to);

        /// Returns the number of the seconds between ***from*** and ***to***.
        static long long secondsBetween(const LocalDateTime& from, const LocalDateTime& to);

        /// Returns the number of the minutes between ***from*** and ***to***.
        static long minutesBetween(const LocalDateTime& from, const LocalDateTime& to);

        /// Returns the number of the hours between ***from*** and ***to***.
        static long hoursBetween(const LocalDateTime& from, const LocalDateTime& to);

        /// Returns the number of the days between ***from*** and ***to***.
        static long daysBetween(const LocalDateTime& from, const LocalDateTime& to);

        /// Returns the number of the weeks between ***from*** and ***to***.
        static long weeksBetween(const LocalDateTime& from, const LocalDateTime& to);
        //@}

    private:
        DateTime mDateTime;
        TimeZone mTimeZone;
    };

    /** 
     * @relates LocalDateTime
     * @name Input/Output Operators 
     */
    //@{
    /// Writes ***ldt*** to stream ***os*** in the format "yyyy-MM-ddThh:mm:ss[zzzz]". See toString() for information about the formatter patterns.
    std::ostream& operator<<(std::ostream& os, const LocalDateTime& ldt);

    /// Reads a local datetime in the format "yyyy-MM-ddThh:mm:ss[zzzz]" from stream ***is*** and stores it in ***ldt***. See toString() for information about the formatter patterns.
    std::istream& operator>>(std::istream& is, LocalDateTime& ldt);
    //@}
}

#endif // SALSABIL_LOCALDATETIME_HPP

