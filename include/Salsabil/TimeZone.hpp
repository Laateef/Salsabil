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

#ifndef SALSABIL_TIMEZONE_HPP
#define SALSABIL_TIMEZONE_HPP

#include "DateTime.hpp"

#include <memory>

namespace Salsabil {

    namespace Internal {
        class TimeZoneImpl;
    }

    /**
     * @class TimeZone
     * @brief TimeZone is an immutable class representing an IANA time zone, such as "Europe/Istanbul".
     * 
     * TimeZone can be used to convert between universal time (UTC) and local time in a specific time zone.
     * 
     * The IANA IDs and database are also known as the Olson IDs and database, named after their creator.
     * The IANA database contains time zone information that represents the history of local time for many representative locations around the globe. It is updated every few months to reflect changes made by political bodies to time zone boundaries, UTC offsets, and daylight-saving rules. The database also maintains a list of leap seconds from 1972 through the present.
     * 
     * TimeZone uses the IANA time zone IDs as defined in the [IANA time zone database](http://www.iana.org/time-zones), ensuring standard TZIDs on all platforms. 
     * Most platforms support the IANA IDs and the IANA Database natively, but on Windows a mapping is required to the native Windows IDs because Windows time zones cover larger geographic areas and are thus less accurate in their conversions. They also do not support as much historic conversion data and so may only be accurate for the current year.
     * TimeZone uses a conversion table derived form the [Unicode CLDR data](http://www.unicode.org/cldr/charts/latest/supplemental/zone_tzid.html) to map between IANA IDs and Windows IDs.
     * 
     * The IANA IDs can and do change on a regular basis, and can vary depending on how recently the database was updated. Hence, it is better to use availableTimeZoneIds() to determine what IANA IDs are available.
     * The IANA database version can be queried using databaseVersion().
     * 
     * Default-constructed TimeZone objects are invalid (calling isValid() on them returns false). TimeZone objects can be created by giving an IANA ID to the constructor. 
     * The current() method returns a TimeZone object that represents the current system IANA time zone.
     * 
     * The time zone information can be accessed though id(), abbreviationAt(), offsetAt(), standardOffsetAt(), daylightOffsetAt(), transitionBefore(), and transitionAfter().
     * To check if a specific time zone is available, or to list all the available time zones in the database, use isAvailable(), availableTimeZones() or availableTimeZoneIds().
     * 
     * TimeZone provides access to the mapping between IANA IDs and Windows ones through the methods toWindowsId(), toIanaId() and toIanaIds().
     * @section offset_section Time Zone Offset
     * The difference between the universal time(UTC) and the local time in a time zone is expressed as std::chrono::seconds offset from UTC, i.e. the number of seconds to add to UTC to obtain the local time. The total offset is comprised of two component parts, the standard time offset and the daylight-saving time offset. The standard time offset is the number of seconds to add to the universal time (UTC) to obtain the standard time in the time zone. The daylight-saving time offset is the number of seconds to add to the standard time to obtain the local time with daylight-saving (DST) in the time zone.
     * 
     * However, for the IANA "Etc/GMT..." ID set, The offset specifies the time duration you must add to the local time to get the universal time. For example: 
     * {@code
     *     // The "Etc/GMT-3" IANA ID is equivalent to the "UTC+3" Windows ID. "GMT-3" and "UTC+3" refer to the same offset; three hours ahead of GMT/UTC.
     *     TimeZone tz1("Etc/GMT-3"); 
     *     std::chrono::seconds offset1 = tz1.offsetAt(DateTime::current()); // offset1 is std::chrono::seconds(10800).
     * 
     *     // Similarly, the "Etc/GMT+3" IANA ID is equivalent to the "UTC-3" Windows ID. "GMT+3" and "UTC-3" refer to the same offset; three hours behind GMT/UTC.
     *     TimeZone tz2("Etc/GMT+3"); 
     *     std::chrono::seconds offset2 = tz2.offsetAt(DateTime::current()); // offset2 is std::chrono::seconds(-10800).
     * 
     *     // However, when you are not using GMT time zones, the offset specifies the time value you must add to the universal time to get the local time.
     *     TimeZone tz3("Europe/Istanbul");
     *     std::chrono::seconds offset3 = tz3.offsetAt(DateTime(DateTime(Date(2018, 1, 13), Time(21, 12, 36)))); // offset3 is std::chrono::seconds(10800).
     * }
     * 
     * TimeZone depends on data obtained from the CLDR data files under the terms of the Unicode Data Files and Software License. See Unicode CLDR (Unicode Common Locale Data Repository) for more details.
     */
    class TimeZone {
    public:
        /// @name Constructors and Destructors
        //@{

