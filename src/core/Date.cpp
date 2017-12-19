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

#include "Date.hpp"
#include <sstream>
#include <iomanip>

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

Date::Days ymdToDays(int year, int month, int day) {
    // Math from the date library at http://github.com/HowardHinnant/date
    auto const y = static_cast<int> (year) - (month <= 2);
    auto const m = static_cast<unsigned> (month);
    auto const d = static_cast<unsigned> (day);
    auto const era = (y >= 0 ? y : y - 399) / 400;
    auto const yoe = static_cast<unsigned> (y - era * 400); // [0, 399]
    auto const doy = (153 * (m > 2 ? m - 3 : m + 9) + 2) / 5 + d - 1; // [0, 365]
    auto const doe = yoe * 365 + yoe / 4 - yoe / 100 + doy; // [0, 146096]

    return Date::Days{era * 146097 + static_cast<long> (doe)};
}

void daysToYmd(Date::Days dys, int* year, int* month, int* day) {
    // Math from the date library at http://github.com/HowardHinnant/date
    auto const z = dys.count();
    auto const era = (z >= 0 ? z : z - 146096) / 146097;
    auto const doe = static_cast<unsigned long> (z - era * 146097); // [0, 146096]
    auto const yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365; // [0, 399]
    auto const y = static_cast<Date::Days::rep> (yoe) + era * 400;
    auto const doy = doe - (365 * yoe + yoe / 4 - yoe / 100); // [0, 365]
    auto const mp = (5 * doy + 2) / 153; // [0, 11]
    auto const m = mp < 10 ? mp + 3 : mp - 9; // [1, 12]
    auto const d = doy - (153 * mp + 2) / 5 + 1; // [1, 31]

    if (year)
        *year = y + (m <= 2);
    if (month)
        *month = m;

    if (day)
        *day = d;
}

Date::Date(int year, int month, int day) : mDateDuration(ymdToDays(year, month, day)) {
}

Date::Date(const Days& days) : mDateDuration(days) {
}

Date::Date() : mDateDuration(std::chrono::duration_cast<Days>(std::chrono::system_clock::now().time_since_epoch()) + Days(719468)) {
}

Date Date::addDays(int days) const {
    return Date(mDateDuration + Days(days));
}

Date Date::subtractDays(int days) const {
    return Date(mDateDuration - Days(days));
}

Date Date::addMonths(int months) const {
    int y, m, d;
    getYearMonthDay(&y, &m, &d);
    const int totalMonths = m + months - 1;
    const int newYear = y + (totalMonths / 12);
    const int newMonth = (totalMonths % 12) + 1;
    const int newDaysInMonth = daysInMonthOfYear(newYear, newMonth);
    const int newDays = newDaysInMonth < d ? newDaysInMonth : d;

    return Date(newYear, newMonth, newDays);
}

Date Date::subtractMonths(int months) const {
    int y, m, d;
    getYearMonthDay(&y, &m, &d);
    const int newYear = y - (std::abs(m - months - 12) / 12);
    const int newMonth = ((11 + m - (months % 12)) % 12) + 1;
    const int newDaysInMonth = daysInMonthOfYear(newYear, newMonth);
    const int newDays = newDaysInMonth < d ? newDaysInMonth : d;

    return Date(newYear, newMonth, newDays);
}

Date Date::addYears(int years) const {
    int y, m, d;
    getYearMonthDay(&y, &m, &d);
    return Date(y + years, m, d);
}

Date Date::subtractYears(int years) const {
    int y, m, d;
    getYearMonthDay(&y, &m, &d);
    return Date(y - years, m, d);
}

void Date::getYearMonthDay(int* year, int* month, int* day) const {
    daysToYmd(mDateDuration, year, month, day);
}

int Date::year() const {
    int y;
    daysToYmd(mDateDuration, &y, nullptr, nullptr);
    return y;
}

int Date::month() const {
    int m;
    daysToYmd(mDateDuration, nullptr, &m, nullptr);
    return m;
}

int Date::day() const {
    int d;
    daysToYmd(mDateDuration, nullptr, nullptr, &d);
    return d;
}

