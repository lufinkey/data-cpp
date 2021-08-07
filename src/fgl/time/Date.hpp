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
	using TimePoint = std::chrono::system_clock::time_point;
	using TimeInterval = std::chrono::system_clock::duration;

	class Date {
		friend class DateFormatter;
		friend bool operator==(const Date&, const Date&);
		friend bool operator!=(const Date&, const Date&);
		friend bool operator<(const Date&, const Date&);
		friend bool operator<=(const Date&, const Date&);
		friend bool operator>(const Date&, const Date&);
		friend bool operator>=(const Date&, const Date&);
		friend Date operator+(const Date&, const TimeInterval&);
		friend Date operator-(const Date&, const TimeInterval&);
	public:
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
		String toISOString(TimeZone timeZone = TimeZone(0)) const;
		
		Date& operator+=(TimeInterval);
		Date& operator-=(TimeInterval);
		
	private:
		TimePoint timePoint;
	};



	#pragma mark Date implementation

	inline bool operator==(const Date& left, const Date& right) {
		return operator==(left.timePoint, right.timePoint);
	}

	inline bool operator!=(const Date& left, const Date& right) {
		return operator!=(left.timePoint, right.timePoint);
	}

	inline bool operator<(const Date& left, const Date& right) {
		return operator<(left.timePoint, right.timePoint);
	}

	inline bool operator<=(const Date& left, const Date& right) {
		return operator<=(left.timePoint, right.timePoint);
	}

	inline bool operator>(const Date& left, const Date& right) {
		return operator>(left.timePoint, right.timePoint);
	}

	inline bool operator>=(const Date& left, const Date& right) {
		return operator>=(left.timePoint, right.timePoint);
	}

	inline Date operator+(const Date& left, const TimeInterval& right) {
		return Date(left.timePoint + right);
	}

	inline Date operator-(const Date& left, const TimeInterval& right) {
		return Date(left.timePoint - right);
	}
}