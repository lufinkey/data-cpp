//
//  Number.hpp
//  DataCpp
//
//  Created by Luis Finke on 9/6/21.
//  Copyright © 2021 Luis Finke. All rights reserved.
//

#pragma once

#include <fgl/data/Common.hpp>
#include <fgl/data/Stringify.hpp>
#include <cmath>
#include <limits>

namespace fgl {
	template<typename To, typename From>
	To integral_cast(From);

	template<typename To, typename From>
	To numeric_cast(From);

	template<typename FloatType>
	bool floatIsIntegral(FloatType);



	#pragma mark Number implementation

	template<typename To, typename From>
	To integral_cast(From value) {
		static_assert(std::is_integral_v<To>, "Destination type must be an integral");
		static_assert(std::is_integral_v<From> || std::is_floating_point_v<From>, "Source type must be integral or floating point");
		if constexpr(std::is_unsigned_v<To>) {
			if(value < 0) {
				throw std::out_of_range("Negative value cannot be represented as an unsigned integral");
			}
		}
		if constexpr(std::is_floating_point_v<From>) {
			// cast from a floating point value
			// get classification of floating point value
			auto fpType = std::fpclassify(value);
			if(fpType == FP_INFINITE) {
				throw std::out_of_range("Infinity cannot be represented as an integral");
			} else if(fpType == FP_NAN) {
				throw std::out_of_range("NaN cannot be represented as an integral");
			} else if(fpType == FP_ZERO) {
				return 0;
			}
			auto newValue = static_cast<To>(value);
			if(value < 0) {
				// ensure that rounding our value up gives the same result as the integral cast
				if(std::ceil(value) != static_cast<From>(newValue)) {
					throw std::out_of_range(
						"Failed to cast "+stringify_type<From>()+" value "+std::to_string(value)+" to "+stringify_type<To>()+" value. Got "+std::to_string(newValue));
				}
			} else {
				// ensure that rounding our value down gives the same result as the integral cast
				if(std::floor(value) != static_cast<From>(newValue)) {
					throw std::out_of_range(
						"Failed to cast "+stringify_type<From>()+" value "+std::to_string(value)+" to "+stringify_type<To>()+" value. Got "+std::to_string(newValue));
				}
			}
			return newValue;
		} else {
			// cast from integral value
			auto newValue = static_cast<To>(value);
			// ensure casting back gives the same result
			if(static_cast<From>(newValue) != value) {
				throw std::out_of_range(
					"Failed to cast "+stringify_type<From>()+" value "+std::to_string(value)+" to "+stringify_type<To>()+" value. Got "+std::to_string(newValue));
			}
			return newValue;
		}
	}


	template<typename To, typename From>
	To numeric_cast(From value) {
		static_assert(std::is_integral_v<To> || std::is_floating_point_v<To>, "Destination type must be integral or floating point");
		static_assert(std::is_integral_v<From> || std::is_floating_point_v<From>, "Source type must be integral or floating point");
		if constexpr(std::is_integral_v<To>) {
			return integral_cast<To>(value);
		} else { //if constexpr(std::is_floating_point_v<To>) {
			auto newValue = static_cast<To>(value);
			if constexpr(std::is_integral_v<From>) {
				// ensure casting back gives the same result
				if(static_cast<From>(newValue) != value) {
					throw std::out_of_range(
						"Failed to cast "+stringify_type<From>()+" value "+std::to_string(value)+" to "+stringify_type<To>()+" value. Got "+std::to_string(newValue));
				}
			} else { //if constexpr(std::is_floating_point_v<From>) {
				// ensure new floating point value is within range
				if(value < std::numeric_limits<To>::min() || value > std::numeric_limits<To>::max()) {
					throw std::out_of_range(
						"Failed to cast "+stringify_type<From>()+" value "+std::to_string(value)+" to "+stringify_type<To>()+" value. Got "+std::to_string(newValue));
				}
			}
			return newValue;
		}
	}

	template<typename FloatType>
	bool floatIsIntegral(FloatType value) {
		static_assert(std::is_integral_v<FloatType> || std::is_floating_point_v<FloatType>, "Type must be integral or floating point");
		if constexpr(std::is_integral_v<FloatType>) {
			return true;
		}
		// get classification of floating point value
		auto fpType = std::fpclassify(value);
		if(fpType == FP_INFINITE) {
			return false;
		} else if(fpType == FP_NAN) {
			return false;
		} else if(fpType == FP_ZERO) {
			return true;
		}
		if(value < 0) {
			// ensure that rounding our value up is the same as the value
			if(std::ceil(value) == value) {
				return true;
			}
		} else {
			// ensure that rounding our value down is the same as the value
			if(std::floor(value) == value) {
				return true;
			}
		}
		return false;
	}
}
