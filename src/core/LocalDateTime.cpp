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

#include "LocalDateTime.hpp"
#include "Definitions.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace Salsabil;

LocalDateTime::LocalDateTime() : mDateTime(), mTimeZone() {
}

LocalDateTime::LocalDateTime(const LocalDateTime& datetime) : mDateTime(datetime.mDateTime), mTimeZone(datetime.mTimeZone) {
}

LocalDateTime::LocalDateTime(LocalDateTime&& datetime) : mDateTime(std::move(datetime.mDateTime)), mTimeZone(std::move(datetime.mTimeZone)) {
}

LocalDateTime::LocalDateTime(const DateTime& datetime, const TimeZone& zone) : mDateTime(datetime), mTimeZone(zone) {
}

LocalDateTime& LocalDateTime::operator=(const LocalDateTime& other) {
    if (this != &other) {
        mDateTime = other.mDateTime;
        mTimeZone = other.mTimeZone;
    }
    return *this;
}

LocalDateTime& LocalDateTime::operator=(LocalDateTime&& other) {
    if (this != &other) {
        mDateTime = std::move(other.mDateTime);
        mTimeZone = std::move(other.mTimeZone);
    }
    return *this;
}

bool LocalDateTime::operator<(const LocalDateTime& other) const {
    return this->toUtc().dateTime() < other.toUtc().dateTime();
}

bool LocalDateTime::operator<=(const LocalDateTime& other) const {
    return this->toUtc().dateTime() <= other.toUtc().dateTime();
}

bool LocalDateTime::operator>(const LocalDateTime& other) const {
    return this->toUtc().dateTime() > other.toUtc().dateTime();
}

bool LocalDateTime::operator>=(const LocalDateTime& other) const {
    return this->toUtc().dateTime() >= other.toUtc().dateTime();
}

bool LocalDateTime::operator==(const LocalDateTime& other) const {
    return this->toUtc().dateTime() == other.toUtc().dateTime();
}

bool LocalDateTime::operator!=(const LocalDateTime& other) const {
    return this->toUtc().dateTime() != other.toUtc().dateTime();
}

LocalDateTime::Nanoseconds LocalDateTime::operator-(const LocalDateTime& other) const {
    return this->dateTime() - other.dateTime();
}

LocalDateTime LocalDateTime::operator-(const Duration& duration) const {
    return LocalDateTime(this->dateTime() - duration, mTimeZone);
}

LocalDateTime LocalDateTime::operator+(const Duration& duration) const {
    return LocalDateTime(this->dateTime() + duration, mTimeZone);
}

bool LocalDateTime::isValid() const {
    return mDateTime.isValid() && mTimeZone.isValid();
}

DateTime LocalDateTime::dateTime() const {
    return mDateTime;
}

TimeZone LocalDateTime::timeZone() const {
    return mTimeZone;
}

Date LocalDateTime::date() const {
    return mDateTime.date();
}

Time LocalDateTime::time() const {
    return mDateTime.time();
}

long LocalDateTime::nanosecond() const {
    return mDateTime.nanosecond();
}

long LocalDateTime::microsecond() const {
    return mDateTime.microsecond();
}

int LocalDateTime::millisecond() const {
    return mDateTime.millisecond();
}

int LocalDateTime::second() const {
    return mDateTime.second();
}

int LocalDateTime::minute() const {
    return mDateTime.minute();
}

int LocalDateTime::hour() const {
    return mDateTime.hour();
}

int LocalDateTime::day() const {
    return mDateTime.day();
}

int LocalDateTime::month() const {
    return mDateTime.month();
}

int LocalDateTime::year() const {
    return mDateTime.year();
}

LocalDateTime::Seconds LocalDateTime::offsetFromUtc() const {
    return mTimeZone.offsetAt(mDateTime);
}

int LocalDateTime::dayOfWeek() const {
    return mDateTime.dayOfWeek();
}

int LocalDateTime::dayOfYear() const {
    return mDateTime.dayOfYear();
}

