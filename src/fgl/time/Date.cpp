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
			return Date::fromTimeVal(_mkgmtime(&timeVal));
		#else
			return Date::fromTimeVal(timegm(&timeVal));
		#endif
	}

	Date Date::fromLocalTm(struct tm timeVal) {
		#ifdef _WIN32
			return Date::fromTimeVal(mktime(&timeVal));
		#else
			return Date::fromTimeVal(timelocal(&timeVal));
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
		auto df = DateFormatter{
			.format = "%Y-%m-%dT%H:%M:%S%z",
			.timeZone = timeZone
		};
		return df.dateFromString(dateString)
			.valueOrThrow(
				  std::invalid_argument(
					  String::join({"failed to parse ISO date string \"",dateString,"\""})));
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

	String Date::toISOString() const {
		auto df = DateFormatter{
			.format = "%Y-%m-%dT%H:%M:%S%.f%z",
			.timeZone = TimeZone(0)
		};
		return df.stringFromDate(*this);
	}
}
