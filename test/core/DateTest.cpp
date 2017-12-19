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

TEST(DateTest, AddsSubtractsDays) {
    ASSERT_THAT(Date(2045, 3, 27).addDays(5), Eq(Date(2045, 4, 1)));
    ASSERT_THAT(Date(2045, 4, 1).subtractDays(5), Eq(Date(2045, 3, 27)));
}

TEST(DateTest, AddsSubtractsMonths) {
    // when the sum of augend and addend is smaller than 12.
    ASSERT_THAT(Date(2012, 3, 27).addMonths(5), Eq(Date(2012, 8, 27)));

    // when the sum of augend and addend is bigger than 12.
    ASSERT_THAT(Date(2012, 8, 27).addMonths(10), Eq(Date(2013, 6, 27)));

    // when the result of addition is 12. due to the representation of month is 1...12 rather than 0...11.  
    ASSERT_THAT(Date(2012, 8, 27).addMonths(4), Eq(Date(2012, 12, 27)));

    // when the minuend is bigger than the subtrahend.
    ASSERT_THAT(Date(2013, 6, 27).subtractMonths(5), Eq(Date(2013, 1, 27)));

    // when the minuend is smaller than the subtrahend.
    ASSERT_THAT(Date(2013, 1, 27).subtractMonths(10), Eq(Date(2012, 3, 27)));

    // when the result of subtraction is 12. due to the representation of month is 1...12 rather than 0...11.  
    ASSERT_THAT(Date(2013, 1, 27).subtractMonths(1), Eq(Date(2012, 12, 27)));

    // when the ending day/month combination does not exist in the resulting month/year, returns the latest valid date.
    ASSERT_THAT(Date(2013, 1, 31).addMonths(1), Eq(Date(2013, 2, 28)));
    ASSERT_THAT(Date(2013, 2, 28).addMonths(1), Eq(Date(2013, 3, 28)));
    ASSERT_THAT(Date(2012, 3, 31).subtractMonths(1), Eq(Date(2012, 2, 29)));
}

TEST(DateTest, AddsSubtractsYears) {
    ASSERT_THAT(Date(1966, 11, 2).addYears(40), Eq(Date(2006, 11, 2)));
    ASSERT_THAT(Date(2006, 11, 2).subtractYears(40), Eq(Date(1966, 11, 2)));
}

