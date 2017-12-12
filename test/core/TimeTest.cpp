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
#include "Time.hpp"

using namespace ::testing;
using namespace Salsabil;

TEST(TimeTest, InitializesToCurrentTimeIfDefaultConstructed) {
    Time myTime;
    std::time_t tTime = std::time(nullptr);
    std::tm* tmTime = std::gmtime(&tTime);

    ASSERT_THAT(myTime.hour(), Eq(tmTime->tm_hour));
    ASSERT_THAT(myTime.minute(), Eq(tmTime->tm_min));
    ASSERT_THAT(myTime.second(), Eq(tmTime->tm_sec));

    ASSERT_TRUE(myTime.isValid());
}

TEST(TimeTest, InitializesToGivenTime) {
    Time myTime(13, 44, 2);
    ASSERT_THAT(myTime.hour(), Eq(13));
    ASSERT_THAT(myTime.minute(), Eq(44));
    ASSERT_THAT(myTime.second(), Eq(2));
    ASSERT_THAT(myTime.millisecond(), Eq(0));
    ASSERT_THAT(myTime.microsecond(), Eq(0));
    ASSERT_THAT(myTime.nanosecond(), Eq(0));
}

TEST(TimeTest, InitializesToGivenTimeWithFractions) {
    Time myTime(13, 44, 2, Time::Nanoseconds(781945521));
    ASSERT_THAT(myTime.hour(), Eq(13));
    ASSERT_THAT(myTime.minute(), Eq(44));
    ASSERT_THAT(myTime.second(), Eq(2));
    ASSERT_THAT(myTime.millisecond(), Eq(781));
    ASSERT_THAT(myTime.microsecond(), Eq(781945));
    ASSERT_THAT(myTime.nanosecond(), Eq(781945521));
}

TEST(TimeTest, isValidReturnsFalseIfHoursExceded24) {
    ASSERT_TRUE(Time(Time::Hours(23)).isValid());
    ASSERT_FALSE(Time(Time::Hours(24)).isValid());
    ASSERT_FALSE(Time(Time::Hours(25)).isValid());
}

TEST(TimeTest, TestsComparisons) {
    ASSERT_TRUE(Time(7, 9, 2, Time::Nanoseconds(675869233)) < Time(7, 45, 22, Time::Nanoseconds(536969233)));
    ASSERT_TRUE(Time(7, 9, 2, Time::Nanoseconds(536969435)) <= Time(7, 9, 2, Time::Nanoseconds(536969435)));
    ASSERT_TRUE(Time(8, 9, 2, Time::Nanoseconds(675869676)) > Time(7, 45, 22, Time::Nanoseconds(536969212)));
    ASSERT_TRUE(Time(7, 46, 2, Time::Nanoseconds(675869112)) >= Time(7, 45, 22, Time::Nanoseconds(536969112)));
    ASSERT_TRUE(Time(15, 4, 12, Time::Nanoseconds(554969231)) == Time(15, 4, 12, Time::Nanoseconds(554969231)));
    ASSERT_TRUE(Time(7, 9, 2, Time::Nanoseconds(675869123)) != Time(4, 45, 22, Time::Nanoseconds(536969321)));
}

TEST(TimeTest, ConstructFromDurations) {
    ASSERT_THAT(Time(Time::Hours(23)), Eq(Time(23, 0, 0)));
    ASSERT_THAT(Time(Time::Minutes(178)), Eq(Time(2, 58, 0, 0)));
    ASSERT_THAT(Time(Time::Seconds(7199)), Eq(Time(1, 59, 59, 0)));
    ASSERT_THAT(Time(Time::Milliseconds(7198943)), Eq(Time(1, 59, 58, 943)));
    ASSERT_THAT(Time(Time::Microseconds(74362675869)), Eq(Time(20, 39, 22, Time::Microseconds(675869))));
    ASSERT_THAT(Time(Time::Nanoseconds(8974362675546)), Eq(Time(2, 29, 34, Time::Nanoseconds(362675546))));
    ASSERT_THAT(Time(Time::Hours(16) + Time::Minutes(18) + Time::Seconds(55) + Time::Milliseconds(178) + Time::Microseconds(221) + Time::Nanoseconds(759)), Eq(Time(16, 18, 55, Time::Nanoseconds(178221759))));
}

