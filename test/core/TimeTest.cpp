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
    ASSERT_THAT(myTime.hours(), Eq(tmTime->tm_hour));
    ASSERT_THAT(myTime.minutes(), Eq(tmTime->tm_min));
    ASSERT_THAT(myTime.seconds(), Eq(tmTime->tm_sec));
}

TEST(TimeTest, isValidReturnsFalseIfHoursExceded24) {
    ASSERT_TRUE(Time().isValid());
    ASSERT_FALSE(Time(Time::Hours(25)).isValid());
}

TEST(TimeTest, AddSubtractHours) {
    Time t(Time::Hours(7));

    ASSERT_THAT(t.hours(), Eq(7));
    ASSERT_THAT(t.minutes(), Eq(0));
    ASSERT_THAT(t.seconds(), Eq(0));
    ASSERT_THAT(t.milliseconds(), Eq(0));
    ASSERT_THAT(t.microseconds(), Eq(0));
    ASSERT_THAT(t.nanoseconds(), Eq(0));

    t = t.addDuration(Time::Hours(2));

    ASSERT_THAT(t.hours(), Eq(9));
    ASSERT_THAT(t.minutes(), Eq(0));
    ASSERT_THAT(t.seconds(), Eq(0));
    ASSERT_THAT(t.milliseconds(), Eq(0));
    ASSERT_THAT(t.microseconds(), Eq(0));
    ASSERT_THAT(t.nanoseconds(), Eq(0));

    t = t.subtractDuration(Time::Hours(2));

    ASSERT_THAT(t.hours(), Eq(7));
    ASSERT_THAT(t.minutes(), Eq(0));
    ASSERT_THAT(t.seconds(), Eq(0));
    ASSERT_THAT(t.milliseconds(), Eq(0));
    ASSERT_THAT(t.microseconds(), Eq(0));
    ASSERT_THAT(t.nanoseconds(), Eq(0));
}

TEST(TimeTest, AddSubtractMinutes) {
    Time t(Time::Minutes(178));

    ASSERT_THAT(t.hours(), Eq(2));
    ASSERT_THAT(t.minutes(), Eq(58));
    ASSERT_THAT(t.seconds(), Eq(0));
    ASSERT_THAT(t.milliseconds(), Eq(0));
    ASSERT_THAT(t.microseconds(), Eq(0));
    ASSERT_THAT(t.nanoseconds(), Eq(0));

    t = t.addDuration(Time::Minutes(2));

    ASSERT_THAT(t.hours(), Eq(3));
    ASSERT_THAT(t.minutes(), Eq(0));
    ASSERT_THAT(t.seconds(), Eq(0));
    ASSERT_THAT(t.milliseconds(), Eq(0));
    ASSERT_THAT(t.microseconds(), Eq(0));
    ASSERT_THAT(t.nanoseconds(), Eq(0));

    t = t.subtractDuration(Time::Minutes(2));
    ASSERT_THAT(t.hours(), Eq(2));
    ASSERT_THAT(t.minutes(), Eq(58));
    ASSERT_THAT(t.seconds(), Eq(0));
    ASSERT_THAT(t.milliseconds(), Eq(0));
    ASSERT_THAT(t.microseconds(), Eq(0));
    ASSERT_THAT(t.nanoseconds(), Eq(0));
}