        /// Default constructor. Constructs an invalid TimeZone object (calling isValid() on it returns false).
        TimeZone();

        /// Copy-constructs a TimeZone object from ***other***.
        TimeZone(const TimeZone& other);

        /// Move-constructs a TimeZone object from ***other***.
        TimeZone(TimeZone&& other);

        /** 
         * @brief Constructs a TimeZone object representing the IANA time zone identified by ***ianaId***. 
         * 
         * @throw Exception if the identifier not found in the IANA time zone database.  
         */
        TimeZone(const std::string& ianaId);

        /// Default destructor.

        virtual ~TimeZone() {
        }
        //@}

        /// @name Assignment Operators
        //@{
        /// @brief Copy assignment operator.
        TimeZone& operator=(const TimeZone& other);

        /// @brief Move assignment operator.
        TimeZone& operator=(const TimeZone&& other);
        //@}

        /// @name Comparison Operators
        //@{
        /// Returns whether this datetime is equal to ***other***.
        bool operator==(const TimeZone& other) const;

        /// Returns whether this datetime is different from ***other***.
        bool operator!=(const TimeZone& other) const;
        //@}

        /// @name Querying Methods
        //@{
        /// Returns whether this time zone is valid. Default-constructed time zones are invalid.
        bool isValid() const;

        /// Returns the IANA ID for this time zone.
        std::string id() const;

        /**
         * @brief Returns the time zone name abbreviation at the given ***datetime***. 
         * 
         * **Note** that the abbreviation is not guaranteed to be unique to this time zone because it may change depending on daylight saving or historical events. So the abbreviation of a time zone should not be used in place of its ID.
         */
        std::string abbreviationAt(const DateTime &datetime) const;

        /** 
         * @brief Returns the total effective offset from UTC at ***datetime***.
         * 
         * This function returns the number of seconds to add to a UTC time to obtain the local time at ***datetime*** in this time zone,  i.e. it is the sum of standardOffsetAt() and daylightOffsetAt() at ***datetime***.
         * For example, for the time zone "Europe/Istanbul" at "2018-01-10", the standard time offset is +10800 seconds and there is no daylight time offset. So this function will return +10800 seconds as a total offset from UTC at this datetime. 
         * 
         * @see @link offset_section Time Zone Offset @endlink
         */
        std::chrono::seconds offsetAt(const DateTime &datetime) const;

        /** 
         * @brief Returns the standard time offset from UTC at ***datetime***. 
         * 
         * This function returns the number of seconds to add to a UTC time to obtain the local standard time at ***datetime*** in this time zone, regardless whether there is any daylight saving offset at that time.
         * For example, for the time zone "Europe/Istanbul" at "2000-06-01", the standard time offset is +7200 seconds and the daylight offset is +3600. So this function will return +7200 seconds regardless of the daylight offset.
         * 
         * @see @link offset_section Time Zone Offset @endlink
         */
        std::chrono::seconds standardOffsetAt(const DateTime &datetime) const;

        /** 
         * @brief Returns the daylight-saving time offset at ***datetime***.
         * 
         * This function returns the number of seconds to add to the standard time (i.e., the UTC time plus the standard offset) to obtain the local daylight-saving time at ***datetime***.
         * For example, for the time zone "Europe/Istanbul", at "2000-02-01" it returns 0 and at "2000-05-01" it returns 3600.
         * 
         * @see @link offset_section Time Zone Offset @endlink
         */
        std::chrono::seconds daylightOffsetAt(const DateTime &datetime) const;

