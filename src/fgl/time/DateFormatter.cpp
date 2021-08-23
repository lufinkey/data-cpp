//
//  DateFormatter.cpp
//  DataCpp
//
//  Created by Luis Finke on 8/5/21.
//  Copyright © 2021 Luis Finke. All rights reserved.
//

#include <fgl/time/DateFormatter.hpp>
#include <regex>
#include <cmath>

namespace fgl {
	struct DateStringToken {
		StringView string;
		
		bool hasDecimal() const {
			size_t pos = string.find('.');
			if(pos == StringView::npos) {
				return false;
			}
			return true;
		}
		
		Optional<size_t> fixedDigits() const {
			String digits;
			for(size_t i=1; i<(string.length() - 1); i++) {
				char c = string[i];
				if(std::isdigit(c)) {
					digits += c;
				}
			}
			if(digits.empty()) {
				return std::nullopt;
			}
			return digits.toArithmeticValue<size_t>();
		}
		
		Optional<size_t> fixedDecimals() const {
			size_t pos = string.find('.');
			if(pos == StringView::npos) {
				return std::nullopt;
			}
			pos += 1;
			size_t endPos = pos;
			for(endPos=pos; endPos<string.length(); endPos++) {
				if(!std::isdigit(string[endPos])) {
					break;
				}
			}
			auto numsSubstr = string.substr(pos, endPos - pos);
			if(numsSubstr.empty()) {
				return std::nullopt;
			}
			return String(numsSubstr.begin(), numsSubstr.end()).toArithmeticValue<size_t>();
		}
		
		char endChar() const {
			return string[string.length() - 1];
		}
	};



	struct DateStringTZ {
		char prefix = 0;
		long hours = 0;
		long minutes = 0;
		
		bool empty() const {
			return (prefix == 0);
		}
		
		std::chrono::seconds offsetSeconds() const {
			if(prefix == 'Z' || prefix == 0) {
				return std::chrono::seconds(0);
			}
			long mult = 1;
			if(prefix == '-') {
				mult = -1;
			}
			return std::chrono::seconds(mult * (hours * 3600) + (minutes * 60));
		}
	};


	Optional<DateStringToken> DateFormatter_findTokenInFormat(const String& format, char token) {
		size_t index = 0;
		bool inPercent = false;
		const char* lastTokenStart = nullptr;
		while(char c = format[index]) {
			if(inPercent) {
				if(std::isdigit(c) || c == 'O' || c == 'E' || c == '.') {
					index++;
					continue;
				}
				if(c == token) {
					size_t count = (format.data() + index + 1) - lastTokenStart;
					return DateStringToken{
						.string = StringView(lastTokenStart, count)
					};
				}
				lastTokenStart = nullptr;
				inPercent = false;
			} else {
				if(c == '%') {
					lastTokenStart = format.data() + index;
					inPercent = true;
				}
			}
			index++;
		}
		return std::nullopt;
	}

