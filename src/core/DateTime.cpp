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

#include "DateTime.hpp"
#include "date/include/date/tz.h"
#include <iostream>

using namespace Salsabil;

const std::string weekdayNameArray[] = {
    "Mon",
    "Tue",
    "Wed",
    "Thu",
    "Fri",
    "Sat",
    "Sun",

    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday",
    "Sunday"
};

const std::string monthNameArray[] = {
    "Jan",
    "Feb",
    "Mar",
    "Apr",
    "May",
    "Jun",
    "Jul",
    "Aug",
    "Sep",
    "Oct",
    "Nov",
    "Dec",

    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"
};

DateTime::DateTime() : mDate(0, 0, 0), mTime(Nanoseconds::zero()) {
}

DateTime::DateTime(const Date& date, const Time& time) : mDate(date), mTime(time) {
}

DateTime::DateTime(const Date& date) : mDate(date), mTime(Nanoseconds::zero()) {
}

DateTime DateTime::currentDateTime() {
    return DateTime(Date::currentDate(), Time::currentTime());
}

bool DateTime::isValid() const {
    return mDate.isValid() && mTime.isValid();
}

Date DateTime::date() const {
    return mDate;
}

Time DateTime::time() const {
    return mTime;
}

void DateTime::getYearMonthDay(int* year, int* month, int* day) const {
    return mDate.getYearMonthDay(year, month, day);
}

int DateTime::year() const {
    return mDate.year();
}

int DateTime::month() const {
    return mDate.month();
}

int DateTime::day() const {
    return mDate.day();
}

int DateTime::hour() const {
    return mTime.hour();
}

int DateTime::minute() const {
    return mTime.minute();
}

int DateTime::second() const {
    return mTime.second();
}

int DateTime::millisecond() const {
    return mTime.millisecond();
}

long DateTime::microsecond() const {
    return mTime.microsecond();
}

long DateTime::nanosecond() const {
    return mTime.nanosecond();
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
    const Duration totalDuration = Nanoseconds(mTime.toNanoseconds()) + duration;
    return DateTime(mDate.addDays(std::chrono::duration_cast<Days>(totalDuration).count()), Time(totalDuration % Days(1)));
}

DateTime DateTime::subtractDuration(const Duration& duration) const {
    return DateTime(mDate.subtractDays(std::abs(std::chrono::duration_cast<Days>(Nanoseconds(mTime.toNanoseconds()) - duration - Days(1)).count())), Time((Days(1) + Nanoseconds(mTime.toNanoseconds()) - (duration % Days(1))) % Days(1)));
}

DateTime DateTime::addHours(int hours) const {
    return addDuration(Hours(hours));
}

DateTime DateTime::subtractHours(int hours) const {
    return subtractDuration(Hours(hours));
}

DateTime DateTime::addMinutes(int minutes) const {
    return addDuration(Minutes(minutes));
}

DateTime DateTime::subtractMinutes(int minutes) const {
    return subtractDuration(Minutes(minutes));
}

DateTime DateTime::addSeconds(int seconds) const {
    return addDuration(Seconds(seconds));
}

DateTime DateTime::subtractSeconds(int seconds) const {
    return subtractDuration(Seconds(seconds));
}

DateTime DateTime::addMilliseconds(int milliseconds) const {
    return addDuration(Milliseconds(milliseconds));
}

DateTime DateTime::subtractMilliseconds(int milliseconds) const {
    return subtractDuration(Milliseconds(milliseconds));
}

DateTime DateTime::addMicroseconds(int microseconds) const {
    return addDuration(Microseconds(microseconds));
}

DateTime DateTime::subtractMicroseconds(int microseconds) const {
    return subtractDuration(Microseconds(microseconds));
}

DateTime DateTime::addNanoseconds(int nanoseconds) const {
    return addDuration(Nanoseconds(nanoseconds));
}

DateTime DateTime::subtractNanoseconds(int nanoseconds) const {
    return subtractDuration(Nanoseconds(nanoseconds));
}

DateTime::Nanoseconds DateTime::operator-(const DateTime& datetime) const {
    return this->toStdDurationSinceEpoch() - datetime.toStdDurationSinceEpoch();
}

DateTime DateTime::operator-(const Duration& duration) const {
    return subtractDuration(duration);
}

DateTime DateTime::operator+(const Duration& duration) const {
    return addDuration(duration);
}

