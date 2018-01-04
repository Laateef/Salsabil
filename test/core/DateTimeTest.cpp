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
#include "DateTime.hpp"

using namespace ::testing;
using namespace Salsabil;

TEST(DateTimeTest, IsInvalidIfDefaultConstructed) {
    DateTime dt;
    ASSERT_FALSE(dt.isValid());
    ASSERT_THAT(dt.year(), Eq(0));
    ASSERT_THAT(dt.month(), Eq(0));
    ASSERT_THAT(dt.day(), Eq(0));
    ASSERT_THAT(dt.hour(), Eq(0));
    ASSERT_THAT(dt.minute(), Eq(0));
    ASSERT_THAT(dt.second(), Eq(0));
    ASSERT_THAT(dt.millisecond(), Eq(0));
    ASSERT_THAT(dt.microsecond(), Eq(0));
    ASSERT_THAT(dt.nanosecond(), Eq(0));
}

TEST(DateTimeTest, ReturnsCurrentDateTime) {
    DateTime dt = DateTime::currentDateTime();
    std::time_t tTime = std::time(nullptr);
    std::tm* tmTime = std::gmtime(&tTime);

    ASSERT_THAT(dt.year(), Eq(tmTime->tm_year + 1900));
    ASSERT_THAT(dt.month(), Eq(tmTime->tm_mon + 1));
    ASSERT_THAT(dt.day(), Eq(tmTime->tm_mday));
    ASSERT_THAT(dt.hour(), Eq(tmTime->tm_hour));
    ASSERT_THAT(dt.minute(), Eq(tmTime->tm_min));
    ASSERT_THAT(dt.second(), Eq(tmTime->tm_sec));
    // can not be tested
    //    ASSERT_THAT(dt.millisecond(), Eq(0));
    //    ASSERT_THAT(dt.microsecond(), Eq(0));
    //    ASSERT_THAT(dt.nanosecond(), Eq(0));
}

TEST(DateTimeTest, ConstructsFromDateAndTime) {
    DateTime dt(Date(1999, 12, 1), Time(8, 55, 21, Time::Nanoseconds(123456789)));
    ASSERT_THAT(dt.year(), Eq(1999));
    ASSERT_THAT(dt.month(), Eq(12));
    ASSERT_THAT(dt.day(), Eq(1));
    ASSERT_THAT(dt.hour(), Eq(8));
    ASSERT_THAT(dt.minute(), Eq(55));
    ASSERT_THAT(dt.second(), Eq(21));
    ASSERT_THAT(dt.millisecond(), Eq(123));
    ASSERT_THAT(dt.microsecond(), Eq(123456));
    ASSERT_THAT(dt.nanosecond(), Eq(123456789));
}

TEST(DateTimeTest, TestsComparisons) {
    ASSERT_TRUE(DateTime(Date(2012, 3, 27), Time(8, 55, 21, Time::Nanoseconds(123456789))) < DateTime(Date(2017, 3, 27), Time(8, 55, 21, Time::Nanoseconds(123456789))));
    ASSERT_TRUE(DateTime(Date(2012, 3, 27), Time(8, 55, 21, Time::Nanoseconds(123456789))) <= DateTime(Date(2017, 3, 27), Time(8, 55, 21, Time::Nanoseconds(123456789))));
    ASSERT_TRUE(DateTime(Date(2012, 3, 27), Time(8, 55, 21, Time::Nanoseconds(123456789))) <= DateTime(Date(2012, 3, 27), Time(8, 55, 21, Time::Nanoseconds(123456789))));
    ASSERT_TRUE(DateTime(Date(2012, 9, 27), Time(8, 55, 21, Time::Nanoseconds(123456789))) > DateTime(Date(2012, 3, 27), Time(8, 55, 21, Time::Nanoseconds(123456789))));
    ASSERT_TRUE(DateTime(Date(2012, 3, 27), Time(9, 55, 21, Time::Nanoseconds(123456789))) >= DateTime(Date(2012, 3, 27), Time(8, 55, 21, Time::Nanoseconds(123456789))));
    ASSERT_TRUE(DateTime(Date(2015, 3, 27), Time(1, 55, 21, Time::Nanoseconds(123456789))) >= DateTime(Date(2015, 3, 27), Time(1, 55, 21, Time::Nanoseconds(123456789))));
    ASSERT_TRUE(DateTime(Date(2012, 3, 27), Time(8, 55, 21, Time::Nanoseconds(123456789))) == DateTime(Date(2012, 3, 27), Time(8, 55, 21, Time::Nanoseconds(123456789))));
    ASSERT_TRUE(DateTime(Date(2017, 12, 6), Time(16, 32, 4, Time::Nanoseconds(987654321))) != DateTime(Date(2012, 3, 27), Time(8, 55, 21, Time::Nanoseconds(123456789))));
}

