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

namespace Salsabil {

    /** 
     * @class Time
     * @brief Time is an immutable time class representing a time without a time zone in the ISO-8601 calendar system, such as 09:55:02. 
     * 
     * Time is represented to nanosecond precision. For example, the value "09:55:02.123456789" can be stored in a Time object.
     * Time uses the 24-hour clock format; it describes the time as the number of hours, minutes, seconds, milliseconds, etc. since midnight.
     * Time provides functions for comparing times, manipulating a time by adding or subtracting intervals and converting the whole time into minutes, seconds or any finer interval.
     * 
     * A Time object can be created by giving the number of hours, minutes, seconds, milliseconds, etc. explicitly. Also, it can be created from std::time_t, std::tm or std::chrono::system_clock::time_point object.
     * 
     * A default-constructed time object is initialized to the current time in system's clock. Note that the accuracy depends on the accuracy of the underlying operating system; not all systems provide 1-millisecond accuracy.
     * The hour(), minute(), second(), millisecond(), microsecond() and nanosecond() functions provide access to the number of hours, minutes, seconds, milliseconds, microseconds and nanoseconds of the time, respectively. 
     * 
     * Time supports C++11 chrono library's way of manipulating time, i.e. time constructors and functions can takes duration parameters such as std::chrono::hours(1), std::chrono::minutes(1), etc. For convenience, Time defines aliases to these intervals which may be used instead of chrono's intervals. Time intervals are Hours, Minutes, Seconds, Milliseconds, Microseconds and Nanoseconds. which correspond to std:chrono::hours, std:chrono::minutes, std:chrono::seconds, std:chrono::milliseconds, std:chrono::microseconds and std:chrono::nanoseconds, respectively.
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
        /** @name Aliases */
        //@{
        using Hours = std::chrono::hours;
        using Minutes = std::chrono::minutes;
        using Seconds = std::chrono::seconds;
        using Milliseconds = std::chrono::milliseconds;
        using Microseconds = std::chrono::microseconds;
        using Nanoseconds = std::chrono::nanoseconds;
        //@}

        /** @name Constructors and Destructors */
        //@{

        /** @brief Constructs a Time object from the given <i>hours</i>, <i>minutes</i> and <i>seconds</i>. */
        explicit Time(int hours, int minutes, int seconds)
        : mTimeDuration(Hours(hours) + Minutes(minutes) + Seconds(seconds)) {
        }

        /** @brief Constructs a Time object from the given <i>hours</i>, <i>minutes</i>, <i>seconds</i> and <i>milliseconds</i>. */
        explicit Time(int hours, int minutes, int seconds, int milliseconds)
        : mTimeDuration(Hours(hours) + Minutes(minutes) + Seconds(seconds) + Milliseconds(milliseconds)) {
        }

        /**
         * @brief Constructs a Time object from the given <i>hours</i>, <i>minutes</i>, <i>seconds</i> and <i>subseconds</i>.
         *  
         * The <i>subseconds</i> parameter can be any fine duration of Time such as Time::Microseconds(54), or any fine duration of chrono such as std::chrono::nanoseconds(435223543).
         */
        explicit Time(int hours, int minutes, int seconds, const Duration& subseconds)
        : mTimeDuration(Hours(hours) + Minutes(minutes) + Seconds(seconds) + subseconds) {
        }

        /** 
         * @brief Constructs a Time object from the given <i>hours</i>, <i>minutes</i>, <i>seconds</i> and <i>subseconds</i>. 
         * 
         * All parameters can be Time::Duration or std::chrono::duration intervals. For example:
         * {@code
         *    Time myTime(Time::Hours(2), Time::Minutes(55), Time::Seconds(10), Time::Nanoseconds(435223543));
         *    Time sameTime(std::chrono::hours(2), std::chrono::::minutes(55), std::chrono::::seconds(10), std::chrono::::nanoseconds(435223543));
         * }
         */
        explicit Time(Hours hours, Minutes minutes, Seconds seconds, const Duration& subseconds)
        : mTimeDuration(hours + minutes + seconds + subseconds) {
        }

        /** 
         * @brief Constructs a Time object from the given <i>duration</i>. 
         * 
         * @param duration can be Time::Duration or std::chrono::duration. For example:
         * {@code
         *    Time myTime(Time::Hours(2) + Time::Minutes(55));
         * }
         */
        explicit Time(const Duration& duration)
        : mTimeDuration(duration) {
        }

