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

#include "Time.hpp"

#include <sstream>
#include <iomanip>

using namespace Salsabil;

Time::Time(int hours, int minutes, int seconds)
: mTimeDuration(Hours(hours) + Minutes(minutes) + Seconds(seconds)) {
}

Time::Time(int hours, int minutes, int seconds, int milliseconds)
: mTimeDuration(Hours(hours) + Minutes(minutes) + Seconds(seconds) + Milliseconds(milliseconds)) {
}

Time::Time(int hours, int minutes, int seconds, const Duration& subseconds)
: mTimeDuration(Hours(hours) + Minutes(minutes) + Seconds(seconds) + subseconds) {
}

Time::Time(Hours hours, Minutes minutes, Seconds seconds, const Duration& subseconds)
: mTimeDuration(hours + minutes + seconds + subseconds) {
}

Time::Time(const Duration& duration)
: mTimeDuration(duration) {
}

Time::Time(const std::chrono::system_clock::time_point& timePoint)
: mTimeDuration(timePoint.time_since_epoch()) {
}

Time::Time(const std::tm& brokenStdTime)
: mTimeDuration(Hours(brokenStdTime.tm_hour) + Minutes(brokenStdTime.tm_min) + Seconds(brokenStdTime.tm_sec)) {
}

Time::Time(std::time_t scalarStdTime)
: mTimeDuration(Seconds(scalarStdTime)) {
}

Time::Time()
: mTimeDuration(std::chrono::duration_cast<Nanoseconds>(std::chrono::system_clock::now().time_since_epoch() % Days(1))) {
}

bool Time::isValid() const {
    if (toHours() >= 24)
        return false;

    return true;
}

int Time::hour() const {
    return std::chrono::duration_cast<Hours>(mTimeDuration % Days(1)).count();
}

int Time::minute() const {
    return std::chrono::duration_cast<Minutes>(mTimeDuration % Hours(1)).count();
}

int Time::second() const {
    return std::chrono::duration_cast<Seconds>(mTimeDuration % Minutes(1)).count();
}

int Time::millisecond() const {
    return std::chrono::duration_cast<Milliseconds>(mTimeDuration % Seconds(1)).count();
}

long Time::microsecond() const {
    return std::chrono::duration_cast<Microseconds>(mTimeDuration % Seconds(1)).count();
}

long Time::nanosecond() const {
    return std::chrono::duration_cast<Nanoseconds>(mTimeDuration % Seconds(1)).count();
}

Time Time::addDuration(const Duration& duration) const {
    return Time(mTimeDuration + duration);
}

Time Time::subtractDuration(const Duration& duration) const {
    return Time(mTimeDuration - duration);
}

Time Time::addHours(int hours) const {
    return Time(mTimeDuration + Hours(hours));
}

Time Time::subtractHours(int hours) const {
    return Time(mTimeDuration - Hours(hours));
}

Time Time::addMinutes(int minutes) const {
    return Time(mTimeDuration + Minutes(minutes));
}

Time Time::subtractMinutes(int minutes) const {
    return Time(mTimeDuration - Minutes(minutes));
}

Time Time::addSeconds(int seconds) const {
    return Time(mTimeDuration + Seconds(seconds));
}

Time Time::subtractSeconds(int seconds) const {
    return Time(mTimeDuration - Seconds(seconds));
}

Time Time::addMilliseconds(int milliseconds) const {
    return Time(mTimeDuration + Milliseconds(milliseconds));
}

Time Time::subtractMilliseconds(int milliseconds) const {
    return Time(mTimeDuration - Milliseconds(milliseconds));
}

Time Time::addMicroseconds(int microseconds) const {
    return Time(mTimeDuration + Microseconds(microseconds));
}

Time Time::subtractMicroseconds(int microseconds) const {
    return Time(mTimeDuration - Microseconds(microseconds));
}

Time Time::addNanoseconds(int nanoseconds) const {
    return Time(mTimeDuration + Nanoseconds(nanoseconds));
}

Time Time::subtractNanoseconds(int nanoseconds) const {
    return Time(mTimeDuration - Nanoseconds(nanoseconds));
}

Time::Nanoseconds Time::operator-(const Time& time) const {
    return Nanoseconds(this->mTimeDuration - time.mTimeDuration);
}

Time Time::operator-(const Duration& duration) const {
    return Time(this->mTimeDuration - duration);
}

Time Time::operator+(const Duration& duration) const {
    return Time(this->mTimeDuration + duration);
}

int Time::toHours() const {
    return std::chrono::duration_cast<Hours>(mTimeDuration).count();
}

int Time::toMinutes() const {
    return std::chrono::duration_cast<Minutes>(mTimeDuration).count();
}

