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

#ifndef SALSABIL_DATE_HPP
#define SALSABIL_DATE_HPP

#include <chrono>

namespace Salsabil {

    class Date {
    public:
        using Seconds = std::chrono::seconds;

        using Days = std::chrono::duration
                <long, std::ratio_multiply<std::ratio<24>, std::chrono::hours::period>>;
        using GregorianYears = std::chrono::duration
                <int, std::ratio_multiply<std::ratio<146097, 400>, Days::period>>;
        using JulianYears = std::chrono::duration
                <int, std::ratio_multiply<std::ratio<1461, 4>, Days::period>>;

        using DateDuration = Days;

        static const std::string weekdayNameArray[14];
        static const std::string monthNameArray[24];

        enum class Weekday {
            Monday = 1, Tuesday = 2, Wednesday = 3, Thursday = 4, Friday = 5, Saturday = 6, Sunday = 7
        };

        enum class Month {
            January = 1, February = 2, March = 3, April = 4, May = 5, June = 6, July = 7, August = 8, September = 9, October = 10, November = 11, December = 12
        };

    public:

        Date() : mDateDuration(std::chrono::duration_cast<Days>(std::chrono::system_clock::now().time_since_epoch()) + Days(719468)) {
        }

        Date(DateDuration duration) : mDateDuration(duration) {
        }

        Date(int year, int month, int day) : mDateDuration(ymdToDays(year, month, day)) {
        }

        Date addDays(long days) const {
            return Date(mDateDuration + Days(days));
        }

        Date subtractDays(int days) const {
            return Date(mDateDuration - Days(days));
        }

        Date addMonths(int months) const {
            const int totalMonths = month() + months - 1;
            return Date(year() + (totalMonths / 12), (totalMonths % 12) + 1, day());
        }

        Date subtractMonths(int months) const {
            return Date(year() - (std::abs(month() - months - 12) / 12), ((11 + month() - (months % 12)) % 12) + 1, day());
        }

        Date addYears(int years) {
            return Date(year() + years, month(), day());
        }

        Date subtractYears(int years) {
            return Date(year() - years, month(), day());
        }

        int toDays() const {
            return mDateDuration.count();
        }

        int year() const {
            int y;
            daysToYmd(mDateDuration, &y, nullptr, nullptr);
            return y;
        }

        int month() const {
            int m;
            daysToYmd(mDateDuration, nullptr, &m, nullptr);
            return m;
        }

        int day() const {
            int d;
            daysToYmd(mDateDuration, nullptr, nullptr, &d);
            return d;
        }

        void getYearMonthDay(int* year, int* month, int* day) const {
            daysToYmd(mDateDuration, year, month, day);
        }

        bool operator<(const Date& date) const {
            return this->mDateDuration < date.mDateDuration;
        }

        bool operator<=(const Date& date) const {
            return this->mDateDuration <= date.mDateDuration;
        }

        bool operator>(const Date& date) const {
            return this->mDateDuration > date.mDateDuration;
        }

        bool operator>=(const Date& date) const {
            return this->mDateDuration >= date.mDateDuration;
        }

        bool operator==(const Date& date) const {
            return this->mDateDuration == date.mDateDuration;
        }

        bool operator!=(const Date& date) const {
            return this->mDateDuration != date.mDateDuration;
        }

        Date operator+(const DateDuration& duration) const {
            return Date(this->mDateDuration + duration);
        }

        Date operator-(const DateDuration& duration) const {
            return Date(this->mDateDuration - duration);
        }

        int dayOfWeek() const {
            return (toDays() % 7 + 2) % 7 + 1;
        }

        int dayOfYear() const {
            return (mDateDuration - ymdToDays(year(), 1, 1) + Days(1)).count();
        }