int Date::dayOfWeek() const {
    return (toDays() % 7 + 2) % 7 + 1;
}

int Date::dayOfYear() const {
    return (mDateDuration - ymdToDays(year(), 1, 1) + Days(1)).count();
}

int Date::daysInMonth() const {
    return daysInMonthOfYear(year(), month());
}

int Date::daysInYear() const {
    return (isLeapYear() ? 366 : 365);
}

bool Date::isLeapYear() const {
    return isLeapYear(year());
}

Date getFirstWeekDate(int year) {
    Date d(year, 1, 1);

    return d.addDays((11 - d.dayOfWeek()) % 7 - 3);
}

int Date::weekOfYear(int* weekYear) const {
    int y = year();
    Date currentDate = *this;
    Date firstWeekDate = getFirstWeekDate(y);
    if (currentDate < firstWeekDate) {
        // if the given date is earlier than the start of the first week of the year
        // that contains it, then the date belongs to the last week of the previous year.
        --y;
        firstWeekDate = getFirstWeekDate(y);
    } else {
        Date nextYearFirstWeekDate = getFirstWeekDate(y + 1);
        if (currentDate >= nextYearFirstWeekDate) {
            // if the given date is on or after the start of the first week of
            // the next year, then the date belongs to the first week of the next year.
            ++y;
            firstWeekDate = nextYearFirstWeekDate;
        }
    }

    int week = daysBetween(currentDate, firstWeekDate) / 7 + 1;

    if (weekYear)
        *weekYear = y;

    return week;
}

std::string Date::dayOfWeekName(bool useShortName) const {
    return weekdayNameArray[useShortName ? dayOfWeek() - 1 : dayOfWeek() + 6];
}

std::string Date::monthName(bool useShortName) const {
    return monthNameArray[useShortName ? month() - 1 : month() + 11];
}

bool Date::operator<(const Date& other) const {
    return this->mDateDuration < other.mDateDuration;
}

bool Date::operator<=(const Date& other) const {
    return this->mDateDuration <= other.mDateDuration;
}

bool Date::operator>(const Date& other) const {
    return this->mDateDuration > other.mDateDuration;
}

bool Date::operator>=(const Date& other) const {
    return this->mDateDuration >= other.mDateDuration;
}

bool Date::operator==(const Date& other) const {
    return this->mDateDuration == other.mDateDuration;
}

bool Date::operator!=(const Date& other) const {
    return this->mDateDuration != other.mDateDuration;
}

long Date::toDays() const {
    return mDateDuration.count();
}

long Date::toJulianDay() const {
    return (mDateDuration + Days(1721120)).count();
}

std::string Date::toString(const std::string& format) const {
    std::stringstream output;

    for (int pos = 0; pos < format.size(); ++pos) {
        int y, m, d;
        getYearMonthDay(&y, &m, &d);
        y = std::abs(y);

        char currChar = format[pos];
        const int charCount = Utility::countIdenticalCharsFrom(pos, format);
        pos += charCount - 1; // skip all identical characters except the last.

        if (currChar == '#') {
            output << (mDateDuration.count() < 0 ? "-" : "+");
        } else if (currChar == 'y') {
            if (charCount == 1) {
                output << y;
            } else if (charCount == 2) {
                y = y - ((y / 100) * 100);
                output << std::setfill('0') << std::setw(2) << y;
            } else if (charCount == 4) {
                output << std::setfill('0') << std::setw(4) << y;
            }
        } else if (currChar == 'M') {
            if (charCount == 1) {
                output << m;
            } else if (charCount == 2) {
                output << std::setfill('0') << std::setw(2) << m;
            } else if (charCount == 3) {
                output << monthName(true);
            } else if (charCount == 4) {
                output << monthName(false);
            }
        } else if (currChar == 'd') {
            if (charCount == 1) {
                output << d;
            } else if (charCount == 2) {
                output << std::setfill('0') << std::setw(2) << d;
            } else if (charCount == 3) {
                output << dayOfWeekName(true);
            } else if (charCount == 4) {
                output << dayOfWeekName(false);
            }
        } else if (currChar == 'E') {
            output << (mDateDuration.count() < 0 ? "BCE" : "CE");
        } else {
            output << currChar;
        }
    }

    return output.str();
}