TEST(TimeTest, AddSubtractHours) {
    ASSERT_THAT(Time(Time::Hours(7)).addHours(2), Eq(Time(9, 0, 0, 0)));
    ASSERT_THAT(Time(Time::Hours(9)).subtractHours(2), Eq(Time(7, 0, 0, 0)));
}

TEST(TimeTest, AddSubtractMinutes) {
    ASSERT_THAT(Time(Time::Minutes(178)).addMinutes(2), Eq(Time(3, 0, 0, 0)));
    ASSERT_THAT(Time(Time::Minutes(180)).subtractMinutes(2), Eq(Time(2, 58, 0, 0)));
}

TEST(TimeTest, AddSubtractSeconds) {
    ASSERT_THAT(Time(Time::Seconds(55)).addSeconds(9), Eq(Time(0, 1, 4, 0)));
    ASSERT_THAT(Time(Time::Seconds(64)).subtractSeconds(9), Eq(Time(0, 0, 55, 0)));
}

TEST(TimeTest, AddSubtractMilliseconds) {
    ASSERT_THAT(Time(Time::Milliseconds(555)).addMilliseconds(445), Eq(Time(0, 0, 1, 0)));
    ASSERT_THAT(Time(Time::Milliseconds(1000)).subtractMilliseconds(445), Eq(Time(0, 0, 0, 555)));
}

TEST(TimeTest, AddSubtractMicroseconds) {
    ASSERT_THAT(Time(Time::Microseconds(555)).addMicroseconds(445), Eq(Time(0, 0, 0, Time::Microseconds(1000))));
    ASSERT_THAT(Time(Time::Microseconds(1000)).subtractMicroseconds(445), Eq(Time(0, 0, 0, Time::Microseconds(555))));
}

TEST(TimeTest, AddSubtractNanoseconds) {
    ASSERT_THAT(Time(Time::Nanoseconds(8974362675556)).addNanoseconds(445), Eq(Time(2, 29, 34, Time::Nanoseconds(362676001))));
    ASSERT_THAT(Time(Time::Nanoseconds(8974362676001)).subtractNanoseconds(445), Eq(Time(2, 29, 34, Time::Nanoseconds(362675556))));
}

TEST(TimeTest, TestsAdditionSubtractionOperators) {
    ASSERT_THAT(Time(11, 23, 11) - Time(10, 23, 11), Eq(Time::Hours(1)));
    ASSERT_THAT(Time(11, 23, 11) - Time::Hours(10), Eq(Time(1, 23, 11)));
    ASSERT_THAT(Time(1, 23, 11) + Time::Hours(10), Eq(Time(11, 23, 11)));
}

TEST(TimeTest, TestsDifferenceBetweenTwoTimes) {
    ASSERT_THAT(Time::hoursBetween(Time(11, 23, 25), Time(10, 23, 29)), Eq(1));
    ASSERT_THAT(Time::minutesBetween(Time(11, 23, 11), Time(11, 53, 11)), Eq(-30));
    ASSERT_THAT(Time::secondsBetween(Time(9, 23, 55), Time(9, 23, 35)), Eq(20));
    ASSERT_THAT(Time::millisecondsBetween(Time(7, 23, 11, 850), Time(7, 23, 12, 900)), Eq(-1050));
    ASSERT_THAT(Time::microsecondsBetween(Time(13, 23, 20, Time::Microseconds(789500)), Time(13, 23, 20, Time::Microseconds(789400))), Eq(100));
    ASSERT_THAT(Time::nanosecondsBetween(Time(18, 56, 5, Time::Nanoseconds(789500235)), Time(18, 56, 5, Time::Nanoseconds(789500135))), Eq(100));
}

TEST(TimeTest, Format12Hours) {
    ASSERT_THAT(Time(23, 45, 2).toString("H:m:s"), StrEq("11:45:2"));
    ASSERT_THAT(Time(0, 45, 2).toString("H:m:s"), StrEq("12:45:2"));
}

TEST(TimeTest, Format12HoursTwoDigits) {
    ASSERT_THAT(Time(3, 45, 2).toString("HH:m:s"), StrEq("03:45:2"));
}

