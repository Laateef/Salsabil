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
#include "LocalDateTime.hpp"

using namespace ::testing;
using namespace Salsabil;

TEST(LocalDateTimeTest, IsInvalidIfDefaultConstructed) {
    LocalDateTime ldt;

    ASSERT_FALSE(ldt.isValid());
    ASSERT_FALSE(ldt.timeZone().isValid());
    ASSERT_FALSE(ldt.dateTime().isValid());
    ASSERT_FALSE(ldt.date().isValid());
    ASSERT_FALSE(ldt.time().isValid());

    ASSERT_THAT(ldt.year(), Eq(0));
    ASSERT_THAT(ldt.month(), Eq(0));
    ASSERT_THAT(ldt.day(), Eq(0));
    ASSERT_THAT(ldt.hour(), Eq(0));
    ASSERT_THAT(ldt.minute(), Eq(0));
    ASSERT_THAT(ldt.second(), Eq(0));
    ASSERT_THAT(ldt.millisecond(), Eq(0));
    ASSERT_THAT(ldt.microsecond(), Eq(0));
    ASSERT_THAT(ldt.nanosecond(), Eq(0));
}

TEST(LocalDateTimeTest, ReturnsDateTimePart) {
    ASSERT_THAT(LocalDateTime(DateTime(Date(1998, 3, 1), Time(23, 4, 19)), TimeZone("Etc/GMT+3")).dateTime(), Eq(DateTime(Date(1998, 3, 1), Time(23, 4, 19))));
}

TEST(LocalDateTimeTest, ReturnsTimeZonePart) {
    ASSERT_THAT(LocalDateTime(DateTime(Date(1998, 3, 1), Time(22, 4, 19)), TimeZone("Etc/GMT+3")).timeZone(), Eq(TimeZone("Etc/GMT+3")));
}

TEST(LocalDateTimeTest, TestsComparisons) {
    DateTime dt1(Date(2012, 3, 27), Time(8, 55, 21));
    DateTime dt2(Date(2012, 3, 27), Time(11, 55, 21));

    ASSERT_TRUE(LocalDateTime(dt1, TimeZone("Etc/GMT-9")) < LocalDateTime(dt2, TimeZone("Etc/GMT-9")));
    ASSERT_TRUE(LocalDateTime(dt1, TimeZone("Etc/GMT-6")) < LocalDateTime(dt1, TimeZone("Etc/GMT-3")));
    ASSERT_TRUE(LocalDateTime(dt1, TimeZone("Etc/GMT-6")) < LocalDateTime(dt2, TimeZone("Etc/GMT-3")));

    ASSERT_TRUE(LocalDateTime(dt1, TimeZone("Etc/GMT+9")) <= LocalDateTime(dt2, TimeZone("Etc/GMT+9")));
    ASSERT_TRUE(LocalDateTime(dt1, TimeZone("Etc/GMT+6")) <= LocalDateTime(dt1, TimeZone("Etc/GMT+3")));
    ASSERT_TRUE(LocalDateTime(dt1, TimeZone("Etc/GMT+6")) <= LocalDateTime(dt2, TimeZone("Etc/GMT+3")));
    ASSERT_TRUE(LocalDateTime(dt1, TimeZone("Etc/GMT-1")) <= LocalDateTime(dt1, TimeZone("Etc/GMT-1")));
    ASSERT_TRUE(LocalDateTime(dt2, TimeZone("Etc/GMT-1")) <= LocalDateTime(dt1, TimeZone("Etc/GMT-4")));

    ASSERT_TRUE(LocalDateTime(dt2, TimeZone("Etc/GMT+9")) > LocalDateTime(dt1, TimeZone("Etc/GMT+9")));
    ASSERT_TRUE(LocalDateTime(dt1, TimeZone("Etc/GMT+6")) > LocalDateTime(dt1, TimeZone("Etc/GMT+3")));
    ASSERT_TRUE(LocalDateTime(dt2, TimeZone("Etc/GMT-3")) > LocalDateTime(dt1, TimeZone("Etc/GMT-6")));

    ASSERT_TRUE(LocalDateTime(dt2, TimeZone("Etc/GMT+9")) >= LocalDateTime(dt1, TimeZone("Etc/GMT+9")));
    ASSERT_TRUE(LocalDateTime(dt1, TimeZone("Etc/GMT+3")) >= LocalDateTime(dt1, TimeZone("Etc/GMT+6")));
    ASSERT_TRUE(LocalDateTime(dt2, TimeZone("Etc/GMT+3")) >= LocalDateTime(dt1, TimeZone("Etc/GMT+6")));
    ASSERT_TRUE(LocalDateTime(dt1, TimeZone("Etc/GMT-1")) >= LocalDateTime(dt1, TimeZone("Etc/GMT-1")));
    ASSERT_TRUE(LocalDateTime(dt2, TimeZone("Etc/GMT-2")) >= LocalDateTime(dt1, TimeZone("Etc/GMT-6")));

    ASSERT_TRUE(LocalDateTime(dt1, TimeZone("Etc/GMT-5")) == LocalDateTime(dt1, TimeZone("Etc/GMT-5")));
    ASSERT_TRUE(LocalDateTime(dt2, TimeZone("Etc/GMT+2")) == LocalDateTime(dt1, TimeZone("Etc/GMT+5")));

    ASSERT_TRUE(LocalDateTime(dt1, TimeZone("Etc/GMT+9")) != LocalDateTime(dt1, TimeZone("Etc/GMT-9")));
    ASSERT_TRUE(LocalDateTime(dt1, TimeZone("Etc/GMT+2")) != LocalDateTime(dt2, TimeZone("Etc/GMT+2")));
}

