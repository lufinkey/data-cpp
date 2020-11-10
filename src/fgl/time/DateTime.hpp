//
//  DateTime.hpp
//  DataCpp
//
//  Created by Luis Finke on 11/10/20.
//  Copyright © 2020 Luis Finke. All rights reserved.
//

#pragma once

#include <fgl/data/Common.hpp>
#include <fgl/data/String.hpp>
#include <chrono>
#include <ctime>

namespace fgl {
	class DateTime {
	public:
		/* Constructs a DateTime object with the current date and time */
		DateTime();
		/* Constructs a DateTime object with the give system clock time point */
		DateTime(std::chrono::system_clock::time_point timePoint);
		/* Constructs local DateTime from GMT time_t */
		DateTime(time_t);
		
		static int getLocalUTCOffset();
		
		uint32_t getMicrosecond() const;
		uint32_t getMillisecond() const;
		uint8_t getSecond() const;
		uint8_t getMinute() const;
		uint8_t getHour() const;
		uint8_t getDayOfMonth() const;
		uint8_t getDayOfWeek() const;
		uint16_t getDayOfYear() const;
		uint8_t getMonth() const;
		int32_t getYear() const;
		
		struct tm toTm() const;
		time_t toTimeType() const;
		std::chrono::system_clock::time_point toChronoTimePoint() const;
		
		String toString(const char format[] = "%Y-%m-%d %H:%M:%S") const;
		String toISO8601String(int utcOffset = getLocalUTCOffset()) const;
		
	private:
		/* microsecond (000000-999999) */
		uint32_t usec;
		/* second (00-61) (generally 00-59. the extra 2 seconds are for leap seconds) */
		uint8_t sec;
		/* minute (00-59) */
		uint8_t min;
		/* hour (00-23) */
		uint8_t hour;
		/* day of the month (01-31) */
		uint8_t mday;
		/* month (01-12) */
		uint8_t mon;
		/* year (0000-9999) */
		int32_t year;
		/* day of the week (sunday=1) (1-7) */
		uint8_t wday;
		/* day of the year (001-366) */
		uint16_t yday;
	};
}
