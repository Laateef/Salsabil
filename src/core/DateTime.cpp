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

#include "DateTime.hpp"
#include "Definitions.hpp"
#include "date/include/date/tz.h"
#include <iostream>

using namespace Salsabil;

DateTime::DateTime() : mDate(), mTime() {
}

DateTime::DateTime(const DateTime& other) : mDate(other.mDate), mTime(other.mTime) {
}

DateTime::DateTime(DateTime&& other) : mDate(std::move(other.mDate)), mTime(std::move(other.mTime)) {
}

DateTime::DateTime(const Duration& duration) : mDate(std::chrono::duration_cast<Days>(duration)), mTime(duration % Days(1)) {
}

DateTime::DateTime(const std::chrono::system_clock::time_point& timePoint)
: DateTime(timePoint.time_since_epoch()) {
}

DateTime::DateTime(const Date& date) : mDate(date), mTime(Nanoseconds::zero()) {
}

DateTime::DateTime(const Date& date, const Time& time) : mDate(date), mTime(time) {
}

DateTime& DateTime::operator=(const DateTime& other) {
    if (*this != other) {
        mDate = other.mDate;
        mTime = other.mTime;
    }
    return *this;
}

DateTime& DateTime::operator=(DateTime&& other) {
    if (*this != other) {
        mDate = std::move(other.mDate);
        mTime = std::move(other.mTime);
    }
    return *this;
}

bool DateTime::operator<(const DateTime& other) const {
    return (this->mDate < other.mDate) || (this->mDate == other.mDate && this->mTime < other.mTime);
}

bool DateTime::operator<=(const DateTime& other) const {
    return (this->mDate <= other.mDate) || (this->mDate == other.mDate && this->mTime <= other.mTime);
}

bool DateTime::operator>(const DateTime& other) const {
    return (this->mDate > other.mDate) || (this->mDate == other.mDate && this->mTime > other.mTime);
}

bool DateTime::operator>=(const DateTime& other) const {
    return (this->mDate >= other.mDate) || (this->mDate == other.mDate && this->mTime >= other.mTime);
}

bool DateTime::operator==(const DateTime& other) const {
    return this->mDate == other.mDate && this->mTime == other.mTime;
}

bool DateTime::operator!=(const DateTime& other) const {
    return this->mDate != other.mDate || this->mTime != other.mTime;
}

DateTime::Nanoseconds DateTime::operator-(const DateTime& other) const {
    return this->toStdDurationSinceEpoch() - other.toStdDurationSinceEpoch();
}

DateTime DateTime::operator-(const Duration& duration) const {
    return subtractDuration(duration);
}

DateTime DateTime::operator+(const Duration& duration) const {
    return addDuration(duration);
}

Date DateTime::date() const {
    return mDate;
}

Time DateTime::time() const {
    return mTime;
}

bool DateTime::isValid() const {
    return mDate.isValid() && mTime.isValid();
}

long DateTime::nanosecond() const {
    return mTime.nanosecond();
}

long DateTime::microsecond() const {
    return mTime.microsecond();
}

int DateTime::millisecond() const {
    return mTime.millisecond();
}

int DateTime::second() const {
    return mTime.second();
}

int DateTime::minute() const {
    return mTime.minute();
}

int DateTime::hour() const {
    return mTime.hour();
}

int DateTime::day() const {
    return mDate.day();
}

int DateTime::month() const {
    return mDate.month();
}

int DateTime::year() const {
    return mDate.year();
}

void DateTime::getYearMonthDay(int* year, int* month, int* day) const {
    return mDate.getYearMonthDay(year, month, day);
}

int DateTime::dayOfWeek() const {
    return mDate.dayOfWeek();
}

int DateTime::dayOfYear() const {
    return mDate.dayOfYear();
}

int DateTime::daysInMonth() const {
    return mDate.daysInMonth();
}

int DateTime::daysInYear() const {
    return mDate.daysInYear();
}

bool DateTime::isLeapYear() const {
    return mDate.isLeapYear();
}

int DateTime::weekOfYear(int* weekYear) const {
    return mDate.weekOfYear(weekYear);
}

std::string DateTime::dayOfWeekName(bool useShortName) const {
    return mDate.dayOfWeekName(useShortName);
}

std::string DateTime::monthName(bool useShortName) const {
    return mDate.monthName(useShortName);
}

DateTime DateTime::addNanoseconds(int nanoseconds) const {
    return addDuration(Nanoseconds(nanoseconds));
}

