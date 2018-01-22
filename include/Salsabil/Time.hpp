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
     * Time uses the 24-hour clock format. Internally, it describes the time as the number of nanoseconds since midnight.
     * Time provides methods for comparing times, manipulating a time by adding or subtracting durations and converting the whole time into minutes, seconds or any finer duration.
     * 
     * A Time object can be created by giving the number of hours, minutes, seconds, milliseconds, etc. explicitly. Also, it can be created from **std::time_t**, **std::tm** or **std::chrono::system_clock::time_point** object.
     * 
     * A default-constructed time object is invalid (calling isValid() on it returns false). The static method current() can be used to get the current time in system's clock. Note that the accuracy depends on the accuracy of the underlying operating system; not all systems provide 1-millisecond accuracy.
     * 
     * The methods hour(), minute(), second(), millisecond(), microsecond() and nanosecond() provide access to the number of hours, minutes, seconds, milliseconds, microseconds and nanoseconds of the time, respectively. 
     * 
     * Time supports C++11 chrono library's way of manipulating time, i.e. time constructors and methods can takes duration parameters such as **std::chrono::hours(1)**, **std::chrono::minutes(1)**, etc. 
     * For convenience, Time defines aliases to these durations which may be used instead of chrono's ones. Time durations are #Hours, #Minutes, #Seconds, #Milliseconds, #Microseconds and #Nanoseconds, which correspond to **std:chrono::hours**, **std:chrono::minutes**, **std:chrono::seconds**, **std:chrono::milliseconds**, **std:chrono::microseconds** and **std:chrono::nanoseconds**, respectively.
     * 
     * The methods toHours(), toMinutes(), toSeconds(), toMilliseconds(), toMicroseconds() and toNanoseconds() return the whole time passed since midnight as a number of hours, minutes, seconds, milliseconds, microseconds or nanoseconds, respectively.
     * 
     * The toString() method can be used to get a textual representation of the time formatted according to a given formatter string.
     * 
     * Time provides the comparison operators to compare two Time objects. Time A is considered earlier than Time B if A is smaller than B, and so on.
     * Time also provides methods to compute the difference between two times in hours, minutes, etc.
     * 
     */
    class Time {
        using Duration = std::chrono::nanoseconds;
        using Days = std::chrono::duration
                <int, std::ratio_multiply<std::ratio<24>, std::chrono::hours::period>>;

    public:
        /// @name Durations
        //@{
        /// Nanosecond duration.
        using Nanoseconds = std::chrono::nanoseconds;
        /// Microsecond duration.
        using Microseconds = std::chrono::microseconds;
        /// Millisecond duration.
        using Milliseconds = std::chrono::milliseconds;
        /// Second duration.
        using Seconds = std::chrono::seconds;
        /// Minute duration.
        using Minutes = std::chrono::minutes;
        /// Hour duration.
        using Hours = std::chrono::hours;
        //@}

        /// @name Constructors and Destructors
        //@{
        /// Default constructor. Constructs an invalid Time object with every field is set to zero (calling isValid() on it, returns false). @see isValid()
        Time();

        /// Copy-constructs a Time object from ***other***.
        Time(const Time& other);

        /// Move-constructs a Time object from ***other***.
        Time(Time&& other);

        /// Constructs a Time object from the standard library **std::time_t** object ***scalarStdTime***.
        explicit Time(std::time_t scalarStdTime);

        /// Constructs a Time object from the standard library **std::tm** object ***brokenStdTime***.
        explicit Time(const std::tm& brokenStdTime);

        /** 
         * @brief Constructs a Time object from time ***duration*** elapsed since midnight. 
         * 
         * @param duration can be **Time::Duration** or **std::chrono::duration**. For example:
         * {@code
         *    Time myTime(Time::Hours(2) + Time::Minutes(55));
         * }
         */
        explicit Time(const Duration& duration);

        /// Constructs a Time object from the standard library **std::chrono::system_clock::time_point** object ***timePoint***.
        explicit Time(const std::chrono::system_clock::time_point& timePoint);

        /// Constructs a Time object from the given ***hours***, ***minutes*** and ***seconds***.
        explicit Time(int hours, int minutes, int seconds);

        /// Constructs a Time object from the given ***hours***, ***minutes***, ***seconds*** and ***milliseconds***.
        explicit Time(int hours, int minutes, int seconds, int milliseconds);

        /**
         * @brief Constructs a Time object from the given ***hours***, ***minutes***, ***seconds*** and ***subseconds***.
         *  
         * The ***subseconds*** parameter can be any fine duration of Time such as **Time::Microseconds(54)**, or any fine duration of chrono such as **std::chrono::nanoseconds(435223543)**.
         */
        explicit Time(int hours, int minutes, int seconds, const Duration& subseconds);

        /** 
         * @brief Constructs a Time object from the given ***hours***, ***minutes***, ***seconds*** and ***subseconds***. 
         * 
         * All parameters can be **Time::Duration** or **std::chrono::duration**. For example:
         * {@code
         *    Time myTime(Time::Hours(2), Time::Minutes(55), Time::Seconds(10), Time::Nanoseconds(435223543));
         *    Time sameTime(std::chrono::hours(2), std::chrono::::minutes(55), std::chrono::::seconds(10), std::chrono::::nanoseconds(435223543));
         * }
         */
        explicit Time(Hours hours, Minutes minutes, Seconds seconds, const Duration& subseconds);

        /// Default destructor.

        virtual ~Time() {
        }
        //@}

        /// @name Assignment Operators
        //@{
        /// Copy assignment operator.
        Time& operator=(const Time& other);

        /// Move assignment operator.
        Time& operator=(Time&& other);
        //@}

        /**  
         * @name Comparison Operators
         * @brief The following operators compare this time to ***other***. For example, if this time is smaller than ***other***, it means that it is earlier than ***other***.
         */
        //@{

        /// Returns whether this time is earlier than ***other***.
        bool operator<(const Time& other) const;

        /// Returns whether this time is earlier than ***other*** or equal to it.
        bool operator<=(const Time& other) const;

        /// Returns whether this time is later than ***other***.
        bool operator>(const Time& other) const;

        /// Returns whether this time is later than ***other*** or equal to it.
        bool operator>=(const Time& other) const;

        /// Returns whether this time is equal to ***other***.
        bool operator==(const Time& other) const;

        /// Returns whether this time is different from ***other***.
        bool operator!=(const Time& other) const;
        //@}

        /// @name Addition/Subtraction Operators
        //@{
        /// Returns the result of adding ***duration*** to this time as a new Time object.
        Time operator+(const Duration& duration) const;

        /// Returns the result of subtracting ***duration*** from this time as a new Time object.
        Time operator-(const Duration& duration) const;

        /// Returns the result of subtracting ***other*** from this time as a **Time::Nanoseconds** duration.
        Nanoseconds operator-(const Time& other) const;
        //@}

        /// @name Querying Methods
        //@{
        /** 
         * @brief Returns whether this time object represents a valid time. 
         * 
         * A valid time is a fraction of a day; the time to be valid must be between 00:00:00 and 23:59:59. Thus, negative times or times containing 24 hours or more are considered invalid.
         * {@code
         *    Time t;
         *    bool state1 = t.isValid(); // returns false.
         *    bool state2 = Time(Time::Hours(-1)).isValid(); // returns false.
         *    bool state3 = Time(Time::Hours(24)).isValid(); // returns false.
         *    t = Time(22, 1, 55);
         *    bool state4 = t.isValid(); // returns true.
         * }
         */
        bool isValid() const;

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
        //@}

        /// @name Addition/Subtraction Methods
        //@{
        /// Returns the result of adding ***nanoseconds*** to this time as a new Time object.
        Time addNanoseconds(int nanoseconds) const;

        /// Returns the result of subtracting ***nanoseconds*** from this time as a new Time object.
        Time subtractNanoseconds(int nanoseconds) const;

        /// Returns the result of adding ***microseconds*** to this time as a new Time object.
        Time addMicroseconds(int microseconds) const;

        /// Returns the result of subtracting ***microseconds*** from this time as a new Time object.
        Time subtractMicroseconds(int microseconds) const;

        /// Returns the result of adding ***milliseconds*** to this time as a new Time object.
        Time addMilliseconds(int milliseconds) const;

        /// Returns the result of subtracting ***milliseconds*** from this time as a new Time object.
        Time subtractMilliseconds(int milliseconds) const;

        /// Returns the result of adding ***seconds*** to this time as a new Time object.
        Time addSeconds(int seconds) const;

        /// Returns the result of subtracting ***seconds*** from this time as a new Time object.
        Time subtractSeconds(int seconds) const;

        /// Returns the result of adding ***minutes*** to this time as a new Time object.
        Time addMinutes(int minutes) const;

        /// Returns the result of subtracting ***minutes*** from this time as a new Time object.
        Time subtractMinutes(int minutes) const;

        /// Returns the result of adding ***hours*** to this time as a new Time object.
        Time addHours(int hours) const;

        /// Returns the result of subtracting ***hours*** from this time as a new Time object.
        Time subtractHours(int hours) const;

        /// Returns the result of adding ***duration*** to this time as a new Time object.
        Time addDuration(const Duration& duration) const;

        /// Returns the result of subtracting ***duration*** from this time as a new Time object.
        Time subtractDuration(const Duration& duration) const;
        //@}

        /// @name Conversion Methods
        //@{
        /// Returns the elapsed nanoseconds since midnight.
        long long toNanosecondsSinceMidnight() const;

        /// Returns the elapsed microseconds since midnight.
        long long toMicrosecondsSinceMidnight() const;

        /// Returns the elapsed milliseconds since midnight.
        long toMillisecondsSinceMidnight() const;

        /// Returns the elapsed seconds since midnight.
        long toSecondsSinceMidnight() const;

        /// Returns the elapsed minutes since midnight.
        int toMinutesSinceMidnight() const;

        /// Returns the elapsed hours since midnight. If this time is invalid, the returned value may exceed 23. @see isValid().
        int toHoursSinceMidnight() const;

        /// Returns the elapsed time since midnight as a #Nanoseconds duration.
        Nanoseconds toStdDurationSinceMidnight() const;

        /// Returns a **std::tm** representation of this time.
        std::tm toBrokenStdTime() const;

        /// Returns a **std::time_t** representation of this time.
        std::time_t toScalarStdTime() const;

        /** 
         * @brief Returns the time as a string formatted according to the formatter string ***format***. 
         * 
         * The formatting string may contain the following patterns:
         *
         *    Pattern   |                    Meaning                    
         *  ----------- | --------------------------------------------- 
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
         * If this time is invalid, an empty string will be returned.
         */
        std::string toString(const std::string& format) const;
        //@}

        /** 
         * @brief Returns a Time object set to the current time obtained from the system clock. 
         * 
         * **Note** that the returned time is not the current local time, rather it is the current system time; the current time in Coordinated Universal %Time (UTC). 
         */
        static Time current();

        /// Returns a Time object set to midnight (i.e., "00:00:00").
        static Time midnight();

        /** 
         * @brief Returns a Time object from the string ***time*** according to the formatter string ***format***.
         * 
         * The formatter patterns are the same patterns used in the method toString(). @see toString() 
         */
        static Time fromString(const std::string& time, const std::string& format);

        /**  
         * @name Calculation Methods
         * @brief The following methods return the time difference between two times; ***from*** and ***to***. If ***from*** is smaller than (earlier than) ***to***, then the difference is negative.
         */
        //@{
        /// Returns the number of nanoseconds between ***from*** and ***to***.
        static long long nanosecondsBetween(const Time& from, const Time& to);

        /// Returns the number of microseconds between ***from*** and ***to***.
        static long long microsecondsBetween(const Time& from, const Time& to);

        /// Returns the number of milliseconds between ***from*** and ***to***.
        static long millisecondsBetween(const Time& from, const Time& to);

        /// Returns the number of seconds between ***from*** and ***to***.
        static long secondsBetween(const Time& from, const Time& to);

        /// Returns the number of minutes between ***from*** and ***to***.
        static int minutesBetween(const Time& from, const Time& to);

        /// Returns the number of hours between ***from*** and ***to***.
        static int hoursBetween(const Time& from, const Time& to);
        //@}

    private:
        Duration mTimeDuration;
    };

    /** 
     * @relates Time
     * @name Input/Output Operators 
     */
    //@{

    /// Writes time ***t*** to stream ***os*** in ISO-8601 time format "hh:mm:ss.fff". See toString() for information about the formatter patterns.
    std::ostream& operator<<(std::ostream& os, const Time& t);

    /// Reads a time in ISO-8601 time format "hh:mm:ss.fff" from stream ***is*** and stores it in time ***t***. See toString() for information about the formatter patterns.
    std::istream& operator>>(std::istream& is, Time& t);
    //@}
}

#endif // SALSABIL_TIME_HPP