TEST(TimeTest, FormatMeridiemLabelSmallLetters) {
    ASSERT_THAT(Time(3, 45, 2).toString("HH:mm:ss a"), StrEq("03:45:02 am"));
    ASSERT_THAT(Time(13, 45, 2).toString("HH:mm:ss a"), StrEq("01:45:02 pm"));
    ASSERT_THAT(Time(Time::Hours(0)).toString("HH:mm:ss a"), StrEq("12:00:00 am"));
    ASSERT_THAT(Time(Time::Hours(12)).toString("HH:mm:ss a"), StrEq("12:00:00 pm"));
}

TEST(TimeTest, FormatMeridiemLabelCapitalLetters) {
    ASSERT_THAT(Time(3, 45, 2).toString("HH:mm:ss A"), StrEq("03:45:02 AM"));
    ASSERT_THAT(Time(13, 45, 2).toString("HH:mm:ss A"), StrEq("01:45:02 PM"));
    ASSERT_THAT(Time(Time::Hours(0)).toString("HH:mm:ss A"), StrEq("12:00:00 AM"));
    ASSERT_THAT(Time(Time::Hours(12)).toString("HH:mm:ss A"), StrEq("12:00:00 PM"));
}

TEST(TimeTest, FormatPM) {
    ASSERT_THAT(Time(3, 45, 2).toString("HH:m:s"), StrEq("03:45:2"));
}

TEST(TimeTest, Format24Hours) {
    ASSERT_THAT(Time(22, 45, 2).toString("h:m:s"), StrEq("22:45:2"));
}

TEST(TimeTest, Format24HoursTwoDigits) {
    ASSERT_THAT(Time(3, 45, 2).toString("hh:m:s"), StrEq("03:45:2"));
}

TEST(TimeTest, FormatMinutes) {
    ASSERT_THAT(Time(Time::Hours(22) + Time::Minutes(5)).toString("h:m"), StrEq("22:5"));
}

TEST(TimeTest, FormatMinutesTwoDigits) {
    ASSERT_THAT(Time(Time::Hours(22) + Time::Minutes(5)).toString("h:mm"), StrEq("22:05"));
}

TEST(TimeTest, FormatSeconds) {
    ASSERT_THAT(Time(Time::Minutes(55) + Time::Seconds(7)).toString("m:s"), StrEq("55:7"));
}

TEST(TimeTest, FormatSecondsTwoDigits) {
    ASSERT_THAT(Time(Time::Minutes(55) + Time::Seconds(7)).toString("m:ss"), StrEq("55:07"));
}

TEST(TimeTest, FormatFractions) {
    Time t(Time::Hours(7) + Time::Minutes(9) + Time::Seconds(2) + Time::Milliseconds(675) + Time::Microseconds(869) + Time::Nanoseconds(93));

    ASSERT_THAT(t.toString("hh:mm:ss.f"), StrEq("07:09:02.6"));
    ASSERT_THAT(t.toString("hh:mm:ss.ff"), StrEq("07:09:02.67"));
    ASSERT_THAT(t.toString("hh:mm:ss.fff"), StrEq("07:09:02.675"));
    ASSERT_THAT(t.toString("hh:mm:ss.ffff"), StrEq("07:09:02.6758"));
    ASSERT_THAT(t.toString("hh:mm:ss.fffff"), StrEq("07:09:02.67586"));
    ASSERT_THAT(t.toString("hh:mm:ss.ffffff"), StrEq("07:09:02.675869"));
    ASSERT_THAT(t.toString("hh:mm:ss.fffffff"), StrEq("07:09:02.6758690"));
    ASSERT_THAT(t.toString("hh:mm:ss.ffffffff"), StrEq("07:09:02.67586909"));
    ASSERT_THAT(t.toString("hh:mm:ss.fffffffff"), StrEq("07:09:02.675869093"));
}