TEST(DateTimeTest, AddsDuration) {
    // Adding time durations which don't affect the date part.
    ASSERT_THAT(DateTime(Date(2045, 3, 27), Time(16, 2, 3, 4)).addDuration(DateTime::Minutes(10)), Eq(DateTime(Date(2045, 3, 27), Time(16, 12, 3, 4))));
    ASSERT_THAT(DateTime(Date(2045, 3, 27), Time(1, 2, 3, 4)).addDuration(DateTime::Hours(3)), Eq(DateTime(Date(2045, 3, 27), Time(4, 2, 3, 4))));
    // Adding time durations which affect the date part.
    ASSERT_THAT(DateTime(Date(2045, 3, 27), Time(16, 2, 3, 4)).addDuration(DateTime::Hours(10)), Eq(DateTime(Date(2045, 3, 28), Time(2, 2, 3, 4))));
    // Adding date durations which only affect the date part.
    ASSERT_THAT(DateTime(Date(2045, 3, 27), Time(1, 2, 3, 4)).addDuration(DateTime::Days(3)), Eq(DateTime(Date(2045, 3, 30), Time(1, 2, 3, 4))));
}

TEST(DateTimeTest, SubtractsDuration) {
    // Subtracting time durations which don't affect the date part.
    ASSERT_THAT(DateTime(Date(2045, 3, 27), Time(16, 2, 3, 4)).subtractDuration(DateTime::Minutes(10)), Eq(DateTime(Date(2045, 3, 27), Time(15, 52, 3, 4))));
    ASSERT_THAT(DateTime(Date(2045, 3, 27), Time(7, 2, 3, 4)).subtractDuration(DateTime::Hours(3)), Eq(DateTime(Date(2045, 3, 27), Time(4, 2, 3, 4))));
    // Subtracting time durations which affect the date part.
    ASSERT_THAT(DateTime(Date(2045, 3, 27), Time(6, 2, 3, 4)).subtractDuration(DateTime::Hours(10)), Eq(DateTime(Date(2045, 3, 26), Time(20, 2, 3, 4))));
    // Subtracting date durations which only affect the date part.
    ASSERT_THAT(DateTime(Date(2045, 3, 27), Time(1, 2, 3, 4)).subtractDuration(DateTime::Days(3)), Eq(DateTime(Date(2045, 3, 24), Time(1, 2, 3, 4))));
}

TEST(DateTimeTest, TestsAdditionSubtractionOperators) {
    ASSERT_THAT(DateTime(Date(2017, 3, 27), Time(22, 19, 53, 4)) - DateTime(Date(2017, 3, 26), Time(22, 12, 53, 4)), Eq(DateTime::Days(1) + DateTime::Minutes(7)));
    ASSERT_THAT(DateTime(Date(2017, 12, 29), Time(23, 12, 53, 4)) + DateTime::Hours(35), Eq(DateTime(Date(2017, 12, 31), Time(10, 12, 53, 4))));
    ASSERT_THAT(DateTime(Date(2017, 12, 31), Time(10, 12, 53, 4)) - DateTime::Hours(35), Eq(DateTime(Date(2017, 12, 29), Time(23, 12, 53, 4))));
}

