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

#ifndef SALSABIL_TIME_HPP
#define SALSABIL_TIME_HPP

#include <chrono>

#include "internal/StringHelper.hpp"

namespace Salsabil {

    /** 
     * @class Time
     * @brief Time is an immutable time class representing a time without a time zone in the ISO-8601 calendar system, such as "09:55:02". 
     * 
     * Time is represented to nanosecond precision. For example, the value "09:55:02.123456789" can be stored in a Time object.
     * Time uses the 24-hour clock format. It describes the time as the number of nanoseconds since midnight.
     * Time provides functions for comparing times, manipulating a time by adding or subtracting intervals and converting the whole time into minutes, seconds or any finer duration.
     * 
     * A Time object can be created by giving the number of hours, minutes, seconds, milliseconds, etc. explicitly. Also, it can be created from std::time_t, std::tm or std::chrono::system_clock::time_point object.
     * 
     * A default-constructed time object is set to the current time in system's clock. Note that the accuracy depends on the accuracy of the underlying operating system; not all systems provide 1-millisecond accuracy.
     * The hour(), minute(), second(), millisecond(), microsecond() and nanosecond() functions provide access to the number of hours, minutes, seconds, milliseconds, microseconds and nanoseconds of the time, respectively. 
     * 
     * Time supports C++11 chrono library's way of manipulating time, i.e. time constructors and functions can takes duration parameters such as std::chrono::hours(1), std::chrono::minutes(1), etc. 
     * For convenience, Time defines aliases to these intervals which may be used instead of chrono's intervals. Time intervals are Hours, Minutes, Seconds, Milliseconds, Microseconds and Nanoseconds, which correspond to std:chrono::hours, std:chrono::minutes, std:chrono::seconds, std:chrono::milliseconds, std:chrono::microseconds and std:chrono::nanoseconds, respectively.
     * 
     * The functions toHours(), toMinutes(), toSeconds(), toMilliseconds(), toMicroseconds() and toNanoseconds() return the whole time passed since midnight as a number of hours, minutes, seconds, milliseconds, microseconds or nanoseconds, respectively.
     * 
     * The toString() function can be used to get a textual representation of the time formatted according to a given formatter string.
     * 
     * Time provides the comparison operators to compare two Time objects. Time A is considered earlier than Time B if A is smaller than B, and so on.
     * Time also provides functions to compute the difference between two times in hours, minutes, etc.
     * 
     */
    class Time {
        using Duration = std::chrono::nanoseconds;
        using Days = std::chrono::duration
                <int, std::ratio_multiply<std::ratio<24>, std::chrono::hours::period>>;

    public:
        /** @name Durations */
        //@{
        /** @brief Nanosecond duration. */
        using Nanoseconds = std::chrono::nanoseconds;
        /** @brief Microsecond duration. */
        using Microseconds = std::chrono::microseconds;
        /** @brief Millisecond duration. */
        using Milliseconds = std::chrono::milliseconds;
        /** @brief Second duration. */
        using Seconds = std::chrono::seconds;
        /** @brief Minute duration. */
        using Minutes = std::chrono::minutes;
        /** @brief Hour duration. */
        using Hours = std::chrono::hours;
        //@}

        /** @name Constructors and Destructors */
        //@{
        /** @brief Constructs a Time object from the given <i>hours</i>, <i>minutes</i> and <i>seconds</i>. */
        explicit Time(int hours, int minutes, int seconds);

        /** @brief Constructs a Time object from the given <i>hours</i>, <i>minutes</i>, <i>seconds</i> and <i>milliseconds</i>. */
        explicit Time(int hours, int minutes, int seconds, int milliseconds);

        /**
         * @brief Constructs a Time object from the given <i>hours</i>, <i>minutes</i>, <i>seconds</i> and <i>subseconds</i>.
         *  
         * The <i>subseconds</i> parameter can be any fine duration of Time such as Time::Microseconds(54), or any fine duration of chrono such as std::chrono::nanoseconds(435223543).
         */
        explicit Time(int hours, int minutes, int seconds, const Duration& subseconds);

        /** 
         * @brief Constructs a Time object from the given <i>hours</i>, <i>minutes</i>, <i>seconds</i> and <i>subseconds</i>. 
         * 
         * All parameters can be Time::Duration or std::chrono::duration. For example:
         * {@code
         *    Time myTime(Time::Hours(2), Time::Minutes(55), Time::Seconds(10), Time::Nanoseconds(435223543));
         *    Time sameTime(std::chrono::hours(2), std::chrono::::minutes(55), std::chrono::::seconds(10), std::chrono::::nanoseconds(435223543));
         * }
         */
        explicit Time(Hours hours, Minutes minutes, Seconds seconds, const Duration& subseconds);