DateTime DateTime::subtractNanoseconds(int nanoseconds) const {
    return subtractDuration(Nanoseconds(nanoseconds));
}

DateTime DateTime::addMicroseconds(int microseconds) const {
    return addDuration(Microseconds(microseconds));
}

DateTime DateTime::subtractMicroseconds(int microseconds) const {
    return subtractDuration(Microseconds(microseconds));
}

DateTime DateTime::addMilliseconds(int milliseconds) const {
    return addDuration(Milliseconds(milliseconds));
}

DateTime DateTime::subtractMilliseconds(int milliseconds) const {
    return subtractDuration(Milliseconds(milliseconds));
}

DateTime DateTime::addSeconds(int seconds) const {
    return addDuration(Seconds(seconds));
}

DateTime DateTime::subtractSeconds(int seconds) const {
    return subtractDuration(Seconds(seconds));
}

DateTime DateTime::addMinutes(int minutes) const {
    return addDuration(Minutes(minutes));
}

DateTime DateTime::subtractMinutes(int minutes) const {
    return subtractDuration(Minutes(minutes));
}

DateTime DateTime::addHours(int hours) const {
    return addDuration(Hours(hours));
}

DateTime DateTime::subtractHours(int hours) const {
    return subtractDuration(Hours(hours));
}

DateTime DateTime::addDays(int days) const {
    return DateTime(mDate.addDays(days), mTime);
}

DateTime DateTime::subtractDays(int days) const {
    return DateTime(mDate.subtractDays(days), mTime);
}

DateTime DateTime::addMonths(int months) const {
    return DateTime(mDate.subtractMonths(months), mTime);
}

DateTime DateTime::subtractMonths(int months) const {
    return DateTime(mDate.subtractMonths(months), mTime);
}

DateTime DateTime::addYears(int years) const {
    return DateTime(mDate.addYears(years), mTime);
}

DateTime DateTime::subtractYears(int years) const {
    return DateTime(mDate.subtractYears(years), mTime);
}

DateTime DateTime::addDuration(const Duration& duration) const {
    if (duration.count() < 0)
        return subtractDuration(-duration);

    const Duration totalDuration = Nanoseconds(mTime.toNanosecondsSinceMidnight()) + duration;
    return DateTime(mDate.addDays(std::chrono::duration_cast<Days>(totalDuration).count()), Time(totalDuration % Days(1)));
}

DateTime DateTime::subtractDuration(const Duration& duration) const {
    if (duration.count() < 0)
        return addDuration(-duration);

    return DateTime(mDate.subtractDays(std::abs(std::chrono::duration_cast<Days>(Nanoseconds(mTime.toNanosecondsSinceMidnight()) - duration - Days(1)).count())), Time((Days(1) + Nanoseconds(mTime.toNanosecondsSinceMidnight()) - (duration % Days(1))) % Days(1)));
}

double DateTime::toJulianDay() const {
    return mDate.toDaysSinceEpoch() + 2440587.5 + static_cast<double> (mTime.toNanosecondsSinceMidnight()) / Nanoseconds(Days(1)).count();
}

long long DateTime::toNanosecondsSinceEpoch() const {
    return (std::chrono::duration_cast<Nanoseconds>(mDate.toStdDurationSinceEpoch() + mTime.toStdDurationSinceMidnight())).count();
}

long long DateTime::toMicrosecondsSinceEpoch() const {
    return (std::chrono::duration_cast<Microseconds>(mDate.toStdDurationSinceEpoch() + mTime.toStdDurationSinceMidnight())).count();
}

long long DateTime::toMillisecondsSinceEpoch() const {
    return (std::chrono::duration_cast<Milliseconds>(mDate.toStdDurationSinceEpoch() + mTime.toStdDurationSinceMidnight())).count();
}

long long DateTime::toSecondsSinceEpoch() const {
    return (std::chrono::duration_cast<Seconds>(mDate.toStdDurationSinceEpoch() + mTime.toStdDurationSinceMidnight())).count();
}

long DateTime::toMinutesSinceEpoch() const {
    return (std::chrono::duration_cast<Minutes>(mDate.toStdDurationSinceEpoch() + mTime.toStdDurationSinceMidnight())).count();
}

long DateTime::toHoursSinceEpoch() const {
    return (std::chrono::duration_cast<Hours>(mDate.toStdDurationSinceEpoch() + mTime.toStdDurationSinceMidnight())).count();
}

long DateTime::toDaysSinceEpoch() const {
    return mDate.toDaysSinceEpoch();
}