        /** 
         * @brief Returns the datetime at which a transition occurred before ***datetime*** in this time zone, or returns the given ***datetime*** if the transition occurs at it.
         * 
         * **Note** that if there is no transition occurred before ***datetime***, an invalid DateTime object will be returned. 
         * {@code
         *     TimeZone tz("Etc/GMT-3");
         *     Datetime previousTransition = tz.transitionBefore(DateTime(Date(2018, 11, 1)));
         *     bool state = previousTransition.isValid(); // returns false, i.e. there is no transition before this datetime.
         * }
         */
        DateTime transitionBefore(const DateTime &datetime) const;

        /// Returns the datetime at which a transition occurred after ***datetime*** in this time zone, or returns the given ***datetime*** if the transition occurs at it.
        DateTime transitionAfter(const DateTime &datetime) const;
        //@}


        /// @name Formatting Method 
        //@{
        /**
         * @brief Returns a string representation of this time zone at ***datetime***, formatted according to the formatter string ***format***.
         * 
         * The formatter string may contain the following patterns:
         * 
         *    Pattern   |                     Meaning                    
         *  ----------- | -------------------------------------------------
         *  z           |  time-zone offset without a colon, such as +0100                        
         *  zz          |  time-zone offset with a colon, such as +01:00                     
         *  zzz         |  time-zone abbreviation, such as "CEST" or "+03"                      
         *  zzzz        |  time-zone ID, such as "Europe/Istanbul"                  
         * 
         * Any character in the formatter string not listed above will be inserted as is into the output string. 
         * If the time zone is invalid, an empty string will be returned.
         */
        std::string toStringAt(const DateTime& datetime, const std::string& format) const;
        //@}

        /// Returns a TimeZone object that represents the system time zone currently in use.
        static TimeZone current();

        /// Returns a TimeZone object that represents the UTC time zone.
        static TimeZone utc();

        /// Returns the available time zones in this system as a list of TimeZone objects.
        static std::vector<TimeZone> availableTimeZones();

        /// Returns the available time zones in this system as a time zone id list.
        static std::vector<std::string> availableTimeZoneIds();

        /// Checks whether the time zone identified by ***ianaId*** exists in this system.
        static bool isAvailable(const std::string& ianaId);

        /// Returns the version of the IANA database in use.
        static std::string databaseVersion();

        /// @name Conversion Methods
        //@{
        /// Returns the Windows ID for the IANA time zone identified by ***ianaId***.
        static std::string toWindowsId(const std::string& ianaId);

        /** 
         * @brief Returns the IANA ID for the Windows time zone identified by ***windowsId***. 
         * 
         * Because a Windows time zone ID may cover several IANA IDs which correspond to several different countries, the second parameter ***territory*** represents the country required. Otherwise, if the second parameter is not passed, the default IANA ID will be returned.
         * For example, the default IANA ID for the Windows time zone "W. Europe Standard Time" is "Europe/Berlin". 
         * For further information about the territory codes that maybe passed or the time zone default IDs, check [Mapping Information](http://www.unicode.org/cldr/charts/latest/supplemental/zone_tzid.html) and [Territory Information](http://www.unicode.org/cldr/charts/latest/supplemental/territory_information.html).  
         */
        static std::string toIanaId(const std::string& windowsId, const std::string& territory = std::string());

        /** 
         * @brief Returns the IANA ID list for the Windows time zone identified by ***windowsId***. 
         * 
         * Because a Windows time zone ID may cover several IANA IDs which correspond to several different countries, this function returns a list of all IANA IDs that map to the windows time zone identified by ***windowsId***. 
         */
        static std::vector<std::string> toIanaIds(const std::string& windowsId);
        //@}

    private:
        TimeZone(const Internal::TimeZoneImpl& impl);
        std::shared_ptr<Internal::TimeZoneImpl> mImpl;
    };
}

#endif // SALSABIL_TIMEZONE_HPP

