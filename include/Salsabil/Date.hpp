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
#include <string>
#include <algorithm>

namespace Salsabil {

    /**
     * @class Date
     * @brief Date is an immutable class representing a date without a time-zone in the ISO-8601 calendar system, such as 2017-12-15.
     * 
     * The ISO-8601 calendar system is the modern civil calendar system used today in most of the world. It is equivalent to the proleptic Gregorian calendar system. 
     * Date describes the date as the number of days since year 1 CE. 
     * 
     * A default-constructed Date objects is initialized to the current date from the system clock. 
     * Date objects can be created by giving the number of years, months and days, explicitly. Also, it can be created from the number of days since year 1 CE.
     * 
     * The year, month and day of the date can be accessed though the functions year(), month() and day(), respectively.
     * Other date fields, such as day-of-year, day-of-week and week-of-year, can also be accessed through dayOfYear(), dayOfWeek() and weekOfYear(), respectively.
     * 
     * Date provides functions for manipulating dates. Years, months, and days can be added to a date (through addYears(), addMonths(), and addDays(), respectively) or subtracted from it (through subtractYears(), subtractMonths(), and subtractDays(), respectively).
     * It also provides functions for comparing dates. Date A is considered earlier than Date B if A is smaller than B, and so on.
     * the function daysBetween() returns how many days between two dates.
     * 
     * The toString() function can be used to get a textual representation of the date formatted according to a given formatter string.
     */

    class Date {
    public:
        /** @name Aliases */
        //@{
        using Days = std::chrono::duration
                <long, std::ratio_multiply<std::ratio<24>, std::chrono::hours::period>>;
        //@}

        /** @name Enumerations */
        //@{

        enum class Weekday {
            Monday = 1,
            Tuesday = 2,
            Wednesday = 3,
            Thursday = 4,
            Friday = 5,
            Saturday = 6,
            Sunday = 7
        };

        enum class Month {
            January = 1,
            February = 2,
            March = 3,
            April = 4,
            May = 5,
            June = 6,
            July = 7,
            August = 8,
            September = 9,
            October = 10,
            November = 11,
            December = 12
        };
        //@}

    public:
        /** @name Constructors and Destructors */
        //@{

        /** @brief Constructs a Date object from the given <i>year</i>, <i>month</i> and <i>day</i>. */
        Date(int year, int month, int day) : mDateDuration(ymdToDays(year, month, day)) {
        }

        /** @brief Constructs a Date object from the given number of days <i>days</i>. */
        Date(const Days& days) : mDateDuration(days) {
        }

        /** @brief Copy-constructs a Date object from the given <i>date</i> object. */
        Date(const Date& date) = default;

        /** @brief Move-constructs a Date object from the given <i>date</i> object. */
        Date(Date&& date) = default;

        /** @brief Default constructor. Constructs a Date object and initializes it to the current date obtained from the system clock. */
        Date() : mDateDuration(std::chrono::duration_cast<Days>(std::chrono::system_clock::now().time_since_epoch()) + Days(719468)) {
        }

        /** @brief Default destructor. */
        virtual ~Date() {
        }
        //@}

        /** @name Manipulation Functions */
        //@{

        /** @brief Returns a new copy of this Date object with the days <i>days</i> added to it. */
        Date addDays(int days) const {
            return Date(mDateDuration + Days(days));
        }

        /** @brief Returns a new copy of this Date object with the days <i>days</i> subtracted from it. */
        Date subtractDays(int days) const {
            return Date(mDateDuration - Days(days));
        }

        /** 
         * @brief Returns a new copy of this Date object with the months <i>months</i> added to it. 
         *          
         * When the ending day/month combination does not exist in the resulting month/year, it returns the latest valid date. For example:
         * {@code
         *    Date d = Date(2013, 1, 31).addMonths(1); //d = Date(2013, 2, 28)
         * }
         */
        Date addMonths(int months) const {
            int y, m, d;
            getYearMonthDay(&y, &m, &d);
            const int totalMonths = m + months - 1;
            const int newYear = y + (totalMonths / 12);
            const int newMonth = (totalMonths % 12) + 1;
            const int newDaysInMonth = daysInMonthOfYear(newYear, newMonth);
            const int newDays = newDaysInMonth < d ? newDaysInMonth : d;

            return Date(newYear, newMonth, newDays);
        }

        /** 
         * @brief Returns a new copy of this Date object with the months <i>months</i> subtracted from it. 
         *
         * When the ending day/month combination does not exist in the resulting month/year, it returns the latest valid date. For example:
         * {@code
         *    Date d = Date(2012, 3, 31).subtractMonths(1); //d = Date(2012, 2, 29)
         * }
         */
        Date subtractMonths(int months) const {
            int y, m, d;
            getYearMonthDay(&y, &m, &d);
            const int newYear = y - (std::abs(m - months - 12) / 12);
            const int newMonth = ((11 + m - (months % 12)) % 12) + 1;
            const int newDaysInMonth = daysInMonthOfYear(newYear, newMonth);
            const int newDays = newDaysInMonth < d ? newDaysInMonth : d;

            return Date(newYear, newMonth, newDays);
        }

