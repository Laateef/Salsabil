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
    // Math from http://howardhinnant.github.io/date_algorithms.html
    auto const y = static_cast<int> (year) - (month <= 2) + (year < 1); // if the year is negative; i.e. before common era, add one year.
    auto const m = static_cast<unsigned> (month);
    auto const d = static_cast<unsigned> (day);
    auto const era = (y >= 0 ? y : y - 399) / 400;
    auto const yoe = static_cast<unsigned> (y - era * 400); // [0, 399]
    auto const doy = (153 * (m > 2 ? m - 3 : m + 9) + 2) / 5 + d - 1; // [0, 365]
    auto const doe = yoe * 365 + yoe / 4 - yoe / 100 + doy; // [0, 146096]

    return Date::Days{era * 146097 + static_cast<long> (doe) - 719468};
}

void daysToYmd(Date::Days dys, int* year, int* month, int* day) {
    // Math from http://howardhinnant.github.io/date_algorithms.html
    auto const z = dys.count() + 719468;
    auto const era = (z >= 0 ? z : z - 146096) / 146097;
    auto const doe = static_cast<unsigned long> (z - era * 146097); // [0, 146096]
    auto const yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365; // [0, 399]
    auto const y = static_cast<Date::Days::rep> (yoe) + era * 400;
    auto const doy = doe - (365 * yoe + yoe / 4 - yoe / 100); // [0, 365]
    auto const mp = (5 * doy + 2) / 153; // [0, 11]
    auto const m = mp < 10 ? mp + 3 : mp - 9; // [1, 12]
    auto const d = doy - (153 * mp + 2) / 5 + 1; // [1, 31]
    if (year) {
        *year = y + (m <= 2);
        *year = *year - (*year < 1); // if the year is negative; i.e. before common era, subtract one year.;
    }
    if (month)
        *month = m;
    if (day)
        *day = d;
}

Date::Date(int year, int month, int day) : mYear(year), mMonth(month), mDay(day) {
}

Date::Date(const Days& days) {
    daysToYmd(days, &mYear, &mMonth, &mDay);
}

Date::Date() : mYear(0), mMonth(0), mDay(0) {
}

bool Date::isValid() const {
    return mYear != 0 && (mMonth > 0 && mMonth < 13) && (mDay > 0 && mDay < (daysInMonthOfYear(mYear, mMonth) + 1));
}

Date Date::addDays(int days) const {
    int y, m, d;
    daysToYmd(Days(ymdToDays(mYear, mMonth, mDay) + Days(days)), &y, &m, &d);
    return Date(y, m, d);
}

Date Date::subtractDays(int days) const {
    int y, m, d;
    daysToYmd(Days(ymdToDays(mYear, mMonth, mDay) - Days(days)), &y, &m, &d);
    return Date(y, m, d);
}

Date Date::addMonths(int months) const {
    if (months < 0)
        return subtractMonths(-months);

    const int totalMonths = mMonth + months - 1;
    const int newYear = mYear + (totalMonths / 12);
    const int newMonth = (totalMonths % 12) + 1;
    const int newDaysInMonth = daysInMonthOfYear(newYear, newMonth);
    const int newDays = newDaysInMonth < mDay ? newDaysInMonth : mDay;

    return Date(newYear, newMonth, newDays);
}

Date Date::subtractMonths(int months) const {
    if (months < 0)
        return addMonths(-months);

    const int newYear = mYear - (std::abs(mMonth - months - 12) / 12);
    const int newMonth = ((11 + mMonth - (months % 12)) % 12) + 1;
    const int newDaysInMonth = daysInMonthOfYear(newYear, newMonth);
    const int newDays = newDaysInMonth < mDay ? newDaysInMonth : mDay;

    return Date(newYear, newMonth, newDays);
}

Date Date::addYears(int years) const {
    const int newYear = mYear + years;
    return Date(newYear > 0 ? newYear : newYear - 1, mMonth, mDay);
}

Date Date::subtractYears(int years) const {
    const int newYear = mYear - years;
    return Date(newYear > 0 ? newYear : newYear - 1, mMonth, mDay);
}

