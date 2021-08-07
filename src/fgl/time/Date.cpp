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

#ifndef _WIN32
	#include <sys/time.h>
#endif

#define DATE_TM_BASEYEAR 1900

namespace fgl {
	Date::Date()
	: Date(std::chrono::system_clock::now()) {
		//
	}

	Date::Date(std::chrono::system_clock::time_point timePoint)
	: timePoint(timePoint) {
		//
	}

	Date::Date(time_t timeVal)
	: Date(std::chrono::system_clock::from_time_t(timeVal)) {
		//
	}

	Date Date::fromGmTm(struct tm timeVal) {
		#ifdef _WIN32
			return Date(_mkgmtime(&timeVal));
		#else
			return Date(timegm(&timeVal));
		#endif
	}

	Date Date::fromLocalTm(struct tm timeVal) {
		#ifdef _WIN32
			return Date(mktime(&timeVal));
		#else
			return Date(timelocal(&timeVal));
		#endif
	}

	Optional<Date> Date::fromGmtString(String dateString, String format) {
		auto df = DateFormatter{
			.format = format,
			.timeZone = TimeZone(0)
		};
		return df.dateFromString(dateString);
	}

	Optional<Date> Date::fromLocalString(String dateString, String format) {
		auto df = DateFormatter{
			.format = format,
			.timeZone = TimeZone::currentAlways()
		};
		return df.dateFromString(dateString);
	}



	struct tm Date::toGmTm() const {
		time_t timeVal = std::chrono::system_clock::to_time_t(timePoint);
		struct tm timeTm;
		auto tmPtr = gmtime_r(&timeVal, &timeTm);
		if(tmPtr == nullptr) {
			throw std::runtime_error("Failed to convert date to GMT tm value");
		}
		return *tmPtr;
	}

	struct tm Date::toLocalTm() const {
		time_t timeVal = std::chrono::system_clock::to_time_t(timePoint);
		struct tm timeTm;
		auto tmPtr = localtime_r(&timeVal, &timeTm);
		if(tmPtr == nullptr) {
			throw std::runtime_error("Failed to convert date to local tm value");
		}
		return *tmPtr;
	}

	time_t Date::toTimeVal() const {
		return std::chrono::system_clock::to_time_t(timePoint);
	}

	const Date::TimePoint& Date::getTimePoint() const {
		return timePoint;
	}


	String Date::toString() const {
		return toGmtString();
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

	String Date::toISO8601String(TimeZone timeZone) const {
		auto df = DateFormatter{
			.format = "%Y-%m-%dT%H:%M:%S%.f%z",
			.timeZone = timeZone
		};
		return df.stringFromDate(*this);
	}
}
