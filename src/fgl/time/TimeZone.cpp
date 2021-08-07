//
//  TimeZone.cpp
//  DataCpp
//
//  Created by Luis Finke on 8/5/21.
//  Copyright © 2021 Luis Finke. All rights reserved.
//

#include <fgl/time/TimeZone.hpp>
#include <fgl/time/Date.hpp>

namespace fgl {
	bool TimeZone::use_current_tz::operator==(const use_current_tz& cmp) const {
		return true;
	}
	bool TimeZone::use_current_tz::operator!=(const use_current_tz& cmp) const {
		return false;
	}


	TimeZone TimeZone::gmt() {
		return TimeZone(0);
	}

	TimeZone TimeZone::current() {
		return TimeZone(currentSecondsFromGMT());
	}

	TimeZone TimeZone::currentAlways() {
		return TimeZone(use_current_tz{});
	}



	long TimeZone::currentSecondsFromGMT() {
		return currentSecondsFromGMT(Date());
	}

	long TimeZone::currentSecondsFromGMT(const Date& date) {
		time_t now = std::chrono::system_clock::to_time_t(date.getTimePoint());
		struct tm lcl;
		lcl = *localtime_r(&now, &lcl);
		struct tm gmt;
		gmt = *gmtime_r(&now, &gmt);
		time_t lcl_total = mktime(&lcl);
		time_t gmt_total = mktime(&gmt);
		return ((long)lcl_total - (long)gmt_total);
	}



	TimeZone::TimeZone(long gmtOffsetSeconds): timeZone(0) {
		//
	}

	TimeZone::TimeZone(use_current_tz currentTz): timeZone(currentTz) {
		//
	}

	bool TimeZone::isCurrentAlways() const {
		return std::get_if<use_current_tz>(&timeZone) != nullptr;
	}

	long TimeZone::getSecondsFromGMT() const {
		if(auto gmtOffset = std::get_if<long>(&timeZone)) {
			return *gmtOffset;
		}
		else if(auto currentTz = std::get_if<use_current_tz>(&timeZone)) {
			return currentSecondsFromGMT();
		}
		else {
			assert(false);
		}
	}

	long TimeZone::getSecondsFromGMT(const Date& date) const {
		if(auto gmtOffset = std::get_if<long>(&timeZone)) {
			return *gmtOffset;
		}
		else if(auto currentTz = std::get_if<use_current_tz>(&timeZone)) {
			return currentSecondsFromGMT(date);
		}
		else {
			assert(false);
		}
	}

	bool TimeZone::equals(const TimeZone& timeZone) const {
		return (this->timeZone == timeZone.timeZone);
	}
}