DateTime::Microseconds DateTime::toStdDurationSinceEpoch() const {

    return std::chrono::duration_cast<Microseconds>(mDate.toStdDurationSinceEpoch() + mTime.toStdDurationSinceMidnight());
}

std::chrono::system_clock::time_point DateTime::toStdTimePoint() const {

    return std::chrono::system_clock::time_point(toStdDurationSinceEpoch());
}

std::tm DateTime::toBrokenStdTime() const {
    int y, m, d;
    getYearMonthDay(&y, &m, &d);
    std::tm cTime = {0};
    cTime.tm_year = y - 1970;
    cTime.tm_year = m;
    cTime.tm_year = d;
    cTime.tm_hour = hour();
    cTime.tm_min = minute();
    cTime.tm_sec = second();

    return cTime;
}

std::time_t DateTime::toScalarStdTime() const {

    return std::time_t(toSecondsSinceEpoch());
}

std::string DateTime::toString(const std::string& format) const {
    if (!isValid())
        return std::string();

    return mDate.toString(mTime.toString(format));
}

DateTime DateTime::current() {
    return DateTime(Date::current(), Time::current());
}

DateTime DateTime::epoch() {
    return DateTime(Date::epoch(), Time::midnight());
}

DateTime DateTime::fromString(const std::string& datetimeString, const std::string& format) {
    int _year = 1, _month = 1, _day = 1;
    int _hour = 0, _minute = 0, _second = 0;
    long _subsecond = 0;

    for (int fmtPos = 0, dtsPos = 0; fmtPos < format.size() && dtsPos < datetimeString.size(); ++fmtPos) {
        const int charCount = Utility::countIdenticalCharsFrom(fmtPos, format);

        if (format[fmtPos] == '#') {
            if (datetimeString[dtsPos] == '+') {
                _year = 1;
                ++dtsPos;
            } else if (datetimeString[dtsPos] == '-') {
                _year = -1;
                ++dtsPos;
            }
        } else if (format[fmtPos] == 'y') {
            if (charCount == 1) {
                _year = _year * Utility::readIntAndAdvancePos(datetimeString, dtsPos, 4);
            } else if (charCount == 2) {
                _year = _year * std::stoi(datetimeString.substr(dtsPos, charCount));
                _year += 2000;
                dtsPos += charCount;
            } else if (charCount == 4) {
                _year = _year * std::stoi(datetimeString.substr(dtsPos, charCount));
                dtsPos += charCount;
            }
            fmtPos += charCount - 1; // skip all identical characters except the last.
        } else if (format[fmtPos] == 'E') {
            if (datetimeString.substr(dtsPos, 2) == "CE") {
                _year = std::abs(_year);
                dtsPos += 2;
            } else if (datetimeString.substr(dtsPos, 3) == "BCE") {
                _year = -std::abs(_year);
                dtsPos += 3;
            }
        } else if (format[fmtPos] == 'M') {
            if (charCount == 1) {
                _month = Utility::readIntAndAdvancePos(datetimeString, dtsPos, 4);
            } else if (charCount == 2) {
                _month = std::stoi(datetimeString.substr(dtsPos, charCount));
                dtsPos += charCount;
            } else if (charCount == 3) {
                _month = (std::find(Internal::monthNameArray, Internal::monthNameArray + 11, datetimeString.substr(dtsPos, charCount)) - Internal::monthNameArray) + 1;
                dtsPos += charCount;
            } else if (charCount == 4) {
                int newPos = dtsPos;
                while (newPos < datetimeString.size() && std::isalpha(datetimeString[newPos])) ++newPos;
                _month = (std::find(Internal::monthNameArray + 12, Internal::monthNameArray + 23, datetimeString.substr(dtsPos, newPos - dtsPos)) - Internal::monthNameArray) - 11;
                dtsPos = newPos;
            }
            fmtPos += charCount - 1; // skip all identical characters except the last.
        } else if (format[fmtPos] == 'd') {
            if (charCount == 1) {
                _day = Utility::readIntAndAdvancePos(datetimeString, dtsPos, 2);
            } else if (charCount == 2) {
                _day = std::stoi(datetimeString.substr(dtsPos, charCount));
                dtsPos += charCount;
            } else if (charCount == 3) {
                // lets the format string and the date string be in sync.
                dtsPos += charCount;
            } else if (charCount == 4) {
                while (dtsPos < datetimeString.size() && std::isalpha(datetimeString[dtsPos])) ++dtsPos;
            }
            fmtPos += charCount - 1; // skip all identical characters except the last.
        } else if (format[fmtPos] == 'h' || format[fmtPos] == 'H') {
            _hour = Utility::readIntAndAdvancePos(datetimeString, dtsPos, 2);
            fmtPos += charCount - 1; // skip all identical characters except the last.
        } else if (format[fmtPos] == 'm') {
            _minute = Utility::readIntAndAdvancePos(datetimeString, dtsPos, 2);
            fmtPos += charCount - 1; // skip all identical characters except the last.
        } else if (format[fmtPos] == 's') {
            _second = Utility::readIntAndAdvancePos(datetimeString, dtsPos, 2);
            fmtPos += charCount - 1; // skip all identical characters except the last.
        } else if (format[fmtPos] == 'f') {
            std::string subsecondString = datetimeString.substr(dtsPos, charCount);
            _subsecond = std::stoi(subsecondString.append(9 - subsecondString.size(), '0'));
            dtsPos += charCount;
            fmtPos += charCount - 1; // skip all identical characters except the last.
        } else if (format[fmtPos] == 'a' || format[fmtPos] == 'A') {
            if (datetimeString.substr(dtsPos, 2) == "pm" || datetimeString.substr(dtsPos, 2) == "PM") {
                _hour = (_hour > 12 ? _hour : _hour + 12);
                dtsPos += 2;
            }
        } else {
            // not a pattern, skip it in the date string.
            ++dtsPos;
        }
    }

    return DateTime(Date(_year, _month, _day), Time(_hour, _minute, _second, Nanoseconds(_subsecond)));
}