int LocalDateTime::daysInMonth() const {
    return mDateTime.daysInMonth();
}

int LocalDateTime::daysInYear() const {
    return mDateTime.daysInYear();
}

bool LocalDateTime::isLeapYear() const {
    return mDateTime.isLeapYear();
}

int LocalDateTime::weekOfYear(int* weekYear) const {
    return mDateTime.weekOfYear(weekYear);
}

std::string LocalDateTime::dayOfWeekName(bool useShortName) const {
    return mDateTime.dayOfWeekName(useShortName);
}

std::string LocalDateTime::monthName(bool useShortName) const {
    return mDateTime.monthName(useShortName);
}

LocalDateTime LocalDateTime::addNanoseconds(int nanoseconds) const {
    return LocalDateTime(mDateTime.addNanoseconds(nanoseconds), mTimeZone);
}

LocalDateTime LocalDateTime::subtractNanoseconds(int nanoseconds) const {
    return LocalDateTime(mDateTime.subtractNanoseconds(nanoseconds), mTimeZone);
}

LocalDateTime LocalDateTime::addMicroseconds(int microseconds) const {
    return LocalDateTime(mDateTime.addMicroseconds(microseconds), mTimeZone);
}

LocalDateTime LocalDateTime::subtractMicroseconds(int microseconds) const {
    return LocalDateTime(mDateTime.subtractMicroseconds(microseconds), mTimeZone);
}

LocalDateTime LocalDateTime::addMilliseconds(int milliseconds) const {
    return LocalDateTime(mDateTime.addMilliseconds(milliseconds), mTimeZone);
}

LocalDateTime LocalDateTime::subtractMilliseconds(int milliseconds) const {
    return LocalDateTime(mDateTime.subtractMilliseconds(milliseconds), mTimeZone);
}

LocalDateTime LocalDateTime::addSeconds(int seconds) const {
    return LocalDateTime(mDateTime.addSeconds(seconds), mTimeZone);
}

LocalDateTime LocalDateTime::subtractSeconds(int seconds) const {
    return LocalDateTime(mDateTime.subtractSeconds(seconds), mTimeZone);
}

LocalDateTime LocalDateTime::addMinutes(int minutes) const {
    return LocalDateTime(mDateTime.addMinutes(minutes), mTimeZone);
}

LocalDateTime LocalDateTime::subtractMinutes(int minutes) const {
    return LocalDateTime(mDateTime.subtractMinutes(minutes), mTimeZone);
}

LocalDateTime LocalDateTime::addHours(int hours) const {
    return LocalDateTime(mDateTime.addHours(hours), mTimeZone);
}

LocalDateTime LocalDateTime::subtractHours(int hours) const {
    return LocalDateTime(mDateTime.subtractHours(hours), mTimeZone);
}

LocalDateTime LocalDateTime::addDays(int days) const {
    return LocalDateTime(mDateTime.addDays(days), mTimeZone);
}

LocalDateTime LocalDateTime::subtractDays(int days) const {
    return LocalDateTime(mDateTime.subtractDays(days), mTimeZone);
}

LocalDateTime LocalDateTime::addMonths(int months) const {
    return LocalDateTime(mDateTime.addMonths(months), mTimeZone);
}

LocalDateTime LocalDateTime::subtractMonths(int months) const {
    return LocalDateTime(mDateTime.subtractMonths(months), mTimeZone);
}

LocalDateTime LocalDateTime::addYears(int years) const {
    return LocalDateTime(mDateTime.addYears(years), mTimeZone);
}

LocalDateTime LocalDateTime::subtractYears(int years) const {
    return LocalDateTime(mDateTime.subtractYears(years), mTimeZone);
}

LocalDateTime LocalDateTime::addDuration(const Duration& duration) const {
    return LocalDateTime(mDateTime.addDuration(duration), mTimeZone);
}

LocalDateTime LocalDateTime::subtractDuration(const Duration& duration) const {
    return LocalDateTime(mDateTime.subtractDuration(duration), mTimeZone);
}