long Time::toSeconds() const {
    return std::chrono::duration_cast<Seconds>(mTimeDuration).count();
}

long Time::toMilliseconds() const {
    return std::chrono::duration_cast<Milliseconds>(mTimeDuration).count();
}

long long Time::toMicroseconds() const {
    return std::chrono::duration_cast<Microseconds>(mTimeDuration).count();
}

long long Time::toNanoseconds() const {
    return std::chrono::duration_cast<Nanoseconds>(mTimeDuration).count();
}

std::chrono::system_clock::time_point Time::toTimePoint() const {
    return std::chrono::system_clock::time_point(mTimeDuration);
}

std::tm Time::toBrokenStdTime() const {
    std::tm cTime = {0};
    cTime.tm_hour = hour();
    cTime.tm_min = minute();
    cTime.tm_sec = second();
    return cTime;
}

std::time_t Time::toScalarStdTime() const {
    return std::time_t(toSeconds());
}

std::string Time::toString(const std::string& format) const {
    std::stringstream output;

    for (int pos = 0; pos < format.size(); ++pos) {
        char currChar = format[pos];
        const int charCount = Utility::countIdenticalCharsFrom(pos, format);
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

Time Time::fromString(const std::string& timeString, const std::string& format) {
    int _hours = 0, _minutes = 0, _seconds = 0;
    long _subseconds = 0;

    for (int fmtPos = 0, timPos = 0; fmtPos < format.size() && timPos < timeString.size(); ++fmtPos) {
        const int patternCharCount = Utility::countIdenticalCharsFrom(fmtPos, format);
        fmtPos += patternCharCount - 1; // skip all identical characters except the last.

        if (format[fmtPos] == 'h' || format[fmtPos] == 'H') {
            _hours = Utility::readIntAndAdvancePos(timeString, timPos, 2);
        } else if (format[fmtPos] == 'm') {
            _minutes = Utility::readIntAndAdvancePos(timeString, timPos, 2);
        } else if (format[fmtPos] == 's') {
            _seconds = Utility::readIntAndAdvancePos(timeString, timPos, 2);
        } else if (format[fmtPos] == 'f') {
            std::string subsecondString = timeString.substr(timPos, patternCharCount);
            _subseconds = std::stoi(subsecondString.append(9 - subsecondString.size(), '0'));
            timPos += patternCharCount;
        } else if (format[fmtPos] == 'a' || format[fmtPos] == 'A') {
            if (timeString.substr(timPos, 2) == "pm" || timeString.substr(timPos, 2) == "PM") {
                _hours = (_hours > 12 ? _hours : _hours + 12);
                timPos += 2;
            }
        } else {
            ++timPos;
        }
    }

    return Time(Hours(_hours) + Minutes(_minutes) + Seconds(_seconds) + Nanoseconds(_subseconds));
}

bool Time::operator<(const Time& time) const {
    return this->mTimeDuration < time.mTimeDuration;
}

bool Time::operator<=(const Time& time) const {
    return this->mTimeDuration <= time.mTimeDuration;
}

bool Time::operator>(const Time& time) const {
    return this->mTimeDuration > time.mTimeDuration;
}

bool Time::operator>=(const Time& time) const {
    return this->mTimeDuration >= time.mTimeDuration;
}

bool Time::operator==(const Time& time) const {
    return this->mTimeDuration == time.mTimeDuration;
}

bool Time::operator!=(const Time& time) const {
    return this->mTimeDuration != time.mTimeDuration;
}

int Time::hoursBetween(const Time& from, const Time& to) {
    return from.toHours() - to.toHours();
}

int Time::minutesBetween(const Time& from, const Time& to) {
    return from.toMinutes() - to.toMinutes();
}

int Time::secondsBetween(const Time& from, const Time& to) {
    return from.toSeconds() - to.toSeconds();
}

int Time::millisecondsBetween(const Time& from, const Time& to) {
    return from.toMilliseconds() - to.toMilliseconds();
}

int Time::microsecondsBetween(const Time& from, const Time& to) {
    return from.toMicroseconds() - to.toMicroseconds();
}

int Time::nanosecondsBetween(const Time& from, const Time& to) {
    return from.toNanoseconds() - to.toNanoseconds();
}

std::ostream& Salsabil::operator<<(std::ostream& os, const Time& t) {
    os << t.toString("hh:mm:ss");
    return os;
}

std::istream& Salsabil::operator>>(std::istream& is, Time& t) {
    const int TimeFormatWidth = 8;
    char result[TimeFormatWidth];
    is.read(result, TimeFormatWidth);
    std::string str(result, TimeFormatWidth);

    t = Time::fromString(str, "hh:mm:ss");
}