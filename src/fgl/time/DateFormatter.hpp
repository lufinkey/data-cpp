//
//  DateFormatter.hpp
//  DataCpp
//
//  Created by Luis Finke on 8/5/21.
//  Copyright © 2021 Luis Finke. All rights reserved.
//

#pragma once

#include <fgl/data/Common.hpp>
#include <fgl/time/TimeZone.hpp>
#include <fgl/time/Date.hpp>
#include <fgl/data/String.hpp>

namespace fgl {
	class DateFormatter {
	public:
		String format;
		TimeZone timeZone = TimeZone::currentAlways();
		std::locale locale;
		
		String stringFromDate(const Date& date) const;
		Optional<Date> dateFromString(String dateString) const;
	};


	TimeInterval parseISO8601Duration(const String& durationString);
}