TEST(LocalDateTimeTest, ReturnsUtcLocalDateTime) {
    ASSERT_THAT(LocalDateTime(DateTime(Date(2018, 1, 13), Time(19, 8, 20)), TimeZone("Europe/Istanbul")).toUtc(), Eq(LocalDateTime(DateTime(Date(2018, 1, 13), Time(16, 8, 20)), TimeZone("Etc/UTC"))));
    ASSERT_THAT(LocalDateTime(DateTime(Date(1998, 3, 1), Time(3, 4, 19)), TimeZone("Etc/GMT+3")).toUtc(), Eq(LocalDateTime(DateTime(Date(1998, 3, 1), Time(6, 4, 19)), TimeZone("Etc/UTC"))));
}

TEST(LocalDateTimeTest, ReturnsCurrentLocalDateTime) {
    LocalDateTime ldt = LocalDateTime::current();
    std::time_t tTime = std::time(nullptr);
    std::tm* tmTime = std::localtime(&tTime);

    ASSERT_THAT(ldt.year(), Eq(tmTime->tm_year + 1900));
    ASSERT_THAT(ldt.month(), Eq(tmTime->tm_mon + 1));
    ASSERT_THAT(ldt.day(), Eq(tmTime->tm_mday));
    ASSERT_THAT(ldt.hour(), Eq(tmTime->tm_hour));
    ASSERT_THAT(ldt.minute(), Eq(tmTime->tm_min));
    ASSERT_THAT(ldt.second(), Eq(tmTime->tm_sec));
    // can not be tested
    //    ASSERT_THAT(ldt.millisecond(), Eq(0));
    //    ASSERT_THAT(ldt.microsecond(), Eq(0));
    //    ASSERT_THAT(ldt.nanosecond(), Eq(0));
}

TEST(LocalDateTimeTest, ReturnsOffsetFromUtc) {
    ASSERT_THAT(LocalDateTime(DateTime(Date(1998, 3, 1), Time(23, 4, 19)), TimeZone("Etc/GMT+3")).offsetFromUtc(), Eq(LocalDateTime::Hours(-3)));
    ASSERT_THAT(LocalDateTime(DateTime(Date(1998, 3, 1), Time(23, 4, 19)), TimeZone("Etc/GMT-3")).offsetFromUtc(), Eq(LocalDateTime::Hours(3)));
}

TEST(LocalDateTimeTest, RerutnsLocalDateTimeInAnotherZone) {
    ASSERT_THAT(LocalDateTime(DateTime(Date(2018, 1, 13), Time(9, 6, 21)), TimeZone("Etc/GMT+2")).toTimeZone(TimeZone("Etc/GMT+4")), Eq(LocalDateTime(DateTime(Date(2018, 1, 13), Time(7, 6, 21)), TimeZone("Etc/GMT+4"))));
    ASSERT_THAT(LocalDateTime(DateTime(Date(2018, 1, 13), Time(9, 6, 21)), TimeZone("Etc/GMT-2")).toTimeZone(TimeZone("Etc/GMT-4")), Eq(LocalDateTime(DateTime(Date(2018, 1, 13), Time(11, 6, 21)), TimeZone("Etc/GMT-4"))));
}