TEST(TimeTest, AddSubtractSeconds) {
    Time t(Time::Seconds(7199));

    ASSERT_THAT(t.hours(), Eq(1));
    ASSERT_THAT(t.minutes(), Eq(59));
    ASSERT_THAT(t.seconds(), Eq(59));
    ASSERT_THAT(t.milliseconds(), Eq(0));
    ASSERT_THAT(t.microseconds(), Eq(0));
    ASSERT_THAT(t.nanoseconds(), Eq(0));

    t = t.addDuration(Time::Seconds(1));

    ASSERT_THAT(t.hours(), Eq(2));
    ASSERT_THAT(t.minutes(), Eq(0));
    ASSERT_THAT(t.seconds(), Eq(0));
    ASSERT_THAT(t.milliseconds(), Eq(0));
    ASSERT_THAT(t.microseconds(), Eq(0));
    ASSERT_THAT(t.nanoseconds(), Eq(0));

    t = t.subtractDuration(Time::Seconds(1));

    ASSERT_THAT(t.hours(), Eq(1));
    ASSERT_THAT(t.minutes(), Eq(59));
    ASSERT_THAT(t.seconds(), Eq(59));
    ASSERT_THAT(t.milliseconds(), Eq(0));
    ASSERT_THAT(t.microseconds(), Eq(0));
    ASSERT_THAT(t.nanoseconds(), Eq(0));
}

TEST(TimeTest, AddSubtractMilliseconds) {
    Time t(Time::Milliseconds(7198943));

    ASSERT_THAT(t.hours(), Eq(1));
    ASSERT_THAT(t.minutes(), Eq(59));
    ASSERT_THAT(t.seconds(), Eq(58));
    ASSERT_THAT(t.milliseconds(), Eq(943));
    ASSERT_THAT(t.microseconds(), Eq(0));
    ASSERT_THAT(t.nanoseconds(), Eq(0));

    t = t.addDuration(Time::Milliseconds(57));

    ASSERT_THAT(t.hours(), Eq(1));
    ASSERT_THAT(t.minutes(), Eq(59));
    ASSERT_THAT(t.seconds(), Eq(59));
    ASSERT_THAT(t.milliseconds(), Eq(0));
    ASSERT_THAT(t.microseconds(), Eq(0));
    ASSERT_THAT(t.nanoseconds(), Eq(0));

    t = t.subtractDuration(Time::Milliseconds(57));

    ASSERT_THAT(t.hours(), Eq(1));
    ASSERT_THAT(t.minutes(), Eq(59));
    ASSERT_THAT(t.seconds(), Eq(58));
    ASSERT_THAT(t.milliseconds(), Eq(943));
    ASSERT_THAT(t.microseconds(), Eq(0));
    ASSERT_THAT(t.nanoseconds(), Eq(0));

}

TEST(TimeTest, AddSubtractMicroseconds) {
    Time t(Time::Microseconds(74362675869));

    ASSERT_THAT(t.hours(), Eq(20));
    ASSERT_THAT(t.minutes(), Eq(39));
    ASSERT_THAT(t.seconds(), Eq(22));
    ASSERT_THAT(t.milliseconds(), Eq(675));
    ASSERT_THAT(t.microseconds(), Eq(869));
    ASSERT_THAT(t.nanoseconds(), Eq(0));

    t = t.addDuration(Time::Microseconds(131));

    ASSERT_THAT(t.hours(), Eq(20));
    ASSERT_THAT(t.minutes(), Eq(39));
    ASSERT_THAT(t.seconds(), Eq(22));
    ASSERT_THAT(t.milliseconds(), Eq(676));
    ASSERT_THAT(t.microseconds(), Eq(0));
    ASSERT_THAT(t.nanoseconds(), Eq(0));

    t = t.subtractDuration(Time::Microseconds(131));

    ASSERT_THAT(t.hours(), Eq(20));
    ASSERT_THAT(t.minutes(), Eq(39));
    ASSERT_THAT(t.seconds(), Eq(22));
    ASSERT_THAT(t.milliseconds(), Eq(675));
    ASSERT_THAT(t.microseconds(), Eq(869));
    ASSERT_THAT(t.nanoseconds(), Eq(0));
}