        /** @brief Returns a new copy of this Date object with the years <i>years</i> added to it. */
        Date addYears(int years) {
            int y, m, d;
            getYearMonthDay(&y, &m, &d);
            return Date(y + years, m, d);
        }

        /** @brief Returns a new copy of this Date object with the years <i>years</i> subtracted from it. */
        Date subtractYears(int years) {
            int y, m, d;
            getYearMonthDay(&y, &m, &d);
            return Date(y - years, m, d);
        }
        //@}

        /** @name Querying Functions */
        //@{

        /** 
         * @brief Set the year, month and day of this date in the parameters year, month and day, respectively. 
         *
         * Calling this function is faster than calling the three equivalents year(), month(), and day(). 
         */
        void getYearMonthDay(int* year, int* month, int* day) const {
            daysToYmd(mDateDuration, year, month, day);
        }

        /** @brief Returns the year of this date as a number. Negative numbers indicate years before 1 CE, for example, year -125 is 125 BCE. */
        int year() const {
            int y;
            daysToYmd(mDateDuration, &y, nullptr, nullptr);
            return y;
        }

        /** @brief Returns the month of this date as a number between 1 and 12, which corresponds to the enumeration #Month. */
        int month() const {
            int m;
            daysToYmd(mDateDuration, nullptr, &m, nullptr);
            return m;
        }

        /** @brief Returns the day of the month of this date as a number between 1 and 31. */
        int day() const {
            int d;
            daysToYmd(mDateDuration, nullptr, nullptr, &d);
            return d;
        }

        /** @brief Returns the weekday of this date as a number between 1 and 7, which corresponds to the enumeration #Weekday. */
        int dayOfWeek() const {
            return (toDays() % 7 + 2) % 7 + 1;
        }

        /** @brief Returns the day of the year of this date as a number between 1 and 365 or 366 on leap years. */
        int dayOfYear() const {
            return (mDateDuration - ymdToDays(year(), 1, 1) + Days(1)).count();
        }

        /** @brief Returns the number of days in the month of this date. It ranges between 28 and 31. */
        int daysInMonth() const {
            return daysInMonthOfYear(year(), month());
        }

        /** @brief Returns the number of days in the year of this date. It is either 365 or 366. */
        int daysInYear() const {
            return (isLeapYear() ? 366 : 365);
        }

        /** 
         * @brief Returns whether the year of this date is a leap year.
         * 
         * For more information see isLeapYear(int).
         */
        bool isLeapYear() const {
            return isLeapYear(year());
        }

