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

        explicit Time(int hours, int minutes, int seconds)
        : mTimeDuration(Hours(hours) + Minutes(minutes) + Seconds(seconds)) {
        }

        explicit Time(int hours, int minutes, int seconds, int milliseconds)
        : mTimeDuration(Hours(hours) + Minutes(minutes) + Seconds(seconds) + Milliseconds(milliseconds)) {
        }

        explicit Time(int hours, int minutes, int seconds, const Duration& fractions)
        : mTimeDuration(Hours(hours) + Minutes(minutes) + Seconds(seconds) + fractions) {
        }

        explicit Time(const Duration& duration)
        : mTimeDuration(duration) {
        }

        explicit Time(const TimePoint& timePoint)
        : mTimeDuration(timePoint.time_since_epoch()) {
        }

        explicit Time(const std::tm& brokenStdTime)
        : mTimeDuration(Hours(brokenStdTime.tm_hour) + Minutes(brokenStdTime.tm_min) + Seconds(brokenStdTime.tm_sec)) {
        }

        explicit Time(std::time_t scalarStdTime)
        : mTimeDuration(Seconds(scalarStdTime)) {
        }

        Time(const Time& time)
        : mTimeDuration(time.mTimeDuration) {
        }

        Time(Time&& time)
        : mTimeDuration(std::move(time.mTimeDuration)) {
        }

        Time()
        : mTimeDuration(std::chrono::duration_cast<Nanoseconds>(std::chrono::system_clock::now().time_since_epoch() % Days(1))) {
        }

        bool isValid() const {
            if (toHours() > 24)
                return false;

            return true;
        }

        int toHours() const {
            return std::chrono::duration_cast<Hours>(mTimeDuration).count();
        }

        int toMinutes() const {
            return std::chrono::duration_cast<Minutes>(mTimeDuration).count();
        }

        long toSeconds() const {
            return std::chrono::duration_cast<Seconds>(mTimeDuration).count();
        }

        long toMilliseconds() const {
            return std::chrono::duration_cast<Milliseconds>(mTimeDuration).count();
        }

        long long toMicroseconds() const {
            return std::chrono::duration_cast<Microseconds>(mTimeDuration).count();
        }

        long long toNanoseconds() const {
            return std::chrono::duration_cast<Nanoseconds>(mTimeDuration).count();
        }

        int hours() const {
            return std::chrono::duration_cast<Hours>(mTimeDuration % Days(1)).count();
        }

        int minutes() const {
            return std::chrono::duration_cast<Minutes>(mTimeDuration % Hours(1)).count();
        }

        int seconds() const {
            return std::chrono::duration_cast<Seconds>(mTimeDuration % Minutes(1)).count();
        }

        int milliseconds() const {
            return std::chrono::duration_cast<Milliseconds>(mTimeDuration % Seconds(1)).count();
        }

        long microseconds() const {
            return std::chrono::duration_cast<Microseconds>(mTimeDuration % Seconds(1)).count();
        }

        long nanoseconds() const {
            return std::chrono::duration_cast<Nanoseconds>(mTimeDuration % Seconds(1)).count();
        }

        Time addDuration(const Duration& duration) const {
            return Time(mTimeDuration + duration);
        }

        Time subtractDuration(const Duration& duration) const {
            return Time(mTimeDuration - duration);
        }

        Time addHours(int hours) const {
            return Time(mTimeDuration + Hours(hours));
        }

        Time subtractHours(int hours) const {
            return Time(mTimeDuration - Hours(hours));
        }

        Time addMinutes(int minutes) const {
            return Time(mTimeDuration + Minutes(minutes));
        }

        Time subtractMinutes(int minutes) const {
            return Time(mTimeDuration - Minutes(minutes));
        }

        Time addSeconds(int seconds) const {
            return Time(mTimeDuration + Seconds(seconds));
        }

        Time subtractSeconds(int seconds) const {
            return Time(mTimeDuration - Seconds(seconds));
        }

        Time addMilliseconds(int milliseconds) const {
            return Time(mTimeDuration + Milliseconds(milliseconds));
        }

        Time subtractMilliseconds(int milliseconds) const {
            return Time(mTimeDuration - Milliseconds(milliseconds));
        }

        Time addMicroseconds(int microseconds) const {
            return Time(mTimeDuration + Microseconds(microseconds));
        }

        Time subtractMicroseconds(int microseconds) const {
            return Time(mTimeDuration - Microseconds(microseconds));
        }

        Time addNanoseconds(int nanoseconds) const {
            return Time(mTimeDuration + Nanoseconds(nanoseconds));
        }

        Time subtractNanoseconds(int nanoseconds) const {
            return Time(mTimeDuration - Nanoseconds(nanoseconds));
        }

        std::string toString(const std::string& format) const {
            std::stringstream output;

            for (int pos = 0; pos < format.size(); ++pos) {
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
                    const int precision = std::count(format.begin() + pos, format.end(), 'f');
                    std::string fraction = std::to_string(std::chrono::duration_cast<Nanoseconds>(mTimeDuration % Seconds(1)).count());
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
                    pos += std::count(format.begin() + pos, format.end(), 'f') - 1; // Skip the f..., have been read.
                } else {
                    input.seekg(input.tellg() + decltype(input)::pos_type(1));
                }
            }

            return Time(Hours(_hours) + Minutes(_minutes) + Seconds(_seconds) + Nanoseconds(_fractions));
        }

        std::chrono::system_clock::time_point toTimePoint() const {
            return std::chrono::system_clock::time_point(mTimeDuration);
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
            return this->mTimeDuration < time.mTimeDuration;
        }

        bool operator<=(const Time& time) const {
            return this->mTimeDuration <= time.mTimeDuration;
        }

        bool operator>(const Time& time) const {
            return this->mTimeDuration > time.mTimeDuration;
        }

        bool operator>=(const Time& time) const {
            return this->mTimeDuration >= time.mTimeDuration;
        }

        bool operator==(const Time& time) const {
            return this->mTimeDuration == time.mTimeDuration;
        }

        bool operator!=(const Time& time) const {
            return this->mTimeDuration != time.mTimeDuration;
        }

        Time& operator=(const Time& time) {
            this->mTimeDuration = time.mTimeDuration;
            return *this;
        }

        Time& operator=(Time&& time) {
            this->mTimeDuration = std::move(time.mTimeDuration);
            return *this;
        }

    private:

        static long fractionsToNanoseconds(long fraction) {
            const int Precision = 9;
            std::string fractionString = std::to_string(fraction);
            std::string padddedFractionString = fractionString.append(Precision - fractionString.size(), '0');
            return std::stol(padddedFractionString);
        }

        Duration mTimeDuration;
    };

    std::ostream& operator<<(std::ostream& os, const Time& t) {
        os << t.toString("hh:mm:ss");
        return os;
    }

    std::istream& operator>>(std::istream& is, Time& t) {
        const int TimeFormatWidth = 8;
        std::vector<char> result(TimeFormatWidth); // vector is guaranteed to be contiguous in C++03
        is.read(&result[0], TimeFormatWidth);
        std::string str(&result[0], &result[TimeFormatWidth]);

        t = t.fromString(str, "hh:mm:ss");
    }

}

#endif // SALSABIL_TIME_HPP