TEST(LocalDateTimeTest, FormatLocalDateTime) {
    ASSERT_THAT(LocalDateTime().toString("d/M/yyyy, hh:mm:ss.fffffffff zz"), StrEq(""));
    ASSERT_THAT(LocalDateTime(DateTime(Date(2018, 1, 14), Time(14, 37, 40)), TimeZone("Europe/Istanbul")).toString("z"), StrEq("+0300"));
    ASSERT_THAT(LocalDateTime(DateTime(Date(2018, 1, 14), Time(14, 37, 40)), TimeZone("Europe/Istanbul")).toString("zz"), StrEq("+03:00"));
    ASSERT_THAT(LocalDateTime(DateTime(Date(2018, 1, 14), Time(14, 37, 40)), TimeZone("Europe/Istanbul")).toString("zzz"), StrEq("+03"));
    ASSERT_THAT(LocalDateTime(DateTime(Date(2018, 1, 14), Time(14, 37, 40)), TimeZone("Europe/Istanbul")).toString("zzzz"), StrEq("Europe/Istanbul"));
}

TEST(LocalDateTimeTest, ReturnsLocalDateTimeFromString) {
    ASSERT_THAT(LocalDateTime::fromString("21/1/2018, 14:18:34.762[Europe/Istanbul]", "d/M/yyyy, hh:mm:ss.fff[zzzz]"), Eq(LocalDateTime(DateTime(Date(2018, 1, 21), Time(14, 18, 34, 762)), TimeZone("Europe/Istanbul"))));
    ASSERT_THAT(LocalDateTime::fromString("21/1/2018, 14:18:34.762[America/Argentina/Buenos_Aires]", "d/M/yyyy, hh:mm:ss.fff[zzzz]"), Eq(LocalDateTime(DateTime(Date(2018, 1, 21), Time(14, 18, 34, 762)), TimeZone("America/Argentina/Buenos_Aires"))));
    ASSERT_THAT(LocalDateTime::fromString("21/1/2018, 14:18:34.762[America/Blanc-Sablon]", "d/M/yyyy, hh:mm:ss.fff[zzzz]"), Eq(LocalDateTime(DateTime(Date(2018, 1, 21), Time(14, 18, 34, 762)), TimeZone("America/Blanc-Sablon"))));
    ASSERT_THAT(LocalDateTime::fromString("21/1/2018, 14:18:34.762[Etc/GMT+1]", "d/M/yyyy, hh:mm:ss.fff[zzzz]"), Eq(LocalDateTime(DateTime(Date(2018, 1, 21), Time(14, 18, 34, 762)), TimeZone("Etc/GMT+1"))));

    // ambiguous time zones cannot be parsed.
    ASSERT_THAT(LocalDateTime::fromString("21/1/2018, 14:18:34.762 CEST", "d/M/yyyy, hh:mm:ss.fff zzz"), Eq(LocalDateTime(DateTime(Date(2018, 1, 21), Time(14, 18, 34, 762)), TimeZone())));
    ASSERT_THAT(LocalDateTime::fromString("21/1/2018, 14:18:34.762UTC-10:00", "d/M/yyyy, hh:mm:ss.fffUTCzz"), Eq(LocalDateTime(DateTime(Date(2018, 1, 21), Time(14, 18, 34, 762)), TimeZone())));
    ASSERT_THAT(LocalDateTime::fromString("21/1/2018, 14:18:34.762+0300", "d/M/yyyy, hh:mm:ss.fff z"), Eq(LocalDateTime(DateTime(Date(2018, 1, 21), Time(14, 18, 34, 762)), TimeZone())));
}

TEST(LocalDateTimeTest, SerializesDeserializes) {
    LocalDateTime ldt;
    std::stringstream ss;
    ss << LocalDateTime(DateTime(Date(2014, 11, 9), Time(2, 44, 21)), TimeZone("Europe/Istanbul"));
    ss >> ldt;
    ASSERT_THAT(ldt, Eq(LocalDateTime(DateTime(Date(2014, 11, 9), Time(2, 44, 21)), TimeZone("Europe/Istanbul"))));
}