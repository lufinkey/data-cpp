//
//  Date.cpp
//  DataCpp
//
//  Created by Luis Finke on 11/10/20.
//  Copyright © 2020 Luis Finke. All rights reserved.
//

#include <fgl/time/Date.hpp>
#include <fgl/time/DateFormatter.hpp>
#include <fgl/data/Number.hpp>
#include <fgl/util/PlatformChecks.hpp>
#include <cmath>
#include <cstring>

#ifndef _WIN32
	#include <sys/time.h>
#endif

namespace fgl {
	inline Date Date_1970() {
		if(Date::epochIs1970()) {
			return Date::epoch();
		} else {
			return Date::fromISOString("1970-01-01T00:00:00Z");
		}
	}

	Optional<bool> Date_epochIs1970 = std::nullopt;
	bool Date::epochIs1970() {
		if(Date_epochIs1970.hasValue()) {
			return Date_epochIs1970.value();
		}
		if(Date::epoch().timePoint == Date::fromISOString("1970-01-01T00:00:00Z").timePoint) {
			Date_epochIs1970 = true;
			return true;
		} else {
			Date_epochIs1970 = false;
			return false;
		}
	}

	Date Date::fromTimeSince1970(TimeInterval timeSince1970) {
		return Date_1970() + timeSince1970;
	}

	Date Date::fromSecondsSince1970(double secsSince1970) {
		auto date1970 = Date_1970();
		auto flatSecsSince1970 = numeric_cast<long long>(secsSince1970);
		auto remainder = secsSince1970 - numeric_cast<double>(flatSecsSince1970);
		auto remainderUnits = numeric_cast<long long>(remainder * numeric_cast<double>(TimeInterval::period::den));
		return date1970 + std::chrono::seconds(flatSecsSince1970) + TimeInterval(remainderUnits);
	}

	Date Date::fromGmTm(struct tm timeVal) {
		#ifdef _WIN32
			return Date::fromCTime(_mkgmtime(&timeVal));
		#else
			return Date::fromCTime(timegm(&timeVal));
		#endif
	}

	Date Date::fromLocalTm(struct tm timeVal) {
		#ifdef _WIN32
			return Date::fromCTime(mktime(&timeVal));
		#else
			return Date::fromCTime(timelocal(&timeVal));
		#endif
	}

	Date Date::fromGmtString(String dateString, String format) {
		auto df = DateFormatter{
			.format = format,
			.timeZone = TimeZone(0)
		};
		return df.dateFromString(dateString)
			.valueOrThrow(
				std::invalid_argument(
					String::join({"failed to parse GMT date string \"",dateString,"\" with format \"",format,"\""})));
	}

	Date Date::fromLocalString(String dateString, String format) {
		auto df = DateFormatter{
			.format = format,
			.timeZone = TimeZone::currentAlways()
		};
		return df.dateFromString(dateString)
			.valueOrThrow(
				std::invalid_argument(
					String::join({"failed to parse local date string \"",dateString,"\" with format \"",format,"\""})));
	}

	Date Date::fromISOString(String dateString, TimeZone timeZone) {
		return maybeFromISOString(dateString, timeZone)
			.valueOrThrow(
				  std::invalid_argument(
					  String::join({"failed to parse ISO date string \"",dateString,"\""})));
	}

	Optional<Date> Date::maybeFromISOString(String dateString, TimeZone timeZone) {
		auto df = DateFormatter{
			.format = "%Y-%m-%dT%H:%M:%S%z",
			.timeZone = timeZone
		};
		return df.dateFromString(dateString);
	}



	Optional<Date> Date::maybeParse(String dateString) {
		ArrayList<DateFormatter> formatters = {
			DateFormatter{
				.format = "%Y-%m-%dT%H:%M:%S%z",
				.timeZone = TimeZone::gmt()
			},
			DateFormatter{
				.format = "%Y-%m-%dT%H:%M:%S",
				.timeZone = TimeZone::currentAlways()
			},
			DateFormatter{
				.format = "%a, %b %d %Y %H:%M:%S %Z",
				.timeZone = TimeZone::gmt()
			},
			DateFormatter{
				.format = "%a, %b %d %Y %H:%M:%S",
				.timeZone = TimeZone::currentAlways()
			},
			DateFormatter{
				.format = "%a %b %d %Y %H:%M:%S %Z",
				.timeZone = TimeZone::gmt()
			},
			DateFormatter{
				.format = "%a %b %d %Y %H:%M:%S",
				.timeZone = TimeZone::currentAlways()
			},
			DateFormatter{
				.format = "%b %d %Y %H:%M:%S %Z",
				.timeZone = TimeZone::gmt()
			},
			DateFormatter{
				.format = "%b %d %Y %H:%M:%S",
				.timeZone = TimeZone::currentAlways()
			}
		};
		for(auto& formatter : formatters) {
			if(auto date = formatter.dateFromString(dateString)) {
				return date.value();
			}
		}
		return std::nullopt;
	}

	Date Date::parse(String dateString) {
		return maybeParse(dateString)
			.valueOrThrow(std::invalid_argument("Invalid date string "+dateString));
	}



	struct tm Date::toGmTm() const {
		time_t timeVal = std::chrono::system_clock::to_time_t(timePoint);
		struct tm timeTm;
		std::memset(&timeTm, 0, sizeof(timeTm));
		timeTm.tm_isdst = -1;
		auto tmPtr = gmtime_r(&timeVal, &timeTm);
		if(tmPtr == nullptr) {
			throw std::runtime_error("Failed to convert date to GMT tm value");
		}
		return *tmPtr;
	}

	struct tm Date::toLocalTm() const {
		time_t timeVal = std::chrono::system_clock::to_time_t(timePoint);
		struct tm timeTm;
		std::memset(&timeTm, 0, sizeof(timeTm));
		timeTm.tm_isdst = -1;
		auto tmPtr = localtime_r(&timeVal, &timeTm);
		if(tmPtr == nullptr) {
			throw std::runtime_error("Failed to convert date to local tm value");
		}
		return *tmPtr;
	}

	TimeInterval Date::timeSince1970() const {
		return timePoint - Date_1970().timePoint;
	}

	double Date::secondsSince1970() const {
		auto date1970 = Date_1970();
		auto timeSince1970 = timePoint - date1970.timePoint;
		auto secondsSince1970 = std::chrono::duration_cast<std::chrono::seconds>(timeSince1970).count();
		auto remainder = timeSince1970 - std::chrono::seconds(secondsSince1970);
		return ((double)secondsSince1970) + ((double)remainder.count() / (double)decltype(remainder)::period::den);
	}


	String Date::toString() const {
		return toISOString();
	}

	String Date::toGmtString(String format) const {
		auto df = DateFormatter{
			.format = format,
			.timeZone = TimeZone(0)
		};
		return df.stringFromDate(*this);
	}

	String Date::toLocalString(String format) const {
		auto df = DateFormatter{
			.format = format,
			.timeZone = TimeZone::currentAlways()
		};
		return df.stringFromDate(*this);
	}

	String Date::toISOString(TimeZone timeZone) const {
		auto df = DateFormatter{
			.format = "%Y-%m-%dT%H:%M:%S%.f%z",
			.timeZone = timeZone
		};
		return df.stringFromDate(*this);
	}
}