TEST(DateTimeTest, FormatDateTime) {
    ASSERT_THAT(DateTime(Date(1999, 5, 18), Time(23, 55, 57, Time::Nanoseconds(123456789))).toString("d/M/yyyy, hh:mm:ss.fffffffff"), StrEq("18/5/1999, 23:55:57.123456789"));
    ASSERT_THAT(DateTime().toString("d/M/yyyy, hh:mm:ss.fffffffff"), StrEq(""));
}

TEST(DateTimeTest, ConstructsFromString) {
    ASSERT_THAT(DateTime::fromString("18/5/1999, 23:55:57.123456789", "d/M/yyyy, hh:mm:ss.fffffffff"), Eq(DateTime(Date(1999, 5, 18), Time(23, 55, 57, Time::Nanoseconds(123456789)))));
}

TEST(DateTimeTest, ToJulainDay) {
    ASSERT_NEAR(DateTime(Date(-4714, 11, 24), Time(12, 0, 0)).toJulianDay(), 0, 0.0000000001);
    ASSERT_NEAR(DateTime(Date(-4714, 11, 26), Time(0, 0, 0)).toJulianDay(), 1.5, 0.0000000001);
    ASSERT_NEAR(DateTime(Date(2017, 12, 31), Time(00, 9, 35)).toJulianDay(), 2458118.506655093, 0.0000000001);
}

TEST(DateTimeTest, FromJulainDay) {
    ASSERT_THAT(DateTime::fromJulianDay(0), Eq(DateTime(Date(-4714, 11, 24), Time(12, 0, 0))));
    ASSERT_THAT(DateTime::fromJulianDay(1.5), Eq(DateTime(Date(-4714, 11, 26), Time(0, 0, 0))));
    ASSERT_THAT(DateTime::fromJulianDay(2458118.506655093), Eq(DateTime(Date(2017, 12, 31), Time(0, 9, 35))));
}

TEST(DateTimeTest, DifferenceBetweenTwoDates) {
    ASSERT_THAT(DateTime::daysBetween(DateTime(Date(1970, 1, 1), Time(23, 2, 36)), DateTime(Date(1971, 1, 1), Time(23, 2, 36))), Eq(365));
    ASSERT_THAT(DateTime::hoursBetween(DateTime(Date(1998, 1, 1), Time(23, 45, 36)), DateTime(Date(1998, 1, 2), Time(12, 2, 36))), Eq(12));
    ASSERT_THAT(DateTime::minutesBetween(DateTime(Date(2000, 1, 2), Time(23, 45, 36)), DateTime(Date(2000, 1, 2), Time(12, 2, 36))), Eq(703));
    ASSERT_THAT(DateTime::secondsBetween(DateTime(Date(2002, 1, 1), Time(15, 45, 36)), DateTime(Date(2002, 1, 2), Time(15, 2, 37))), Eq(83821));
    ASSERT_THAT(DateTime::millisecondsBetween(DateTime(Date(2017, 1, 15), Time(12, 45, 36, 123)), DateTime(Date(2017, 1, 15), Time(12, 45, 37))), Eq(877));
    ASSERT_THAT(DateTime::microsecondsBetween(DateTime(Date(2017, 1, 15), Time(12, 45, 36, DateTime::Microseconds(123001))), DateTime(Date(2017, 1, 15), Time(12, 45, 37))), Eq(876999));
}

TEST(DateTimeTest, SerializesDeserializes) {
    DateTime dt;
    std::stringstream ss;
    ss << DateTime(Date(2014, 11, 9), Time(2, 44, 21, 165));
    ss >> dt;
    ASSERT_THAT(dt, Eq(DateTime(Date(2014, 11, 9), Time(2, 44, 21, 165))));
}