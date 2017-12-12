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

#include "gmock/gmock.h"
#include "Date.hpp"
#include "date/date.h"

using namespace ::testing;
using namespace Salsabil;

TEST(DateTest, ReturnsYearMonthDay) {
    int year, month, day;

    Date(1989, 3, 12).getYearMonthDay(&year, &month, &day);
    Date(1989, 3, 12).getYearMonthDay(&year, &month, nullptr);
    Date(1989, 3, 12).getYearMonthDay(&year, nullptr, nullptr);
    Date(1989, 3, 12).getYearMonthDay(nullptr, nullptr, nullptr);
    Date(1989, 3, 12).getYearMonthDay(0, &month, &day);
    Date(1989, 3, 12).getYearMonthDay(0, 0, &day);
    Date(1989, 3, 12).getYearMonthDay(0, 0, 0);

    ASSERT_THAT(year, Eq(1989));
    ASSERT_THAT(month, Eq(3));
    ASSERT_THAT(day, Eq(12));
}

TEST(DateTest, InitializesToCurrentDateIfDefaultConstructed) {
    Date myDate;
    std::time_t tTime = std::time(nullptr);
    std::tm* tmTime = std::gmtime(&tTime);
    ASSERT_THAT(myDate.year(), Eq(tmTime->tm_year + 1900));
    ASSERT_THAT(myDate.month(), Eq(tmTime->tm_mon + 1));
    ASSERT_THAT(myDate.day(), Eq(tmTime->tm_mday));
}

TEST(DateTest, ConstructsFromYearMonthDay) {
    Date myDate(2012, 3, 27);
    ASSERT_THAT(myDate.year(), Eq(2012));
    ASSERT_THAT(myDate.month(), Eq(3));
    ASSERT_THAT(myDate.day(), Eq(27));
}

TEST(DateTest, TestsComparisons) {
    ASSERT_TRUE(Date(2012, 3, 27) == Date(2012, 3, 27));
    ASSERT_TRUE(Date(2012, 3, 1) != Date(2012, 3, 2));
    ASSERT_TRUE(Date(2012, 3, 1) < Date(2012, 3, 2));
    ASSERT_TRUE(Date(2012, 3, 2) <= Date(2012, 3, 2));
    ASSERT_TRUE(Date(2012, 3, 2) <= Date(2012, 3, 3));
    ASSERT_TRUE(Date(2012, 3, 3) > Date(2012, 3, 2));
    ASSERT_TRUE(Date(2012, 3, 3) >= Date(2012, 3, 3));
    ASSERT_TRUE(Date(2012, 3, 4) >= Date(2012, 3, 3));
}

TEST(DateTest, AddsSubtractsDuration) {
    ASSERT_TRUE(Date(2012, 1, 1) + Date::Days(30) == Date(2012, 1, 31));
    ASSERT_TRUE(Date(2012, 1, 31) - Date::Days(30) == Date(2012, 1, 1));
}

TEST(DateTest, AddsSubtractsDays) {
    ASSERT_TRUE(Date(2045, 3, 27).addDays(5) == Date(2045, 4, 1));
    ASSERT_TRUE(Date(2045, 4, 1).subtractDays(5) == Date(2045, 3, 27));
}

TEST(DateTest, AddsSubtractsMonths) {
    // when the sum of augend and addend is smaller than 12.
    ASSERT_TRUE(Date(2012, 3, 27).addMonths(5) == Date(2012, 8, 27));

    // when the sum of augend and addend is bigger than 12.
    ASSERT_TRUE(Date(2012, 8, 27).addMonths(10) == Date(2013, 6, 27));

    // when the result of addition is 12. due to the representation of month is 1...12 rather than 0...11.  
    ASSERT_TRUE(Date(2012, 8, 27).addMonths(4) == Date(2012, 12, 27));

    // when the minuend is bigger than the subtrahend.
    ASSERT_TRUE(Date(2013, 6, 27).subtractMonths(5) == Date(2013, 1, 27));

    // when the minuend is smaller than the subtrahend.
    ASSERT_TRUE(Date(2013, 1, 27).subtractMonths(10) == Date(2012, 3, 27));

    // when the result of subtraction is 12. due to the representation of month is 1...12 rather than 0...11.  
    ASSERT_TRUE(Date(2013, 1, 27).subtractMonths(1) == Date(2012, 12, 27));

}

TEST(DateTest, AddsSubtractsYears) {
    ASSERT_TRUE(Date(1966, 11, 2).addYears(40) == Date(2006, 11, 2));
    ASSERT_TRUE(Date(2006, 11, 2).subtractYears(40) == Date(1966, 11, 2));
}

TEST(DateTest, TestsWhetherTheYearIsLeap) {
    ASSERT_FALSE(Date::isLeapYear(2011));
    ASSERT_TRUE(Date::isLeapYear(2012));
}

TEST(DateTest, ReturnsDayOfWeek) {
    ASSERT_THAT(Date(1970, 1, 1).dayOfWeek(), Eq(static_cast<int> (Date::Weekday::Thursday)));
    ASSERT_THAT(Date(2001, 1, 1).dayOfWeek(), Eq(1));
    ASSERT_THAT(Date(2002, 1, 1).dayOfWeek(), Eq(2));
    ASSERT_THAT(Date(2003, 1, 1).dayOfWeek(), Eq(3));
    ASSERT_THAT(Date(2004, 1, 1).dayOfWeek(), Eq(4));
    ASSERT_THAT(Date(2010, 1, 1).dayOfWeek(), Eq(5));
    ASSERT_THAT(Date(2005, 1, 1).dayOfWeek(), Eq(6));
    ASSERT_THAT(Date(2006, 1, 1).dayOfWeek(), Eq(7));
}