LocalDateTime LocalDateTime::toUtc() const {
    return LocalDateTime(mDateTime.subtractDuration(mTimeZone.offsetAt(mDateTime)), TimeZone::utc());
}

LocalDateTime LocalDateTime::current() {
    return LocalDateTime(DateTime::current(), TimeZone::utc()).toTimeZone(TimeZone::current());
}

LocalDateTime LocalDateTime::toTimeZone(const TimeZone &timeZone) const {
    return LocalDateTime(this->toUtc().addDuration(timeZone.offsetAt(mDateTime)).dateTime(), timeZone);
}

std::string LocalDateTime::toString(const std::string& format) const {
    if (!isValid())
        return std::string();

    return mTimeZone.toStringAt(mDateTime, mDateTime.toString(format));
}

LocalDateTime LocalDateTime::fromString(const std::string& dateTime, const std::string& format) {
    int _year = 1, _month = 1, _day = 1;
    int _hour = 0, _minute = 0, _second = 0;
    long _subsecond = 0;
    TimeZone tz;

    for (int fmtPos = 0, dtsPos = 0; fmtPos < format.size() && dtsPos < dateTime.size(); ++fmtPos) {
        const int charCount = Utility::countIdenticalCharsFrom(fmtPos, format);

        if (format[fmtPos] == '#') {
            if (dateTime[dtsPos] == '+') {
                _year = 1;
                ++dtsPos;
            } else if (dateTime[dtsPos] == '-') {
                _year = -1;
                ++dtsPos;
            }
        } else if (format[fmtPos] == 'y') {
            if (charCount == 1) {
                _year = _year * Utility::readIntAndAdvancePos(dateTime, dtsPos, 4);
            } else if (charCount == 2) {
                _year = _year * std::stoi(dateTime.substr(dtsPos, charCount));
                _year += 2000;
                dtsPos += charCount;
            } else if (charCount == 4) {
                _year = _year * std::stoi(dateTime.substr(dtsPos, charCount));
                dtsPos += charCount;
            }
            fmtPos += charCount - 1; // skip all identical characters except the last.
        } else if (format[fmtPos] == 'E') {
            if (dateTime.substr(dtsPos, 2) == "CE") {
                _year = std::abs(_year);
                dtsPos += 2;
            } else if (dateTime.substr(dtsPos, 3) == "BCE") {
                _year = -std::abs(_year);
                dtsPos += 3;
            }
        } else if (format[fmtPos] == 'M') {
            if (charCount == 1) {
                _month = Utility::readIntAndAdvancePos(dateTime, dtsPos, 4);
            } else if (charCount == 2) {
                _month = std::stoi(dateTime.substr(dtsPos, charCount));
                dtsPos += charCount;
            } else if (charCount == 3) {
                _month = (std::find(Internal::monthNameArray, Internal::monthNameArray + 11, dateTime.substr(dtsPos, charCount)) - Internal::monthNameArray) + 1;
                dtsPos += charCount;
            } else if (charCount == 4) {
                int newPos = dtsPos;
                while (newPos < dateTime.size() && std::isalpha(dateTime[newPos])) ++newPos;
                _month = (std::find(Internal::monthNameArray + 12, Internal::monthNameArray + 23, dateTime.substr(dtsPos, newPos - dtsPos)) - Internal::monthNameArray) - 11;
                dtsPos = newPos;
            }
            fmtPos += charCount - 1;
        } else if (format[fmtPos] == 'd') {
            if (charCount == 1) {
                _day = Utility::readIntAndAdvancePos(dateTime, dtsPos, 2);
            } else if (charCount == 2) {
                _day = std::stoi(dateTime.substr(dtsPos, charCount));
                dtsPos += charCount;
            } else if (charCount == 3) {
                // lets the format string and the date string be in sync.
                dtsPos += charCount;
            } else if (charCount == 4) {
                while (dtsPos < dateTime.size() && std::isalpha(dateTime[dtsPos])) ++dtsPos;
            }
            fmtPos += charCount - 1;
        } else if (format[fmtPos] == 'h' || format[fmtPos] == 'H') {
            _hour = Utility::readIntAndAdvancePos(dateTime, dtsPos, 2);
            fmtPos += charCount - 1;
        } else if (format[fmtPos] == 'm') {
            _minute = Utility::readIntAndAdvancePos(dateTime, dtsPos, 2);
            fmtPos += charCount - 1;
        } else if (format[fmtPos] == 's') {
            _second = Utility::readIntAndAdvancePos(dateTime, dtsPos, 2);
            fmtPos += charCount - 1;
        } else if (format[fmtPos] == 'f') {
            std::string subsecondString = dateTime.substr(dtsPos, charCount);
            _subsecond = std::stoi(subsecondString.append(9 - subsecondString.size(), '0'));
            dtsPos += charCount;
            fmtPos += charCount - 1;
        } else if (format[fmtPos] == 'a' || format[fmtPos] == 'A') {
            if (dateTime.substr(dtsPos, 2) == "pm" || dateTime.substr(dtsPos, 2) == "PM") {
                _hour = (_hour > 12 ? _hour : _hour + 12);
                dtsPos += 2;
            }
        } else if (format[fmtPos] == 'z' && charCount == 4) {
            int newPos = dtsPos;
            while (newPos < dateTime.size() && (std::isalnum(dateTime[newPos]) || dateTime[newPos] == '/' || dateTime[newPos] == '_' || dateTime[newPos] == '-' || dateTime[newPos] == '+')) ++newPos;
            tz = TimeZone(dateTime.substr(dtsPos, newPos - dtsPos));
            dtsPos = newPos;
            fmtPos += charCount - 1;
        } else {
            // not a pattern, skip it in the date string.
            ++dtsPos;
        }

    }

    return LocalDateTime(DateTime(Date(_year, _month, _day), Time(_hour, _minute, _second, Nanoseconds(_subsecond))), tz);
}