        /** 
         * @brief Constructs a Time object from <i>duration</i> elapsed since midnight. 
         * 
         * @param duration can be Time::Duration or std::chrono::duration. For example:
         * {@code
         *    Time myTime(Time::Hours(2) + Time::Minutes(55));
         * }
         */
        explicit Time(const Duration& duration);

        /** @brief Constructs a Time object from the standard library's chrono time point <i>timePoint</i>. */
        explicit Time(const std::chrono::system_clock::time_point& timePoint);

        /** @brief Constructs a Time object from a standard library <b>std::tm</b> object <i>brokenStdTime</i>. */
        explicit Time(const std::tm& brokenStdTime);

        /** @brief Constructs a Time object from a standard library <b>std::time_t</b> object <i>scalarStdTime</i>. */
        explicit Time(std::time_t scalarStdTime);

        /** @brief Copy-constructs a Time object from <i>other</i>. */
        Time(const Time& other) = default;

        /** @brief Move-constructs a Time object from <i>other</i>. */
        Time(Time&& other) = default;

        /** @brief Default constructor. Constructs a Time object that is set to midnight. */
        Time();

        /** @brief Default destructor. */
        virtual ~Time() {
        }
        //@}

        /** 
         * @brief Returns a Time object set to the current time obtained from the system clock. 
         * 
         * <b>Note</b> that the returned time is not the current local time, rather it is the current system time; the current time in Coordinated Universal Time (UTC). 
         */
        static Time currentTime();

        /** 
         * @brief Returns whether this time object represents a valid time. 
         * 
         * A valid time is a fraction of a day; the time to be valid must be between 00:00:00 and 23:59:59. Thus, negative times or times containing 24 hours or more are considered invalid.
         * {@code
         *    Time t; // set to midnight
         *    t.isValid(); // return true
         *    t = Time(Time::Hours(-1));
         *    t.isValid(); // return false
         *    t = Time(Time::Hours(24));
         *    t.isValid(); // return false
         *    t = Time(22, 1, 55);
         *    t.isValid(); // return true
         * }
         */
        bool isValid() const;

        /** @name Querying Functions */
        //@{
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
        //@}

        /**  
         * @name Addition/Subtraction Functions and Operators
         * @brief These functions return a new Time object with some time duration added/subtracted.
         */
        //@{

        /** @brief Returns the result of adding <i>duration</i> to this time as a new Time object. */
        Time addDuration(const Duration& duration) const;

        /** @brief Returns the result of subtracting <i>duration</i> from this time as a new Time object. */
        Time subtractDuration(const Duration& duration) const;

        /** @brief Returns the result of adding <i>hours</i> to this time as a new Time object. */
        Time addHours(int hours) const;

        /** @brief Returns the result of subtracting <i>hours</i> from this time as a new Time object. */
        Time subtractHours(int hours) const;

        /** @brief Returns the result of adding <i>minutes</i> to this time as a new Time object. */
        Time addMinutes(int minutes) const;

        /** @brief Returns the result of subtracting <i>minutes</i> from this time as a new Time object. */
        Time subtractMinutes(int minutes) const;

        /** @brief Returns the result of adding <i>seconds</i> to this time as a new Time object. */
        Time addSeconds(int seconds) const;

        /** @brief Returns the result of subtracting <i>seconds</i> from this time as a new Time object. */
        Time subtractSeconds(int seconds) const;

        /** @brief Returns the result of adding <i>milliseconds</i> to this time as a new Time object. */
        Time addMilliseconds(int milliseconds) const;

        /** @brief Returns the result of subtracting <i>milliseconds</i> from this time as a new Time object. */
        Time subtractMilliseconds(int milliseconds) const;

        /** @brief Returns the result of adding <i>microseconds</i> to this time as a new Time object. */
        Time addMicroseconds(int microseconds) const;

        /** @brief Returns the result of subtracting <i>microseconds</i> from this time as a new Time object. */
        Time subtractMicroseconds(int microseconds) const;

        /** @brief Returns the result of adding <i>nanoseconds</i> to this time as a new Time object. */
        Time addNanoseconds(int nanoseconds) const;

        /** @brief Returns the result of subtracting <i>nanoseconds</i> from this time as a new Time object. */
        Time subtractNanoseconds(int nanoseconds) const;

        /** @brief Returns the result of subtracting <i>other</i> from this time as a Time::Nanoseconds duration. */
        Nanoseconds operator-(const Time& other) const;

        /** @brief Returns the result of subtracting <i>duration</i> from this time as a new Time object. */
        Time operator-(const Duration& duration) const;