	Optional<struct tm> DateFormatter_tmFromFormat(String dateString, String formatString, String* fracSecs, DateStringTZ* tzInfo) {
		struct tm timeTm;
		std::memset(&timeTm, 0, sizeof(timeTm));
		timeTm.tm_isdst = -1;
		// parse fractional seconds if specified
		if(auto tokenOpt = DateFormatter_findTokenInFormat(formatString, 'f')) {
			// parse fractional seconds %f token
			auto& token = tokenOpt.value();
			bool hasDecimal = token.hasDecimal();
			Optional<size_t> fixedLength;
			if(hasDecimal) {
				fixedLength = token.fixedDecimals();
			} else {
				fixedLength = token.fixedDigits();
			}
			// parse up before the f token, then manually parse fractional seconds
			auto adjustedFormat = formatString.substring(0, (token.string.begin() - formatString.data()));
			if(const char* endParse = strptime(dateString.c_str(), adjustedFormat.c_str(), &timeTm)) {
				String fracSecStr;
				fracSecStr.reserve(9);
				const char* parseIt = endParse;
				if(hasDecimal) {
					if(parseIt[0] == '.') {
						parseIt++;
					} else {
						return std::nullopt;
					}
				}
				size_t maxLength = fixedLength.valueOr(9);
				while(parseIt[0] && fracSecStr.length() < maxLength && std::isdigit(parseIt[0])) {
					fracSecStr += parseIt[0];
					parseIt++;
				}
				if(fixedLength && fracSecStr.length() != fixedLength.value()) {
					return std::nullopt;
				}
				if(fracSecs != nullptr) {
					*fracSecs = fracSecStr;
				}
				formatString = formatString.substring(0, (token.string.begin() - formatString.data())) + token.string.end();
				dateString = dateString.substring(0, (endParse - dateString.data())) + parseIt;
			}
		}
		// otherwise, parse fractional seconds after seconds or T (for compatability)
		else if(auto tokenOpt = DateFormatter_findTokenInFormat(formatString, 'S')
				.valueOrMaybe(DateFormatter_findTokenInFormat(formatString, 'T'))) {
			// look for fractional seconds after token
			auto& token = tokenOpt.value();
			// parse up to S character, then parse fractional seconds
			auto adjustedFormat = formatString.substring(0, (token.string.end() - formatString.data()));
			if(const char* endParse = strptime(dateString.c_str(), adjustedFormat.c_str(), &timeTm)) {
				String fracSecStr;
				size_t maxDigits = 6;
				fracSecStr.reserve(maxDigits);
				// TODO set number of digits by possible specified size of %S input token (ex: %6S)
				size_t fracSecOffset = 0;
				if(endParse[fracSecOffset] == '.') {
					// parse no more than 6 digits
					fracSecOffset++;
					while(char c = endParse[fracSecOffset]) {
						if(!std::isdigit(c)) {
							break;
						}
						fracSecStr += c;
						fracSecOffset++;
						if(fracSecOffset > maxDigits) {
							break;
						}
					}
				}
				// remove fractional seconds from date string
				if(!fracSecStr.empty()) {
					dateString = dateString.substring(0, endParse - dateString.data()) + (endParse + fracSecOffset);
				}
				if(fracSecs != nullptr) {
					*fracSecs = fracSecStr;
				}
			}
		}
		// parse time zone if specified
		if(auto tokenOpt = DateFormatter_findTokenInFormat(formatString, 'z')) {
			// parse time zone %z token
			auto& token = tokenOpt.value();
			// parse up before the z token, then manually parse the time zone
			auto adjustedFormat = formatString.substring(0, (token.string.begin() - formatString.data()));
			if(const char* endParse = strptime(dateString.c_str(), adjustedFormat.c_str(), &timeTm)) {
				const char* endTZ = endParse;
				if(endParse[0] == 'Z') {
					endTZ += 1;
					if(tzInfo != nullptr) {
						*tzInfo = DateStringTZ{
							.prefix = 'Z',
							.hours = 0,
							.minutes = 0
						};
					}
				}
				else if(endParse[0] == '-' || endParse[0] == '+') {
					char prefix = endParse[0];
					endTZ += 1;
					String digits;
					digits.reserve(4);
					for(size_t i=0; i<4; i++) {
						char c = endTZ[0];
						if(std::isdigit(c)) {
							digits += c;
						} else {
							break;
						}
						endTZ++;
					}
					if(digits.length() < 4) {
						return std::nullopt;
					}
					if(tzInfo != nullptr) {
						*tzInfo = DateStringTZ{
							.prefix = prefix,
							.hours = digits.substring(0, 2).toArithmeticValue<long>(),
							.minutes = digits.substring(2, 2).toArithmeticValue<long>()
						};
					}
				}
				else {
					return std::nullopt;
				}
				// remove time zone from date string
				dateString = dateString.substring(0, (endParse - dateString.data())) + endTZ;
				formatString = formatString.substring(0, (token.string.begin() - formatString.data())) + token.string.end();
			}
		}
		// parse raw string
		if(strptime(dateString.c_str(), formatString.c_str(), &timeTm) == nullptr) {
			return std::nullopt;
		}
		return timeTm;
	}

	String DateFormatter_tmToString(struct tm tmTime, String formatString, std::chrono::system_clock::duration fracSecs, long gmtOffsetSecs, const String& tzName) {
		// replace %f token with fractional seconds
		if(auto tokenOpt = DateFormatter_findTokenInFormat(formatString, 'f')) {
			auto& token = tokenOpt.value();
			size_t fullDigitsCount = std::to_string(std::chrono::system_clock::duration::period::den - 1).length();
			auto fracSecsStr = std::to_string(fracSecs.count());
			if(fracSecsStr.length() < fullDigitsCount) {
				fracSecsStr = std::string(fullDigitsCount - fracSecsStr.length(), '0') + fracSecsStr;
			}
			auto digitsCount = token.fixedDigits();
			auto decimalCount = token.fixedDecimals();
			bool hasDecimal = false;
			if(digitsCount) {
				if(fracSecsStr.length() < digitsCount.value()) {
					fracSecsStr += std::string(digitsCount.value() - fracSecsStr.length(), '0');
				}
				else if(fracSecsStr.length() > digitsCount.value()) {
					fracSecsStr = fracSecsStr.substr(0, digitsCount.value());
				}
			}
			else if(decimalCount) {
				hasDecimal = true;
				if(fracSecsStr.length() < decimalCount.value()) {
					fracSecsStr += std::string(decimalCount.value() - fracSecsStr.length(), '0');
				}
				else if(fracSecsStr.length() > decimalCount.value()) {
					fracSecsStr = fracSecsStr.substr(0, decimalCount.value());
				}
			}
			else {
				hasDecimal = token.hasDecimal();
				size_t maxDecs = 3;
				if(fracSecsStr.length() < maxDecs) {
					fracSecsStr += std::string(maxDecs - fracSecsStr.length(), '0');
				}
				else if(fracSecsStr.length() > maxDecs) {
					fracSecsStr = fracSecsStr.substr(0, maxDecs);
				}
			}
			formatString = formatString.substring(0, (token.string.begin() - formatString.data())) + (hasDecimal ? "." : "") + fracSecsStr + token.string.end();
		}
		// replace %z token with time zone
		if(auto tokenOpt = DateFormatter_findTokenInFormat(formatString, 'z')) {
			auto& token = tokenOpt.value();
			auto tzString = TimeZone::gmtOffsetString(gmtOffsetSecs);
			formatString = formatString.substring(0, (token.string.begin() - formatString.data())) + tzString + token.string.end();
		}
		// replace %Z token with time zone name
		if(auto tokenOpt = DateFormatter_findTokenInFormat(formatString, 'Z')) {
			auto& token = tokenOpt.value();
			formatString = formatString.substring(0, (token.string.begin() - formatString.data())) + tzName + token.string.end();
		}
		// format string
		char buffer[2048];
		strftime(buffer, 2048, formatString.c_str(), &tmTime);
		return buffer;
	}