        /** @brief Constructs a Time object from the standard library chrono time point <i>timePoint</i>. */
        explicit Time(const std::chrono::system_clock::time_point& timePoint)
        : mTimeDuration(timePoint.time_since_epoch()) {
        }

        /** @brief Constructs a Time object from a standard library <b>std::tm</b> object <i>brokenStdTime</i>. */
        explicit Time(const std::tm& brokenStdTime)
        : mTimeDuration(Hours(brokenStdTime.tm_hour) + Minutes(brokenStdTime.tm_min) + Seconds(brokenStdTime.tm_sec)) {
        }

        /** @brief Constructs a Time object from a standard library <b>std::time_t</b> object <i>scalarStdTime</i>. */
        explicit Time(std::time_t scalarStdTime)
        : mTimeDuration(Seconds(scalarStdTime)) {
        }

        /** @brief Copy-constructs a Time object from the given <i>time</i> object. */
        Time(const Time& time) = default;

        /** @brief Move-constructs a Time object from the given <i>time</i> object. */
        Time(Time&& time) = default;

        /** @brief Default constructor. Constructs a Time object and initializes it to the current time obtained from the system clock. */
        Time()
        : mTimeDuration(std::chrono::duration_cast<Nanoseconds>(std::chrono::system_clock::now().time_since_epoch() % Days(1))) {
        }

        /** @brief Default destructor. */
        virtual ~Time() {
        }
        //@}

        /** 
         * @brief Returns whether this time object represents a valid time. A valid Time object doesn't have more than 23 hours.
         * 
         * {@code
         *    Time t(Time::Hours(24));
         *    t.isValid(); // return false
         *    t = Time(22, 1, 55);
         *    t.isValid(); // return true
         * }
         */
        bool isValid() const {
            if (toHours() >= 24)
                return false;

            return true;
        }

        /**  
         * @name Querying Functions
         * @brief These functions retrieve the different parts of time.
         */
        //@{

        /** @brief Returns the hour of day (0, 23). */
        int hour() const {
            return std::chrono::duration_cast<Hours>(mTimeDuration % Days(1)).count();
        }

        /** @brief Returns the minute of hour (0, 59). */
        int minute() const {
            return std::chrono::duration_cast<Minutes>(mTimeDuration % Hours(1)).count();
        }

        /** @brief Returns the second of minute (0, 59). */
        int second() const {
            return std::chrono::duration_cast<Seconds>(mTimeDuration % Minutes(1)).count();
        }

        /** @brief Returns the millisecond of second (0, 999). */
        int millisecond() const {
            return std::chrono::duration_cast<Milliseconds>(mTimeDuration % Seconds(1)).count();
        }

        /** @brief Returns the microsecond of second (0, 999999). */
        long microsecond() const {
            return std::chrono::duration_cast<Microseconds>(mTimeDuration % Seconds(1)).count();
        }

        /** @brief Returns the nanosecond of second (0, 999999999). */
        long nanosecond() const {
            return std::chrono::duration_cast<Nanoseconds>(mTimeDuration % Seconds(1)).count();
        }
        //@}

        /**  
         * @name Addition/Subtraction Functions and Operators
         * @brief These functions return a new Time object with some time interval added/subtracted.
         */
        //@{

        /** @brief Returns a new copy of this Time object with the time duration <i>duration</i> added to it. */
        Time addDuration(const Duration& duration) const {
            return Time(mTimeDuration + duration);
        }

        /** @brief Returns a new copy of this Time object with the time duration <i>duration</i> subtracted from it. */
        Time subtractDuration(const Duration& duration) const {
            return Time(mTimeDuration - duration);
        }

        /** @brief Returns a new copy of this Time object with the hours <i>hours</i> added to it. */
        Time addHours(int hours) const {
            return Time(mTimeDuration + Hours(hours));
        }

        /** @brief Returns a new copy of this Time object with the hours <i>hours</i> subtracted from it. */
        Time subtractHours(int hours) const {
            return Time(mTimeDuration - Hours(hours));
        }

        /** @brief Returns a new copy of this Time object with the minutes <i>minutes</i> added to it. */
        Time addMinutes(int minutes) const {
            return Time(mTimeDuration + Minutes(minutes));
        }

        /** @brief Returns a new copy of this Time object with the minutes <i>minutes</i> subtracted from it. */
        Time subtractMinutes(int minutes) const {
            return Time(mTimeDuration - Minutes(minutes));
        }

