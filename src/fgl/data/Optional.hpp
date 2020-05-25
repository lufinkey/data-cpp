//
//  Optional.hpp
//  DataCpp
//
//  Created by Luis Finke on 11/17/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#include <optional>
#include <functional>
#include <variant>
#include <tuple>
#include <fgl/data/Common.hpp>
#include <fgl/util/PlatformChecks.hpp>

namespace fgl {
	class Any;

	template<typename T>
	class Optional: public std::optional<T> {
	public:
		using std::optional<T>::optional;
		using std::optional<T>::operator->;
		using std::optional<T>::operator*;
		using std::optional<T>::operator=;
		using std::optional<T>::operator bool;
		using std::optional<T>::has_value;
		using std::optional<T>::value;
		using std::optional<T>::value_or;
		using std::optional<T>::swap;
		using std::optional<T>::reset;
		using std::optional<T>::emplace;
		template<typename U=T>
		Optional(U&& value);
		
		template<typename U=T>
		Optional<T>& operator=(U&& value);
		
		inline operator std::optional<T>&();
		inline operator const std::optional<T>&() const;
		inline operator std::optional<T>&&() &&;
		
		inline bool hasValue() const;
		template<typename U>
		constexpr T valueOr(U&& defaultValue) const&;
		template<typename U>
		constexpr T valueOr(U&& defaultValue) &&;
		
		inline Any toAny() const;
	};

	template<typename T>
	inline bool operator==(const Optional<T>& left, const Optional<T>& right);
	template<typename T>
	inline bool operator!=(const Optional<T>& left, const Optional<T>& right);
	template<typename T>
	inline bool operator<(const Optional<T>& left, const Optional<T>& right);
	template<typename T>
	inline bool operator<=(const Optional<T>& left, const Optional<T>& right);
	template<typename T>
	inline bool operator>(const Optional<T>& left, const Optional<T>& right);
	template<typename T>
	inline bool operator>=(const Optional<T>& left, const Optional<T>& right);

	template<typename T>
	inline bool operator==(const Optional<T>& left, std::nullopt_t right);
	template<typename T>
	inline bool operator==(std::nullopt_t left, const Optional<T>& right);
	template<typename T>
	inline bool operator!=(const Optional<T>& left, std::nullopt_t right);
	template<typename T>
	inline bool operator!=(std::nullopt_t left, const Optional<T>& right);
	template<typename T>
	inline bool operator<(const Optional<T>& left, std::nullopt_t right);
	template<typename T>
	inline bool operator<(std::nullopt_t left, const Optional<T>& right);
	template<typename T>
	inline bool operator<=(const Optional<T>& left, std::nullopt_t right);
	template<typename T>
	inline bool operator<=(std::nullopt_t left, const Optional<T>& right);
	template<typename T>
	inline bool operator>(const Optional<T>& left, std::nullopt_t right);
	template<typename T>
	inline bool operator>(std::nullopt_t left, const Optional<T>& right);
	template<typename T>
	inline bool operator>=(const Optional<T>& left, std::nullopt_t right);
	template<typename T>
	inline bool operator>=(std::nullopt_t left, const Optional<T>& right);

	template<typename T, typename U>
	inline bool operator==(const Optional<T>& left, const U& right);
	template<typename T, typename U>
	inline bool operator==(const U& left, const Optional<T>& right);
	template<typename T, typename U>
	inline bool operator!=(const Optional<T>& left, const U& right);
	template<typename T, typename U>
	inline bool operator!=(const U& left, const Optional<T>& right);
	template<typename T, typename U>
	inline bool operator<(const Optional<T>& left, const U& right);
	template<typename T, typename U>
	inline bool operator<(const U& left, const Optional<T>& right);
	template<typename T, typename U>
	inline bool operator<=(const Optional<T>& left, const U& right);
	template<typename T, typename U>
	inline bool operator<=(const U& left, const Optional<T>& right);
	template<typename T, typename U>
	inline bool operator>(const Optional<T>& left, const U& right);
	template<typename T, typename U>
	inline bool operator>(const U& left, const Optional<T>& right);
	template<typename T, typename U>
	inline bool operator>=(const Optional<T>& left, const U& right);
	template<typename T, typename U>
	inline bool operator>=(const U& left, const Optional<T>& right);

	template<typename T>
	using OptionalRef = Optional<std::reference_wrapper<T>>;

	template<typename T>
	struct is_optional: std::false_type {};
	template<typename T>
	struct is_optional<Optional<T>>: std::true_type {};
	template<typename T>
	struct is_optional<std::optional<T>>: std::true_type {};

	template<typename T>
	struct optionalize_t {
		using type = Optional<T>;
	};
	template<typename T>
	struct optionalize_t<Optional<T>> {
		using type = T;
	};

	template<typename T>
	using Optionalized = typename optionalize_t<T>::type;

	template<typename T>
	inline Optionalized<T> maybe(T value);
	template<typename Func>
	auto maybeTry(Func func) -> Optionalized<decltype(func())>;



#pragma mark Optional implementation

	template<typename T>
	template<typename U>
	Optional<T>::Optional(U&& value): std::optional<T>(value) {
		//
	}
	
	template<typename T>
	template<typename U>
	Optional<T>& Optional<T>::operator=(U&& value) {
		std::optional<T>::operator=(value);
		return *this;
	}

	template<typename T>
	Optional<T>::operator std::optional<T>&() {
		return *this;
	}

	template<typename T>
	Optional<T>::operator const std::optional<T>&() const {
		return *this;
	}

	template<typename T>
	Optional<T>::operator std::optional<T>&&() && {
		return *this;
	}
	
