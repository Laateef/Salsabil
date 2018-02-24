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
#include "Time.hpp"
#include <sstream>

using namespace Salsabil;

TEST_CASE("TimeTest") {

    SUBCASE("IsInvalidIfDefaultConstructed") {
        Time myTime;
        CHECK_FALSE(myTime.isValid());
        CHECK(myTime.hour() == 0);
        CHECK(myTime.minute() == 0);
        CHECK(myTime.second() == 0);
        CHECK(myTime.millisecond() == 0);
        CHECK(myTime.microsecond() == 0);
        CHECK(myTime.nanosecond() == 0);
    }

    SUBCASE("ReturnsCurrentTime") {
        Time myTime = Time::current();
        std::time_t tTime = std::time(nullptr);
        std::tm* tmTime = std::gmtime(&tTime);

        CHECK(myTime.hour() == tmTime->tm_hour);
        CHECK(myTime.minute() == tmTime->tm_min);
        CHECK(myTime.second() == tmTime->tm_sec);

        CHECK(myTime.isValid());
    }

    SUBCASE("ReturnsMidnight") {
        Time myTime = Time::midnight();
        CHECK(myTime.hour() == 0);
        CHECK(myTime.minute() == 0);
        CHECK(myTime.second() == 0);
        CHECK(myTime.millisecond() == 0);
        CHECK(myTime.microsecond() == 0);
        CHECK(myTime.nanosecond() == 0);
    }

    SUBCASE("InitializesToGivenTime") {
        Time myTime(13, 44, 2);
        CHECK(myTime.hour() == 13);
        CHECK(myTime.minute() == 44);
        CHECK(myTime.second() == 2);
        CHECK(myTime.millisecond() == 0);
        CHECK(myTime.microsecond() == 0);
        CHECK(myTime.nanosecond() == 0);
    }

    SUBCASE("InitializesToGivenTimeWithFractions") {
        Time myTime(13, 44, 2, Time::Nanoseconds(781945521));
        CHECK(myTime.hour() == 13);
        CHECK(myTime.minute() == 44);
        CHECK(myTime.second() == 2);
        CHECK(myTime.millisecond() == 781);
        CHECK(myTime.microsecond() == 781945);
        CHECK(myTime.nanosecond() == 781945521);
    }

    SUBCASE("IsValidOnlyIfWithinDayDurationNoLessNoMore") {
        CHECK_FALSE(Time(Time::Hours(-1)).isValid());
        CHECK(Time(Time::Hours(23)).isValid());
        CHECK_FALSE(Time(Time::Hours(24)).isValid());
        CHECK_FALSE(Time(Time::Hours(25)).isValid());
    }

    SUBCASE("TestsComparisons") {
        CHECK(Time(7, 9, 2, Time::Nanoseconds(675869233)) < Time(7, 45, 22, Time::Nanoseconds(536969233)));
        CHECK(Time(7, 9, 2, Time::Nanoseconds(536969435)) <= Time(7, 9, 2, Time::Nanoseconds(536969435)));
        CHECK(Time(8, 9, 2, Time::Nanoseconds(675869676)) > Time(7, 45, 22, Time::Nanoseconds(536969212)));
        CHECK(Time(7, 46, 2, Time::Nanoseconds(675869112)) >= Time(7, 45, 22, Time::Nanoseconds(536969112)));
        CHECK(Time(15, 4, 12, Time::Nanoseconds(554969231)) == Time(15, 4, 12, Time::Nanoseconds(554969231)));
        CHECK(Time(7, 9, 2, Time::Nanoseconds(675869123)) != Time(4, 45, 22, Time::Nanoseconds(536969321)));
    }

    SUBCASE("ConstructFromDurations") {
        CHECK(Time(Time::Hours(23)) == Time(23, 0, 0));
        CHECK(Time(Time::Minutes(178)) == Time(2, 58, 0, 0));
        CHECK(Time(Time::Seconds(7199)) == Time(1, 59, 59, 0));
        CHECK(Time(Time::Milliseconds(7198943)) == Time(1, 59, 58, 943));
        CHECK(Time(Time::Microseconds(74362675869)) == Time(20, 39, 22, Time::Microseconds(675869)));
        CHECK(Time(Time::Nanoseconds(8974362675546)) == Time(2, 29, 34, Time::Nanoseconds(362675546)));
        CHECK(Time(Time::Hours(16) + Time::Minutes(18) + Time::Seconds(55) + Time::Milliseconds(178) + Time::Microseconds(221) + Time::Nanoseconds(759)) == Time(16, 18, 55, Time::Nanoseconds(178221759)));
    }

    SUBCASE("AddSubtractHours") {
        CHECK(Time(Time::Hours(7)).addHours(2) == Time(9, 0, 0, 0));
        CHECK(Time(Time::Hours(9)).subtractHours(2) == Time(7, 0, 0, 0));
    }

    SUBCASE("AddSubtractMinutes") {
        CHECK(Time(Time::Minutes(178)).addMinutes(2) == Time(3, 0, 0, 0));
        CHECK(Time(Time::Minutes(180)).subtractMinutes(2) == Time(2, 58, 0, 0));
    }

    SUBCASE("AddSubtractSeconds") {
        CHECK(Time(Time::Seconds(55)).addSeconds(9) == Time(0, 1, 4, 0));
        CHECK(Time(Time::Seconds(64)).subtractSeconds(9) == Time(0, 0, 55, 0));
    }

    SUBCASE("AddSubtractMilliseconds") {
        CHECK(Time(Time::Milliseconds(555)).addMilliseconds(445) == Time(0, 0, 1, 0));
        CHECK(Time(Time::Milliseconds(1000)).subtractMilliseconds(445) == Time(0, 0, 0, 555));
    }

    SUBCASE("AddSubtractMicroseconds") {
        CHECK(Time(Time::Microseconds(555)).addMicroseconds(445) == Time(0, 0, 0, Time::Microseconds(1000)));
        CHECK(Time(Time::Microseconds(1000)).subtractMicroseconds(445) == Time(0, 0, 0, Time::Microseconds(555)));
    }

    SUBCASE("AddSubtractNanoseconds") {
        CHECK(Time(Time::Nanoseconds(8974362675556)).addNanoseconds(445) == Time(2, 29, 34, Time::Nanoseconds(362676001)));
        CHECK(Time(Time::Nanoseconds(8974362676001)).subtractNanoseconds(445) == Time(2, 29, 34, Time::Nanoseconds(362675556)));
    }

    SUBCASE("TestsAdditionSubtractionOperators") {
        CHECK(Time(11, 23, 11) - Time(10, 23, 11) == Time::Hours(1));
        CHECK(Time(11, 23, 11) - Time::Hours(10) == Time(1, 23, 11));
        CHECK(Time(1, 23, 11) + Time::Hours(10) == Time(11, 23, 11));
    }

    SUBCASE("TestsDifferenceBetweenTwoTimes") {
        CHECK(Time::hoursBetween(Time(10, 23, 25), Time(11, 23, 29)) == 1);
        CHECK(Time::minutesBetween(Time(11, 23, 11), Time(11, 53, 11)) == 30);
        CHECK(Time::secondsBetween(Time(9, 23, 55), Time(9, 23, 35)) == -20);
        CHECK(Time::millisecondsBetween(Time(7, 23, 11, 850), Time(7, 23, 12, 900)) == 1050);
        CHECK(Time::microsecondsBetween(Time(13, 23, 20, Time::Microseconds(789500)), Time(13, 23, 20, Time::Microseconds(789400))) == -100);
        CHECK(Time::nanosecondsBetween(Time(18, 56, 5, Time::Nanoseconds(789500235)), Time(18, 56, 5, Time::Nanoseconds(789500135))) == -100);
    }

    SUBCASE("Format12Hours") {
        CHECK(Time(23, 45, 2).toString("H:m:s") == "11:45:2");
        CHECK(Time(0, 45, 2).toString("H:m:s") == "12:45:2");
    }

    SUBCASE("Format12HoursTwoDigits") {
        CHECK(Time(3, 45, 2).toString("HH:m:s") == "03:45:2");
    }

    SUBCASE("FormatMeridiemLabelSmallLetters") {
        CHECK(Time(3, 45, 2).toString("HH:mm:ss a") == "03:45:02 am");
        CHECK(Time(13, 45, 2).toString("HH:mm:ss a") == "01:45:02 pm");
        CHECK(Time(Time::Hours(0)).toString("HH:mm:ss a") == "12:00:00 am");
        CHECK(Time(Time::Hours(12)).toString("HH:mm:ss a") == "12:00:00 pm");
    }

    SUBCASE("FormatMeridiemLabelCapitalLetters") {
        CHECK(Time(3, 45, 2).toString("HH:mm:ss A") == "03:45:02 AM");
        CHECK(Time(13, 45, 2).toString("HH:mm:ss A") == "01:45:02 PM");
        CHECK(Time(Time::Hours(0)).toString("HH:mm:ss A") == "12:00:00 AM");
        CHECK(Time(Time::Hours(12)).toString("HH:mm:ss A") == "12:00:00 PM");
    }

    SUBCASE("ReserveNonPatternExpressions") {
        CHECK(Time(21, 52, 41).toString("hhmmss ieee") == "215241 ieee");
    }

    SUBCASE("FormatPM") {
        CHECK(Time(3, 45, 2).toString("HH:m:s") == "03:45:2");
    }

    SUBCASE("Format24Hours") {
        CHECK(Time(22, 45, 2).toString("h:m:s") == "22:45:2");
    }

    SUBCASE("Format24HoursTwoDigits") {
        CHECK(Time(3, 45, 2).toString("hh:m:s") == "03:45:2");
    }

    SUBCASE("FormatMinutes") {
        CHECK(Time(Time::Hours(22) + Time::Minutes(5)).toString("h:m") == "22:5");
    }

    SUBCASE("FormatMinutesTwoDigits") {
        CHECK(Time(Time::Hours(22) + Time::Minutes(5)).toString("h:mm")  == "22:05");
    }

    SUBCASE("FormatSeconds") {
        CHECK(Time(Time::Minutes(55) + Time::Seconds(7)).toString("m:s") == "55:7");
    }

    SUBCASE("FormatSecondsTwoDigits") {
        CHECK(Time(Time::Minutes(55) + Time::Seconds(7)).toString("m:ss") == "55:07");
    }

    SUBCASE("FormatFractions") {
        Time t(Time::Hours(7) + Time::Minutes(9) + Time::Seconds(2) + Time::Milliseconds(675) + Time::Microseconds(869) + Time::Nanoseconds(93));

        CHECK(t.toString("hh:mm:ss.f") == "07:09:02.6");
        CHECK(t.toString("hh:mm:ss.ff") == "07:09:02.67");
        CHECK(t.toString("hh:mm:ss.fff") == "07:09:02.675");
        CHECK(t.toString("hh:mm:ss.ffff") == "07:09:02.6758");
        CHECK(t.toString("hh:mm:ss.fffff") == "07:09:02.67586");
        CHECK(t.toString("hh:mm:ss.ffffff") == "07:09:02.675869");
        CHECK(t.toString("hh:mm:ss.fffffff") == "07:09:02.6758690");
        CHECK(t.toString("hh:mm:ss.ffffffff") == "07:09:02.67586909");
        CHECK(t.toString("hh:mm:ss.fffffffff") == "07:09:02.675869093");
    }

    SUBCASE("FormattingFractionsWhenMillisecondsAreZero") {
        Time t(Time::Hours(7) + Time::Minutes(9) + Time::Seconds(2) + Time::Microseconds(869) + Time::Nanoseconds(93));

        CHECK(t.toString("hh:mm:ss.f") == "07:09:02.0");
        CHECK(t.toString("hh:mm:ss.ff") == "07:09:02.00");
        CHECK(t.toString("hh:mm:ss.fff") == "07:09:02.000");
        CHECK(t.toString("hh:mm:ss.ffff") == "07:09:02.0008");
        CHECK(t.toString("hh:mm:ss.fffff") == "07:09:02.00086");
        CHECK(t.toString("hh:mm:ss.ffffff") == "07:09:02.000869");
        CHECK(t.toString("hh:mm:ss.fffffff") == "07:09:02.0008690");
        CHECK(t.toString("hh:mm:ss.ffffffff") == "07:09:02.00086909");
        CHECK(t.toString("hh:mm:ss.fffffffff") == "07:09:02.000869093");
    }

    SUBCASE("FormattingFractionsWhenMicrosecondsAreZero") {
        Time t(Time::Hours(7) + Time::Minutes(9) + Time::Seconds(2) + Time::Milliseconds(675) + Time::Nanoseconds(44));

        CHECK(t.toString("h:m:s") == "7:9:2");
        CHECK(t.toString("hh:mm:ss") == "07:09:02");
        CHECK(t.toString("hh:mm:ss.f") == "07:09:02.6");
        CHECK(t.toString("hh:mm:ss.ff") == "07:09:02.67");
        CHECK(t.toString("hh:mm:ss.fff") == "07:09:02.675");
        CHECK(t.toString("hh:mm:ss.ffff") == "07:09:02.6750");
        CHECK(t.toString("hh:mm:ss.fffff") == "07:09:02.67500");
        CHECK(t.toString("hh:mm:ss.ffffff") == "07:09:02.675000");
        CHECK(t.toString("hh:mm:ss.fffffff") == "07:09:02.6750000");
        CHECK(t.toString("hh:mm:ss.ffffffff") == "07:09:02.67500004");
        CHECK(t.toString("hh:mm:ss.fffffffff") == "07:09:02.675000044");
    }

    SUBCASE("FormatsFractionsWhenNanosecondsAreZero") {
        Time t(Time::Hours(7) + Time::Minutes(9) + Time::Seconds(2) + Time::Milliseconds(675) + Time::Microseconds(869));

        CHECK(t.toString("h:m:s") == "7:9:2");
        CHECK(t.toString("hh:mm:ss") == "07:09:02");
        CHECK(t.toString("hh:mm:ss.f") == "07:09:02.6");
        CHECK(t.toString("hh:mm:ss.ff") == "07:09:02.67");
        CHECK(t.toString("hh:mm:ss.fff") == "07:09:02.675");
        CHECK(t.toString("hh:mm:ss.ffff") == "07:09:02.6758");
        CHECK(t.toString("hh:mm:ss.fffff") == "07:09:02.67586");
        CHECK(t.toString("hh:mm:ss.ffffff") == "07:09:02.675869");
        CHECK(t.toString("hh:mm:ss.fffffff") == "07:09:02.6758690");
        CHECK(t.toString("hh:mm:ss.ffffffff") == "07:09:02.67586900");
        CHECK(t.toString("hh:mm:ss.fffffffff") == "07:09:02.675869000");
        CHECK(t.toString("hh:mm:ss.fff fff fff") == "07:09:02.675 675 675");
    }

    SUBCASE("FormatTimeInCongestedFormat") {
        CHECK(Time(3, 45, 2).toString("hhmmss") == "034502");
    }

    SUBCASE("CreatesTimeFromFormattedString") {
        CHECK(Time::fromString("9", "h") == Time(Time::Hours(9)));
        CHECK(Time::fromString("01", "hh") == Time(Time::Hours(1)));
        CHECK(Time::fromString("9", "H") == Time(Time::Hours(9)));
        CHECK(Time::fromString("12", "H") == Time(Time::Hours(12)));
        CHECK(Time::fromString("01", "HH") == Time(Time::Hours(1)));
        CHECK(Time::fromString("01 pm", "HH a") == Time(Time::Hours(13)));
        CHECK(Time::fromString("01 PM", "HH A") == Time(Time::Hours(13)));
        CHECK(Time::fromString("3", "m") == Time(Time::Minutes(3)));
        CHECK(Time::fromString("03", "mm") == Time(Time::Minutes(3)));
        CHECK(Time::fromString("37", "s") == Time(Time::Seconds(37)));
        CHECK(Time::fromString("06", "ss") == Time(Time::Seconds(6)));
        CHECK(Time::fromString("1", "f") == Time(Time::Milliseconds(100)));
        CHECK(Time::fromString("12", "ff") == Time(Time::Milliseconds(120)));
        CHECK(Time::fromString("123", "fff") == Time(Time::Milliseconds(123)));
        CHECK(Time::fromString("1234", "ffff") == Time(Time::Microseconds(123400)));
        CHECK(Time::fromString("12345", "fffff") == Time(Time::Microseconds(123450)));
        CHECK(Time::fromString("123456", "ffffff") == Time(Time::Microseconds(123456)));
        CHECK(Time::fromString("1234567", "fffffff") == Time(Time::Nanoseconds(123456700)));
        CHECK(Time::fromString("12345678", "ffffffff") == Time(Time::Nanoseconds(123456780)));
        CHECK(Time::fromString("123456789", "fffffffff") == Time(Time::Nanoseconds(123456789)));
        CHECK(Time::fromString("14:32:09.123456789", "hh:mm:ss.fffffffff") == Time(14, 32, 9, Time::Nanoseconds(123456789)));
        CHECK(Time::fromString("143209", "hhmmss") == Time(14, 32, 9));
        CHECK(Time::fromString("ieee 143209", "ieee hhmmss") == Time(14, 32, 9));
    }

    SUBCASE("ReturnsTimeAsNanoseconds") {
        CHECK(Time(23, 56, 33, Time::Nanoseconds(978432162)).toNanosecondsSinceMidnight() == 86193978432162);
    }

    SUBCASE("ReturnsTimeAsMicroseconds") {
        CHECK(Time(23, 56, 33, Time::Nanoseconds(978432162)).toMicrosecondsSinceMidnight() == 86193978432);
    }

    SUBCASE("ReturnsTimeAsMilliseconds") {
        CHECK(Time(23, 56, 33, Time::Nanoseconds(978432162)).toMillisecondsSinceMidnight() == 86193978);
    }

    SUBCASE("ReturnsTimeAsSeconds") {
        CHECK(Time(23, 56, 33, Time::Nanoseconds(978432162)).toSecondsSinceMidnight() == 86193);
    }

    SUBCASE("ReturnsTimeAsMinutes") {
        CHECK(Time(23, 56, 33, Time::Nanoseconds(978432162)).toMinutesSinceMidnight() == 1436);
    }

    SUBCASE("ReturnsTimeAsHours") {
        CHECK(Time(23, 56, 33, Time::Nanoseconds(978432162)).toHoursSinceMidnight() == 23);
    }

    SUBCASE("ReturnsBrokenStdTimeRepresentation") {
        Time myTime(14, 32, 9);
        std::tm tmTime = myTime.toBrokenStdTime();
        CHECK(tmTime.tm_hour == myTime.hour());
        CHECK(tmTime.tm_min == myTime.minute());
        CHECK(tmTime.tm_sec == myTime.second());
    }

    SUBCASE("ReturnsScalarStdTimeRepresentation") {
        Time myTime(14, 32, 9);
        std::time_t tTime = myTime.toScalarStdTime();
        CHECK(tTime == myTime.toSecondsSinceMidnight());
    }

    SUBCASE("SerializesDeserializes") {
        Time myTime;
        std::stringstream ss;
        ss << Time(14, 32, 9);
        ss >> myTime;
        CHECK(myTime == Time(14, 32, 9));
    }
}