        /** @brief Returns a new copy of this Time object with the seconds <i>seconds</i> added to it. */
        Time addSeconds(int seconds) const {
            return Time(mTimeDuration + Seconds(seconds));
        }

        /** @brief Returns a new copy of this Time object with the seconds <i>seconds</i> subtracted from it. */
        Time subtractSeconds(int seconds) const {
            return Time(mTimeDuration - Seconds(seconds));
        }

        /** @brief Returns a new copy of this Time object with the milliseconds <i>milliseconds</i> added to it. */
        Time addMilliseconds(int milliseconds) const {
            return Time(mTimeDuration + Milliseconds(milliseconds));
        }

        /** @brief Returns a new copy of this Time object with the milliseconds <i>milliseconds</i> subtracted from it. */
        Time subtractMilliseconds(int milliseconds) const {
            return Time(mTimeDuration - Milliseconds(milliseconds));
        }

        /** @brief Returns a new copy of this Time object with the microseconds <i>microseconds</i> added to it. */
        Time addMicroseconds(int microseconds) const {
            return Time(mTimeDuration + Microseconds(microseconds));
        }

        /** @brief Returns a new copy of this Time object with the microseconds <i>microseconds</i> subtracted from it. */
        Time subtractMicroseconds(int microseconds) const {
            return Time(mTimeDuration - Microseconds(microseconds));
        }

        /** @brief Returns a new copy of this Time object with the nanoseconds <i>nanoseconds</i> added to it. */
        Time addNanoseconds(int nanoseconds) const {
            return Time(mTimeDuration + Nanoseconds(nanoseconds));
        }

        /** @brief Returns a new copy of this Time object with the nanoseconds <i>nanoseconds</i> subtracted from it. */
        Time subtractNanoseconds(int nanoseconds) const {
            return Time(mTimeDuration - Nanoseconds(nanoseconds));
        }

        /** @brief Returns the result of subtracting <i>time</i> from this time as Time::Nanoseconds. */
        Nanoseconds operator-(const Time& time) const {
            return Nanoseconds(this->mTimeDuration - time.mTimeDuration);
        }

        /** @brief Returns the result of subtracting <i>duration</i> from this time as a new Time object. */
        Time operator-(const Duration& duration) const {
            return Time(this->mTimeDuration - duration);
        }

        /** @brief Returns the result of adding <i>duration</i> to this time as a new Time object. */
        Time operator+(const Duration& duration) const {
            return Time(this->mTimeDuration + duration);
        }
        //@}

        /**  
         * @name Conversion Functions
         * @brief These functions convert the time container or representation to another container or representation. For example, a Time object can be converted to std::tm object or could be converted to a number of minutes such as 3200.
         */
        //@{

        /** @brief Returns the elapsed hours since midnight. The returned value may exceed 23. */
        int toHours() const {
            return std::chrono::duration_cast<Hours>(mTimeDuration).count();
        }

        /** @brief Returns the elapsed minutes since midnight. */
        int toMinutes() const {
            return std::chrono::duration_cast<Minutes>(mTimeDuration).count();
        }

        /** @brief Returns the elapsed seconds since midnight. */
        long toSeconds() const {
            return std::chrono::duration_cast<Seconds>(mTimeDuration).count();
        }

        /** @brief Returns the elapsed milliseconds since midnight. */
        long toMilliseconds() const {
            return std::chrono::duration_cast<Milliseconds>(mTimeDuration).count();
        }

        /** @brief Returns the elapsed microseconds since midnight. */
        long long toMicroseconds() const {
            return std::chrono::duration_cast<Microseconds>(mTimeDuration).count();
        }

        /** @brief Returns the elapsed nanoseconds since midnight. */
        long long toNanoseconds() const {
            return std::chrono::duration_cast<Nanoseconds>(mTimeDuration).count();
        }

        /** @brief Returns a std::chrono::system_clock::time_point representation of this time. */
        std::chrono::system_clock::time_point toTimePoint() const {
            return std::chrono::system_clock::time_point(mTimeDuration);
        }

        /** @brief Returns a std::tm representation of this time. */
        std::tm toBrokenStdTime() const {
            std::tm cTime = {0};
            cTime.tm_hour = hour();
            cTime.tm_min = minute();
            cTime.tm_sec = second();
            return cTime;
        }