TEST(TimeTest, FormattingFractionsWhenMillisecondsAreZero) {
    Time t(Time::Hours(7) + Time::Minutes(9) + Time::Seconds(2) + Time::Microseconds(869) + Time::Nanoseconds(93));

    ASSERT_THAT(t.toString("hh:mm:ss.f"), StrEq("07:09:02.0"));
    ASSERT_THAT(t.toString("hh:mm:ss.ff"), StrEq("07:09:02.00"));
    ASSERT_THAT(t.toString("hh:mm:ss.fff"), StrEq("07:09:02.000"));
    ASSERT_THAT(t.toString("hh:mm:ss.ffff"), StrEq("07:09:02.0008"));
    ASSERT_THAT(t.toString("hh:mm:ss.fffff"), StrEq("07:09:02.00086"));
    ASSERT_THAT(t.toString("hh:mm:ss.ffffff"), StrEq("07:09:02.000869"));
    ASSERT_THAT(t.toString("hh:mm:ss.fffffff"), StrEq("07:09:02.0008690"));
    ASSERT_THAT(t.toString("hh:mm:ss.ffffffff"), StrEq("07:09:02.00086909"));
    ASSERT_THAT(t.toString("hh:mm:ss.fffffffff"), StrEq("07:09:02.000869093"));
}

TEST(TimeTest, FormattingFractionsWhenMicrosecondsAreZero) {
    Time t(Time::Hours(7) + Time::Minutes(9) + Time::Seconds(2) + Time::Milliseconds(675) + Time::Nanoseconds(44));

    ASSERT_THAT(t.toString("h:m:s"), StrEq("7:9:2"));
    ASSERT_THAT(t.toString("hh:mm:ss"), StrEq("07:09:02"));
    ASSERT_THAT(t.toString("hh:mm:ss.f"), StrEq("07:09:02.6"));
    ASSERT_THAT(t.toString("hh:mm:ss.ff"), StrEq("07:09:02.67"));
    ASSERT_THAT(t.toString("hh:mm:ss.fff"), StrEq("07:09:02.675"));
    ASSERT_THAT(t.toString("hh:mm:ss.ffff"), StrEq("07:09:02.6750"));
    ASSERT_THAT(t.toString("hh:mm:ss.fffff"), StrEq("07:09:02.67500"));
    ASSERT_THAT(t.toString("hh:mm:ss.ffffff"), StrEq("07:09:02.675000"));
    ASSERT_THAT(t.toString("hh:mm:ss.fffffff"), StrEq("07:09:02.6750000"));
    ASSERT_THAT(t.toString("hh:mm:ss.ffffffff"), StrEq("07:09:02.67500004"));
    ASSERT_THAT(t.toString("hh:mm:ss.fffffffff"), StrEq("07:09:02.675000044"));
}

TEST(TimeTest, FormatsFractionsWhenNanosecondsAreZero) {
    Time t(Time::Hours(7) + Time::Minutes(9) + Time::Seconds(2) + Time::Milliseconds(675) + Time::Microseconds(869));

    ASSERT_THAT(t.toString("h:m:s"), StrEq("7:9:2"));
    ASSERT_THAT(t.toString("hh:mm:ss"), StrEq("07:09:02"));
    ASSERT_THAT(t.toString("hh:mm:ss.f"), StrEq("07:09:02.6"));
    ASSERT_THAT(t.toString("hh:mm:ss.ff"), StrEq("07:09:02.67"));
    ASSERT_THAT(t.toString("hh:mm:ss.fff"), StrEq("07:09:02.675"));
    ASSERT_THAT(t.toString("hh:mm:ss.ffff"), StrEq("07:09:02.6758"));
    ASSERT_THAT(t.toString("hh:mm:ss.fffff"), StrEq("07:09:02.67586"));
    ASSERT_THAT(t.toString("hh:mm:ss.ffffff"), StrEq("07:09:02.675869"));
    ASSERT_THAT(t.toString("hh:mm:ss.fffffff"), StrEq("07:09:02.6758690"));
    ASSERT_THAT(t.toString("hh:mm:ss.ffffffff"), StrEq("07:09:02.67586900"));
    ASSERT_THAT(t.toString("hh:mm:ss.fffffffff"), StrEq("07:09:02.675869000"));
}

