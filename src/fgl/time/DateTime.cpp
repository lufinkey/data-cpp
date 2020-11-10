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
	DateTime::DateTime()
	: DateTime(std::chrono::system_clock::now()) {
		//
	}

	DateTime::DateTime(std::chrono::system_clock::time_point timePoint)
	: DateTime(std::chrono::system_clock::to_time_t(timePoint)) {
		auto us = std::chrono::time_point_cast<std::chrono::microseconds>(timePoint) - std::chrono::time_point_cast<std::chrono::seconds>(timePoint);
		usec = (int)std::chrono::duration_cast<std::chrono::microseconds>(us).count();
	}

	DateTime::DateTime(time_t timeVal) {
		struct tm lcl = *localtime(&timeVal);
		usec = 0;
		sec = (uint8_t)lcl.tm_sec;
		min = (uint8_t)lcl.tm_min;
		hour = (uint8_t)lcl.tm_hour;
		mday = (uint8_t)lcl.tm_mday;
		mon = (uint8_t)(lcl.tm_mon + 1);
		year = (int32_t)(lcl.tm_year + DATETIME_TM_BASEYEAR);
		wday = (uint8_t)(lcl.tm_wday + 1);
		yday = (uint16_t)(lcl.tm_yday + 1);
	}

	int DateTime::getLocalUTCOffset() {
		time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		struct tm lcl = *localtime(&now);
		struct tm gmt = *gmtime(&now);
		time_t lcl_total = mktime(&lcl);
		time_t gmt_total = mktime(&gmt);
		return (int)((lcl_total - gmt_total)/60);
	}

	uint32_t DateTime::getMicrosecond() const {
		return usec;
	}

	uint32_t DateTime::getMillisecond() const {
		return usec / 1000;
	}
	
	uint8_t DateTime::getSecond() const {
		return sec;
	}
	
	uint8_t DateTime::getMinute() const {
		return min;
	}
	
	uint8_t DateTime::getHour() const {
		return hour;
	}
	
	uint8_t DateTime::getDayOfMonth() const {
		return mday;
	}
	
	uint8_t DateTime::getDayOfWeek() const {
		return wday;
	}
	
	uint16_t DateTime::getDayOfYear() const {
		return yday;
	}
	
	uint8_t DateTime::getMonth() const {
		return mon;
	}
	
	int32_t DateTime::getYear() const {
		return year;
	}



	struct tm DateTime::toTm() const {
		struct tm tmTime;
		tmTime.tm_sec = (int)sec;
		tmTime.tm_min = (int)min;
		tmTime.tm_hour = (int)hour;
		tmTime.tm_mday = (int)mday;
		tmTime.tm_mon = (int)mon - 1;
		tmTime.tm_year = (int)year - DATETIME_TM_BASEYEAR;
		tmTime.tm_wday = (int)wday - 1;
		tmTime.tm_yday = (int)yday - 1;
		tmTime.tm_isdst = -1;
		#ifdef HAVE_TM_GMTOFF
			tmTime.tm_gmtoff = 0;
		#endif
		return tmTime;
	}

	time_t DateTime::toTimeType() const {
		struct tm tmTime = toTm();
		#ifdef _WIN32
		time_t timeVal = _mkgmtime(&tmTime);
		#else
		time_t timeVal = timegm(&tmTime);
		#endif
		return timeVal;
	}

	std::chrono::system_clock::time_point DateTime::toChronoTimePoint() const {
		auto timePoint = std::chrono::system_clock::from_time_t(toTimeType());
		return timePoint + std::chrono::microseconds(usec);
	}



	String DateTime::toString(const char format[]) const {
		struct tm tmTime = toTm();
		char buffer[1024];
		strftime(buffer, 1024, format, &tmTime);
		return buffer;
	}

	String DateTime::toISO8601String(int utcOffset) const {
		struct tm tmTime = toTm();
		char buffer[56];
		strftime(buffer, 56, "%Y-%m-%dT%H:%M:%S", &tmTime);
		String str = buffer;
		
		if(utcOffset == 0) {
			str += 'Z';
		} else {
			int gmtoff = std::abs(utcOffset);

			int gmtoff_min = gmtoff % 60;
			String gmtoff_min_str;
			gmtoff_min_str += gmtoff_min;
			while(gmtoff_min_str.length() < 2) {
				gmtoff_min_str = '0' + gmtoff_min_str;
			}

			int gmtoff_hour = gmtoff/60;
			String gmtoff_hour_str;
			gmtoff_hour_str += gmtoff_hour;
			while(gmtoff_hour_str.length() < 2) {
				gmtoff_hour_str = '0' + gmtoff_hour_str;
			}
			
			if(utcOffset < 0) {
				str += '-';
			}
			else {
				str += '+';
			}
			str += gmtoff_hour;
			str += ':';
			str += gmtoff_min;
		}
		return str;
	}
}
