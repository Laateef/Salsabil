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
#include "LocalDateTime.hpp"
#include <sstream>

using namespace Salsabil;

TEST_CASE("LocalDateTimeTest") {

    SUBCASE("IsInvalidIfDefaultConstructed") {
        LocalDateTime ldt;

        CHECK_FALSE(ldt.isValid());
        CHECK_FALSE(ldt.timeZone().isValid());
        CHECK_FALSE(ldt.dateTime().isValid());
        CHECK_FALSE(ldt.date().isValid());
        CHECK_FALSE(ldt.time().isValid());

        CHECK(ldt.year() == 0);
        CHECK(ldt.month() == 0);
        CHECK(ldt.day() == 0);
        CHECK(ldt.hour() == 0);
        CHECK(ldt.minute() == 0);
        CHECK(ldt.second() == 0);
        CHECK(ldt.millisecond() == 0);
        CHECK(ldt.microsecond() == 0);
        CHECK(ldt.nanosecond() == 0);
    }

    SUBCASE("ReturnsDateTimePart") {
        CHECK(LocalDateTime(DateTime(Date(1998, 3, 1), Time(23, 4, 19)), TimeZone("Etc/GMT+3")).dateTime() == DateTime(Date(1998, 3, 1), Time(23, 4, 19)));
    }

    SUBCASE("ReturnsTimeZonePart") {
        CHECK(LocalDateTime(DateTime(Date(1998, 3, 1), Time(22, 4, 19)), TimeZone("Etc/GMT+3")).timeZone() == TimeZone("Etc/GMT+3"));
    }

    SUBCASE("TestsComparisons") {
        DateTime dt1(Date(2012, 3, 27), Time(8, 55, 21));
        DateTime dt2(Date(2012, 3, 27), Time(11, 55, 21));

        CHECK(LocalDateTime(dt1, TimeZone("Etc/GMT-9")) < LocalDateTime(dt2, TimeZone("Etc/GMT-9")));
        CHECK(LocalDateTime(dt1, TimeZone("Etc/GMT-6")) < LocalDateTime(dt1, TimeZone("Etc/GMT-3")));
        CHECK(LocalDateTime(dt1, TimeZone("Etc/GMT-6")) < LocalDateTime(dt2, TimeZone("Etc/GMT-3")));

        CHECK(LocalDateTime(dt1, TimeZone("Etc/GMT+9")) <= LocalDateTime(dt2, TimeZone("Etc/GMT+9")));
        CHECK(LocalDateTime(dt1, TimeZone("Etc/GMT+6")) <= LocalDateTime(dt1, TimeZone("Etc/GMT+3")));
        CHECK(LocalDateTime(dt1, TimeZone("Etc/GMT+6")) <= LocalDateTime(dt2, TimeZone("Etc/GMT+3")));
        CHECK(LocalDateTime(dt1, TimeZone("Etc/GMT-1")) <= LocalDateTime(dt1, TimeZone("Etc/GMT-1")));
        CHECK(LocalDateTime(dt2, TimeZone("Etc/GMT-1")) <= LocalDateTime(dt1, TimeZone("Etc/GMT-4")));

        CHECK(LocalDateTime(dt2, TimeZone("Etc/GMT+9")) > LocalDateTime(dt1, TimeZone("Etc/GMT+9")));
        CHECK(LocalDateTime(dt1, TimeZone("Etc/GMT+6")) > LocalDateTime(dt1, TimeZone("Etc/GMT+3")));
        CHECK(LocalDateTime(dt2, TimeZone("Etc/GMT-3")) > LocalDateTime(dt1, TimeZone("Etc/GMT-6")));

        CHECK(LocalDateTime(dt2, TimeZone("Etc/GMT+9")) >= LocalDateTime(dt1, TimeZone("Etc/GMT+9")));
        CHECK(LocalDateTime(dt1, TimeZone("Etc/GMT+3")) >= LocalDateTime(dt1, TimeZone("Etc/GMT+6")));
        CHECK(LocalDateTime(dt2, TimeZone("Etc/GMT+3")) >= LocalDateTime(dt1, TimeZone("Etc/GMT+6")));
        CHECK(LocalDateTime(dt1, TimeZone("Etc/GMT-1")) >= LocalDateTime(dt1, TimeZone("Etc/GMT-1")));
        CHECK(LocalDateTime(dt2, TimeZone("Etc/GMT-2")) >= LocalDateTime(dt1, TimeZone("Etc/GMT-6")));

        CHECK(LocalDateTime(dt1, TimeZone("Etc/GMT-5")) == LocalDateTime(dt1, TimeZone("Etc/GMT-5")));
        CHECK(LocalDateTime(dt2, TimeZone("Etc/GMT+2")) == LocalDateTime(dt1, TimeZone("Etc/GMT+5")));

        CHECK(LocalDateTime(dt1, TimeZone("Etc/GMT+9")) != LocalDateTime(dt1, TimeZone("Etc/GMT-9")));
        CHECK(LocalDateTime(dt1, TimeZone("Etc/GMT+2")) != LocalDateTime(dt2, TimeZone("Etc/GMT+2")));
    }

    SUBCASE("ReturnsUtcLocalDateTime") {
        CHECK(LocalDateTime(DateTime(Date(2018, 1, 13), Time(19, 8, 20)), TimeZone("Europe/Istanbul")).toUtc() == LocalDateTime(DateTime(Date(2018, 1, 13), Time(16, 8, 20)), TimeZone("Etc/UTC")));
        CHECK(LocalDateTime(DateTime(Date(1998, 3, 1), Time(3, 4, 19)), TimeZone("Etc/GMT+3")).toUtc() == LocalDateTime(DateTime(Date(1998, 3, 1), Time(6, 4, 19)), TimeZone("Etc/UTC")));
    }

    SUBCASE("ReturnsCurrentLocalDateTime") {
        LocalDateTime ldt = LocalDateTime::current();
        std::time_t tTime = std::time(nullptr);
        std::tm* tmTime = std::localtime(&tTime);

        CHECK(ldt.year() == (tmTime->tm_year + 1900));
        CHECK(ldt.month() == (tmTime->tm_mon + 1));
        CHECK(ldt.day() == tmTime->tm_mday);
        CHECK(ldt.hour() == tmTime->tm_hour);
        CHECK(ldt.minute() == tmTime->tm_min);
        CHECK(ldt.second() == tmTime->tm_sec);
        // can not be tested
        //    CHECK(ldt.millisecond() == 0);
        //    CHECK(ldt.microsecond() == 0);
        //    CHECK(ldt.nanosecond() == 0);
    }

    SUBCASE("ReturnsOffsetFromUtc") {
        CHECK(LocalDateTime(DateTime(Date(1998, 3, 1), Time(23, 4, 19)), TimeZone("Etc/GMT+3")).offsetFromUtc() == LocalDateTime::Hours(-3));
        CHECK(LocalDateTime(DateTime(Date(1998, 3, 1), Time(23, 4, 19)), TimeZone("Etc/GMT-3")).offsetFromUtc() == LocalDateTime::Hours(3));
    }

    SUBCASE("RerutnsLocalDateTimeInAnotherZone") {
        CHECK(LocalDateTime(DateTime(Date(2018, 1, 13), Time(9, 6, 21)), TimeZone("Etc/GMT+2")).toTimeZone(TimeZone("Etc/GMT+4")) == LocalDateTime(DateTime(Date(2018, 1, 13), Time(7, 6, 21)), TimeZone("Etc/GMT+4")));
        CHECK(LocalDateTime(DateTime(Date(2018, 1, 13), Time(9, 6, 21)), TimeZone("Etc/GMT-2")).toTimeZone(TimeZone("Etc/GMT-4")) == LocalDateTime(DateTime(Date(2018, 1, 13), Time(11, 6, 21)), TimeZone("Etc/GMT-4")));
    }

    SUBCASE("FormatLocalDateTime") {
        CHECK(LocalDateTime().toString("d/M/yyyy, hh:mm:ss.fffffffff zz") == "");
        CHECK(LocalDateTime(DateTime(Date(2018, 1, 14), Time(14, 37, 40)), TimeZone("Europe/Istanbul")).toString("z") == "+0300");
        CHECK(LocalDateTime(DateTime(Date(2018, 1, 14), Time(14, 37, 40)), TimeZone("Europe/Istanbul")).toString("zz") == "+03:00");
        CHECK(LocalDateTime(DateTime(Date(2018, 1, 14), Time(14, 37, 40)), TimeZone("Europe/Istanbul")).toString("zzz") == "+03");
        CHECK(LocalDateTime(DateTime(Date(2018, 1, 14), Time(14, 37, 40)), TimeZone("Europe/Istanbul")).toString("zzzz") == "Europe/Istanbul");
    }

    SUBCASE("ReturnsLocalDateTimeFromString") {
        CHECK(LocalDateTime::fromString("21/1/2018, 14:18:34.762[Europe/Istanbul]", "d/M/yyyy, hh:mm:ss.fff[zzzz]") == LocalDateTime(DateTime(Date(2018, 1, 21), Time(14, 18, 34, 762)), TimeZone("Europe/Istanbul")));
        CHECK(LocalDateTime::fromString("21/1/2018, 14:18:34.762[America/Argentina/Buenos_Aires]", "d/M/yyyy, hh:mm:ss.fff[zzzz]") == LocalDateTime(DateTime(Date(2018, 1, 21), Time(14, 18, 34, 762)), TimeZone("America/Argentina/Buenos_Aires")));
        CHECK(LocalDateTime::fromString("21/1/2018, 14:18:34.762[America/Blanc-Sablon]", "d/M/yyyy, hh:mm:ss.fff[zzzz]") == LocalDateTime(DateTime(Date(2018, 1, 21), Time(14, 18, 34, 762)), TimeZone("America/Blanc-Sablon")));
        CHECK(LocalDateTime::fromString("21/1/2018, 14:18:34.762[Etc/GMT+1]", "d/M/yyyy, hh:mm:ss.fff[zzzz]") == LocalDateTime(DateTime(Date(2018, 1, 21), Time(14, 18, 34, 762)), TimeZone("Etc/GMT+1")));

        // ambiguous time zones cannot be parsed.
        CHECK(LocalDateTime::fromString("21/1/2018, 14:18:34.762 CEST", "d/M/yyyy, hh:mm:ss.fff zzz") == LocalDateTime(DateTime(Date(2018, 1, 21), Time(14, 18, 34, 762)), TimeZone()));
        CHECK(LocalDateTime::fromString("21/1/2018, 14:18:34.762UTC-10:00", "d/M/yyyy, hh:mm:ss.fffUTCzz") == LocalDateTime(DateTime(Date(2018, 1, 21), Time(14, 18, 34, 762)), TimeZone()));
        CHECK(LocalDateTime::fromString("21/1/2018, 14:18:34.762+0300", "d/M/yyyy, hh:mm:ss.fff z") == LocalDateTime(DateTime(Date(2018, 1, 21), Time(14, 18, 34, 762)), TimeZone()));
    }

    SUBCASE("SerializesDeserializes") {
        LocalDateTime ldt;
        std::stringstream ss;
        ss << LocalDateTime(DateTime(Date(2014, 11, 9), Time(2, 44, 21)), TimeZone("Europe/Istanbul"));
        ss >> ldt;
        CHECK(ldt == LocalDateTime(DateTime(Date(2014, 11, 9), Time(2, 44, 21)), TimeZone("Europe/Istanbul")));
    }
}