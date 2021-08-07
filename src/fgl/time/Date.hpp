//
//  Date.hpp
//  DataCpp
//
//  Created by Luis Finke on 11/10/20.
//  Copyright © 2020 Luis Finke. All rights reserved.
//

#pragma once

#include <fgl/data/Common.hpp>
#include <fgl/data/Optional.hpp>
#include <fgl/data/String.hpp>
#include <fgl/time/TimeZone.hpp>
#include <chrono>
#include <ctime>

namespace fgl {
	class Date {
		friend class DateFormatter;
	public:
		using TimePoint = std::chrono::system_clock::time_point;
		
		Date();
		Date(time_t);
		Date(TimePoint timePoint);
		
		static Date fromGmTm(struct tm);
		static Date fromLocalTm(struct tm);
		static Optional<Date> fromGmtString(String dateString, String format);
		static Optional<Date> fromLocalString(String dateString, String format);
		
		struct tm toGmTm() const;
		struct tm toLocalTm() const;
		time_t toTimeVal() const;
		const TimePoint& getTimePoint() const;
		
		String toString() const;
		String toGmtString(String format = "%Y-%m-%dT%H:%M:%S%.f%z") const;
		String toLocalString(String format = "%Y-%m-%dT%H:%M:%S%.f%z") const;
		String toISO8601String(TimeZone timeZone = TimeZone(0)) const;
		
	private:
		TimePoint timePoint;
	};
}
