//
//  TimeZone.hpp
//  DataCpp
//
//  Created by Luis Finke on 8/5/21.
//  Copyright © 2021 Luis Finke. All rights reserved.
//

#pragma once

#include <fgl/data/Common.hpp>
#include <fgl/data/String.hpp>
#include <fgl/data/ArrayList.hpp>
#include <chrono>
#include <variant>

namespace fgl {
	class Date;

	class TimeZone {
	public:
		static TimeZone gmt();
		static TimeZone current();
		static TimeZone currentAlways();
		
		static ArrayList<String> knownTimeZoneIdentifiers();
		static ArrayList<TimeZone> knownTimeZones();
		static String timeZoneDataVersion();
		
		static long currentSecondsFromGMT();
		static long currentSecondsFromGMT(const Date&);
		static String gmtOffsetString(long gmtOffsetSeconds);
		
		//TimeZone(const std::chrono::time_zone*);
		explicit TimeZone(long gmtOffsetSeconds);
		
		String identifier() const;
		bool isCurrentAlways() const;
		
		long getSecondsFromGMT() const;
		long getSecondsFromGMT(const Date&) const;
		
		bool equals(const TimeZone&) const;
		
	private:
		struct use_current_tz {
			bool operator==(const use_current_tz&) const;
			bool operator!=(const use_current_tz&) const;
		};
		
		TimeZone(use_current_tz);
		
		//std::variant<long,use_current_tz,const std::chrono::time_zone*> timeZone;
		std::variant<long,use_current_tz> timeZone;
	};

	bool operator==(const TimeZone& left, const TimeZone& right);
	bool operator!=(const TimeZone& left, const TimeZone& right);
}
