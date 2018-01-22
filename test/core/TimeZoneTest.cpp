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

#include "gmock/gmock.h"
#include "TimeZone.hpp"
#include "DateTime.hpp"
#include "date/tz.h"
#include "Exception.hpp"

using namespace ::testing;
using namespace Salsabil;

TEST(TimeZoneTest, IsInvalidIfDefaultConstructed) {
    TimeZone tz;
    ASSERT_FALSE(tz.isValid());
    ASSERT_THAT(tz.id(), StrEq(""));
    ASSERT_THAT(tz.abbreviationAt(DateTime()), StrEq(""));
    ASSERT_THAT(tz.offsetAt(DateTime()), Eq(std::chrono::seconds(0)));
    ASSERT_THAT(tz.standardOffsetAt(DateTime()), Eq(std::chrono::seconds(0)));
    ASSERT_THAT(tz.daylightOffsetAt(DateTime()), Eq(std::chrono::minutes(0)));
    ASSERT_FALSE(tz.transitionBefore(DateTime()).isValid());
    ASSERT_FALSE(tz.transitionAfter(DateTime()).isValid());
}

TEST(TimeZoneTest, ThrowsIfTimeZoneNotFound) {
    ASSERT_THROW(TimeZone("Disney/Mickey_Mouse"), Exception);
}

TEST(TimeZoneTest, ReturnsTimeZoneId) {
    TimeZone tz("Europe/Istanbul");
    ASSERT_THAT(tz.id(), Eq("Europe/Istanbul"));
}

TEST(TimeZoneTest, ReturnsAbbreviationGivenTimeZone) {
    TimeZone tz("Europe/Berlin");
    ASSERT_THAT(tz.abbreviationAt(DateTime()), Eq(""));
    ASSERT_THAT(tz.abbreviationAt(DateTime(Date(2018, 1, 6), Time(20, 52, 14))), Eq("CET"));
    // additional tests
    tz = TimeZone("America/Tijuana");
    ASSERT_THAT(tz.abbreviationAt(DateTime(Date(1921, 1, 1), Time(0, 0, 0))), Eq("LMT"));
    ASSERT_THAT(tz.abbreviationAt(DateTime(Date(1922, 1, 1), Time(7, 59, 59))), Eq("LMT"));
    ASSERT_THAT(tz.abbreviationAt(DateTime(Date(1922, 1, 1), Time(8, 0, 0))), Eq("MST"));
    ASSERT_THAT(tz.abbreviationAt(DateTime(Date(1924, 1, 1), Time(6, 59, 59))), Eq("MST"));
    ASSERT_THAT(tz.abbreviationAt(DateTime(Date(1924, 1, 1), Time(7, 0, 0))), Eq("PST"));
    ASSERT_THAT(tz.abbreviationAt(DateTime(Date(1927, 6, 11), Time(6, 59, 59))), Eq("PST"));
    ASSERT_THAT(tz.abbreviationAt(DateTime(Date(1927, 6, 11), Time(7, 0, 0))), Eq("MST"));
}

TEST(TimeZoneTest, ReturnsOffsetAtGivenDateTimeInTimeZone) {
    ASSERT_THAT(TimeZone("Etc/GMT-3").offsetAt(DateTime(Date(2018, 1, 13), Time(18, 56, 10))), Eq(std::chrono::hours(3)));
    ASSERT_THAT(TimeZone("Etc/GMT+3").offsetAt(DateTime(Date(2018, 1, 13), Time(18, 56, 10))), Eq(std::chrono::hours(-3)));

    TimeZone tz("Europe/Istanbul");
    ASSERT_THAT(tz.offsetAt(DateTime()), Eq(std::chrono::seconds(0))); // daylight saving
    ASSERT_THAT(tz.offsetAt(DateTime(Date(2018, 1, 6), Time(21, 47, 14))), Eq(std::chrono::hours(3)));
    ASSERT_THAT(tz.offsetAt(DateTime(Date(2018, 8, 1), Time(21, 47, 14))), Eq(std::chrono::hours(3))); // no daylight saving
    ASSERT_THAT(tz.offsetAt(DateTime(Date(2016, 1, 5), Time(21, 0, 0))), Eq(std::chrono::hours(2)));
    ASSERT_THAT(tz.offsetAt(DateTime(Date(2016, 8, 1), Time(21, 0, 0))), Eq(std::chrono::hours(3))); // daylight saving
}

