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

#include "TimeZone.hpp"
#include "date/include/date/tz.h"
#include "Exception.hpp"
#include "iostream"
#include "iomanip"

namespace Salsabil {

    namespace Internal {

        class TimeZoneImpl {
        public:

            TimeZoneImpl(const date::time_zone* ptr) : mTimeZonePtr(ptr) {
            }

            TimeZoneImpl(const std::string& idByIANA) : mTimeZonePtr(date::locate_zone(idByIANA)) {
            }

            const date::time_zone* mTimeZonePtr;
        };
    }
}

using namespace Salsabil;
using namespace Salsabil::Internal;

TimeZone::TimeZone(const TimeZoneImpl& impl) : mImpl(new TimeZoneImpl(impl.mTimeZonePtr)) {
}

TimeZone::TimeZone() : mImpl(nullptr) {
}

TimeZone::TimeZone(const TimeZone& other) : mImpl(other.mImpl) {
}

TimeZone::TimeZone(TimeZone&& other) : mImpl(std::move(other.mImpl)) {
}

TimeZone::TimeZone(const std::string& idByIANA) : mImpl(nullptr) {
    try {
        mImpl = std::shared_ptr<TimeZoneImpl>(new TimeZoneImpl(idByIANA));
    } catch (const std::exception& exp) {
        throw Exception("Time zone " + idByIANA + " is not found");
    }
}

bool TimeZone::operator==(const TimeZone& other) const {
    return this->mImpl->mTimeZonePtr == other.mImpl->mTimeZonePtr;
}

bool TimeZone::operator!=(const TimeZone& other) const {
    return !this->operator==(other);
}

TimeZone& TimeZone::operator=(const TimeZone& other) {
    if (this != &other) {
        mImpl = other.mImpl;
    }
    return *this;
}

TimeZone& TimeZone::operator=(const TimeZone&& other) {
    if (this != &other) {
        mImpl = std::move(other.mImpl);
    }
    return *this;
}

bool TimeZone::isValid() const {
    return mImpl != nullptr;
}

std::string TimeZone::id() const {
    if (!mImpl)
        return std::string();

    return mImpl->mTimeZonePtr->name();
}

std::string TimeZone::abbreviationAt(const DateTime &datetime) const {
    if (!mImpl || !datetime.isValid())
        return std::string();

    return mImpl->mTimeZonePtr->get_info(datetime.toStdTimePoint()).abbrev;
}

std::chrono::seconds TimeZone::offsetAt(const DateTime &datetime) const {
    if (!mImpl || !datetime.isValid())
        return std::chrono::seconds(0);

    return mImpl->mTimeZonePtr->get_info(datetime.toStdTimePoint()).offset;
}

std::chrono::seconds TimeZone::daylightOffsetAt(const DateTime &datetime) const {
    if (!mImpl || !datetime.isValid())
        return std::chrono::seconds(0);

    return mImpl->mTimeZonePtr->get_info(datetime.toStdTimePoint()).save;
}

std::chrono::seconds TimeZone::standardOffsetAt(const DateTime &datetime) const {
    if (!mImpl || !datetime.isValid())
        return std::chrono::seconds(0);

    auto info = mImpl->mTimeZonePtr->get_info(datetime.toStdTimePoint());
    return info.offset - info.save;
}

DateTime TimeZone::transitionBefore(const DateTime &datetime) const {
    if (!mImpl || !datetime.isValid())
        return DateTime();

    const DateTime& begin = DateTime(mImpl->mTimeZonePtr->get_info(datetime.toStdTimePoint()).begin);

    if (begin.date() > Date(1700, 1, 1))
        return begin;

    return DateTime();
}

DateTime TimeZone::transitionAfter(const DateTime &datetime) const {
    if (!mImpl || !datetime.isValid())
        return DateTime();

    return DateTime(mImpl->mTimeZonePtr->get_info(datetime.toStdTimePoint()).end.time_since_epoch());
}

std::string TimeZone::toStringAt(const DateTime& datetime, const std::string& format) const {
    if (!isValid() || !datetime.isValid())
        return std::string();

    std::stringstream output;
    for (int pos = 0; pos < format.size(); ++pos) {
        const int charCount = Utility::countIdenticalCharsFrom(pos, format);

        if (format[pos] == 'z') {
            if (charCount == 1) {
                std::chrono::seconds _seconds = offsetAt(datetime);
                output << (_seconds.count() < 0 ? '-' : '+')
                        << std::setfill('0') << std::setw(2) << std::abs(std::chrono::duration_cast<std::chrono::hours>(_seconds).count())
                        << std::setfill('0') << std::setw(2) << std::abs(std::chrono::duration_cast<std::chrono::minutes>(_seconds % std::chrono::hours(1)).count());
            } else if (charCount == 2) {
                std::chrono::seconds _seconds = offsetAt(datetime);
                output << (_seconds.count() < 0 ? '-' : '+')
                        << std::setfill('0') << std::setw(2) << std::abs(std::chrono::duration_cast<std::chrono::hours>(_seconds).count())
                        << ':'
                        << std::setfill('0') << std::setw(2) << std::abs(std::chrono::duration_cast<std::chrono::minutes>(_seconds % std::chrono::hours(1)).count());
            } else if (charCount == 3) {
                output << abbreviationAt(datetime);
            } else if (charCount == 4) {
                output << id();
            }
            pos += charCount - 1; // skip all identical characters except the last.
        } else {
            output << format[pos];
        }
    }

    return output.str();
}

TimeZone TimeZone::current() {
    return TimeZone(date::current_zone());
}

TimeZone TimeZone::utc() {
    return TimeZone("Etc/UTC");
}

std::vector<TimeZone> TimeZone::availableTimeZones() {
    std::vector<TimeZone> zoneList;
    for (const auto& z : date::get_tzdb().zones)
        zoneList.push_back(TimeZone(TimeZoneImpl(&z)));

    return zoneList;
}

std::vector<std::string> TimeZone::availableTimeZoneIds() {
    std::vector<std::string> idList;
    for (const auto& z : date::get_tzdb().zones)
        idList.push_back(z.name());

    return idList;
}

bool TimeZone::isAvailable(const std::string& idByIANA) {
    for (const auto& z : availableTimeZones())
        if (z.id() == idByIANA)
            return true;

    return false;
}

std::string TimeZone::databaseVersion() {
    return date::get_tzdb().version;
}

std::string TimeZone::toWindowsId(const std::string& ianaId) {
    for (const auto& z : date::get_tzdb().mappings)
        if (z.type == ianaId)
            return z.other;

    return std::string();
}

std::string TimeZone::toIanaId(const std::string& windowsId, const std::string& territory) {
    std::string _territory = territory.empty() ? "001" : territory;
    for (const auto& z : date::get_tzdb().mappings)
        if (z.other == windowsId && z.territory == _territory) {
            return z.type;
        }

    return std::string();
}

std::vector<std::string> TimeZone::toIanaIds(const std::string& windowsId) {
    std::vector<std::string> idList;
    for (const auto& z : date::get_tzdb().mappings)
        if (z.other == windowsId)
            idList.push_back(z.type);

    return idList;
}
