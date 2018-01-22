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

#include "Time.hpp"

#include <sstream>
#include <iomanip>

using namespace Salsabil;

Time::Time()
: mTimeDuration(Hours(24)) {
}

Time::Time(const Time& other) : mTimeDuration(other.mTimeDuration) {
}

Time::Time(Time&& other) : mTimeDuration(std::move(other.mTimeDuration)) {
}

Time::Time(std::time_t scalarStdTime)
: mTimeDuration(Seconds(scalarStdTime)) {
}

Time::Time(const std::tm& brokenStdTime)
: mTimeDuration(Hours(brokenStdTime.tm_hour) + Minutes(brokenStdTime.tm_min) + Seconds(brokenStdTime.tm_sec)) {
}

Time::Time(const Duration& duration)
: mTimeDuration(duration) {
}

Time::Time(const std::chrono::system_clock::time_point& timePoint)
: mTimeDuration(timePoint.time_since_epoch()) {
}

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

Time& Time::operator=(const Time& other) {
    this->mTimeDuration = other.mTimeDuration;
    return *this;
}

Time& Time::operator=(Time&& other) {
    this->mTimeDuration = std::move(other.mTimeDuration);
    return *this;
}

bool Time::operator<(const Time& other) const {
    return this->mTimeDuration < other.mTimeDuration;
}

bool Time::operator<=(const Time& other) const {
    return this->mTimeDuration <= other.mTimeDuration;
}

bool Time::operator>(const Time& other) const {
    return this->mTimeDuration > other.mTimeDuration;
}

bool Time::operator>=(const Time& other) const {
    return this->mTimeDuration >= other.mTimeDuration;
}

bool Time::operator==(const Time& other) const {
    return this->mTimeDuration == other.mTimeDuration;
}

bool Time::operator!=(const Time& other) const {
    return this->mTimeDuration != other.mTimeDuration;
}

Time Time::operator+(const Duration& duration) const {
    return Time(this->mTimeDuration + duration);
}

Time Time::operator-(const Duration& duration) const {
    return Time(this->mTimeDuration - duration);
}

Time::Nanoseconds Time::operator-(const Time& other) const {
    return Nanoseconds(this->mTimeDuration - other.mTimeDuration);
}

bool Time::isValid() const {
    if (toNanosecondsSinceMidnight() < 0 || toHoursSinceMidnight() >= 24)
        return false;

    return true;
}

long Time::nanosecond() const {
    return std::chrono::duration_cast<Nanoseconds>(mTimeDuration % Seconds(1)).count();
}

long Time::microsecond() const {
    return std::chrono::duration_cast<Microseconds>(mTimeDuration % Seconds(1)).count();
}

int Time::millisecond() const {
    return std::chrono::duration_cast<Milliseconds>(mTimeDuration % Seconds(1)).count();
}

int Time::second() const {
    return std::chrono::duration_cast<Seconds>(mTimeDuration % Minutes(1)).count();
}

int Time::minute() const {
    return std::chrono::duration_cast<Minutes>(mTimeDuration % Hours(1)).count();
}

int Time::hour() const {
    return std::chrono::duration_cast<Hours>(mTimeDuration % Days(1)).count();
}

Time Time::addNanoseconds(int nanoseconds) const {
    return Time(mTimeDuration + Nanoseconds(nanoseconds));
}

Time Time::subtractNanoseconds(int nanoseconds) const {
    return Time(mTimeDuration - Nanoseconds(nanoseconds));
}

Time Time::addMicroseconds(int microseconds) const {
    return Time(mTimeDuration + Microseconds(microseconds));
}

Time Time::subtractMicroseconds(int microseconds) const {
    return Time(mTimeDuration - Microseconds(microseconds));
}

Time Time::addMilliseconds(int milliseconds) const {
    return Time(mTimeDuration + Milliseconds(milliseconds));
}

Time Time::subtractMilliseconds(int milliseconds) const {
    return Time(mTimeDuration - Milliseconds(milliseconds));
}

Time Time::addSeconds(int seconds) const {
    return Time(mTimeDuration + Seconds(seconds));
}

Time Time::subtractSeconds(int seconds) const {
    return Time(mTimeDuration - Seconds(seconds));
}

Time Time::addMinutes(int minutes) const {
    return Time(mTimeDuration + Minutes(minutes));
}

Time Time::subtractMinutes(int minutes) const {
    return Time(mTimeDuration - Minutes(minutes));
}

Time Time::addHours(int hours) const {
    return Time(mTimeDuration + Hours(hours));
}

Time Time::subtractHours(int hours) const {
    return Time(mTimeDuration - Hours(hours));
}

Time Time::addDuration(const Duration& duration) const {
    return Time(mTimeDuration + duration);
}

Time Time::subtractDuration(const Duration& duration) const {
    return Time(mTimeDuration - duration);
}

long long Time::toNanosecondsSinceMidnight() const {
    return std::chrono::duration_cast<Nanoseconds>(mTimeDuration).count();
}

long long Time::toMicrosecondsSinceMidnight() const {
    return std::chrono::duration_cast<Microseconds>(mTimeDuration).count();
}

long Time::toMillisecondsSinceMidnight() const {
    return std::chrono::duration_cast<Milliseconds>(mTimeDuration).count();
}

long Time::toSecondsSinceMidnight() const {
    return std::chrono::duration_cast<Seconds>(mTimeDuration).count();
}

int Time::toMinutesSinceMidnight() const {
    return std::chrono::duration_cast<Minutes>(mTimeDuration).count();
}

