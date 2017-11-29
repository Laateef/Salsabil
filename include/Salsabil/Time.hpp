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

    class Time {
    public:
        using TimePoint = std::chrono::system_clock::time_point;
        using Duration = std::chrono::nanoseconds;
        using Days = std::chrono::duration
                <int, std::ratio_multiply<std::ratio<24>, std::chrono::hours::period>>;
        using Hours = std::chrono::hours;
        using Minutes = std::chrono::minutes;
        using Seconds = std::chrono::seconds;
        using Milliseconds = std::chrono::milliseconds;
        using Microseconds = std::chrono::microseconds;
        using Nanoseconds = std::chrono::nanoseconds;

        explicit Time(int8_t hours, int8_t minutes, int8_t seconds)
        : mTimePoint(Hours(hours) + Minutes(minutes) + Seconds(seconds)) {
        }

        explicit Time(int8_t hours, int8_t minutes, int8_t seconds, int32_t fraction)
        : mTimePoint(Hours(hours) + Minutes(minutes) + Seconds(seconds) + Nanoseconds(fractionToNanoseconds(fraction))) {
        }

        explicit Time(const Duration& duration)
        : mTimePoint(duration) {
        }

        explicit Time(const TimePoint& timePoint)
        : mTimePoint(timePoint.time_since_epoch()) {
        }

        explicit Time(const std::tm& brokenStdTime)
        : mTimePoint(Hours(brokenStdTime.tm_hour) + Minutes(brokenStdTime.tm_min) + Seconds(brokenStdTime.tm_sec)) {
        }

        explicit Time(std::time_t scalarStdTime)
        : mTimePoint(Seconds(scalarStdTime)) {
        }

        Time(const Time& time)
        : mTimePoint(time.mTimePoint) {
        }

        Time(Time&& time)
        : mTimePoint(std::move(time.mTimePoint)) {
        }

        Time()
        : mTimePoint(std::chrono::system_clock::now().time_since_epoch()) {
        }

        bool isValid() const {
            if (toHours() > 24)
                return false;
            return true;
        }

        int8_t toHours() const {
            return std::chrono::duration_cast<Hours>(mTimePoint).count();
        }

        int16_t toMinutes() const {
            return std::chrono::duration_cast<Minutes>(mTimePoint).count();
        }

        int32_t toSeconds() const {
            return std::chrono::duration_cast<Seconds>(mTimePoint).count();
        }

        int32_t toMilliseconds() const {
            return std::chrono::duration_cast<Milliseconds>(mTimePoint).count();
        }

        int64_t toMicroseconds() const {
            return std::chrono::duration_cast<Microseconds>(mTimePoint).count();
        }

        int64_t toNanoseconds() const {
            return std::chrono::duration_cast<Nanoseconds>(mTimePoint).count();
        }

        int hours() const {
            return std::chrono::duration_cast<Hours>(mTimePoint % Days(1)).count();
        }

        int minutes() const {
            return std::chrono::duration_cast<Minutes>(mTimePoint % Hours(1)).count();
        }

        int seconds() const {
            return std::chrono::duration_cast<Seconds>(mTimePoint % Minutes(1)).count();
        }

        int milliseconds() const {
            return std::chrono::duration_cast<Milliseconds>(mTimePoint % Seconds(1)).count();
        }

        int microseconds() const {
            return std::chrono::duration_cast<Microseconds>(mTimePoint % Milliseconds(1)).count();
        }

        int nanoseconds() const {
            return std::chrono::duration_cast<Nanoseconds>(mTimePoint % Microseconds(1)).count();
        }

        Time addDuration(const Duration& duration) const {
            return Time(mTimePoint + duration);
        }

        Time subtractDuration(const Duration& duration) const {
            return Time(mTimePoint - duration);
        }

        std::string toString(const std::string& format) const {
            std::stringstream output;

            for (std::size_t pos = 0; pos < format.size(); ++pos) {
                char currChar = format[pos];
                char nextChar = (pos + 1) < format.size() ? format[pos + 1] : '\0';
                if (currChar == 'h') {
                    if (nextChar == 'h') {
                        ++pos; // Skip 'h' the next loop. for the sake of not looping over it again.
                        if (hours() < 10) {
                            output << '0';
                        }
                    }
                    output << hours();
                } else if (currChar == 'H') {
                    int hours12f = hours() > 12 ? hours() - 12 : hours();
                    if (nextChar == 'H') {
                        ++pos; // Skip 'H' the next loop. for the sake of not looping over it again.
                        if (hours12f < 10) {
                            output << '0';
                        }
                    }
                    output << hours12f;
                } else if (currChar == 'm') {
                    if (nextChar == 'm') {
                        ++pos; // Skip 'm' the next loop. for the sake of not looping over it again.
                        if (minutes() < 10) {
                            output << '0';
                        }
                    }
                    output << minutes();
                } else if (currChar == 's') {
                    if (nextChar == 's') {
                        ++pos; //Skip 's' the next loop. for the sake of not looping over it again.
                        if (seconds() < 10) {
                            output << '0';
                        }
                    }
                    output << seconds();
                } else if (currChar == 'f') {
                    std::size_t precision = std::count(format.begin() + pos, format.end(), 'f');
                    std::string fraction = std::to_string(std::chrono::duration_cast<Nanoseconds>(mTimePoint % Seconds(1)).count());
                    std::string padddedFraction = fraction.insert(0, 9 - fraction.size(), '0');
                    output << padddedFraction.substr(0, precision);
                    pos += precision - 1; // Skip the f..., have been read.
                } else if (currChar == 'A') {
                    output << (hours() >= 12 ? "PM" : "AM");
                } else if (currChar == 'a') {
                    output << (hours() >= 12 ? "pm" : "am");
                } else {
                    output << currChar;
                }
            }

            return output.str();
        }

        static Time fromString(const std::string& timeString, const std::string& format) {
            std::stringstream input;
            input << timeString;
            int _hours = 0, _minutes = 0, _seconds = 0;
            int64_t _fractions = 0;

            for (std::size_t pos = 0; pos < format.size(); ++pos) {
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
                    _fractions = fractionToNanoseconds(_fractions);
                    pos += std::count(format.begin() + pos, format.end(), 'f') - 1; // Skip the f..., have been read.
                } else {
                    input.seekg(input.tellg() + decltype(input)::pos_type(1));
                }
            }

            return Time(Hours(_hours) + Minutes(_minutes) + Seconds(_seconds) + Nanoseconds(_fractions));
        }

        std::chrono::system_clock::time_point toTimePoint() const {
            return std::chrono::system_clock::time_point(mTimePoint);
        }

        std::tm toBrokenStdTime() const {
            std::tm cTime = {0};
            cTime.tm_hour = hours();
            cTime.tm_min = minutes();
            cTime.tm_sec = seconds();
            return cTime;
        }

        std::time_t toScalarStdTime() const {
            return std::time_t(toSeconds());
        }

        bool operator<(const Time& time) const {
            return this->mTimePoint < time.mTimePoint;
        }

        bool operator<=(const Time& time) const {
            return this->mTimePoint <= time.mTimePoint;
        }

        bool operator>(const Time& time) const {
            return this->mTimePoint > time.mTimePoint;
        }

        bool operator>=(const Time& time) const {
            return this->mTimePoint >= time.mTimePoint;
        }

        bool operator==(const Time& time) const {
            return this->mTimePoint == time.mTimePoint;
        }

        bool operator!=(const Time& time) const {
            return this->mTimePoint != time.mTimePoint;
        }

        Time& operator=(const Time& time) {
            this->mTimePoint = time.mTimePoint;
            return *this;
        }

        Time& operator=(Time&& time) {
            this->mTimePoint = std::move(time.mTimePoint);
            return *this;
        }

    private:

        static int32_t fractionToNanoseconds(int32_t fraction) {
            const char Precision = 9;
            std::string fractionString = std::to_string(fraction);
            std::string padddedFractionString = fractionString.append(Precision - fractionString.size(), '0');
            return std::stol(padddedFractionString);
        }

        Duration mTimePoint;
    };

    std::ostream& operator<<(std::ostream& os, const Time& t) {
        os << t.toString("hh:mm:ss");
        return os;
    }

    std::istream& operator>>(std::istream& is, Time& t) {
        const char TimeFormatWidth = 8;
        std::vector<char> result(TimeFormatWidth); // vector is guaranteed to be contiguous in C++03
        is.read(&result[0], TimeFormatWidth);
        std::string str(&result[0], &result[TimeFormatWidth]);

        t = t.fromString(str, "hh:mm:ss");
    }

}

#endif // SALSABIL_TIME_HPP

