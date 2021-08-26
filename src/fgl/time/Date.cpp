//
//  Date.cpp
//  DataCpp
//
//  Created by Luis Finke on 11/10/20.
//  Copyright © 2020 Luis Finke. All rights reserved.
//

#include <fgl/time/Date.hpp>
#include <fgl/time/DateFormatter.hpp>
#include <fgl/util/PlatformChecks.hpp>
#include <cmath>
#include <cstring>

#ifndef _WIN32
	#include <sys/time.h>
#endif

namespace fgl {
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