        int daysInMonth() const {
            switch (month()) {
                case 1: return 31;
                case 2: return (isLeapYear(year()) ? 29: 28);
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

        int daysInYear() const {
            return (isLeapYear(year()) ? 366 : 365);
        }

        static int daysBetween(const Date& from, const Date& to) {
            return std::abs((from.mDateDuration - to.mDateDuration).count());
        }

        static bool isLeapYear(int year) {
            return (year % 4 == 0) && (year % 100 != 0 || year % 400 == 0);
        }

        static int leapYearsBefore(int year) {
            --year;
            return (year / 4) - (year / 100) + (year / 400);
        }

        int weekOfYear(int* weekYear) {
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

        /*
  Division, rounding down (rather than towards zero).

  From C++11 onwards, integer division is defined to round towards zero, so we
  can rely on that when implementing this.  This is only used with denominator b
  > 0, so we only have to treat negative numerator, a, specially.
         */
        static inline long long floordiv(long long a, int b) {
            return (a - (a < 0 ? b - 1 : 0)) / b;
        }

        static inline int floordiv(int a, int b) {
            return (a - (a < 0 ? b - 1 : 0)) / b;
        }

        std::string dayOfWeekName(bool useShortName) {
            return weekdayNameArray[useShortName ? dayOfWeek() - 1 : dayOfWeek() + 6];
        }

        std::string monthName(bool useShortName) {
            return monthNameArray[useShortName ? month() - 1 : month() + 11];
        }

        long toJulianDay() const {
            /*
             * Math from Fliegel, H.F., van Flandern, T.C., "A machine algorithm for processing
             * calendar dates", Communications of the ACM, Vol.11(10), pp.657 (1968) 
             */

            int y, m, d;
            getYearMonthDay(&y, &m, &d);
            return d - 32075 + 1461 * (y + 4800 + (m - 14) / 12) / 4 + 367 * (m - 2 - (m - 14) / 12 * 12) / 12 - 3 * ((y + 4900 + (m - 14) / 12) / 100) / 4;
        }

        static Date fromJulianDay(long julianDay) {
            /*
             * Math from Fliegel, H.F., van Flandern, T.C., "A machine algorithm for processing
             * calendar dates", Communications of the ACM, Vol.11(10), pp.657 (1968) 
             */
            auto L = julianDay + 68569;
            auto const N = 4 * L / 146097;
            L = L - (146097 * N + 3) / 4;
            auto I = 4000 * (L + 1) / 1461001;
            L = L - 1461 * I / 4 + 31;
            auto J = 80 * L / 2447;
            auto const K = L - 2447 * J / 80;
            L = J / 11;
            J = J + 2 - 12 * L;
            I = 100 * (N - 49) + I + L;
            return Date(I, J, K);
        }

    private:

        Date getFirstWeekDate(int year) {
            Date d(year, 1, 1);
            return d + Days((11 - d.dayOfWeek()) % 7 - 3);
        }

        Days from_julian_to_days(int year, int month, int day) const {
            // Math from the date library at http://github.com/HowardHinnant/date
            static_assert(std::numeric_limits<unsigned>::digits >= 18,
                    "This algorithm has not been ported to a 16 bit unsigned integer");
            static_assert(std::numeric_limits<int>::digits >= 20,
                    "This algorithm has not been ported to a 16 bit signed integer");
            auto const y = static_cast<int> (year) - (month <= 2);
            auto const m = static_cast<unsigned> (month);
            auto const d = static_cast<unsigned> (day);
            auto const era = (y >= 0 ? y : y - 3) / 4;
            auto const yoe = static_cast<unsigned> (y - era * 4); // [0, 3]
            auto const doy = (153 * (m > 2 ? m - 3 : m + 9) + 2) / 5 + d - 1; // [0, 365]
            auto const doe = yoe * 365 + doy; // [0, 1460]
            return Days{era * 1461 + static_cast<int> (doe) - 719470};
        }

        void from_days_to_julians(Days dp, int* year, int* month, int* day) {
            // Math from the date library at http://github.com/HowardHinnant/date
            static_assert(std::numeric_limits<unsigned>::digits >= 18,
                    "This algorithm has not been ported to a 16 bit unsigned integer");
            static_assert(std::numeric_limits<int>::digits >= 20,
                    "This algorithm has not been ported to a 16 bit signed integer");
            auto const z = dp.count() + 719470;
            auto const era = (z >= 0 ? z : z - 1460) / 1461;
            auto const doe = static_cast<unsigned> (z - era * 1461); // [0, 1460]
            auto const yoe = (doe - doe / 1460) / 365; // [0, 3]
            auto const y = static_cast<Days::rep> (yoe) + era * 4;
            auto const doy = doe - 365 * yoe; // [0, 365]
            auto const mp = (5 * doy + 2) / 153; // [0, 11]
            auto const d = doy - (153 * mp + 2) / 5 + 1; // [1, 31]
            auto const m = mp < 10 ? mp + 3 : mp - 9; // [1, 12]

            if (year)
                *year = y + (m <= 2);
            if (month)
                *month = m;
            if (day)
                *day = d;
        }

        Days ymdToDays(int year, int month, int day) const {
            // Math from the date library at http://github.com/HowardHinnant/date
            auto const y = static_cast<int> (year) - (month <= 2);
            auto const m = static_cast<unsigned> (month);
            auto const d = static_cast<unsigned> (day);
            auto const era = (y >= 0 ? y : y - 399) / 400;
            auto const yoe = static_cast<unsigned> (y - era * 400); // [0, 399]
            auto const doy = (153 * (m > 2 ? m - 3 : m + 9) + 2) / 5 + d - 1; // [0, 365]
            auto const doe = yoe * 365 + yoe / 4 - yoe / 100 + doy; // [0, 146096]
            return Days{era * 146097 + static_cast<long> (doe)};
        }

        void daysToYmd(Days dys, int* year, int* month, int* day) const {
            // Math from the date library at http://github.com/HowardHinnant/date
            auto const z = dys.count();
            auto const era = (z >= 0 ? z : z - 146096) / 146097;
            auto const doe = static_cast<unsigned long> (z - era * 146097); // [0, 146096]
            auto const yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365; // [0, 399]
            auto const y = static_cast<Days::rep> (yoe) + era * 400;
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

        DateDuration mDateDuration;
    };

    std::ostream& operator<<(std::ostream& os, const Date& d) {
        os << std::setfill('0') << std::setw(4) << d.year() << "-" << std::setw(2) << d.month() << "-" << d.day();
        return os;
    }

    const std::string Date::weekdayNameArray[] = {
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

    const std::string Date::monthNameArray[] = {
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

}
#endif // SALSABIL_DATE_HPP 