double DateTime::toJulianDay() const {
    return mDate.toDaysSinceEpoch() + 2440587.5 + static_cast<double> (mTime.toNanoseconds()) / Nanoseconds(Days(1)).count();
}

long DateTime::toDaysSinceEpoch() const {
    return mDate.toDaysSinceEpoch();
}

long DateTime::toHoursSinceEpoch() const {
    return (std::chrono::duration_cast<Hours>(mDate.toStdDurationSinceEpoch() + mTime.toStdDuration())).count();
}

long DateTime::toMinutesSinceEpoch() const {
    return (std::chrono::duration_cast<Minutes>(mDate.toStdDurationSinceEpoch() + mTime.toStdDuration())).count();
}

long long DateTime::toSecondsSinceEpoch() const {
    return (std::chrono::duration_cast<Seconds>(mDate.toStdDurationSinceEpoch() + mTime.toStdDuration())).count();
}

long long DateTime::toMillisecondsSinceEpoch() const {

    return (std::chrono::duration_cast<Milliseconds>(mDate.toStdDurationSinceEpoch() + mTime.toStdDuration())).count();
}

long long DateTime::toMicrosecondsSinceEpoch() const {

    return (std::chrono::duration_cast<Microseconds>(mDate.toStdDurationSinceEpoch() + mTime.toStdDuration())).count();
}

