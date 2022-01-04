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
	/// Parses a string into a date, and formats a date into a string
	class DateFormatter {
	public:
		/// The format of the string to be either read from or outputted
		String format;
		/// When parsing, this represents the time zone of the date string (this is overridden if the string provides a time zone)
		/// When formatting, this represents the time zone the output string should be displayed in
		TimeZone timeZone = TimeZone::currentAlways();
		/// The locale of the date string
		std::locale locale;
		
		/// Formats a date string for the given date
		String stringFromDate(const Date& date) const;
		/// Parses a date from the given string. Returns `std::nullopt` if a valid date could not be parsed
		Optional<Date> dateFromString(String dateString) const;
	};

	/// Parses an ISO8601 duration string into a time interval
	TimeInterval parseISO8601Duration(const String& durationString);
}