DateTime DateTime::fromJulianDay(double julianDay) {
    const long integer = static_cast<long> (julianDay);
    const double fractional = julianDay - integer;
    const long millisecondCount = Milliseconds(Days(1)).count() * fractional;

    return DateTime(Date(Days(integer - 2440587))).subtractHours(12).addDuration(Milliseconds(millisecondCount));
}

long long DateTime::nanosecondsBetween(const DateTime& from, const DateTime & to) {
    return std::abs(std::chrono::duration_cast<Nanoseconds>((from.mDate.toStdDurationSinceEpoch() + from.mTime.toStdDurationSinceMidnight()) - (to.mDate.toStdDurationSinceEpoch() + to.mTime.toStdDurationSinceMidnight())).count());
}

long long DateTime::microsecondsBetween(const DateTime& from, const DateTime & to) {
    return std::abs(std::chrono::duration_cast<Microseconds>(from.toStdDurationSinceEpoch() - to.toStdDurationSinceEpoch()).count());
}

long long DateTime::millisecondsBetween(const DateTime& from, const DateTime & to) {
    return std::abs(std::chrono::duration_cast<Milliseconds>(from.toStdDurationSinceEpoch() - to.toStdDurationSinceEpoch()).count());
}

long long DateTime::secondsBetween(const DateTime& from, const DateTime & to) {
    return std::abs(std::chrono::duration_cast<Seconds>(from.toStdDurationSinceEpoch() - to.toStdDurationSinceEpoch()).count());
}

long DateTime::minutesBetween(const DateTime& from, const DateTime & to) {
    return std::abs(std::chrono::duration_cast<Minutes>(from.toStdDurationSinceEpoch() - to.toStdDurationSinceEpoch()).count());
}

long DateTime::hoursBetween(const DateTime& from, const DateTime & to) {
    return std::abs(std::chrono::duration_cast<Hours>(from.toStdDurationSinceEpoch() - to.toStdDurationSinceEpoch()).count());
}

long DateTime::daysBetween(const DateTime& from, const DateTime & to) {
    return std::abs(std::chrono::duration_cast<Days>(from.toStdDurationSinceEpoch() - to.toStdDurationSinceEpoch()).count());
}

long DateTime::weeksBetween(const DateTime& from, const DateTime & to) {
    return std::abs(std::chrono::duration_cast<Weeks>(from.toStdDurationSinceEpoch() - to.toStdDurationSinceEpoch()).count());
}

std::ostream & Salsabil::operator<<(std::ostream& os, const DateTime & dt) {
    os << dt.toString("yyyy-MM-ddThh:mm:ss.fff");

    return os;
}

std::istream & Salsabil::operator>>(std::istream& is, DateTime & dt) {
    const int DateTimeFormatWidth = 23;
    char result[DateTimeFormatWidth];
    is.read(result, DateTimeFormatWidth);
    dt = DateTime::fromString(std::string(result, DateTimeFormatWidth), "yyyy-MM-ddThh:mm:ss.fff");

    return is;
}