Date Date::currentDate() {
    return Date(std::chrono::duration_cast<Days>(std::chrono::system_clock::now().time_since_epoch()));
}

void Date::getYearMonthDay(int* year, int* month, int* day) const {
    if (year)
        *year = mYear;
    if (month)
        *month = mMonth;
    if (day)
        *day = mDay;
}

int Date::year() const {
    return mYear;
}

int Date::month() const {
    return mMonth;
}

int Date::day() const {
    return mDay;
}

int Date::dayOfWeek() const {
    return (toDaysSinceEpoch() % 7 + 3) % 7 + 1;
}

int Date::dayOfYear() const {
    return toDaysSinceEpoch() - ymdToDays(year(), 1, 1).count() + 1;
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

    int week = daysBetween(firstWeekDate, currentDate) / 7 + 1;

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
    return this->year() < other.year() || (this->year() == other.year() && this->month() < other.month()) || (this->year() == other.year() && this->month() == other.month() && this->day() < other.day());
}

bool Date::operator<=(const Date& other) const {
    return this->operator<(other) || this->operator==(other);
}

bool Date::operator>(const Date& other) const {
    return this->year() > other.year() || (this->year() == other.year() && this->month() > other.month()) || (this->year() == other.year() && this->month() == other.month() && this->day() > other.day());
}

bool Date::operator>=(const Date& other) const {
    return this->operator>(other) || this->operator==(other);
}

bool Date::operator==(const Date& other) const {
    return this->year() == other.year() && this->month() == other.month() && this->day() == other.day();
}

bool Date::operator!=(const Date& other) const {
    return this->year() != other.year() || this->month() != other.month() || this->day() != other.day();
}

long Date::toDaysSinceEpoch() const {
    return ymdToDays(year(), month(), day()).count();
}

Date::Days Date::toStdDurationSinceEpoch() const {
    return Days(toDaysSinceEpoch());
}

long Date::toJulianDay() const {
    return toDaysSinceEpoch() + 2440588;
}

std::string Date::toString(const std::string& format) const {
    if (!isValid())
        return std::string();

    std::stringstream output;

    for (int pos = 0; pos < format.size(); ++pos) {
        int y, m, d;
        getYearMonthDay(&y, &m, &d);
        y = std::abs(y);

        char currChar = format[pos];
        const int charCount = Utility::countIdenticalCharsFrom(pos, format);

        if (currChar == '#') {
            output << (year() < 0 ? "-" : "+");
        } else if (currChar == 'y') {
            if (charCount == 1) {
                output << y;
            } else if (charCount == 2) {
                y = y - ((y / 100) * 100);
                output << std::setfill('0') << std::setw(2) << y;
            } else if (charCount == 4) {
                output << std::setfill('0') << std::setw(4) << y;
            }
            pos += charCount - 1; // skip all identical characters except the last.
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
            pos += charCount - 1; // skip all identical characters except the last.
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
            pos += charCount - 1; // skip all identical characters except the last.
        } else if (currChar == 'E') {
            output << (year() < 0 ? "BCE" : "CE");
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
            fmtPos += charCount - 1; // skip all identical characters except the last.
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
            fmtPos += charCount - 1; // skip all identical characters except the last.
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
            fmtPos += charCount - 1; // skip all identical characters except the last.
        } else {
            // not a pattern, skip it in the date string.
            ++datPos;
        }
    }

    return Date(y, m, d);
}

Date Date::fromJulianDay(long julianDay) {
    return Date(Days(julianDay - 2440588));
}

int Date::daysBetween(const Date& from, const Date& to) {
    return to.toDaysSinceEpoch() - from.toDaysSinceEpoch();
}

int Date::weeksBetween(const Date& from, const Date& to) {
    return daysBetween(from, to) / 7;
}

bool Date::isLeapYear(int year) {
    // no year 0 in the Gregorian calendar, the first year before the common era is -1 (year 1 BCE). So, -1, -5, -9 etc are leap years.
    if (year < 1)
        ++year;

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
    return is;
}
