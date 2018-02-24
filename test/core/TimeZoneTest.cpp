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

#include "doctest.h"
#include "TimeZone.hpp"
#include "DateTime.hpp"
#include "date/tz.h"
#include "Exception.hpp"

using namespace Salsabil;

TEST_CASE("TimeZoneTest") {

    SUBCASE("IsInvalidIfDefaultConstructed") {
        TimeZone tz;
        CHECK_FALSE(tz.isValid());
        CHECK(tz.id() == "");

        CHECK(tz.abbreviationAt(DateTime()) == "");
        CHECK(tz.offsetAt(DateTime()) == std::chrono::seconds(0));
        CHECK(tz.standardOffsetAt(DateTime()) == std::chrono::seconds(0));
        CHECK(tz.daylightOffsetAt(DateTime()) == std::chrono::minutes(0));
        CHECK_FALSE(tz.transitionBefore(DateTime()).isValid());
        CHECK_FALSE(tz.transitionAfter(DateTime()).isValid());
    }

    SUBCASE("ThrowsIfTimeZoneNotFound") {

        CHECK_THROWS_AS(TimeZone("Disney/Mickey_Mouse"), Exception);
    }

    SUBCASE("ReturnsTimeZoneId") {

        TimeZone tz("Europe/Istanbul");
        CHECK(tz.id() == "Europe/Istanbul");
    }

    SUBCASE("ReturnsAbbreviationGivenTimeZone") {

        TimeZone tz("Europe/Berlin");

        CHECK(tz.abbreviationAt(DateTime()) == "");
        CHECK(tz.abbreviationAt(DateTime(Date(2018, 1, 6), Time(20, 52, 14))) == "CET");
        // additional tests
        tz = TimeZone("America/Tijuana");
        CHECK(tz.abbreviationAt(DateTime(Date(1921, 1, 1), Time(0, 0, 0))) == "LMT");
        CHECK(tz.abbreviationAt(DateTime(Date(1922, 1, 1), Time(7, 59, 59))) == "LMT");
        CHECK(tz.abbreviationAt(DateTime(Date(1922, 1, 1), Time(8, 0, 0))) == "MST");
        CHECK(tz.abbreviationAt(DateTime(Date(1924, 1, 1), Time(6, 59, 59))) == "MST");
        CHECK(tz.abbreviationAt(DateTime(Date(1924, 1, 1), Time(7, 0, 0))) == "PST");
        CHECK(tz.abbreviationAt(DateTime(Date(1927, 6, 11), Time(6, 59, 59))) == "PST");
        CHECK(tz.abbreviationAt(DateTime(Date(1927, 6, 11), Time(7, 0, 0))) == "MST");
    }

    SUBCASE("ReturnsOffsetAtGivenDateTimeInTimeZone") {

        CHECK(TimeZone("Etc/GMT-3").offsetAt(DateTime(Date(2018, 1, 13), Time(18, 56, 10))) == std::chrono::hours(3));
        CHECK(TimeZone("Etc/GMT+3").offsetAt(DateTime(Date(2018, 1, 13), Time(18, 56, 10))) == std::chrono::hours(-3));

        TimeZone tz("Europe/Istanbul");

        CHECK(tz.offsetAt(DateTime()) == std::chrono::seconds(0)); // daylight saving
        CHECK(tz.offsetAt(DateTime(Date(2018, 1, 6), Time(21, 47, 14))) == std::chrono::hours(3));
        CHECK(tz.offsetAt(DateTime(Date(2018, 8, 1), Time(21, 47, 14))) == std::chrono::hours(3)); // no daylight saving
        CHECK(tz.offsetAt(DateTime(Date(2016, 1, 5), Time(21, 0, 0))) == std::chrono::hours(2));
        CHECK(tz.offsetAt(DateTime(Date(2016, 8, 1), Time(21, 0, 0))) == std::chrono::hours(3)); // daylight saving
    }

    SUBCASE("ReturnsDaylightOffsetAtGivenDateTimeInTimeZone") {

        TimeZone tz("Europe/Istanbul");
        CHECK(tz.daylightOffsetAt(DateTime()) == std::chrono::seconds(0));
        CHECK(tz.daylightOffsetAt(DateTime(Date(1964, 5, 15), Time(0, 0, 0))) == std::chrono::minutes(60));
        // additional tests
        CHECK(tz.daylightOffsetAt(DateTime(Date(1973, 11, 4), Time(3, 0, 0))) == std::chrono::minutes(0));
        CHECK(tz.daylightOffsetAt(DateTime(Date(2000, 3, 25), Time(22, 59, 59))) == std::chrono::minutes(0));
        CHECK(tz.daylightOffsetAt(DateTime(Date(2000, 3, 25), Time(23, 0, 0))) == std::chrono::minutes(60));
        CHECK(tz.daylightOffsetAt(DateTime(Date(2000, 10, 28), Time(22, 59, 59))) == std::chrono::minutes(60));
        CHECK(tz.daylightOffsetAt(DateTime(Date(2000, 10, 28), Time(23, 0, 0))) == std::chrono::minutes(0));
        CHECK(tz.daylightOffsetAt(DateTime(Date(2018, 1, 6), Time(21, 47, 14))) == std::chrono::minutes(0));
        CHECK(tz.daylightOffsetAt(DateTime(Date(2018, 8, 1), Time(21, 47, 14))) == std::chrono::minutes(0)); //no daylight saving
    }

    SUBCASE("ReturnsStandardOffsetAtGivenDateTimeInTimeZone") {

        TimeZone tz("Europe/Istanbul");
        CHECK(tz.standardOffsetAt(DateTime()) == std::chrono::seconds(0));
        CHECK(tz.standardOffsetAt(DateTime(Date(2018, 1, 6), Time(21, 47, 14))) == std::chrono::hours(3));
        CHECK(tz.standardOffsetAt(DateTime(Date(2018, 8, 1), Time(21, 47, 14))) == std::chrono::hours(3)); // no daylight saving
        CHECK(tz.standardOffsetAt(DateTime(Date(2016, 1, 5), Time(21, 0, 0))) == std::chrono::hours(2));
        CHECK(tz.standardOffsetAt(DateTime(Date(2016, 8, 1), Time(21, 0, 0))) == std::chrono::hours(2)); // daylight saving
    }

    SUBCASE("ReturnsPreviousAndNextTransition") {

        TimeZone tz("Europe/Istanbul");

        CHECK_FALSE(tz.transitionBefore(DateTime()).isValid());
        CHECK(tz.transitionBefore(DateTime(Date(2005, 1, 6), Time(21, 47, 14))) == DateTime(Date(2004, 10, 30), Time(23, 0, 0)));
        CHECK_FALSE(tz.transitionAfter(DateTime()).isValid());
        CHECK(tz.transitionAfter(DateTime(Date(2005, 1, 6), Time(21, 47, 14))) == DateTime(Date(2005, 3, 26), Time(23, 0, 0)));
    }

    SUBCASE("TestsEquality") {

        CHECK(TimeZone("Europe/Istanbul") == TimeZone("Europe/Istanbul"));
        CHECK(TimeZone("Europe/Berlin") != TimeZone("Europe/Istanbul"));
    }

    SUBCASE("ReturnsUTCTimeZone") {

        TimeZone tz = TimeZone::utc();
        CHECK(tz.isValid());
        CHECK(tz.id() == "Etc/UTC");
        CHECK(tz.abbreviationAt(DateTime(Date(2018, 1, 11), Time(0, 17, 0))) == "UTC");
        CHECK(tz.offsetAt(DateTime(Date(2018, 1, 11), Time(0, 17, 0))) == std::chrono::seconds::zero());
        CHECK(tz.standardOffsetAt(DateTime(Date(2018, 1, 11), Time(0, 17, 0))) == std::chrono::seconds::zero());
        CHECK(tz.daylightOffsetAt(DateTime(Date(2018, 1, 11), Time(0, 17, 0))) == std::chrono::seconds::zero());
        CHECK_FALSE(tz.transitionBefore(DateTime(Date(2018, 1, 11), Time(0, 17, 0))).isValid());
        CHECK_FALSE(tz.transitionAfter(DateTime(Date(2018, 1, 11), Time(0, 17, 0))).isValid());
    }

    SUBCASE("isTimeZoneAvailable") {

        CHECK(TimeZone::isAvailable("Europe/Istanbul"));
        CHECK(TimeZone::isAvailable("Europe/Berlin"));
        CHECK_FALSE(TimeZone::isAvailable("Disney/Mickey_Mouse"));
    }

    SUBCASE("ReturnsDatabaseVersion") {

        CHECK_FALSE(TimeZone::databaseVersion().empty());
    }

    SUBCASE("ReturnsWindowsId") {

        CHECK(TimeZone::toWindowsId("Europe/Istanbul") == "Turkey Standard Time");
    }

    SUBCASE("ReturnsIanaIdFromWindowsId") {
        CHECK(TimeZone::toIanaId("W. Europe Standard Time") == "Europe/Berlin");
        CHECK(TimeZone::toIanaId("W. Europe Standard Time", "IT") == "Europe/Rome");
    }
}