long long LocalDateTime::nanosecondsBetween(const LocalDateTime& from, const LocalDateTime& to) {
    return DateTime::nanosecondsBetween(from.dateTime(), to.dateTime());
}

long long LocalDateTime::microsecondsBetween(const LocalDateTime& from, const LocalDateTime& to) {
    return DateTime::microsecondsBetween(from.dateTime(), to.dateTime());
}

long long LocalDateTime::millisecondsBetween(const LocalDateTime& from, const LocalDateTime& to) {
    return DateTime::millisecondsBetween(from.dateTime(), to.dateTime());
}

long long LocalDateTime::secondsBetween(const LocalDateTime& from, const LocalDateTime& to) {
    return DateTime::secondsBetween(from.dateTime(), to.dateTime());
}

long LocalDateTime::minutesBetween(const LocalDateTime& from, const LocalDateTime& to) {
    return DateTime::minutesBetween(from.dateTime(), to.dateTime());
}

long LocalDateTime::hoursBetween(const LocalDateTime& from, const LocalDateTime& to) {
    return DateTime::hoursBetween(from.dateTime(), to.dateTime());
}

long LocalDateTime::daysBetween(const LocalDateTime& from, const LocalDateTime& to) {
    return DateTime::daysBetween(from.dateTime(), to.dateTime());
}

long LocalDateTime::weeksBetween(const LocalDateTime& from, const LocalDateTime& to) {
    return DateTime::weeksBetween(from.dateTime(), to.dateTime());
}

std::ostream & Salsabil::operator<<(std::ostream& os, const LocalDateTime& ldt) {
    os << ldt.toString("yyyy-MM-ddThh:mm:ss[zzzz]");

    return os;
}

std::istream & Salsabil::operator>>(std::istream& is, LocalDateTime& ldt) {
    std::string str;
    is >> str;
    ldt = LocalDateTime::fromString(str, "yyyy-MM-ddThh:mm:ss[zzzz]");

    return is;
}
