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
#include "DateTime.hpp"
#include <sstream>

using namespace Salsabil;

TEST_CASE("DateTimeTest") {

    SUBCASE("IsInvalidIfDefaultConstructed") {
        DateTime dt;
        CHECK_FALSE(dt.isValid());
        CHECK(dt.year() == 0);
        CHECK(dt.month() == 0);
        CHECK(dt.day() == 0);
        CHECK(dt.hour() == 0);
        CHECK(dt.minute() == 0);
        CHECK(dt.second() == 0);
        CHECK(dt.millisecond() == 0);
        CHECK(dt.microsecond() == 0);
        CHECK(dt.nanosecond() == 0);
    }

    SUBCASE("ReturnsCurrentDateTime") {
        DateTime dt = DateTime::current();
        std::time_t tTime = std::time(nullptr);
        std::tm* tmTime = std::gmtime(&tTime);

        CHECK(dt.year() == tmTime->tm_year + 1900);
        CHECK(dt.month() == tmTime->tm_mon + 1);
        CHECK(dt.day() == tmTime->tm_mday);
        CHECK(dt.hour() == tmTime->tm_hour);
        CHECK(dt.minute() == tmTime->tm_min);
        CHECK(dt.second() == tmTime->tm_sec);
        // can not be tested
        //    CHECK(dt.millisecond() == 0);
        //    CHECK(dt.microsecond() == 0);
        //    CHECK(dt.nanosecond() == 0);
    }

    SUBCASE("ReturnsEpoch") {
        CHECK(DateTime::epoch() == DateTime(Date(1970, 1, 1), Time(0, 0, 0)));
    }

    SUBCASE("ConstructsFromDateAndTime") {
        DateTime dt(Date(1999, 12, 1), Time(8, 55, 21, Time::Nanoseconds(123456789)));
        CHECK(dt.year() == 1999);
        CHECK(dt.month() == 12);
        CHECK(dt.day() == 1);
        CHECK(dt.hour() == 8);
        CHECK(dt.minute() == 55);
        CHECK(dt.second() == 21);
        CHECK(dt.millisecond() == 123);
        CHECK(dt.microsecond() == 123456);
        CHECK(dt.nanosecond() == 123456789);
    }

    SUBCASE("TestsComparisons") {
        CHECK(DateTime(Date(2012, 3, 27), Time(8, 55, 21, Time::Nanoseconds(123456789))) < DateTime(Date(2017, 3, 27), Time(8, 55, 21, Time::Nanoseconds(123456789))));
        CHECK(DateTime(Date(2012, 3, 27), Time(8, 55, 21, Time::Nanoseconds(123456789))) <= DateTime(Date(2017, 3, 27), Time(8, 55, 21, Time::Nanoseconds(123456789))));
        CHECK(DateTime(Date(2012, 3, 27), Time(8, 55, 21, Time::Nanoseconds(123456789))) <= DateTime(Date(2012, 3, 27), Time(8, 55, 21, Time::Nanoseconds(123456789))));
        CHECK(DateTime(Date(2012, 9, 27), Time(8, 55, 21, Time::Nanoseconds(123456789))) > DateTime(Date(2012, 3, 27), Time(8, 55, 21, Time::Nanoseconds(123456789))));
        CHECK(DateTime(Date(2012, 3, 27), Time(9, 55, 21, Time::Nanoseconds(123456789))) >= DateTime(Date(2012, 3, 27), Time(8, 55, 21, Time::Nanoseconds(123456789))));
        CHECK(DateTime(Date(2015, 3, 27), Time(1, 55, 21, Time::Nanoseconds(123456789))) >= DateTime(Date(2015, 3, 27), Time(1, 55, 21, Time::Nanoseconds(123456789))));
        CHECK(DateTime(Date(2012, 3, 27), Time(8, 55, 21, Time::Nanoseconds(123456789))) == DateTime(Date(2012, 3, 27), Time(8, 55, 21, Time::Nanoseconds(123456789))));
        CHECK(DateTime(Date(2017, 12, 6), Time(16, 32, 4, Time::Nanoseconds(987654321))) != DateTime(Date(2012, 3, 27), Time(8, 55, 21, Time::Nanoseconds(123456789))));
    }

    SUBCASE("AddsDuration") {
        // Adding time durations which don't affect the date part.
        CHECK(DateTime(Date(2045, 3, 27), Time(16, 2, 3, 4)).addDuration(DateTime::Minutes(10)) == DateTime(Date(2045, 3, 27), Time(16, 12, 3, 4)));
        CHECK(DateTime(Date(2045, 3, 27), Time(1, 2, 3, 4)).addDuration(DateTime::Hours(3)) == DateTime(Date(2045, 3, 27), Time(4, 2, 3, 4)));
        // Adding time durations which affect the date part.
        CHECK(DateTime(Date(2045, 3, 27), Time(16, 2, 3, 4)).addDuration(DateTime::Hours(10)) == DateTime(Date(2045, 3, 28), Time(2, 2, 3, 4)));
        // Adding date durations which only affect the date part.
        CHECK(DateTime(Date(2045, 3, 27), Time(1, 2, 3, 4)).addDuration(DateTime::Days(3)) == DateTime(Date(2045, 3, 30), Time(1, 2, 3, 4)));
        // Adding a negative duration
        CHECK(DateTime(Date(2045, 3, 27), Time(1, 2, 3, 4)).addDuration(-DateTime::Days(3)) == DateTime(Date(2045, 3, 24), Time(1, 2, 3, 4)));
    }

    SUBCASE("SubtractsDuration") {
        // Subtracting time durations which don't affect the date part.
        CHECK(DateTime(Date(2045, 3, 27), Time(16, 2, 3, 4)).subtractDuration(DateTime::Minutes(10)) == DateTime(Date(2045, 3, 27), Time(15, 52, 3, 4)));
        CHECK(DateTime(Date(2045, 3, 27), Time(7, 2, 3, 4)).subtractDuration(DateTime::Hours(3)) == DateTime(Date(2045, 3, 27), Time(4, 2, 3, 4)));
        // Subtracting time durations which affect the date part.
        CHECK(DateTime(Date(2045, 3, 27), Time(6, 2, 3, 4)).subtractDuration(DateTime::Hours(10)) == DateTime(Date(2045, 3, 26), Time(20, 2, 3, 4)));
        // Subtracting date durations which only affect the date part.
        CHECK(DateTime(Date(2045, 3, 27), Time(1, 2, 3, 4)).subtractDuration(DateTime::Days(3)) == DateTime(Date(2045, 3, 24), Time(1, 2, 3, 4)));
        // Subtracting a negative duration
        CHECK(DateTime(Date(2045, 3, 27), Time(1, 2, 3, 4)).subtractDuration(-DateTime::Days(3)) == DateTime(Date(2045, 3, 30), Time(1, 2, 3, 4)));
    }

    SUBCASE("TestsAdditionSubtractionOperators") {
        CHECK((DateTime(Date(2017, 3, 27), Time(22, 19, 53, 4)) - DateTime(Date(2017, 3, 26), Time(22, 12, 53, 4))) == DateTime::Days(1) + DateTime::Minutes(7));
        CHECK((DateTime(Date(2017, 12, 29), Time(23, 12, 53, 4)) + DateTime::Hours(35)) == DateTime(Date(2017, 12, 31), Time(10, 12, 53, 4)));
        CHECK((DateTime(Date(2017, 12, 31), Time(10, 12, 53, 4)) - DateTime::Hours(35)) == DateTime(Date(2017, 12, 29), Time(23, 12, 53, 4)));
    }

    SUBCASE("FormatDateTime") {
        CHECK(DateTime().toString("d/M/yyyy, hh:mm:ss.fffffffff") == "");
        CHECK(DateTime(Date(1999, 5, 18), Time(23, 55, 57, Time::Nanoseconds(123456789))).toString("d/M/yyyy, hh:mm:ss.fffffffff") == "18/5/1999, 23:55:57.123456789");
    }

    SUBCASE("ConstructsFromString") {
        CHECK(DateTime::fromString("18/5/1999, 23:55:57.123456789", "d/M/yyyy, hh:mm:ss.fffffffff") == DateTime(Date(1999, 5, 18), Time(23, 55, 57, Time::Nanoseconds(123456789))));
    }

    SUBCASE("ToJulainDay") {
        CHECK(DateTime(Date(-4714, 11, 24), Time(12, 0, 0)).toJulianDay() == doctest::Approx(0).epsilon(0.0000000001));
        CHECK(DateTime(Date(-4714, 11, 26), Time(0, 0, 0)).toJulianDay() == doctest::Approx(1.5).epsilon(0.0000000001));
        CHECK(DateTime(Date(2017, 12, 31), Time(00, 9, 35)).toJulianDay() == doctest::Approx(2458118.506655093).epsilon(0.0000000001));
    }

    SUBCASE("FromJulainDay") {
        CHECK(DateTime::fromJulianDay(0) == DateTime(Date(-4714, 11, 24), Time(12, 0, 0)));
        CHECK(DateTime::fromJulianDay(1.5) == DateTime(Date(-4714, 11, 26), Time(0, 0, 0)));
        CHECK(DateTime::fromJulianDay(2458118.506655093) == DateTime(Date(2017, 12, 31), Time(0, 9, 35)));
    }

    SUBCASE("DifferenceBetweenTwoDates") {
        CHECK(DateTime::nanosecondsBetween(DateTime(Date(2017, 1, 15), Time(12, 45, 36, DateTime::Nanoseconds(123001013))), DateTime(Date(2017, 1, 15), Time(12, 45, 37))) == 876998987);
        CHECK(DateTime::microsecondsBetween(DateTime(Date(2017, 1, 15), Time(12, 45, 36, DateTime::Microseconds(123001))), DateTime(Date(2017, 1, 15), Time(12, 45, 37))) == 876999);
        CHECK(DateTime::millisecondsBetween(DateTime(Date(2017, 1, 15), Time(12, 45, 36, 123)), DateTime(Date(2017, 1, 15), Time(12, 45, 37))) == 877);
        CHECK(DateTime::secondsBetween(DateTime(Date(2002, 1, 1), Time(15, 45, 36)), DateTime(Date(2002, 1, 2), Time(15, 2, 37))) == 83821);
        CHECK(DateTime::minutesBetween(DateTime(Date(2000, 1, 2), Time(23, 45, 36)), DateTime(Date(2000, 1, 2), Time(12, 2, 36))) == 703);
        CHECK(DateTime::hoursBetween(DateTime(Date(1998, 1, 1), Time(23, 45, 36)), DateTime(Date(1998, 1, 2), Time(12, 2, 36))) == 12);
        CHECK(DateTime::daysBetween(DateTime(Date(1970, 1, 1), Time(23, 2, 36)), DateTime(Date(1971, 1, 1), Time(23, 2, 36))) == 365);
    }

    SUBCASE("SerializesDeserializes") {
        DateTime dt;
        std::stringstream ss;
        ss << DateTime(Date(2014, 11, 9), Time(2, 44, 21, 987));
        ss >> dt;
        CHECK(dt == DateTime(Date(2014, 11, 9), Time(2, 44, 21, 987)));
    }
}