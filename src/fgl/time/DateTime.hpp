//
//  DateTime.hpp
//  DataCpp
//
//  Created by Luis Finke on 11/10/20.
//  Copyright © 2020 Luis Finke. All rights reserved.
//

#pragma once

#include <fgl/data/Common.hpp>
#include <fgl/data/Optional.hpp>
#include <fgl/data/String.hpp>
#include <chrono>
#include <ctime>

namespace fgl {
	class DateTime {
	public:
		static int getLocalGMTOffset();
		
		DateTime();
		DateTime(time_t);
		DateTime(std::chrono::system_clock::time_point timePoint);
		
		static DateTime fromGmTm(struct tm);
		static DateTime fromLocalTm(struct tm);
		static Optional<DateTime> fromGmtString(const char* dateString, const char* format);
		static Optional<DateTime> fromGmtString(const std::string& dateString, const std::string& format);
		static Optional<DateTime> fromLocalString(const char* dateString, const char* format);
		static Optional<DateTime> fromLocalString(const std::string& dateString, const std::string& format);
		
		struct tm toGmTm() const;
		struct tm toLocalTm() const;
		time_t toTimeType() const;
		const std::chrono::system_clock::time_point& getTimePoint() const;
		
		String toString() const;
		String toGmtString(const char format[] = "%Y-%m-%d %H:%M:%S") const;
		String toLocalString(const char format[] = "%Y-%m-%d %H:%M:%S") const;
		String toISO8601String(int gmtOffset = 0) const;
		
	private:
		std::chrono::system_clock::time_point timePoint;
	};
}
