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
     * the number of days since the epoch "1970-01-01 00:00:00.000 UTC"
     * the number of days since the epoch "1970-01-01 00:00:00.000 UTC" plus the number of nanoseconds since midnight. 
     * 
     * Default-constructed DateTime objects are invalid(calling isValid() on them returns false) and are set to "0000-00-00 00:00:00". 
     * DateTime objects can be created by giving a Date object and a Time object. It can also be created by giving only a Date object, in this case the time part is considered to be at midnight.
     * Also, a DateTime object can be created from a formatted string through fromString() or from a Julian day through fromJulianDay().
     * The function currentDateTime() returns the current datetime obtained from the system clock.
     * 
     * The datetime fields can be accessed though the functions year(), month(), day(), hour(), minute(), second(), millisecond(), microsecond() and nanosecond().
     * Other fields, such as day-of-year, day-of-week and week-of-year, can also be accessed through dayOfYear(), dayOfWeek() and weekOfYear(), respectively.
     * 
     * DateTime provides functions for manipulating datetimes. Years, months, days, hours, minutes, seconds, milliseconds, microseconds, and nanoseconds can be added to a datetime (through addYears(), addMonths(), addDays(), addHours(), addMinutes(), addSeconds(), addMilliseconds(), addMicroseconds() and addNanoseconds(), respectively) 
     * or subtracted from it (through subtractYears(), subtractMonths(), subtractDays(), subtractHours(), subtractMinutes(), subtractSeconds(), subtractMilliseconds(), subtractMicroseconds() and subtractNanoseconds(), respectively).
     * It also provides functions for comparison. DateTime A is considered earlier than DateTime B if A is smaller than B, and so on.
     * the functions weeksBetween(), daysBetween(), hoursBetween(), minutesBetween(), secondsBetween(), millisecondsBetween(), and microsecondsBetween() returns how many weeks, days, hours, minutes, seconds, milliseconds and microseconds respectively, between two DateTime objects.
     * 
     * The toString() function can be used to get a textual representation of the datetime formatted according to a given formatter string.
     */

    class DateTime {
        using Duration = std::chrono::nanoseconds;

    public:
        /** @name Durations */
        //@{
        /** @brief Nanosecond duration. */
        using Nanoseconds = Time::Nanoseconds;
        /** @brief Microsecond duration. */
        using Microseconds = Time::Microseconds;
        /** @brief Millisecond duration. */
        using Milliseconds = Time::Milliseconds;
        /** @brief Second duration. */
        using Seconds = Time::Seconds;
        /** @brief Minute duration. */
        using Minutes = Time::Minutes;
        /** @brief Hour duration. */
        using Hours = Time::Hours;
        /** @brief Day duration. */
        using Days = Date::Days;
        //@}

        /** @name Enumerations */
        //@{
        /** @brief Weekday enumeration. */
        using Weekday = Date::Weekday;
        /** @brief Month enumeration. */
        using Month = Date::Month;
        //@}

        /** @name Constructors and Destructors */
        //@{

        /** @brief Default constructor. Constructs an invalid DateTime object i.e. an object in which every field is set to zero. */
        DateTime();

        /** @brief Copy-constructs a DateTime object from <i>datetime</i>. */
        DateTime(const DateTime& datetime) = default;

        /** @brief Move-constructs a DateTime object from <i>datetime</i>. */
        DateTime(DateTime&& datetime) = default;

        /** @brief Constructs a DateTime object from <i>date</i> and <i>time</i>. */
        DateTime(const Date& date, const Time& time);

        /** @brief Constructs a DateTime object from <i>date</i>, leaving the time part at midnight (i.e. 00:00:00). */
        DateTime(const Date& date);

        /** @brief Default destructor. */
        virtual ~DateTime() {
        }
        //@}

        /** 
         * @brief Returns a DateTime object set to the current datetime obtained from the system clock.
         * 
         * <b>Note</b> that the returned datetime is not the current local datetime, rather it is the current system datetime; the current datetime in Coordinated Universal %Time (UTC). 
         */
        static DateTime currentDateTime();

        /** 
         * @brief Returns whether this datetime object represents a valid datetime. 
         * 
         * See Date#isValid() and Time#isValid().
         */
        bool isValid() const;

        /** @name Querying Functions */
        //@{

        /** @brief Returns the date part of this datetime. */
        Date date() const;

        /** @brief Returns the time part of this datetime. */
        Time time() const;

        /** @brief Set the year, month and day in the parameters <i>year</i>, <i>month</i> and <i>day</i>, respectively. */
        void getYearMonthDay(int* year, int* month, int* day) const;

        /** @brief Returns the year as a number. There is no year 0. Negative numbers indicate years before 1 CE, that is, year -1 is year 1 BCE, year -2 is year 2 BCE, and so on. */
        int year() const;

        /** @brief Returns the month as a number between 1 and 12, which corresponds to the enumeration Date#Month. */
        int month() const;

        /** @brief Returns the day of the month as a number between 1 and 31. */
        int day() const;

        /** @brief Returns the hour of day (0, 23). */
        int hour() const;

        /** @brief Returns the minute of hour (0, 59). */
        int minute() const;

        /** @brief Returns the second of minute (0, 59). */
        int second() const;

        /** @brief Returns the millisecond of second (0, 999). */
        int millisecond() const;

        /** @brief Returns the microsecond of second (0, 999999). */
        long microsecond() const;

        /** @brief Returns the nanosecond of second (0, 999999999). */
        long nanosecond() const;

        /** @brief Returns the weekday as a number between 1 and 7, which corresponds to the enumeration Date#Weekday. */
        int dayOfWeek() const;

        /** @brief Returns the day of the year as a number between 1 and 365 ( 1 to 366 on leap years). */
        int dayOfYear() const;

        /** @brief Returns the number of days in the current month. It ranges between 28 and 31. */
        int daysInMonth() const;

        /** @brief Returns the number of days in the current year. It is either 365 or 366. */
        int daysInYear() const;

        /** 
         * @brief Returns whether the year of this datetime is a leap year.
         * 
         * For more information see isLeapYear(int).
         */
        bool isLeapYear() const;

        /** 
         * @brief Returns the week number of the year of this datetime, which ranges between 1 and 53, and stores the year in weekYear unless it is not specified (defaulted to nullptr).
         * 
         * According to ISO-8601, weeks start on Monday and the first Thursday of a year is always in week 1 of that year. Most years have 52 weeks, but some have 53.
         * <i>yearWeek</i> is not always the same as year(). For example, the date of "1 January 2000" falls in the week 52 of the year 1999, the date of "31 December 2002" falls in the week 1 of the year 2003, and the date of "1 January 2010" falls in the week 53 of the year 2009.
         */
        int weekOfYear(int* weekYear = nullptr) const;

        /** 
         * @brief Returns the name of the weekday of this datetime. 
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
         * @brief Returns the name of the month of this datetime. 
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

        /** @name Manipulation Functions */
        //@{

        /** @brief Returns a new DateTime object representing this datetime with <i>days</i> added to it. */
        DateTime addDays(int days) const;

        /** @brief Returns a new DateTime object representing this datetime with <i>days</i> subtracted from it. */
        DateTime subtractDays(int days) const;

        /** 
         * @brief Returns a new DateTime object representing this datetime with <i>months</i> added to it. 
         * 
         * See also Date#addMonths().         
         */
        DateTime addMonths(int months) const;

        /** 
         * @brief Returns a new DateTime object representing this datetime with <i>months</i> subtracted from it. 
         *
         * See also Date#subtractMonths().         
         */
        DateTime subtractMonths(int months) const;

        /** @brief Returns a new DateTime object representing this datetime with <i>years</i> added to it. */
        DateTime addYears(int years) const;

        /** @brief Returns a new DateTime object representing this datetime with <i>years</i> subtracted from it. */
        DateTime subtractYears(int years) const;

        /** @brief Returns a new DateTime object representing this datetime with <i>duration</i> added to it. */
        DateTime addDuration(const Duration& duration) const;

        /** @brief Returns a new DateTime object representing this datetime with <i>duration</i> subtracted from it. */
        DateTime subtractDuration(const Duration& duration) const;

        /** @brief Returns a new DateTime object representing this datetime with <i>hours</i> added to it. */
        DateTime addHours(int hours) const;

        /** @brief Returns a new DateTime object representing this datetime with <i>hours</i> subtracted from it. */
        DateTime subtractHours(int hours) const;

        /** @brief Returns a new DateTime object representing this datetime with <i>minutes</i> added to it. */
        DateTime addMinutes(int minutes) const;

        /** @brief Returns a new DateTime object representing this datetime with <i>minutes</i> subtracted from it. */
        DateTime subtractMinutes(int minutes) const;

        /** @brief Returns a new DateTime object representing this datetime with <i>seconds</i> added to it. */
        DateTime addSeconds(int seconds) const;

        /** @brief Returns a new DateTime object representing this datetime with <i>seconds</i> subtracted from it. */
        DateTime subtractSeconds(int seconds) const;

        /** @brief Returns a new DateTime object representing this datetime with <i>milliseconds</i> added to it. */
        DateTime addMilliseconds(int milliseconds) const;

        /** @brief Returns a new DateTime object representing this datetime with <i>milliseconds</i> subtracted from it. */
        DateTime subtractMilliseconds(int milliseconds) const;

        /** @brief Returns a new DateTime object representing this datetime with <i>microseconds</i> added to it. */
        DateTime addMicroseconds(int microseconds) const;

        /** @brief Returns a new DateTime object representing this datetime with <i>microseconds</i> subtracted from it. */
        DateTime subtractMicroseconds(int microseconds) const;

        /** @brief Returns a new DateTime object representing this datetime with <i>nanoseconds</i> added to it. */
        DateTime addNanoseconds(int nanoseconds) const;

        /** @brief Returns a new DateTime object representing this datetime with <i>nanoseconds</i> subtracted from it. */
        DateTime subtractNanoseconds(int nanoseconds) const;

        /** @brief Returns the result of subtracting <i>datetime</i> from this datetime as #DateTime::Nanoseconds duration. */
        Nanoseconds operator-(const DateTime& datetime) const;

        /** @brief Returns the result of subtracting <i>duration</i> from this datetime as a new DateTime object. */
        DateTime operator-(const Duration& duration) const;

        /** @brief Returns the result of adding <i>duration</i> to this datetime as a new DateTime object. */
        DateTime operator+(const Duration& duration) const;
        //@}

        /** @name Comparison Operators */
        //@{

        /** @brief Returns whether this datetime is earlier than <i>other</i>. */
        bool operator<(const DateTime& other) const;

        /** @brief Returns whether this datetime is earlier than <i>other</i> or equal to it. */
        bool operator<=(const DateTime& other) const;

        /** @brief Returns whether this datetime is later than <i>other</i>. */
        bool operator>(const DateTime& other) const;

        /** @brief Returns whether this datetime is later than <i>other</i> or equal to it. */
        bool operator>=(const DateTime& other) const;

        /** @brief Returns whether this datetime is equal to <i>other</i>. */
        bool operator==(const DateTime& other) const;

        /** @brief Returns whether this datetime is different from <i>other</i>. */
        bool operator!=(const DateTime& other) const;
        //@}

        /** @name Assignment Operators */
        //@{
        /** @brief Copy assignment operator. */
        DateTime& operator=(const DateTime& date) = default;

        /** @brief Move assignment operator. */
        DateTime& operator=(DateTime&& date) = default;
        //@}

        /** @name Conversion Functions */
        //@{

        /** @brief Returns the number of elapsed days since "1970-01-01 00:00:00.000 UTC", not counting leap seconds. */
        long toDaysSinceEpoch() const;

        /** @brief Returns the number of elapsed hours since "1970-01-01 00:00:00.000 UTC, not counting leap seconds. */
        long toHoursSinceEpoch() const;

        /** @brief Returns the number of elapsed minutes since "1970-01-01 00:00:00.000 UTC, not counting leap seconds. */
        long toMinutesSinceEpoch() const;

        /** @brief Returns the number of elapsed seconds since "1970-01-01 00:00:00.000 UTC, not counting leap seconds. */
        long long toSecondsSinceEpoch() const;

        /** @brief Returns the number of elapsed milliseconds since "1970-01-01 00:00:00.000 UTC", not counting leap seconds. */
        long long toMillisecondsSinceEpoch() const;

        /** @brief Returns the number of elapsed microseconds since "1970-01-01 00:00:00.000 UTC", not counting leap seconds. */
        long long toMicrosecondsSinceEpoch() const;

        /** @brief Returns the number of elapsed nanoseconds since "1970-01-01 00:00:00.000 UTC", not counting leap seconds. */
        long long toNanosecondsSinceEpoch() const;

        /** @brief Returns a std::chrono::microseconds duration since "1970-01-01 00:00:00.000 UTC", not counting leap seconds. */
        Microseconds toStdDurationSinceEpoch() const;

        /** @brief Returns a std::chrono::system_clock::time_point representation of this datetime. */
        std::chrono::system_clock::time_point toStdTimePoint() const;

        /** @brief Returns a std::tm representation of this datetime. */
        std::tm toBrokenStdTime() const;

        /** @brief Returns a std::time_t representation of this datetime. */
        std::time_t toScalarStdTime() const;

        /** 
         * @brief Returns the corresponding Julian Day Number (JDN) of this datetime as a double where the integral part represents the day count and the fractional part represents the time since midday Universal %Time (UT).
         * 
         * The JDN is the consecutive numbering of days and fractions since noon Universal %Time (UT) on 1 January  4713 BCE in the proleptic Julian calendar, which occurs on 24 November 4714 BCE in the proleptic Gregorian Calender.
         * That is, the JDN 0 corresponds to "12:00:00 UTC, 24 November 4714 BCE", the JDN 1.5 represents "00:00:00 UTC, 26 November 4714 BCE", and the JDN of today at "00:09:35 UTC, 31 December 2017 CE" is 2458118.506655093. 
         * 
         * <b>Note</b> that The date to be converted is considered Gregorian. Also, the current Gregorian rules are extended backwards and forwards. There is no year 0. The first year before the common era (i.e. year 1 BCE) is year -1, year -2 is year 2 BCE, and so on.
         */
        double toJulianDay() const;

        /** 
         * @brief Returns the datetime as a string formatted according to the formatter string <i>format</i>. 
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
         * <tr><td>h<td>one-digit hour (0, 23)\n
         * <tr><td>hh<td>two-digit hour (00, 23)\n
         * <tr><td>H<td>one-digit hour (1, 12)\n
         * <tr><td>HH<td>two-digit hour (01, 12)\n
         * <tr><td>m<td>one-digit minute (0, 59)\n
         * <tr><td>mm<td>two-digit minute (00, 59)\n
         * <tr><td>s<td>one-digit second (0, 59)\n
         * <tr><td>ss<td>two-digit second (00, 59)\n
         * <tr><td>f<td>one-digit subsecond (0, 9)\n
         * <tr><td>ff<td>two-digit subsecond (00, 99)\n
         * <tr><td>fff<td>three-digit subsecond (000, 999)\n
         * <tr><td>ffff<td>four-digit subsecond (0000, 9999)\n
         * <tr><td>fffff<td>five-digit subsecond (00000, 99999)\n
         * <tr><td>ffffff<td>six-digit subsecond (000000, 999999)\n
         * <tr><td>fffffff<td>seven-digit subsecond (0000000, 9999999)\n
         * <tr><td>ffffffff<td>eight-digit subsecond (00000000, 99999999)\n
         * <tr><td>fffffffff<td>nine-digit subsecond (000000000, 999999999)\n
         * <tr><td>a<td>before/after noon indicator(i.e. am or pm)\n
         * <tr><td>A<td>before/after noon indicator(i.e. AM or PM)\n
         * </table>
         * Any character in the formatter string not listed above will be inserted as is into the output string. 
         * If the datetime is invalid, an empty string will be returned.
         * 
         * See also dayOfWeekName() and monthName()
         */
        std::string toString(const std::string& format) const;
        //@}

        /** 
         * @brief Returns a DateTime object that represents the datetime parsed from the string <i>datetime</i> formatted according to <i>format</i>.
         * 
         * For further information about the <i>format</i> parameter, see toString(). 
         */
        static DateTime fromString(const std::string& datetime, const std::string& format);

        /** @brief Returns a DateTime object corresponding to the Julian day <i>julianDay</i>. See also toJulianDay(). */
        static DateTime fromJulianDay(double julianDay);

        /** @brief Returns the number of the days between <i>from</i> and <i>to</i>. */
        static long long daysBetween(const DateTime& from, const DateTime& to);

        /** @brief Returns the number of the hours between <i>from</i> and <i>to</i>. */
        static long long hoursBetween(const DateTime& from, const DateTime& to);

        /** @brief Returns the number of the minutes between <i>from</i> and <i>to</i>. */
        static long long minutesBetween(const DateTime& from, const DateTime& to);

        /** @brief Returns the number of the seconds between <i>from</i> and <i>to</i>. */
        static long long secondsBetween(const DateTime& from, const DateTime& to);

        /** @brief Returns the number of the milliseconds between <i>from</i> and <i>to</i>. */
        static long long millisecondsBetween(const DateTime& from, const DateTime& to);

        /** @brief Returns the number of the microseconds between <i>from</i> and <i>to</i>. */
        static long long microsecondsBetween(const DateTime& from, const DateTime& to);

        /** 
         * @brief Returns whether <i>year</i> is a leap year.
         * 
         * See also Date#isLeapYear().
         */
        static bool isLeapYear(int year);

        /** @brief Returns the number of days in <i>month</i> of <i>year</i>. It ranges between 28 and 31. */
        static int daysInMonthOfYear(int year, int month);

    private:
        Date mDate;
        Time mTime;
    };

    /** 
     * @relates DateTime
     * @name Serialization/Deserialization Functions 
     */
    //@{

    /** @brief Writes <i>dt</i> to stream <i>os</i> in ISO-8601 date and time format "yyyy-MM-ddThh:mm:ss.fff". */
    std::ostream& operator<<(std::ostream& os, const DateTime& dt);

    /** @brief Reads a datetime in ISO-8601 date and time format "yyyy-MM-ddThh:mm:ss.fff" from stream <i>is</i> and stores it in <i>dt</i>. */
    std::istream& operator>>(std::istream& is, DateTime& dt);
    //@}
}

#endif // SALSABIL_DATETIME_HPP