        /** @brief Returns a std::time_t representation of this time. */
        std::time_t toScalarStdTime() const {
            return std::time_t(toSeconds());
        }

        /** 
         * @brief Returns the time as a string formatted according to the formatting string <i>format</i>. 
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
         * Any character in the formatting string not listed above will be inserted as is into the output string. 
         */
        std::string toString(const std::string& format) const {
            std::stringstream output;

            for (int pos = 0; pos < format.size(); ++pos) {
                char currChar = format[pos];
                const int charCount = countIdenticalCharsFrom(pos, format);
                pos += charCount - 1; // skip all identical characters except the last.

                if (currChar == 'h') {
                    if (charCount == 1) {
                        output << hour();
                    } else if (charCount == 2) {
                        output << std::setfill('0') << std::setw(2) << hour();
                    }
                } else if (currChar == 'H') {
                    int hours12f = ((hour() == 0 || hour() == 12) ? 12 : hour() % 12);
                    if (charCount == 1) {
                        output << hours12f;
                    } else if (charCount == 2) {
                        output << std::setfill('0') << std::setw(2) << hours12f;
                    }
                } else if (currChar == 'm') {
                    if (charCount == 1) {
                        output << minute();
                    } else if (charCount == 2) {
                        output << std::setfill('0') << std::setw(2) << minute();
                    }
                } else if (currChar == 's') {
                    if (charCount == 1) {
                        output << second();
                    } else if (charCount == 2) {
                        output << std::setfill('0') << std::setw(2) << second();
                    }
                } else if (currChar == 'f') {
                    std::string subseconds = std::to_string(std::chrono::duration_cast<Nanoseconds>(mTimeDuration % Seconds(1)).count());
                    std::string padddedSubsecondsString = subseconds.insert(0, 9 - subseconds.size(), '0');
                    output << padddedSubsecondsString.substr(0, charCount);
                } else if (currChar == 'A') {
                    output << (hour() >= 12 ? "PM" : "AM");
                } else if (currChar == 'a') {
                    output << (hour() >= 12 ? "pm" : "am");
                } else {
                    output << currChar;
                }
            }

            return output.str();
        }
        //@}

        /** 
         * @brief Returns a Time object that represents the time parsed from the string <i>timeString</i> according to the format <i>format</i>.
         * 
         * For further information about the <i>format</i> parameter, see toString(). 
         */
        static Time fromString(const std::string& timeString, const std::string& format) {
            std::stringstream input;
            input << timeString;
            int _hours = 0, _minutes = 0, _seconds = 0;
            long _fractions = 0;

            for (int pos = 0; pos < format.size(); ++pos) {
                char currChar = format[pos];
                char nextChar = (pos + 1) < format.size() ? format[pos + 1] : '\0';
                if (currChar == 'h') {
                    if (nextChar == 'h') {
                        ++pos; // Skip 'h' the next loop. for the sake of not looping over it again.
                    }
                    input >> _hours;
                } else if (currChar == 'H') {
                    if (nextChar == 'H') {
                        ++pos; // Skip 'h' the next loop. for the sake of not looping over it again.
                    }
                    input >> _hours;
                    _hours = (_hours > 12 ? _hours - 12 : _hours);
                } else if (currChar == 'a') {
                    std::string meridiem;
                    input >> meridiem;
                    if (meridiem == "pm") {
                        _hours = (_hours > 12 ? _hours : _hours + 12);

                    }
                } else if (currChar == 'A') {
                    std::string meridiem;
                    input >> meridiem;
                    if (meridiem == "PM") {
                        _hours = (_hours > 12 ? _hours : _hours + 12);
                    }
                } else if (currChar == 'm') {
                    if (nextChar == 'm') {
                        ++pos; // Skip 'm' the next loop. for the sake of not looping over it again.
                    }
                    input >> _minutes;
                } else if (currChar == 's') {
                    if (nextChar == 's') {
                        ++pos; // Skip 's' the next loop. for the sake of not looping over it again.
                    }
                    input >> _seconds;
                } else if (currChar == 'f') {
                    input >> _fractions;
                    _fractions = fractionsToNanoseconds(_fractions);
                    pos += countIdenticalCharsFrom(pos, format) - 1; // Skip f..., have been read.
                } else {
                    input.seekg(input.tellg() + decltype(input)::pos_type(1));
                }
            }

            return Time(Hours(_hours) + Minutes(_minutes) + Seconds(_seconds) + Nanoseconds(_fractions));
        }