TEST(TimeZoneTest, ReturnsDaylightOffsetAtGivenDateTimeInTimeZone) {
    TimeZone tz("Europe/Istanbul");
    ASSERT_THAT(tz.daylightOffsetAt(DateTime()), Eq(std::chrono::seconds(0)));
    ASSERT_THAT(tz.daylightOffsetAt(DateTime(Date(1964, 5, 15), Time(0, 0, 0))), Eq(std::chrono::minutes(60)));
    // additional tests
    ASSERT_THAT(tz.daylightOffsetAt(DateTime(Date(1973, 11, 4), Time(3, 0, 0))), Eq(std::chrono::minutes(0)));
    ASSERT_THAT(tz.daylightOffsetAt(DateTime(Date(2000, 3, 25), Time(22, 59, 59))), Eq(std::chrono::minutes(0)));
    ASSERT_THAT(tz.daylightOffsetAt(DateTime(Date(2000, 3, 25), Time(23, 0, 0))), Eq(std::chrono::minutes(60)));
    ASSERT_THAT(tz.daylightOffsetAt(DateTime(Date(2000, 10, 28), Time(22, 59, 59))), Eq(std::chrono::minutes(60)));
    ASSERT_THAT(tz.daylightOffsetAt(DateTime(Date(2000, 10, 28), Time(23, 0, 0))), Eq(std::chrono::minutes(0)));
    ASSERT_THAT(tz.daylightOffsetAt(DateTime(Date(2018, 1, 6), Time(21, 47, 14))), Eq(std::chrono::minutes(0)));
    ASSERT_THAT(tz.daylightOffsetAt(DateTime(Date(2018, 8, 1), Time(21, 47, 14))), Eq(std::chrono::minutes(0))); //no daylight saving
}

TEST(TimeZoneTest, ReturnsStandardOffsetAtGivenDateTimeInTimeZone) {
    TimeZone tz("Europe/Istanbul");
    ASSERT_THAT(tz.standardOffsetAt(DateTime()), Eq(std::chrono::seconds(0)));
    ASSERT_THAT(tz.standardOffsetAt(DateTime(Date(2018, 1, 6), Time(21, 47, 14))), Eq(std::chrono::hours(3)));
    ASSERT_THAT(tz.standardOffsetAt(DateTime(Date(2018, 8, 1), Time(21, 47, 14))), Eq(std::chrono::hours(3))); // no daylight saving
    ASSERT_THAT(tz.standardOffsetAt(DateTime(Date(2016, 1, 5), Time(21, 0, 0))), Eq(std::chrono::hours(2)));
    ASSERT_THAT(tz.standardOffsetAt(DateTime(Date(2016, 8, 1), Time(21, 0, 0))), Eq(std::chrono::hours(2))); // daylight saving
}

TEST(TimeZoneTest, ReturnsPreviousAndNextTransition) {
    TimeZone tz("Europe/Istanbul");
    ASSERT_FALSE(tz.transitionBefore(DateTime()).isValid());
    ASSERT_THAT(tz.transitionBefore(DateTime(Date(2005, 1, 6), Time(21, 47, 14))), Eq(DateTime(Date(2004, 10, 30), Time(23, 0, 0))));
    ASSERT_FALSE(tz.transitionAfter(DateTime()).isValid());
    ASSERT_THAT(tz.transitionAfter(DateTime(Date(2005, 1, 6), Time(21, 47, 14))), Eq(DateTime(Date(2005, 3, 26), Time(23, 0, 0))));
}

TEST(TimeZoneTest, TestsEquality) {
    ASSERT_TRUE(TimeZone("Europe/Istanbul") == TimeZone("Europe/Istanbul"));
    ASSERT_TRUE(TimeZone("Europe/Berlin") != TimeZone("Europe/Istanbul"));
}

TEST(TimeZoneTest, ReturnsUTCTimeZone) {
    TimeZone tz = TimeZone::utc();
    ASSERT_TRUE(tz.isValid());
    ASSERT_THAT(tz.id(), StrEq("Etc/UTC"));
    ASSERT_THAT(tz.abbreviationAt(DateTime(Date(2018, 1, 11), Time(0, 17, 0))), StrEq("UTC"));
    ASSERT_THAT(tz.offsetAt(DateTime(Date(2018, 1, 11), Time(0, 17, 0))), Eq(std::chrono::seconds::zero()));
    ASSERT_THAT(tz.standardOffsetAt(DateTime(Date(2018, 1, 11), Time(0, 17, 0))), Eq(std::chrono::seconds::zero()));
    ASSERT_THAT(tz.daylightOffsetAt(DateTime(Date(2018, 1, 11), Time(0, 17, 0))), Eq(std::chrono::seconds::zero()));
    ASSERT_FALSE(tz.transitionBefore(DateTime(Date(2018, 1, 11), Time(0, 17, 0))).isValid());
    ASSERT_FALSE(tz.transitionAfter(DateTime(Date(2018, 1, 11), Time(0, 17, 0))).isValid());
}

TEST(TimeZoneTest, isTimeZoneAvailable) {
    ASSERT_TRUE(TimeZone::isAvailable("Europe/Istanbul"));
    ASSERT_TRUE(TimeZone::isAvailable("Europe/Berlin"));
    ASSERT_FALSE(TimeZone::isAvailable("Disney/Mickey_Mouse"));
}

TEST(TimeZoneTest, ReturnsDatabaseVersion) {
    ASSERT_FALSE(TimeZone::databaseVersion().empty());
}

TEST(TimeZoneTest, ReturnsWindowsId) {
    ASSERT_THAT(TimeZone::toWindowsId("Europe/Istanbul"), StrEq("Turkey Standard Time"));
}

TEST(TimeZoneTest, ReturnsIanaIdFromWindowsId) {
    ASSERT_THAT(TimeZone::toIanaId("W. Europe Standard Time"), StrEq("Europe/Berlin"));
    ASSERT_THAT(TimeZone::toIanaId("W. Europe Standard Time", "IT"), StrEq("Europe/Rome"));
}