Date Date::fromString(const std::string& dateString, const std::string& format) {
    int y = 1, m = 1, d = 1;

    for (int fmtPos = 0, datPos = 0; fmtPos < format.size() && datPos < dateString.size(); ++fmtPos) {
        const int charCount = Utility::countIdenticalCharsFrom(fmtPos, format);
        fmtPos += charCount - 1; // skip all identical characters except the last.

        if (format[fmtPos] == '#') {
            if (dateString[datPos] == '+') {
                y = 1;
                ++datPos;
            } else if (dateString[datPos] == '-') {
                y = -1;
                ++datPos;
            }
        } else if (format[fmtPos] == 'y') {
            if (charCount == 1) {
                y = y * Utility::readIntAndAdvancePos(dateString, datPos, 4);
            } else if (charCount == 2) {
                y = y * std::stoi(dateString.substr(datPos, charCount));
                y += 2000;
                datPos += charCount;
            } else if (charCount == 4) {
                y = y * std::stoi(dateString.substr(datPos, charCount));
                datPos += charCount;
            }
        } else if (format[fmtPos] == 'E') {
            if (dateString.substr(datPos, 2) == "CE") {
                y = std::abs(y);
                datPos += 2;
            } else if (dateString.substr(datPos, 3) == "BCE") {
                y = -std::abs(y);
                datPos += 3;
            }
        } else if (format[fmtPos] == 'M') {
            if (charCount == 1) {
                m = Utility::readIntAndAdvancePos(dateString, datPos, 4);
            } else if (charCount == 2) {
                m = std::stoi(dateString.substr(datPos, charCount));
                datPos += charCount;
            } else if (charCount == 3) {
                m = (std::find(monthNameArray, monthNameArray + 11, dateString.substr(datPos, charCount)) - monthNameArray) + 1;
                datPos += charCount;
            } else if (charCount == 4) {
                int newPos = datPos;
                while (newPos < dateString.size() && std::isalpha(dateString[newPos])) ++newPos;
                m = (std::find(monthNameArray + 12, monthNameArray + 23, dateString.substr(datPos, newPos - datPos)) - monthNameArray) - 11;
                datPos = newPos;
            }
        } else if (format[fmtPos] == 'd') {
            if (charCount == 1) {
                d = Utility::readIntAndAdvancePos(dateString, datPos, 2);
            } else if (charCount == 2) {
                d = std::stoi(dateString.substr(datPos, charCount));
                datPos += charCount;
            } else if (charCount == 3) {
                // lets the format string and the date string be in sync.
                datPos += charCount;
            } else if (charCount == 4) {
                while (datPos < dateString.size() && std::isalpha(dateString[datPos])) ++datPos;
            }
        } else {
            // not a pattern, skip it in the date string.
            ++datPos;
        }
    }

    return Date(y, m, d);
}

Date Date::fromJulianDay(long julianDay) {
    return Date(Days(julianDay - 1721120));
}

int Date::daysBetween(const Date& from, const Date& to) {
    return std::abs((from.mDateDuration - to.mDateDuration).count());
}

bool Date::isLeapYear(int year) {

    return (year % 4 == 0) && (year % 100 != 0 || year % 400 == 0);
}

int Date::daysInMonthOfYear(int year, int month) {
    switch (month) {

        case 1: return 31;
        case 2: return (isLeapYear(year) ? 29: 28);
        case 3: return 31;
        case 4: return 30;
        case 5: return 31;
        case 6: return 30;
        case 7: return 31;
        case 8: return 31;
        case 9: return 30;
        case 10: return 31;
        case 11: return 30;
        case 12: return 31;
    }

    return -1;
}

std::ostream& Salsabil::operator<<(std::ostream& os, const Date& d) {
    os << d.toString("yyyy-MM-dd");
    return os;
}

std::istream& Salsabil::operator>>(std::istream& is, Date& d) {
    const int DateFormatWidth = 10;
    char result[DateFormatWidth];
    is.read(result, DateFormatWidth);
    std::string str(result, DateFormatWidth);

    d = Date::fromString(str, "yyyy-MM-dd");
}