TEST(DateTest, TestsWhetherTheYearIsLeap) {
    ASSERT_FALSE(Date::isLeapYear(2011));
    ASSERT_TRUE(Date(2012, 1, 1).isLeapYear());
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
    ASSERT_THAT(Date::daysInMonthOfYear(1970, 1), Eq(31));
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

TEST(DateTest, ReturnsNumberOfDaysBetweenTwoDates) {
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

TEST(DateTest, FormatYear) {
    ASSERT_THAT(Date(572, 4, 22).toString("y"), StrEq("572"));
    ASSERT_THAT(Date(1999, 4, 13).toString("yy"), StrEq("99"));
    ASSERT_THAT(Date(1901, 6, 11).toString("yy"), StrEq("01"));
    ASSERT_THAT(Date(1999, 7, 4).toString("yyyy"), StrEq("1999"));
    ASSERT_THAT(Date(-795, 7, 23).toString("yyyy"), StrEq("0795"));
    ASSERT_THAT(Date(-1795, 7, 23).toString("#yyyy"), StrEq("-1795"));
    ASSERT_THAT(Date(1795, 7, 23).toString("#yyyy"), StrEq("+1795"));
    ASSERT_THAT(Date(-1795, 7, 23).toString("yyyy E"), StrEq("1795 BCE"));
    ASSERT_THAT(Date(1795, 7, 23).toString("yyyy E"), StrEq("1795 CE"));
    ASSERT_THAT(Date(1795, 7, 23).toString("yy . yy"), StrEq("95 . 95"));
}

TEST(DateTest, FormatMonth) {
    ASSERT_THAT(Date(572, 4, 22).toString("M"), StrEq("4"));
    ASSERT_THAT(Date(1999, 5, 13).toString("MM"), StrEq("05"));
    ASSERT_THAT(Date(1999, 11, 13).toString("MM"), StrEq("11"));
    ASSERT_THAT(Date(1901, 6, 11).toString("MMM"), StrEq("Jun"));
    ASSERT_THAT(Date(1999, 7, 30).toString("MMMM"), StrEq("July"));
}

TEST(DateTest, FormatDay) {
    ASSERT_THAT(Date(572, 4, 22).toString("d"), StrEq("22"));
    ASSERT_THAT(Date(1999, 4, 3).toString("dd"), StrEq("03"));
    ASSERT_THAT(Date(1901, 6, 11).toString("dd"), StrEq("11"));
    ASSERT_THAT(Date(1999, 7, 4).toString("ddd"), StrEq("Sun"));
    ASSERT_THAT(Date(2017, 12, 15).toString("dddd"), StrEq("Friday"));
}

TEST(DateTest, FormatDate) {
    // examples for testing purposes.
    ASSERT_THAT(Date(572, 4, 22).toString("y.M.d"), StrEq("572.4.22"));
    ASSERT_THAT(Date(2017, 4, 3).toString("yy.MM.dd"), StrEq("17.04.03"));
    ASSERT_THAT(Date(2007, 5, 11).toString("yy.MMM.dd"), StrEq("07.May.11"));
    ASSERT_THAT(Date(2017, 12, 10).toString("ddd dd.MM.yyyy"), StrEq("Sun 10.12.2017"));
    ASSERT_THAT(Date(2017, 12, 15).toString("dddd dd MMMM yyyy"), StrEq("Friday 15 December 2017"));
    ASSERT_THAT(Date(2017, 12, 16).toString("yyyy-MM-dd E"), StrEq("2017-12-16 CE"));
    ASSERT_THAT(Date(-2017, 12, 16).toString("#yyyy.MM.dd"), StrEq("-2017.12.16"));
    ASSERT_THAT(Date(2017, 12, 19).toString("yyyyMMdd"), StrEq("20171219"));
}

TEST(DateTest, ParsesYearInDateString) {
    ASSERT_THAT(Date::fromString("572", "y"), Eq(Date(572, 1, 1)));
    ASSERT_THAT(Date::fromString("12", "yy"), Eq(Date(2012, 1, 1)));
    ASSERT_THAT(Date::fromString("1999", "yyyy"), Eq(Date(1999, 1, 1)));
    ASSERT_THAT(Date::fromString("-572", "#y"), Eq(Date(-572, 1, 1)));
    ASSERT_THAT(Date::fromString("+572", "#y"), Eq(Date(572, 1, 1)));
    ASSERT_THAT(Date::fromString("-1999", "#yyyy"), Eq(Date(-1999, 1, 1)));
    ASSERT_THAT(Date::fromString("+1999", "#yyyy"), Eq(Date(1999, 1, 1)));
    ASSERT_THAT(Date::fromString("572 CE", "y E"), Eq(Date(572, 1, 1)));
    ASSERT_THAT(Date::fromString("572 BCE", "y E"), Eq(Date(-572, 1, 1)));
    ASSERT_THAT(Date::fromString("1999 CE", "yyyy E"), Eq(Date(1999, 1, 1)));
    ASSERT_THAT(Date::fromString("1999 BCE", "yyyy E"), Eq(Date(-1999, 1, 1)));
}

TEST(DateTest, ParsesMonthInDateString) {
    ASSERT_THAT(Date::fromString("1", "M"), Eq(Date(1, 1, 1)));
    ASSERT_THAT(Date::fromString("02", "MM"), Eq(Date(1, 2, 1)));
    ASSERT_THAT(Date::fromString("Aug", "MMM"), Eq(Date(1, 8, 1)));
    ASSERT_THAT(Date::fromString("September", "MMMM"), Eq(Date(1, 9, 1)));
    ASSERT_THAT(Date::fromString("January, 2009", "MMMM, yyyy"), Eq(Date(2009, 1, 1)));
    ASSERT_THAT(Date::fromString("December, 2011", "MMMM, yyyy"), Eq(Date(2011, 12, 1)));
}

TEST(DateTest, ParsesDayInDateString) {
    ASSERT_THAT(Date::fromString("1", "d"), Eq(Date(1, 1, 1)));
    ASSERT_THAT(Date::fromString("02", "dd"), Eq(Date(1, 1, 2)));
    ASSERT_THAT(Date::fromString("Thu, 22.05.17", "ddd, dd.MM.yy"), Eq(Date(2017, 5, 22)));
    ASSERT_THAT(Date::fromString("Thursday, 01.12.1989", "dddd, dd.MM.yyyy"), Eq(Date(1989, 12, 1)));
}

TEST(DateTest, ReturnsDateFromString) {
    ASSERT_THAT(Date::fromString("572.4.22", "y.M.d"), Eq(Date(572, 4, 22)));
    ASSERT_THAT(Date::fromString("17.04.03", "yy.MM.dd"), Eq(Date(2017, 4, 3)));
    ASSERT_THAT(Date::fromString("07.May.11", "yy.MMM.dd"), Eq(Date(2007, 5, 11)));
    ASSERT_THAT(Date::fromString("Sun 10.12.2017", "ddd dd.MM.yyyy"), Eq(Date(2017, 12, 10)));
    ASSERT_THAT(Date::fromString("Friday 15 December 2017", "dddd dd MMMM yyyy"), Eq(Date(2017, 12, 15)));
    ASSERT_THAT(Date::fromString("2017-12-16 CE", "yyyy-MM-dd E"), Eq(Date(2017, 12, 16)));
    ASSERT_THAT(Date::fromString("-2017.12.16", "#yyyy.MM.dd"), Eq(Date(-2017, 12, 16)));
    ASSERT_THAT(Date::fromString("20171219", "yyyyMMdd"), Eq(Date(2017, 12, 19)));
}

TEST(DateTest, SerializesDeserializes) {
    Date d;
    std::stringstream ss;
    ss << Date(2014, 11, 9);
    ss >> d;
    ASSERT_THAT(d, Eq(Date(2014, 11, 9)));
}