TEST(TimeTest, AddSubtractNanoseconds) {
    Time t(Time::Nanoseconds(8974362675546));

    ASSERT_THAT(t.hours(), Eq(2));
    ASSERT_THAT(t.minutes(), Eq(29));
    ASSERT_THAT(t.seconds(), Eq(34));
    ASSERT_THAT(t.milliseconds(), Eq(362));
    ASSERT_THAT(t.microseconds(), Eq(675));
    ASSERT_THAT(t.nanoseconds(), Eq(546));

    t = t.addDuration(Time::Nanoseconds(455));

    ASSERT_THAT(t.hours(), Eq(2));
    ASSERT_THAT(t.minutes(), Eq(29));
    ASSERT_THAT(t.seconds(), Eq(34));
    ASSERT_THAT(t.milliseconds(), Eq(362));
    ASSERT_THAT(t.microseconds(), Eq(676));
    ASSERT_THAT(t.nanoseconds(), Eq(1));

    t = t.subtractDuration(Time::Nanoseconds(455));

    ASSERT_THAT(t.hours(), Eq(2));
    ASSERT_THAT(t.minutes(), Eq(29));
    ASSERT_THAT(t.seconds(), Eq(34));
    ASSERT_THAT(t.milliseconds(), Eq(362));
    ASSERT_THAT(t.microseconds(), Eq(675));
    ASSERT_THAT(t.nanoseconds(), Eq(546));
}

TEST(TimeTest, Format12Hours) {
    ASSERT_THAT(Time(Time::Hours(23) + Time::Minutes(45) + Time::Seconds(2)).toString("H:m:s"), StrEq("11:45:2"));
}

TEST(TimeTest, Format12HoursTwoDigits) {
    ASSERT_THAT(Time(Time::Hours(3) + Time::Minutes(45) + Time::Seconds(2)).toString("HH:m:s"), StrEq("03:45:2"));
}

TEST(TimeTest, FormatMeridiemLabelSmallLetters) {
    ASSERT_THAT(Time(Time::Hours(3) + Time::Minutes(45) + Time::Seconds(2)).toString("HH:mm:ss a"), StrEq("03:45:02 am"));
    ASSERT_THAT(Time(Time::Hours(13) + Time::Minutes(45) + Time::Seconds(2)).toString("HH:mm:ss a"), StrEq("01:45:02 pm"));
    ASSERT_THAT(Time(Time::Hours(0)).toString("HH:mm:ss a"), StrEq("00:00:00 am"));
    ASSERT_THAT(Time(Time::Hours(12)).toString("HH:mm:ss a"), StrEq("12:00:00 pm"));
    ASSERT_THAT(Time(Time::Hours(24)).toString("HH:mm:ss a"), StrEq("00:00:00 am"));
}

TEST(TimeTest, FormatMeridiemLabelCapitalLetters) {
    ASSERT_THAT(Time(Time::Hours(3) + Time::Minutes(45) + Time::Seconds(2)).toString("HH:mm:ss A"), StrEq("03:45:02 AM"));
    ASSERT_THAT(Time(Time::Hours(13) + Time::Minutes(45) + Time::Seconds(2)).toString("HH:mm:ss A"), StrEq("01:45:02 PM"));
    ASSERT_THAT(Time(Time::Hours(0)).toString("HH:mm:ss A"), StrEq("00:00:00 AM"));
    ASSERT_THAT(Time(Time::Hours(12)).toString("HH:mm:ss A"), StrEq("12:00:00 PM"));
    ASSERT_THAT(Time(Time::Hours(24)).toString("HH:mm:ss A"), StrEq("00:00:00 AM"));
}

TEST(TimeTest, FormatPM) {
    ASSERT_THAT(Time(Time::Hours(3) + Time::Minutes(45) + Time::Seconds(2)).toString("HH:m:s"), StrEq("03:45:2"));
}

TEST(TimeTest, Format24Hours) {
    ASSERT_THAT(Time(Time::Hours(22) + Time::Minutes(45) + Time::Seconds(2)).toString("h:m:s"), StrEq("22:45:2"));
}