	template<typename T>
	bool Optional<T>::hasValue() const {
		return has_value();
	}

	template<typename T>
	template<typename U>
	constexpr T Optional<T>::valueOr(U&& defaultValue) const& {
		return std::optional<T>::template value_or<U>(defaultValue);
	}

	template<typename T>
	template<typename U>
	constexpr T Optional<T>::valueOr(U&& defaultValue) && {
		return std::optional<T>::template value_or<U>(defaultValue);
	}

	template<typename T>
	Optionalized<T> maybe(T value) {
		if constexpr(std::is_same<T,Optionalized<T>>::value) {
			return value;
		} else {
			return Optionalized<T>(value);
		}
	}

	template<typename Func>
	auto maybeTry(Func func) -> Optionalized<decltype(func())> {
		try {
			return func();
		} catch(...) {
			return std::nullopt;
		}
	}



	template<typename T>
	bool operator==(const Optional<T>& left, const Optional<T>& right) {
		return operator==((const std::optional<T>&)left, (const std::optional<T>&)right);
	}

	template<typename T>
	bool operator!=(const Optional<T>& left, const Optional<T>& right) {
		return operator!=((const std::optional<T>&)left, (const std::optional<T>&)right);
	}

	template<typename T>
	bool operator<(const Optional<T>& left, const Optional<T>& right) {
		return operator<((const std::optional<T>&)left, (const std::optional<T>&)right);
	}

	template<typename T>
	bool operator<=(const Optional<T>& left, const Optional<T>& right) {
		return operator<=((const std::optional<T>&)left, (const std::optional<T>&)right);
	}

	template<typename T>
	bool operator>(const Optional<T>& left, const Optional<T>& right) {
		return operator>((const std::optional<T>&)left, (const std::optional<T>&)right);
	}

	template<typename T>
	bool operator>=(const Optional<T>& left, const Optional<T>& right) {
		return operator>=((const std::optional<T>&)left, (const std::optional<T>&)right);
	}

	template<typename T>
	bool operator==(const Optional<T>& left, std::nullopt_t right) {
		return operator==((const std::optional<T>&)left, right);
	}

	template<typename T>
	bool operator==(std::nullopt_t left, const Optional<T>& right) {
		return operator==(left, (const std::optional<T>&)right);
	}

	template<typename T>
	bool operator!=(const Optional<T>& left, std::nullopt_t right) {
		return operator!=((const std::optional<T>&)left, right);
	}

	template<typename T>
	bool operator!=(std::nullopt_t left, const Optional<T>& right) {
		return operator!=(left, (const std::optional<T>&)right);
	}

	template<typename T>
	bool operator<(const Optional<T>& left, std::nullopt_t right) {
		return operator<((const std::optional<T>&)left, right);
	}

	template<typename T>
	bool operator<(std::nullopt_t left, const Optional<T>& right) {
		return operator<(left, (const std::optional<T>&)right);
	}

	template<typename T>
	bool operator<=(const Optional<T>& left, std::nullopt_t right) {
		return operator<=((const std::optional<T>&)left, right);
	}

	template<typename T>
	bool operator<=(std::nullopt_t left, const Optional<T>& right) {
		return operator<=(left, (const std::optional<T>&)right);
	}

	template<typename T>
	bool operator>(const Optional<T>& left, std::nullopt_t right) {
		return operator>((const std::optional<T>&)left, right);
	}

	template<typename T>
	bool operator>(std::nullopt_t left, const Optional<T>& right) {
		return operator>(left, (const std::optional<T>&)right);
	}

	template<typename T>
	bool operator>=(const Optional<T>& left, std::nullopt_t right) {
		return operator>=((const std::optional<T>&)left, right);
	}

	template<typename T>
	bool operator>=(std::nullopt_t left, const Optional<T>& right) {
		return operator>=(left, (const std::optional<T>&)right);
	}

	template<typename T, typename U>
	bool operator==(const Optional<T>& left, const U& right) {
		return operator==((const std::optional<T>&)left, right);
	}

	template<typename T, typename U>
	bool operator==(const U& left, const Optional<T>& right) {
		return operator==(left, (const std::optional<T>&)right);
	}

	template<typename T, typename U>
	bool operator!=(const Optional<T>& left, const U& right) {
		return operator!=((const std::optional<T>&)left, right);
	}

	template<typename T, typename U>
	bool operator!=(const U& left, const Optional<T>& right) {
		return operator!=(left, (const std::optional<T>&)right);
	}

	template<typename T, typename U>
	bool operator<(const Optional<T>& left, const U& right) {
		return operator<((const std::optional<T>&)left, right);
	}

	template<typename T, typename U>
	bool operator<(const U& left, const Optional<T>& right) {
		return operator<(left, (const std::optional<T>&)right);
	}

	template<typename T, typename U>
	bool operator<=(const Optional<T>& left, const U& right) {
		return operator<=((const std::optional<T>&)left, right);
	}

	template<typename T, typename U>
	bool operator<=(const U& left, const Optional<T>& right) {
		return operator<=(left, (const std::optional<T>&)right);
	}

	template<typename T, typename U>
	bool operator>(const Optional<T>& left, const U& right) {
		return operator>((const std::optional<T>&)left, right);
	}

	template<typename T, typename U>
	bool operator>(const U& left, const Optional<T>& right) {
		return operator>(left, (const std::optional<T>&)right);
	}

	template<typename T, typename U>
	bool operator>=(const Optional<T>& left, const U& right) {
		return operator>=((const std::optional<T>&)left, right);
	}

	template<typename T, typename U>
	bool operator>=(const U& left, const Optional<T>& right) {
		return operator>=(left, (const std::optional<T>&)right);
	}
}