TEST(DateTest, ReturnsDayOfYear) {
    ASSERT_THAT(Date(1970, 1, 1).dayOfYear(), Eq(1));
    ASSERT_THAT(Date(2017, 12, 2).dayOfYear(), Eq(336));
    ASSERT_THAT(Date(2064, 2, 29).dayOfYear(), Eq(60));
}

TEST(DateTest, ReturnsDaysInMonth) {
    ASSERT_THAT(Date(1970, 1, 1).daysInMonth(), Eq(31));
    ASSERT_THAT(Date(1970, 2, 1).daysInMonth(), Eq(28));
    ASSERT_THAT(Date(2012, 2, 1).daysInMonth(), Eq(29));
    ASSERT_THAT(Date(2055, 3, 1).daysInMonth(), Eq(31));
    ASSERT_THAT(Date(2013, 4, 1).daysInMonth(), Eq(30));
    ASSERT_THAT(Date(2025, 5, 1).daysInMonth(), Eq(31));
    ASSERT_THAT(Date(2036, 6, 1).daysInMonth(), Eq(30));
    ASSERT_THAT(Date(2057, 7, 1).daysInMonth(), Eq(31));
    ASSERT_THAT(Date(2088, 8, 1).daysInMonth(), Eq(31));
    ASSERT_THAT(Date(2009, 9, 1).daysInMonth(), Eq(30));
    ASSERT_THAT(Date(2001, 10, 1).daysInMonth(), Eq(31));
    ASSERT_THAT(Date(2023, 11, 1).daysInMonth(), Eq(30));
    ASSERT_THAT(Date(2023, 12, 1).daysInMonth(), Eq(31));
}

TEST(DateTest, ReturnsDaysInYear) {
    ASSERT_THAT(Date(1970, 1, 1).daysInYear(), Eq(365));
    ASSERT_THAT(Date(2012, 2, 2).daysInYear(), Eq(366));
}

TEST(DateTest, ReturnsDaysBetweenTwoDates) {
    ASSERT_THAT(Date::daysBetween(Date(1970, 1, 1), Date(1971, 1, 1)), Eq(365));
    ASSERT_THAT(Date::daysBetween(Date(2012, 1, 1), Date(2016, 1, 1)), Eq(1461));
}

TEST(DateTest, ReturnsWeekOfYear) {
    int weekYear;

    // when the week is in the middle of the year
    ASSERT_THAT(Date(2017, 12, 3).weekOfYear(&weekYear), Eq(48));
    ASSERT_THAT(weekYear, Eq(2017));

    // when the week is at the start of year
    ASSERT_THAT(Date(2002, 12, 31).weekOfYear(&weekYear), Eq(1));
    ASSERT_THAT(weekYear, Eq(2003));

    // when the week is at the end of year
    ASSERT_THAT(Date(2000, 1, 1).weekOfYear(&weekYear), Eq(52));
    ASSERT_THAT(weekYear, Eq(1999));

    // when the week is at the end of 53-week-year
    ASSERT_THAT(Date(2010, 1, 1).weekOfYear(&weekYear), Eq(53));
    ASSERT_THAT(weekYear, Eq(2009));
}

TEST(DateTest, ToJulainDay) {
    ASSERT_THAT(Date(-4713, 11, 24).toJulianDay(), Eq(0));
    ASSERT_THAT(Date(-4713, 11, 25).toJulianDay(), Eq(1));
    ASSERT_THAT(Date(1970, 1, 1).toJulianDay(), Eq(2440588));
    ASSERT_THAT(Date(2000, 1, 1).toJulianDay(), Eq(2451545));
    ASSERT_THAT(Date(2017, 12, 4).toJulianDay(), Eq(2458092));
}

TEST(DateTest, FromJulianDay) {
    ASSERT_THAT(Date::fromJulianDay(0), Eq(Date(-4713, 11, 24)));
    ASSERT_THAT(Date::fromJulianDay(1), Eq(Date(-4713, 11, 25)));
    ASSERT_THAT(Date::fromJulianDay(2440588), Eq(Date(1970, 1, 1)));
    ASSERT_THAT(Date::fromJulianDay(2451545), Eq(Date(2000, 1, 1)));
    ASSERT_THAT(Date::fromJulianDay(2458092), Eq(Date(2017, 12, 4)));
}

TEST(DateTest, AddsSubtractsYearsMMM) {
    using namespace date;
    date::year_month_day ymd(year(2017), month(12), day(2));
    //            std::cout << weekday(sys_days(ymd)) << std::endl;

    // 31556952 --- 1 year in seconds
    //    ymd += years{46};
    //
    //    ASSERT_THAT(ymd.year().operator int(), Eq(2012));
    //    ASSERT_THAT(ymd.month().operator unsigned(), Eq(2));
    //    ASSERT_THAT(ymd.day().operator unsigned(), Eq(29));
    //
    //    ymd += months{12};
    //    ASSERT_THAT(ymd.year().operator int(), Eq(2013));
    //    ASSERT_THAT(ymd.month().operator unsigned(), Eq(2));
    //    ASSERT_THAT(ymd.day().operator unsigned(), Eq(29));
    //
    //    ASSERT_FALSE(ymd.ok());

    //    ymd -= years{40};
    //    std::cout << std::chrono::seconds(ymd.operator sys_days().time_since_epoch()) << std::endl;
    //    ASSERT_THAT(ymd.year().operator int(), Eq(1966));
    //    ASSERT_THAT(ymd.month().operator unsigned(), Eq(11));
    //    ASSERT_THAT(ymd.day().operator unsigned(), Eq(2));
}