TEST(TimeTest, CreatesTimeFromFormattedString) {
    ASSERT_THAT(Time::fromString("9", "h"), Time(Time::Hours(9)));
    ASSERT_THAT(Time::fromString("01", "hh"), Time(Time::Hours(1)));
    ASSERT_THAT(Time::fromString("9", "H"), Time(Time::Hours(9)));
    ASSERT_THAT(Time::fromString("12", "H"), Time(Time::Hours(12)));
    ASSERT_THAT(Time::fromString("01", "HH"), Time(Time::Hours(1)));
    ASSERT_THAT(Time::fromString("01 pm", "HH a"), Time(Time::Hours(13)));
    ASSERT_THAT(Time::fromString("01 PM", "HH A"), Time(Time::Hours(13)));
    ASSERT_THAT(Time::fromString("1 PM", "HH A"), Time(Time::Hours(13)));
    ASSERT_THAT(Time::fromString("3", "m"), Time(Time::Minutes(3)));
    ASSERT_THAT(Time::fromString("03", "mm"), Time(Time::Minutes(3)));
    ASSERT_THAT(Time::fromString("37", "s"), Time(Time::Seconds(37)));
    ASSERT_THAT(Time::fromString("06", "ss"), Time(Time::Seconds(6)));
    ASSERT_THAT(Time::fromString("1", "f"), Time(Time::Milliseconds(100)));
    ASSERT_THAT(Time::fromString("12", "ff"), Time(Time::Milliseconds(120)));
    ASSERT_THAT(Time::fromString("123", "fff"), Time(Time::Milliseconds(123)));
    ASSERT_THAT(Time::fromString("1234", "ffff"), Time(Time::Microseconds(123400)));
    ASSERT_THAT(Time::fromString("12345", "fffff"), Time(Time::Microseconds(123450)));
    ASSERT_THAT(Time::fromString("123456", "ffffff"), Time(Time::Microseconds(123456)));
    ASSERT_THAT(Time::fromString("1234567", "fffffff"), Time(Time::Nanoseconds(123456700)));
    ASSERT_THAT(Time::fromString("12345678", "ffffffff"), Time(Time::Nanoseconds(123456780)));
    ASSERT_THAT(Time::fromString("123456789", "fffffffff"), Time(Time::Nanoseconds(123456789)));
    ASSERT_THAT(Time::fromString("14:32:09.123456789", "hh:mm:ss.fffffffff"), Time(14, 32, 9, Time::Nanoseconds(123456789)));
}

TEST(TimeTest, ReturnsTimeAsNanoseconds) {
    ASSERT_THAT(Time(23, 56, 33, Time::Nanoseconds(978432162)).toNanoseconds(), Eq(86193978432162));
}

TEST(TimeTest, ReturnsTimeAsMicroseconds) {
    ASSERT_THAT(Time(23, 56, 33, Time::Nanoseconds(978432162)).toMicroseconds(), Eq(86193978432));
}

TEST(TimeTest, ReturnsTimeAsMilliseconds) {
    ASSERT_THAT(Time(23, 56, 33, Time::Nanoseconds(978432162)).toMilliseconds(), Eq(86193978));
}

TEST(TimeTest, ReturnsTimeAsSeconds) {
    ASSERT_THAT(Time(23, 56, 33, Time::Nanoseconds(978432162)).toSeconds(), Eq(86193));
}

TEST(TimeTest, ReturnsTimeAsMinutes) {
    ASSERT_THAT(Time(23, 56, 33, Time::Nanoseconds(978432162)).toMinutes(), Eq(1436));
}

TEST(TimeTest, ReturnsTimeAsHours) {
    ASSERT_THAT(Time(23, 56, 33, Time::Nanoseconds(978432162)).toHours(), Eq(23));
}

TEST(TimeTest, ReturnsBrokenStdTimeRepresentation) {
    Time myTime(14, 32, 9);
    std::tm tmTime = myTime.toBrokenStdTime();
    ASSERT_THAT(tmTime.tm_hour, Eq(myTime.hour()));
    ASSERT_THAT(tmTime.tm_min, Eq(myTime.minute()));
    ASSERT_THAT(tmTime.tm_sec, Eq(myTime.second()));
}

TEST(TimeTest, ReturnsScalarStdTimeRepresentation) {
    Time myTime(14, 32, 9);
    std::time_t tTime = myTime.toScalarStdTime();
    ASSERT_THAT(tTime, Eq(myTime.toSeconds()));
}

TEST(TimeTest, SerializesToDeserializesFromStream) {
    Time myTime;
    std::stringstream ss;
    ss << Time(14, 32, 9);
    ss >> myTime;
    ASSERT_THAT(myTime, Eq(Time(14, 32, 9)));
}



