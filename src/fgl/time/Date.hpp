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
		friend TimeInterval operator-(const Date&, const Date&);
	public:
		inline Date(TimePoint timePoint);
		
		inline static Date now();
		inline static Date epoch();
		inline static Date fromCTime(time_t);
		static Date fromTimeSince1970(TimeInterval timeSince1970);
		static Date fromSecondsSince1970(double secsSince1970);
		static Date fromGmTm(struct tm);
		static Date fromLocalTm(struct tm);
		static Date fromGmtString(String dateString, String format);
		static Date fromLocalString(String dateString, String format);
		static Date fromISOString(String dateString, TimeZone timeZone = TimeZone(0));
		static Optional<Date> maybeFromISOString(String dateString, TimeZone timeZone = TimeZone(0));
		
		static Date parse(String dateString);
		static Optional<Date> maybeParse(String dateString);
		
		static bool epochIs1970();
		
		inline bool isEpoch() const;
		inline TimeInterval timeSinceEpoch() const;
		TimeInterval timeSince1970() const;
		double secondsSince1970() const;
		
		struct tm toGmTm() const;
		struct tm toLocalTm() const;
		inline time_t toCTime() const;
		inline const TimePoint& getTimePoint() const;
		
		String toString() const;
		String toGmtString(String format = "%Y-%m-%dT%H:%M:%S%.f%z") const;
		String toLocalString(String format = "%Y-%m-%dT%H:%M:%S%.f%z") const;
		String toISOString(TimeZone timeZone = TimeZone(0)) const;
		
		inline Date& operator+=(TimeInterval interval);
		inline Date& operator-=(TimeInterval interval);
		
	private:
		TimePoint timePoint;
	};



	#pragma mark Date implementation

	Date Date::now() {
		return Date(std::chrono::system_clock::now());
	}

	Date Date::epoch() {
		return Date(std::chrono::system_clock::time_point());
	}

	Date Date::fromCTime(time_t timeVal) {
		return Date(std::chrono::system_clock::from_time_t(timeVal));
	}


	Date::Date(std::chrono::system_clock::time_point timePoint)
	: timePoint(timePoint) {
		//
	}

	bool Date::isEpoch() const {
		return (epoch() == *this);
	}

	TimeInterval Date::timeSinceEpoch() const {
		return timePoint.time_since_epoch();
	}

	time_t Date::toCTime() const {
		return std::chrono::system_clock::to_time_t(timePoint);
	}

	const TimePoint& Date::getTimePoint() const {
		return timePoint;
	}


	Date& Date::operator+=(TimeInterval interval) {
		timePoint += interval;
		return *this;
	}
	Date& Date::operator-=(TimeInterval interval) {
		timePoint -= interval;
		return *this;
	}


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

	inline TimeInterval operator-(const Date& left, const Date& right) {
		return left.timePoint - right.timePoint;
	}
}