int Time::toHoursSinceMidnight() const {
    return std::chrono::duration_cast<Hours>(mTimeDuration).count();
}

Time::Nanoseconds Time::toStdDurationSinceMidnight() const {
    return mTimeDuration;
}

std::tm Time::toBrokenStdTime() const {
    std::tm cTime = {0};
    cTime.tm_hour = hour();
    cTime.tm_min = minute();
    cTime.tm_sec = second();
    return cTime;
}

std::time_t Time::toScalarStdTime() const {
    return std::time_t(toSecondsSinceMidnight());
}

std::string Time::toString(const std::string& format) const {
    if (!isValid())
        return std::string();

    std::stringstream output;

    for (int pos = 0; pos < format.size(); ++pos) {
        const int patternLength = Utility::countIdenticalCharsFrom(pos, format);

        if (format[pos] == 'h') {
            output << std::setfill('0') << std::setw(patternLength) << hour();
            pos += patternLength - 1; // skip all identical characters except the last.
        } else if (format[pos] == 'H') {
            int hours12f = ((hour() == 0 || hour() == 12) ? 12 : hour() % 12);
            output << std::setfill('0') << std::setw(patternLength) << hours12f;
            pos += patternLength - 1;
        } else if (format[pos] == 'm') {
            output << std::setfill('0') << std::setw(patternLength) << minute();
            pos += patternLength - 1;
        } else if (format[pos] == 's') {
            output << std::setfill('0') << std::setw(patternLength) << second();
            pos += patternLength - 1;
        } else if (format[pos] == 'f') {
            std::string subseconds = std::to_string(std::chrono::duration_cast<Nanoseconds>(mTimeDuration % Seconds(1)).count());
            std::string padddedSubsecondsString = subseconds.insert(0, 9 - subseconds.size(), '0');
            output << padddedSubsecondsString.substr(0, patternLength);
            pos += patternLength - 1;
        } else if (format[pos] == 'A') {
            output << (hour() >= 12 ? "PM" : "AM");
        } else if (format[pos] == 'a') {
            output << (hour() >= 12 ? "pm" : "am");
        } else {
            output << format[pos];
        }
    }

    return output.str();
}

Time Time::current() {
    return Time(std::chrono::duration_cast<Nanoseconds>(std::chrono::system_clock::now().time_since_epoch() % Days(1)));
}

Time Time::midnight() {
    return Time(Nanoseconds::zero());
}

Time Time::fromString(const std::string& time, const std::string& format) {
    int _hour = 0, _minute = 0, _second = 0;
    long _subsecond = 0;

    for (int fmtPos = 0, timPos = 0; fmtPos < format.size() && timPos < time.size(); ++fmtPos) {
        const int patternLength = Utility::countIdenticalCharsFrom(fmtPos, format);

        if (format[fmtPos] == 'h' || format[fmtPos] == 'H') {
            _hour = Utility::readIntAndAdvancePos(time, timPos, 2);
            fmtPos += patternLength - 1; // skip all identical characters except the last.
        } else if (format[fmtPos] == 'm') {
            _minute = Utility::readIntAndAdvancePos(time, timPos, 2);
            fmtPos += patternLength - 1;
        } else if (format[fmtPos] == 's') {
            _second = Utility::readIntAndAdvancePos(time, timPos, 2);
            fmtPos += patternLength - 1;
        } else if (format[fmtPos] == 'f') {
            std::string subsecondString = time.substr(timPos, patternLength);
            _subsecond = std::stoi(subsecondString.append(9 - subsecondString.size(), '0'));
            timPos += patternLength;
            fmtPos += patternLength - 1;
        } else if (format[fmtPos] == 'a' || format[fmtPos] == 'A') {
            if (time.substr(timPos, 2) == "pm" || time.substr(timPos, 2) == "PM") {
                _hour = (_hour > 12 ? _hour : _hour + 12);
                timPos += 2;
            }
        } else {
            ++timPos;
        }
    }

    return Time(Hours(_hour) + Minutes(_minute) + Seconds(_second) + Nanoseconds(_subsecond));
}

int Time::hoursBetween(const Time& from, const Time& to) {
    return to.toHoursSinceMidnight() - from.toHoursSinceMidnight();
}

int Time::minutesBetween(const Time& from, const Time& to) {
    return to.toMinutesSinceMidnight() - from.toMinutesSinceMidnight();
}

long Time::secondsBetween(const Time& from, const Time& to) {
    return to.toSecondsSinceMidnight() - from.toSecondsSinceMidnight();
}

long Time::millisecondsBetween(const Time& from, const Time& to) {
    return to.toMillisecondsSinceMidnight() - from.toMillisecondsSinceMidnight();
}

long long Time::microsecondsBetween(const Time& from, const Time& to) {
    return to.toMicrosecondsSinceMidnight() - from.toMicrosecondsSinceMidnight();
}

long long Time::nanosecondsBetween(const Time& from, const Time& to) {
    return to.toNanosecondsSinceMidnight() - from.toNanosecondsSinceMidnight();
}

std::ostream& Salsabil::operator<<(std::ostream& os, const Time& t) {
    os << t.toString("hh:mm:ss.fff");
    return os;
}

std::istream& Salsabil::operator>>(std::istream& is, Time& t) {
    const int TimeFormatWidth = 12;
    char result[TimeFormatWidth];
    is.read(result, TimeFormatWidth);
    
    t = Time::fromString(std::string(result, TimeFormatWidth), "hh:mm:ss.fff");
    return is;
}