//
//  DateTime.cpp
//  DataCpp
//
//  Created by Luis Finke on 11/10/20.
//  Copyright © 2020 Luis Finke. All rights reserved.
//

#include <fgl/time/DateTime.hpp>
#include <fgl/util/PlatformChecks.hpp>
#include <cmath>

#ifndef _WIN32
	#include <sys/time.h>
#endif

#define DATETIME_TM_BASEYEAR 1900

namespace fgl {
	int DateTime::getLocalGMTOffset() {
		time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		struct tm lcl = *localtime(&now);
		struct tm gmt = *gmtime(&now);
		time_t lcl_total = mktime(&lcl);
		time_t gmt_total = mktime(&gmt);
		return (int)((lcl_total - gmt_total)/60);
	}


	DateTime::DateTime()
	: DateTime(std::chrono::system_clock::now()) {
		//
	}

	DateTime::DateTime(std::chrono::system_clock::time_point timePoint)
	: timePoint(timePoint) {
		//
	}

	DateTime::DateTime(time_t timeVal)
	: DateTime(std::chrono::system_clock::from_time_t(timeVal)) {
		//
	}

	DateTime DateTime::fromGmTm(struct tm timeVal) {
		#ifdef _WIN32
			return DateTime(_mkgmtime(&timeVal));
		#else
			return DateTime(timegm(&timeVal));
		#endif
	}

	DateTime DateTime::fromLocalTm(struct tm timeVal) {
		#ifdef _WIN32
			return DateTime(mktime(&timeVal));
		#else
			return DateTime(timelocal(&timeVal));
		#endif
	}

	Optional<DateTime> DateTime::fromGmtString(const char* dateString, const char* format) {
		struct tm timeTm;
		if(strptime(dateString, format, &timeTm) == nullptr) {
			return std::nullopt;
		}
		return fromGmTm(timeTm);
	}

	Optional<DateTime> DateTime::fromGmtString(const std::string& dateString, const std::string& format) {
		return fromGmtString(dateString.c_str(), format.c_str());
	}

	Optional<DateTime> DateTime::fromLocalString(const char* dateString, const char* format) {
		struct tm timeTm;
		if(strptime(dateString, format, &timeTm) == nullptr) {
			return std::nullopt;
		}
		return fromLocalTm(timeTm);
	}

	Optional<DateTime> DateTime::fromLocalString(const std::string& dateString, const std::string& format) {
		return fromLocalString(dateString.c_str(), format.c_str());
	}



	struct tm DateTime::toGmTm() const {
		time_t timeVal = std::chrono::system_clock::to_time_t(timePoint);
		struct tm timeTm;
		auto tmPtr = gmtime_r(&timeVal, &timeTm);
		if(tmPtr == nullptr) {
			throw std::runtime_error("Failed to convert date to GMT tm value");
		}
		return *tmPtr;
	}

	struct tm DateTime::toLocalTm() const {
		time_t timeVal = std::chrono::system_clock::to_time_t(timePoint);
		struct tm timeTm;
		auto tmPtr = localtime_r(&timeVal, &timeTm);
		if(tmPtr == nullptr) {
			throw std::runtime_error("Failed to convert date to local tm value");
		}
		return *tmPtr;
	}

	time_t DateTime::toTimeType() const {
		return std::chrono::system_clock::to_time_t(timePoint);
	}

	const std::chrono::system_clock::time_point& DateTime::getTimePoint() const {
		return timePoint;
	}


	String DateTime::toString() const {
		return toGmtString();
	}

	String DateTime::toGmtString(const char format[]) const {
		auto timeTm = toGmTm();
		char buffer[1024];
		size_t bufferLength = strftime(buffer, 1024, format, &timeTm);
		return String(buffer, bufferLength);
	}

	String DateTime::toLocalString(const char format[]) const {
		auto timeTm = toLocalTm();
		char buffer[1024];
		size_t bufferLength = strftime(buffer, 1024, format, &timeTm);
		return String(buffer, bufferLength);
	}

	String DateTime::toISO8601String(int gmtOffset) const {
		struct tm tmTime = toGmTm();
		char buffer[56];
		strftime(buffer, 56, "%Y-%m-%dT%H:%M:%S", &tmTime);
		String str = buffer;
		
		if(gmtOffset == 0) {
			str += 'Z';
		} else {
			int gmtoff = std::abs(gmtOffset);

			int gmtoff_min = gmtoff % 60;
			String gmtoff_min_str;
			gmtoff_min_str += std::to_string(gmtoff_min);
			while(gmtoff_min_str.length() < 2) {
				gmtoff_min_str = '0' + gmtoff_min_str;
			}

			int gmtoff_hour = gmtoff / 60;
			String gmtoff_hour_str;
			gmtoff_hour_str += std::to_string(gmtoff_hour);
			while(gmtoff_hour_str.length() < 2) {
				gmtoff_hour_str = '0' + gmtoff_hour_str;
			}
			
			if(gmtOffset < 0) {
				str += '-';
			} else {
				str += '+';
			}
			str += gmtoff_hour_str;
			str += ':';
			str += gmtoff_min_str;
		}
		return str;
	}
}