TEST(TimeTest, Format24HoursTwoDigits) {
    ASSERT_THAT(Time(Time::Hours(3) + Time::Minutes(45) + Time::Seconds(2)).toString("hh:m:s"), StrEq("03:45:2"));
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

TEST(TimeTest, Comparisons) {
    ASSERT_TRUE(Time(7, 9, 2, 675869233) < Time(7, 45, 22, 536969233));
    ASSERT_TRUE(Time(7, 9, 2, 536969435) <= Time(7, 9, 2, 536969435));
    ASSERT_TRUE(Time(8, 9, 2, 675869676) > Time(7, 45, 22, 536969212));
    ASSERT_TRUE(Time(7, 46, 2, 675869112) >= Time(7, 45, 22, 536969112));
    ASSERT_TRUE(Time(15, 4, 12, 554969231) == Time(15, 4, 12, 554969231));
    ASSERT_TRUE(Time(7, 9, 2, 675869123) != Time(4, 45, 22, 536969321));
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
    ASSERT_THAT(Time::fromString("1234", "ffff"), Time(Time::Milliseconds(123) + Time::Microseconds(400)));
    ASSERT_THAT(Time::fromString("12345", "fffff"), Time(Time::Milliseconds(123) + Time::Microseconds(450)));
    ASSERT_THAT(Time::fromString("123456", "ffffff"), Time(Time::Milliseconds(123) + Time::Microseconds(456)));
    ASSERT_THAT(Time::fromString("1234567", "fffffff"), Time(Time::Milliseconds(123) + Time::Microseconds(456) + Time::Nanoseconds(700)));
    ASSERT_THAT(Time::fromString("12345678", "ffffffff"), Time(Time::Milliseconds(123) + Time::Microseconds(456) + Time::Nanoseconds(780)));
    ASSERT_THAT(Time::fromString("123456789", "fffffffff"), Time(Time::Milliseconds(123) + Time::Microseconds(456) + Time::Nanoseconds(789)));
    ASSERT_THAT(Time::fromString("14:32:09.123456789", "hh:mm:ss.fffffffff"), Time(14, 32, 9, 123456789));
}

TEST(TimeTest, ReturnsTimeAsNanoseconds) {
    ASSERT_THAT(Time(23, 56, 33, 978432162).toNanoseconds(), Eq(86193978432162));
}

TEST(TimeTest, ReturnsTimeAsMicroseconds) {
    ASSERT_THAT(Time(23, 56, 33, 978432162).toMicroseconds(), Eq(86193978432));
}

TEST(TimeTest, ReturnsTimeAsMilliseconds) {
    ASSERT_THAT(Time(23, 56, 33, 978432162).toMilliseconds(), Eq(86193978));
}

TEST(TimeTest, ReturnsTimeAsSeconds) {
    ASSERT_THAT(Time(23, 56, 33, 978432162).toSeconds(), Eq(86193));
}

TEST(TimeTest, ReturnsTimeAsMinutes) {
    ASSERT_THAT(Time(23, 56, 33, 978432162).toMinutes(), Eq(1436));
}

TEST(TimeTest, ReturnsTimeAsHours) {
    ASSERT_THAT(Time(23, 56, 33, 978432162).toHours(), Eq(23));
}

TEST(TimeTest, ReturnsBrokenStdTimeRepresentation) {
    Time myTime(14, 32, 9);
    std::tm tmTime = myTime.toBrokenStdTime();
    ASSERT_THAT(tmTime.tm_hour, Eq(myTime.hours()));
    ASSERT_THAT(tmTime.tm_min, Eq(myTime.minutes()));
    ASSERT_THAT(tmTime.tm_sec, Eq(myTime.seconds()));
}

TEST(TimeTest, ReturnsScalarStdTimeRepresentation) {
    Time myTime(14, 32, 9);
    std::time_t tTime = myTime.toScalarStdTime();
    ASSERT_THAT(tTime, Eq(myTime.toSeconds()));
}