        /**  
         * @name Comparison Operators
         * @brief These functions compare this time to another time.
         */
        //@{

        /** @brief Returns whether this time is earlier than <i>time</i>. */
        bool operator<(const Time& time) const {
            return this->mTimeDuration < time.mTimeDuration;
        }

        /** @brief Returns whether this time is earlier than <i>time</i> or equal to it. */
        bool operator<=(const Time& time) const {
            return this->mTimeDuration <= time.mTimeDuration;
        }

        /** @brief Returns whether this time is later than <i>time</i>. */
        bool operator>(const Time& time) const {
            return this->mTimeDuration > time.mTimeDuration;
        }

        /** @brief Returns whether this time is later than <i>time</i> or equal to it. */
        bool operator>=(const Time& time) const {
            return this->mTimeDuration >= time.mTimeDuration;
        }

        /** @brief Returns whether this time is equal to <i>time</i>. */
        bool operator==(const Time& time) const {
            return this->mTimeDuration == time.mTimeDuration;
        }

        /** @brief Returns whether this time is different from <i>time</i>. */
        bool operator!=(const Time& time) const {
            return this->mTimeDuration != time.mTimeDuration;
        }
        //@}

        /**  
         * @name Assignment Operators
         */
        //@{
        /** @brief Copy assignment operator. */
        Time& operator=(const Time& time) = default;

        /** @brief Move assignment operator. */
        Time& operator=(Time&& time) = default;
        //@}

        /**  
         * @name Calculating Difference Between Times
         * @brief These functions return the duration between time <i>from</i> and time <i>to</i>. If <i>to</i> is earlier than (smaller than) <i>from</i>, then the difference is negative.
         */
        //@{

        /** @brief Returns the number of hours between time <i>from</i> and time <i>to</i>. */
        static int hoursBetween(const Time& from, const Time& to) {
            return from.toHours() - to.toHours();
        }

        /** @brief Returns the number of minutes between time <i>from</i> and time <i>to</i>. */
        static int minutesBetween(const Time& from, const Time& to) {
            return from.toMinutes() - to.toMinutes();
        }

        /** @brief Returns the number of seconds between time <i>from</i> and time <i>to</i>. */
        static int secondsBetween(const Time& from, const Time& to) {
            return from.toSeconds() - to.toSeconds();
        }

        /** @brief Returns the number of milliseconds between time <i>from</i> and time <i>to</i>. */
        static int millisecondsBetween(const Time& from, const Time& to) {
            return from.toMilliseconds() - to.toMilliseconds();
        }

        /** @brief Returns the number of microseconds between time <i>from</i> and time <i>to</i>. */
        static int microsecondsBetween(const Time& from, const Time& to) {
            return from.toMicroseconds() - to.toMicroseconds();
        }

        /** @brief Returns the number of nanoseconds between time <i>from</i> and time <i>to</i>. */
        static int nanosecondsBetween(const Time& from, const Time& to) {
            return from.toNanoseconds() - to.toNanoseconds();
        }
        //@}

    private:

        static int countIdenticalCharsFrom(std::size_t pos, const std::string& str) {
            int idx = pos + 1;

            while (idx < str.size() && str[idx] == str[pos])
                ++idx;

            return idx - pos;
        }

        static long fractionsToNanoseconds(long fraction) {
            const int Precision = 9;
            std::string fractionString = std::to_string(fraction);
            std::string padddedFractionString = fractionString.append(Precision - fractionString.size(), '0');
            return std::stol(padddedFractionString);
        }

        Duration mTimeDuration;
    };

    /** 
     * @relates Time
     * @name Serialization/Deserialization Functions 
     */
    //@{

    /** @brief Writes time <i>t</i> to stream <i>os</i> in ISO-8601 time format "hh:mm:ss". */
    std::ostream& operator<<(std::ostream& os, const Time& t) {
        os << t.toString("hh:mm:ss");
        return os;
    }

    /** @brief Reads a time in ISO-8601 time format "hh:mm:ss" from stream <i>is</i> and stores it in time <i>t</i>. */
    std::istream& operator>>(std::istream& is, Time& t) {
        const int TimeFormatWidth = 8;
        char result[TimeFormatWidth];
        is.read(result, TimeFormatWidth);
        std::string str(result, TimeFormatWidth);

        t = Time::fromString(str, "hh:mm:ss");
    }
    //@}
}

#endif // SALSABIL_TIME_HPP