	String DateFormatter::stringFromDate(const Date& date) const {
		long secondsFromGMT = timeZone.getSecondsFromGMT(date);
		auto tzName = timeZone.identifier();
		// get fractional seconds
		auto secDiff = date.timePoint - std::chrono::system_clock::from_time_t(date.toCTime());
		// get tm time
		struct tm timeTm;
		std::memset(&timeTm, 0, sizeof(timeTm));
		timeTm.tm_isdst = -1;
		if(timeZone.isCurrentAlways()) {
			timeTm = date.toLocalTm();
		} else if(secondsFromGMT == 0) {
			timeTm = date.toGmTm();
		} else {
			auto adjustedDate = date;
			adjustedDate.timePoint += std::chrono::seconds(secondsFromGMT);
			timeTm = adjustedDate.toGmTm();
		}
		return DateFormatter_tmToString(timeTm, format, secDiff, secondsFromGMT, tzName);
	}

	Optional<Date> DateFormatter::dateFromString(String dateString) const {
		String fracSecs;
		DateStringTZ tzInfo;
		auto timeTm = DateFormatter_tmFromFormat(dateString, format, &fracSecs, &tzInfo);
		if(!timeTm) {
			return std::nullopt;
		}
		Date date = ([&]() {
			// parse based on time zone
			if(tzInfo.empty()) {
				// parse based on timeZone property
				if(timeZone.isCurrentAlways()) {
					return Date::fromLocalTm(timeTm.value());
				} else {
					return Date::fromGmTm(timeTm.value());
				}
			} else {
				// parse based on string time zone
				auto date = Date::fromGmTm(timeTm.value());
				date.timePoint -= tzInfo.offsetSeconds();
				return date;
			}
		})();
		// attach fractional seconds
		if(!fracSecs.empty()) {
			while(fracSecs.length() < 9 && (fracSecs.length() % 3) != 0) {
				fracSecs += '0';
			}
			if(fracSecs.length() > 9) {
				fracSecs = fracSecs.substring(0, 9);
			}
			if(fracSecs.length() == 3) {
					date.timePoint += std::chrono::milliseconds(fracSecs.toArithmeticValue<long>());
			}
			else if(fracSecs.length() == 6) {
				date.timePoint += std::chrono::microseconds(fracSecs.toArithmeticValue<long>());
			}
			else { // 9
				date.timePoint += std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::nanoseconds(fracSecs.toArithmeticValue<long>()));
			}
		}
		return date;
	}



	TimeInterval parseISO8601Duration(const String& durationString) {
		// code from https://stackoverflow.com/a/34532410/1846536
		std::smatch match;
		std::regex regex;
		if(std::regex_match(durationString, std::regex("^((?!T).)*$"))) {
			regex = std::regex("^P([[:d:]]+Y)?([[:d:]]+M)?([[:d:]]+D)?$");
		} else {
			regex = std::regex("^P([[:d:]]+Y)?([[:d:]]+M)?([[:d:]]+D)?T([[:d:]]+H)?([[:d:]]+M)?([[:d:]]+S|[[:d:]]+\\.[[:d:]]+S)?$");
		}
		std::regex_search(durationString, match, regex);
		if (match.empty()) {
			throw std::invalid_argument("\""+durationString+"\" is not a valid ISO8601 duration");
		}
		std::vector<double> vec = {0,0,0,0,0,0}; // years, months, days, hours, minutes, seconds
		for (size_t i = 1; i < match.size(); ++i) {
			if (match[i].matched) {
				std::string str = match[i];
				str.pop_back(); // remove last character.
				vec[i-1] = std::stod(str);
			}
		}
		double duration = (31556926.0 * vec[0]) +  // years
		                  (2629743.83 * vec[1]) +  // months
		                  (86400.0 * vec[2]) +  // days
		                  (3600.0 * vec[3]) +  // hours
		                  (60.0 * vec[4]) +  // minutes
		                  (1.0 * vec[5]); // seconds
		return std::chrono::seconds((long long)duration);
	}
}
