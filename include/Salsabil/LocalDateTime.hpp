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

#ifndef SALSABIL_LOCALDATETIME_HPP
#define SALSABIL_LOCALDATETIME_HPP

#include "DateTime.hpp"
#include "TimeZone.hpp"

namespace Salsabil {

    class LocalDateTime : public DateTime {
    public:
        /** @name Durations */
        //@{
        /** @brief Nanosecond duration. */
        using Nanoseconds = DateTime::Nanoseconds;
        /** @brief Microsecond duration. */
        using Microseconds = DateTime::Microseconds;
        /** @brief Millisecond duration. */
        using Milliseconds = DateTime::Milliseconds;
        /** @brief Second duration. */
        using Seconds = DateTime::Seconds;
        /** @brief Minute duration. */
        using Minutes = DateTime::Minutes;
        /** @brief Hour duration. */
        using Hours = DateTime::Hours;
        /** @brief Day duration. */
        using Days = DateTime::Days;
        //@}

        /** @name Constructors and Destructors */
        //@{

        /** @brief Default constructor. Constructs an invalid LocalDateTime object. */
        LocalDateTime();

        /** @brief Constructs a LocalDateTime object from the given <i>date</i> and <i>time</i>. */
        LocalDateTime(const DateTime& datetime, const TimeZone& zone);

        /** @brief Copy-constructs a LocalDateTime object from <i>datetime</i>. */
        LocalDateTime(const LocalDateTime& datetime);

        /** @brief Move-constructs a LocalDateTime object from <i>datetime</i>. */
        LocalDateTime(LocalDateTime&& datetime);

        /** @brief Default destructor. */
        virtual ~LocalDateTime() {
        }
        //@}

        /** @brief Returns whether this datetime is valid. */
        bool isValid() const;

        /** 
         * @brief Returns the datetime of this local datetime. 
         * 
         * The returned datetime is the one that was passed to the constructor. For example:
         * {@code
         *     LocalDateTime ldt(DateTime(Date(1998, 3, 1), Time(23, 4, 19)), TimeZone("Etc/GMT+3"));
         *     DateTime dt = ldt.datetime(); // returns DateTime(Date(1998, 3, 1), Time(23, 4, 19)).
         * }
         */
        DateTime datetime() const;

        /** @brief Returns the time zone of this local datetime. */
        TimeZone timezone() const;

        /** @name Comparison Operators */
        //@{
        /** 
         * @brief Returns whether this local datetime is earlier than <i>other</i>. 
         * 
         * The LocalDateTime objects to be compared are first converted to UTC and then compared   are   time is converted to UTC and then this datetime The time zone is taken into account. For example
         * {@code
         *     DateTime dt1(Date(2018, 1, 13), Time(9, 6, 21));
         *     DateTime dt2(Date(2018, 1, 13), Time(12, 6, 21));
         *    
         *     bool isEarlier1 = LocalDateTime(dt1, TimeZone("Etc/GMT+2")) < LocalDateTime(dt2, TimeZone("Etc/GMT+2")); // isEarlier1 is true.
         *     bool isEarlier2 = LocalDateTime(dt1, TimeZone("Etc/GMT+6")) < LocalDateTime(dt1, TimeZone("Etc/GMT+3")); // isEarlier1 is true.
         *     bool isEarlier3 = LocalDateTime(dt1, TimeZone("Etc/GMT+6")) < LocalDateTime(dt2, TimeZone("Etc/GMT+3")); // isEarlier1 is true.
         * }
         */
        bool operator<(const LocalDateTime& other) const;

        /** @brief Returns whether this local datetime is earlier than <i>other</i> or equal to it. */
        bool operator<=(const LocalDateTime& other) const;

        /** @brief Returns whether this local datetime is later than <i>other</i>. */
        bool operator>(const LocalDateTime& other) const;

        /** @brief Returns whether this local datetime is later than <i>other</i> or equal to it. */
        bool operator>=(const LocalDateTime& other) const;

        /** @brief Returns whether this local datetime is equal to <i>other</i>. */
        bool operator==(const LocalDateTime& other) const;

        /** @brief Returns whether this local datetime is different from <i>other</i>. */
        bool operator!=(const LocalDateTime& other) const;
        //@}

        /** @name Assignment Operators */
        //@{
        /** @brief Copy assignment operator. */
        LocalDateTime& operator=(const LocalDateTime& date);

        /** @brief Move assignment operator. */
        LocalDateTime& operator=(LocalDateTime&& date);
        //@}

        /** 
         * @brief Returns this local datetime in Coordinating Universal Time (UTC). 
         * 
         * This method returns the datetime that was passed to the constructor but with "Etc/UTC" time zone. For example:
         * {@code
         *     LocalDateTime ldt(DateTime(Date(1998, 3, 1), Time(23, 4, 19)), TimeZone("Etc/GMT+3"));
         *     LocalDateTime inUTC = ldt.toUTC(); // returns LocalDateTime(DateTime(Date(1998, 3, 1), Time(20, 4, 19)), TimeZone("Etc/UTC")).
         * }
         */
        LocalDateTime toUtc() const;

        /** 
         * @brief Returns the datetime offset from UTC as a LocalDateTime::Seconds duration. 
         * 
         * {@code
         *     LocalDateTime ldt(DateTime(Date(1998, 3, 1), Time(23, 4, 19)), TimeZone("Etc/GMT+3"));
         *     LocalDateTime::Seconds offset = ldt.offestFromUtc(); // LocalDateTime::Seconds(-10800)
         * }
         */
        Seconds offestFromUtc() const;

        /** @brief Returns the current system local datetime. */
        static LocalDateTime current();

    private:
        TimeZone mZone;
    };

    /** 
     * @relates LocalDateTime
     * @name Serialization/Deserialization Functions 
     */
    //@{

    /** @brief Writes <i>ldt</i> to stream <i>os</i> in ISO-8601 date and time format "yyyy-MM-ddThh:mm:ss.fff". */
    std::ostream& operator<<(std::ostream& os, const LocalDateTime& ldt);

    /** @brief Reads a datetime in ISO-8601 date and time format "yyyy-MM-ddThh:mm:ss.fff" from stream <i>is</i> and stores it in <i>dt</i>. */
    std::istream& operator>>(std::istream& is, LocalDateTime& dt);
    //@}
}

#endif // SALSABIL_LOCALDATETIME_HPP