        /** 
         * @brief Returns the week of the year of this date, which ranges between 1 and 53, and stores the year in weekYear unless it is not specified (will be defaulted to nullptr).
         * 
         * According to ISO-8601, weeks start on Monday and the first Thursday of a year is always in week 1 of that year. Most years have 52 weeks, but some have 53.
         * yearWeek is not always the same as year(). For example, the date of '1 January 2000' falls in the week 52 of the year 1999, the date of '31 December 2002' falls in the week 1 of the year 2003, and the date of '1 January 2010' falls in the week 53 of the year 2009.
         */
        int weekOfYear(int* weekYear = nullptr) {
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

        /** 
         * @brief Returns the name of the weekday of this date. 
         * 
         * @param useShortName If true, returns the short name of the weekday such as "Sat". If false (the default), returns the long name such as "Saturday". 
         * The short and long names of the weekdays are named according to following convention:
         * <table>
         * <tr><th>Weekday No.<th>Short Name<th>Long Name
         * <tr><td>1<td>Mon<td>Monday
         * <tr><td>2<td>Tue<td>Tuesday
         * <tr><td>3<td>Wed<td>Wednesday
         * <tr><td>4<td>Thu<td>Thursday
         * <tr><td>5<td>Fri<td>Friday
         * <tr><td>6<td>Sat<td>Saturday
         * <tr><td>7<td>Sun<td>Sunday
         * </table>
         */
        std::string dayOfWeekName(bool useShortName = false) const {
            return weekdayNameArray[useShortName ? dayOfWeek() - 1 : dayOfWeek() + 6];
        }

        /** 
         * @brief Returns the name of the month of this date. 
         * 
         * @param useShortName If true, returns the short name of the month such as "Jan". If false(the default), returns the long name such as "January". 
         * The short and long names of the months are named according to following convention:
         * <table>
         * <tr><th>Month No.<th>Short Name<th>Long Name
         * <tr><td>1<td>Jan<td>January
         * <tr><td>2<td>Feb<td>February
         * <tr><td>3<td>Mar<td>March
         * <tr><td>4<td>Apr<td>April
         * <tr><td>5<td>May<td>May
         * <tr><td>6<td>Jun<td>June
         * <tr><td>7<td>Jul<td>July
         * <tr><td>8<td>Aug<td>August
         * <tr><td>9<td>Sep<td>September
         * <tr><td>10<td>Oct<td>October
         * <tr><td>11<td>Nov<td>November
         * <tr><td>12<td>Dec<td>December
         * </table>
         */
        std::string monthName(bool useShortName = false) const {
            return monthNameArray[useShortName ? month() - 1 : month() + 11];
        }
        //@}

        /** @name Comparison Operators */
        //@{

        /** @brief Returns whether this date is earlier than the date of <i>other</i>. */
        bool operator<(const Date& other) const {
            return this->mDateDuration < other.mDateDuration;
        }

        /** @brief Returns whether this date is earlier than the date of <i>other</i> or equal to it. */
        bool operator<=(const Date& other) const {
            return this->mDateDuration <= other.mDateDuration;
        }

        /** @brief Returns whether this date is later than the date of <i>other</i>. */
        bool operator>(const Date& other) const {
            return this->mDateDuration > other.mDateDuration;
        }

        /** @brief Returns whether this date is later than the date of <i>other</i> or equal to it. */
        bool operator>=(const Date& other) const {
            return this->mDateDuration >= other.mDateDuration;
        }

        /** @brief Returns whether this date is equal to the date of <i>other</i>. */
        bool operator==(const Date& other) const {
            return this->mDateDuration == other.mDateDuration;
        }

        /** @brief Returns whether this date is different from the date of <i>other</i>. */
        bool operator!=(const Date& other) const {
            return this->mDateDuration != other.mDateDuration;
        }
        //@}

        /** @name Assignment Operators */
        //@{
        /** @brief Copy assignment operator. */
        Date& operator=(const Date& date) = default;

        /** @brief Move assignment operator. */
        Date& operator=(Date&& date) = default;
        //@}

        /** @name Conversion Functions */
        //@{

        /** @brief Returns the number of elapsed days since 1 CE. */
        long toDays() const {
            return mDateDuration.count();
        }

        /** @brief Returns the corresponding Julian Day Number (JDN) of this date. JDN is the consecutive numbering of days since the beginning of the Julian Period (1 January 4713 BCE).  */
        long toJulianDay() const {
            return (mDateDuration + Days(1721120)).count();
        }

        /** 
         * @brief Returns the date as a string formatted according to the formatter string <i>format</i>. 
         * 
         * The formatter string may contain the following patterns:
         * <table>
         * <tr><th>Pattern<th>Meaning
         * <tr><td>#<td>the era of year as a positive sign(+) or negative sign(-)
         * <tr><td>E<td>the era of year as CE or BCE
         * <tr><td>y<td>the year as one digit or more (1, 9999)
         * <tr><td>yy<td>the year of era as two digits (00, 99)
         * <tr><td>yyyy<td>the year as four digits (0000, 9999)
         * <tr><td>M<td>the month of year as one digit or more (1, 12)
         * <tr><td>MM<td>the month of year as two digits (01, 12)
         * <tr><td>MMM<td>the month of year as short name (e.g. "Feb")
         * <tr><td>MMMM<td>the month of year as long name (e.g. "February")
         * <tr><td>d<td>the day of month as one digit or more (1, 31)
         * <tr><td>dd<td>the day of month as two digits (00, 31)
         * <tr><td>ddd<td>the day of week as short (e.g. "Fri")
         * <tr><td>dddd<td>the day of week as long name (e.g. "Friday")
         * </table>
         * Any character in the formatter string not listed above will be inserted as is into the output string. 
         * 
         * See also dayOfWeekName() and monthName()
         */
        std::string toString(const std::string& format) const {
            std::stringstream output;

            for (int pos = 0; pos < format.size(); ++pos) {
                int y, m, d;
                getYearMonthDay(&y, &m, &d);
                y = std::abs(y);

                char currChar = format[pos];
                const int charCount = countIdenticalCharsFrom(pos, format);
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
        //@}

        /** 
         * @brief Returns a Date object that represents the date parsed from the string <i>dateString</i> according to the format <i>format</i>.
         * 
         * For further information about the <i>format</i> parameter, see toString(). 
         */
        static Date fromString(const std::string& dateString, const std::string& format) {
            int y = 1, m = 1, d = 1;

            for (int fmtPos = 0, datPos = 0; fmtPos < format.size() && datPos < dateString.size(); ++fmtPos) {
                char currChar = format[fmtPos];
                const int charCount = countIdenticalCharsFrom(fmtPos, format);
                fmtPos += charCount - 1; // skip all identical characters except the last.

                if (currChar == '#') {
                    if (dateString[datPos] == '+') {
                        y = 1;
                        ++datPos;
                    } else if (dateString[datPos] == '-') {
                        y = -1;
                        ++datPos;
                    }
                } else if (currChar == 'y') {
                    if (charCount == 1) {
                        std::string yStr = readIntFromString(dateString, datPos);
                        y = y * std::stoi(yStr);
                        datPos += yStr.size();
                    } else if (charCount == 2) {
                        y = y * std::stoi(dateString.substr(datPos, charCount));
                        y += 2000;
                        datPos += charCount;
                    } else if (charCount == 4) {
                        y = y * std::stoi(dateString.substr(datPos, charCount));
                        datPos += charCount;
                    }
                } else if (currChar == 'E') {
                    if (dateString.substr(datPos, 2) == "CE") {
                        y = std::abs(y);
                        datPos += 2;
                    } else if (dateString.substr(datPos, 3) == "BCE") {
                        y = -std::abs(y);
                        datPos += 3;
                    }
                } else if (currChar == 'M') {
                    if (charCount == 1) {
                        std::string mStr = readIntFromString(dateString, datPos);
                        m = std::stoi(mStr);
                        datPos += mStr.size();
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
                } else if (currChar == 'd') {
                    if (charCount == 1) {
                        std::string dStr = readIntFromString(dateString, datPos);
                        d = std::stoi(dStr);
                        datPos += dStr.size();
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
                    datPos += 1;
                }
            }
            //                        m = (std::find(monthNameArray + ((charCount == 4) * 12), monthNameArray + ((charCount == 4) * 12) + 11, dateString.substr(datPos, newPos - datPos)) - monthNameArray) - ((charCount == 4) * 12) + 1;

            return Date(y, m, d);
        }

        /** @brief Returns a Date object corresponding to the Julian day <i>julianDay</i>. */
        static Date fromJulianDay(long julianDay) {

            return Date(Days(julianDay - 1721120));
        }

        /** 
         * @brief Returns the number of the days between the dates <i>from</i> and <i>to</i>, excluding the last day. 
         *
         * For example:
         * {@code
         *    int num = Date::daysBetween(Date(1999, 1, 1), Date(1999, 1, 3));  // num = 2
         * } 
         */
        static int daysBetween(const Date& from, const Date& to) {

            return std::abs((from.mDateDuration - to.mDateDuration).count());
        }

        /** 
         * @brief Returns whether the year <i>year</i> is a leap year.
         * 
         * According to the ISO proleptic calendar system rules, a year is a leap year if it is divisible by four without remainder. However, years divisible by 100, are not leap years, with the exception of years divisible by 400 which are.
         * For example, 1904 is a leap year it is divisible by 4. 1900 was not a leap year as it is divisible by 100, however 2000 was a leap year as it is divisible by 400.
         */
        static bool isLeapYear(int year) {

            return (year % 4 == 0) && (year % 100 != 0 || year % 400 == 0);
        }

        /** @brief Returns the number of days in the month <i>month</i> of year <i>year</i>. It ranges between 28 and 31. */
        static int daysInMonthOfYear(int year, int month) {
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

    private:

        static int countIdenticalCharsFrom(std::size_t pos, const std::string& str) {
            int idx = pos + 1;

            while (idx < str.size() && str[idx] == str[pos])
                ++idx;

            return idx - pos;
        }

        static std::string readIntFromString(const std::string& str, int pos) {
            int newPos = pos;
            while (newPos < str.size() && std::isdigit(str[newPos])) ++newPos;

            return str.substr(pos, newPos - pos);
        }

        Date getFirstWeekDate(int year) {
            Date d(year, 1, 1);

            return d.addDays((11 - d.dayOfWeek()) % 7 - 3);
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

        static const std::string weekdayNameArray[14];
        static const std::string monthNameArray[24];

        Days mDateDuration;
    };

    /** 
     * @relates Date
     * @name Serialization/Deserialization Functions 
     */
    //@{

    /** @brief Writes date <i>d</i> to stream <i>os</i> in ISO-8601 date format "yyyy-MM-dd". */
    std::ostream& operator<<(std::ostream& os, const Date& d) {
        os << d.toString("yyyy-MM-dd");
        return os;
    }

    /** @brief Reads a date in ISO-8601 date format "yyyy-MM-dd" from stream <i>is</i> and stores it in date <i>d</i>. */
    std::istream& operator>>(std::istream& is, Date& d) {
        const int DateFormatWidth = 10;
        char result[DateFormatWidth];
        is.read(result, DateFormatWidth);
        std::string str(result, DateFormatWidth);

        d = Date::fromString(str, "yyyy-MM-dd");
    }
    //@}

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