DateTime::Microseconds DateTime::toStdDurationSinceEpoch() const {

    return std::chrono::duration_cast<Microseconds>(mDate.toStdDurationSinceEpoch() + mTime.toStdDuration());
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

DateTime DateTime::fromString(const std::string& datetimeString, const std::string& format) {
    int y = 1, m = 1, d = 1;
    int _hours = 0, _minutes = 0, _seconds = 0;
    long _subseconds = 0;


    for (int fmtPos = 0, dtsPos = 0; fmtPos < format.size() && dtsPos < datetimeString.size(); ++fmtPos) {
        const int charCount = Utility::countIdenticalCharsFrom(fmtPos, format);

        if (format[fmtPos] == '#') {
            if (datetimeString[dtsPos] == '+') {
                y = 1;
                ++dtsPos;
            } else if (datetimeString[dtsPos] == '-') {
                y = -1;
                ++dtsPos;
            }
        } else if (format[fmtPos] == 'y') {
            if (charCount == 1) {
                y = y * Utility::readIntAndAdvancePos(datetimeString, dtsPos, 4);
            } else if (charCount == 2) {
                y = y * std::stoi(datetimeString.substr(dtsPos, charCount));
                y += 2000;
                dtsPos += charCount;
            } else if (charCount == 4) {
                y = y * std::stoi(datetimeString.substr(dtsPos, charCount));
                dtsPos += charCount;
            }
            fmtPos += charCount - 1; // skip all identical characters except the last.
        } else if (format[fmtPos] == 'E') {
            if (datetimeString.substr(dtsPos, 2) == "CE") {
                y = std::abs(y);
                dtsPos += 2;
            } else if (datetimeString.substr(dtsPos, 3) == "BCE") {
                y = -std::abs(y);
                dtsPos += 3;
            }
        } else if (format[fmtPos] == 'M') {
            if (charCount == 1) {
                m = Utility::readIntAndAdvancePos(datetimeString, dtsPos, 4);
            } else if (charCount == 2) {
                m = std::stoi(datetimeString.substr(dtsPos, charCount));
                dtsPos += charCount;
            } else if (charCount == 3) {
                m = (std::find(monthNameArray, monthNameArray + 11, datetimeString.substr(dtsPos, charCount)) - monthNameArray) + 1;
                dtsPos += charCount;
            } else if (charCount == 4) {
                int newPos = dtsPos;
                while (newPos < datetimeString.size() && std::isalpha(datetimeString[newPos])) ++newPos;
                m = (std::find(monthNameArray + 12, monthNameArray + 23, datetimeString.substr(dtsPos, newPos - dtsPos)) - monthNameArray) - 11;
                dtsPos = newPos;
            }
            fmtPos += charCount - 1; // skip all identical characters except the last.
        } else if (format[fmtPos] == 'd') {
            if (charCount == 1) {
                d = Utility::readIntAndAdvancePos(datetimeString, dtsPos, 2);
            } else if (charCount == 2) {
                d = std::stoi(datetimeString.substr(dtsPos, charCount));
                dtsPos += charCount;
            } else if (charCount == 3) {
                // lets the format string and the date string be in sync.
                dtsPos += charCount;
            } else if (charCount == 4) {
                while (dtsPos < datetimeString.size() && std::isalpha(datetimeString[dtsPos])) ++dtsPos;
            }
            fmtPos += charCount - 1; // skip all identical characters except the last.
        } else if (format[fmtPos] == 'h' || format[fmtPos] == 'H') {
            _hours = Utility::readIntAndAdvancePos(datetimeString, dtsPos, 2);
            fmtPos += charCount - 1; // skip all identical characters except the last.
        } else if (format[fmtPos] == 'm') {
            _minutes = Utility::readIntAndAdvancePos(datetimeString, dtsPos, 2);
            fmtPos += charCount - 1; // skip all identical characters except the last.
        } else if (format[fmtPos] == 's') {
            _seconds = Utility::readIntAndAdvancePos(datetimeString, dtsPos, 2);
            fmtPos += charCount - 1; // skip all identical characters except the last.
        } else if (format[fmtPos] == 'f') {
            std::string subsecondString = datetimeString.substr(dtsPos, charCount);
            _subseconds = std::stoi(subsecondString.append(9 - subsecondString.size(), '0'));
            dtsPos += charCount;
            fmtPos += charCount - 1; // skip all identical characters except the last.
        } else if (format[fmtPos] == 'a' || format[fmtPos] == 'A') {
            if (datetimeString.substr(dtsPos, 2) == "pm" || datetimeString.substr(dtsPos, 2) == "PM") {
                _hours = (_hours > 12 ? _hours : _hours + 12);
                dtsPos += 2;
            }
        } else {
            // not a pattern, skip it in the date string.
            ++dtsPos;
        }
    }

    return DateTime(Date(y, m, d), Time(_hours, _minutes, _seconds, Nanoseconds(_subseconds)));
}

DateTime DateTime::fromJulianDay(double julianDay) {
    const long integer = static_cast<long> (julianDay);
    const double fractional = julianDay - integer;
    const long millisecondCount = Milliseconds(Days(1)).count() * fractional;

    return DateTime(Date(Days(integer - 2440587))).subtractHours(12).addDuration(Milliseconds(millisecondCount));
}

long long DateTime::daysBetween(const DateTime& from, const DateTime & to) {

    return std::abs(std::chrono::duration_cast<Days>(from.toStdDurationSinceEpoch() - to.toStdDurationSinceEpoch()).count());
}

long long DateTime::hoursBetween(const DateTime& from, const DateTime & to) {

    return std::abs(std::chrono::duration_cast<Hours>(from.toStdDurationSinceEpoch() - to.toStdDurationSinceEpoch()).count());
}

long long DateTime::minutesBetween(const DateTime& from, const DateTime & to) {

    return std::abs(std::chrono::duration_cast<Minutes>(from.toStdDurationSinceEpoch() - to.toStdDurationSinceEpoch()).count());
}

long long DateTime::secondsBetween(const DateTime& from, const DateTime & to) {

    return std::abs(std::chrono::duration_cast<Seconds>(from.toStdDurationSinceEpoch() - to.toStdDurationSinceEpoch()).count());
}

long long DateTime::millisecondsBetween(const DateTime& from, const DateTime & to) {

    return std::abs(std::chrono::duration_cast<Milliseconds>(from.toStdDurationSinceEpoch() - to.toStdDurationSinceEpoch()).count());
}

long long DateTime::microsecondsBetween(const DateTime& from, const DateTime & to) {

    return std::abs(std::chrono::duration_cast<Microseconds>(from.toStdDurationSinceEpoch() - to.toStdDurationSinceEpoch()).count());
}

bool DateTime::isLeapYear(int year) {

    return Date::isLeapYear(year);
}

int DateTime::daysInMonthOfYear(int year, int month) {

    return Date::daysInMonthOfYear(year, month);
}

std::ostream & Salsabil::operator<<(std::ostream& os, const DateTime & dt) {
    os << dt.date().toString("yyyy-MM-dd") << 'T' << dt.time().toString("hh:mm:ss.fff");

    return os;
}

std::istream & Salsabil::operator>>(std::istream& is, DateTime & dt) {
    const int DateTimeFormatWidth = 23;
    char result[DateTimeFormatWidth];
    is.read(result, DateTimeFormatWidth);
    std::string str(result, DateTimeFormatWidth);

    dt = DateTime(Date::fromString(str.substr(0, 10), "yyyy-MM-dd"), Time::fromString(str.substr(11, 12), "hh:mm:ss.fff"));
    return is;
}
