//
//  TimeZone.cpp
//  DataCpp
//
//  Created by Luis Finke on 8/5/21.
//  Copyright © 2021 Luis Finke. All rights reserved.
//

#include <fgl/time/TimeZone.hpp>
#include <fgl/time/Date.hpp>
#include <cmath>

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
		return currentSecondsFromGMT(Date::now());
	}

	long TimeZone::currentSecondsFromGMT(const Date& currentDate) {
		struct tm lcl;
		memset(&lcl, 0, sizeof(lcl));
		lcl.tm_isdst = -1;
		
		struct tm gmt;
		memset(&gmt, 0, sizeof(gmt));
		gmt.tm_isdst = -1;
		
		time_t now = std::chrono::system_clock::to_time_t(currentDate.getTimePoint());
		lcl = *localtime_r(&now, &lcl);
		gmt = *gmtime_r(&now, &gmt);
		time_t lcl_total = mktime(&lcl);
		time_t gmt_total = mktime(&gmt);
		return ((long)lcl_total - (long)gmt_total);
	}

	String TimeZone::gmtOffsetString(long gmtOffsetSeconds) {
		long hours = gmtOffsetSeconds / 3600;
		long mins = (long)std::round(((double)gmtOffsetSeconds / 60.0) - ((double)hours * 60));
		if(hours == 0 && mins == 0) {
			return "Z";
		}
		String str;
		if(gmtOffsetSeconds < 0) {
			str += '-';
		} else {
			str += '+';
		}
		auto hoursStr = std::to_string(hours);
		while(hoursStr.length() < 2) {
			hoursStr = "0" + hoursStr;
		}
		auto minsStr = std::to_string(mins);
		while(minsStr.length() < 2) {
			minsStr = "0" + minsStr;
		}
		str += hoursStr;
		str += minsStr;
		return str;
	}



	TimeZone::TimeZone(long gmtOffsetSeconds): timeZone(0) {
		//
	}

	TimeZone::TimeZone(use_current_tz currentTz): timeZone(currentTz) {
		//
	}

	String TimeZone::identifier() const {
		if(auto* gmtOffsetPtr = std::get_if<long>(&timeZone)) {
			long gmtOffset = *gmtOffsetPtr;
			auto offsetString = gmtOffsetString(gmtOffset);
			if(offsetString == "Z") {
				return "GMT";
			}
			return "GMT"+offsetString;
		}
		else if(auto* currentTz = std::get_if<use_current_tz>(&timeZone)) {
			auto localTm = Date::now().toLocalTm();
			std::stringstream ss;
			ss << std::put_time(&localTm, "%Z");
			return ss.str();
		}
		else {
			assert(false);
		}
	}

	bool TimeZone::isCurrentAlways() const {
		return std::get_if<use_current_tz>(&timeZone) != nullptr;
	}

	long TimeZone::getSecondsFromGMT() const {
		if(auto* gmtOffsetPtr = std::get_if<long>(&timeZone)) {
			return *gmtOffsetPtr;
		}
		else if(auto* currentTz = std::get_if<use_current_tz>(&timeZone)) {
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