        /** @brief Returns the result of adding <i>duration</i> to this time as a new Time object. */
        Time operator+(const Duration& duration) const;
        //@}

        /**  
         * @name Conversion Functions
         * @brief These functions convert the time container or representation to another container or representation. For example, a Time object can be converted to std::tm object or could be converted to a number of minutes such as 3200.
         */
        //@{

        /** @brief Returns the elapsed hours since midnight. If this time is invalid, the returned may exceed 23. See isValid(). */
        int toHours() const;

        /** @brief Returns the elapsed minutes since midnight. */
        int toMinutes() const;

        /** @brief Returns the elapsed seconds since midnight. */
        long toSeconds() const;

        /** @brief Returns the elapsed milliseconds since midnight. */
        long toMilliseconds() const;

        /** @brief Returns the elapsed microseconds since midnight. */
        long long toMicroseconds() const;

        /** @brief Returns the elapsed nanoseconds since midnight. */
        long long toNanoseconds() const;

        /** @brief Returns a std::chrono::nanoseconds duration that represents this time. */
        Nanoseconds toStdDuration() const;

        /** @brief Returns a std::tm representation of this time. */
        std::tm toBrokenStdTime() const;

        /** @brief Returns a std::time_t representation of this time. */
        std::time_t toScalarStdTime() const;

        /** 
         * @brief Returns the time as a string formatted according to the formatter string <i>format</i>. 
         * 
         * The formatting string may contain the following patterns:
         * <table>
         * <tr><th>Pattern<th>Meaning
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
         */
        std::string toString(const std::string& format) const;
        //@}

        /** 
         * @brief Returns a Time object from the string <i>time</i> according to the formatter <i>format</i>.
         * 
         * For further information about the <i>format</i> parameter, see toString(). 
         */
        static Time fromString(const std::string& time, const std::string& format);

        /**  
         * @name Comparison Operators
         * @brief These functions compare this time to another time.
         */
        //@{

        /** @brief Returns whether this time is earlier than <i>other</i>. */
        bool operator<(const Time& other) const;

        /** @brief Returns whether this time is earlier than <i>other</i> or equal to it. */
        bool operator<=(const Time& other) const;

        /** @brief Returns whether this time is later than <i>other</i>. */
        bool operator>(const Time& other) const;

        /** @brief Returns whether this time is later than <i>other</i> or equal to it. */
        bool operator>=(const Time& other) const;

        /** @brief Returns whether this time is equal to <i>other</i>. */
        bool operator==(const Time& other) const;

        /** @brief Returns whether this time is different from <i>other</i>. */
        bool operator!=(const Time& other) const;
        //@}

        /**  
         * @name Assignment Operators
         */
        //@{
        /** @brief Copy assignment operator. */
        Time& operator=(const Time& other) = default;

        /** @brief Move assignment operator. */
        Time& operator=(Time&& other) = default;
        //@}

        /**  
         * @name Calculating Difference Between Times
         * @brief These functions return the duration between two times <i>from</i> and <i>to</i>. If <i>from</i> is earlier than (smaller than) <i>to</i>, then the difference is negative.
         */
        //@{

        /** @brief Returns the number of hours between <i>from</i> and <i>to</i>. */
        static int hoursBetween(const Time& from, const Time& to);

        /** @brief Returns the number of minutes between <i>from</i> and <i>to</i>. */
        static int minutesBetween(const Time& from, const Time& to);

        /** @brief Returns the number of seconds between <i>from</i> and <i>to</i>. */
        static int secondsBetween(const Time& from, const Time& to);

        /** @brief Returns the number of milliseconds between <i>from</i> and <i>to</i>. */
        static int millisecondsBetween(const Time& from, const Time& to);

        /** @brief Returns the number of microseconds between <i>from</i> and <i>to</i>. */
        static int microsecondsBetween(const Time& from, const Time& to);

        /** @brief Returns the number of nanoseconds between <i>from</i> and <i>to</i>. */
        static int nanosecondsBetween(const Time& from, const Time& to);
        //@}

    private:
        Duration mTimeDuration;
    };

    /** 
     * @relates Time
     * @name Serialization/Deserialization Functions 
     */
    //@{

    /** @brief Writes time <i>t</i> to stream <i>os</i> in ISO-8601 time format "hh:mm:ss". */
    std::ostream& operator<<(std::ostream& os, const Time& t);

    /** @brief Reads a time in ISO-8601 time format "hh:mm:ss" from stream <i>is</i> and stores it in time <i>t</i>. */
    std::istream& operator>>(std::istream& is, Time& t);
    //@}
}

#endif // SALSABIL_TIME_HPP