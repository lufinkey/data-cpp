//
//  Optional.hpp
//  DataCpp
//
//  Created by Luis Finke on 11/17/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#include <fgl/data/Common.hpp>
#include <fgl/util/PlatformChecks.hpp>
#include <optional>
#include <functional>
#include <variant>
#include <tuple>

namespace fgl {
	class Any;

	template<typename T>
	class Optional;

	template<typename T>
	using OptionalRef = Optional<std::reference_wrapper<T>>;

	template<typename T>
	struct is_optional: std::false_type {};
	template<typename T>
	struct is_optional<Optional<T>>: std::true_type {};
	template<typename T>
	struct is_optional<std::optional<T>>: std::true_type {};

	template<typename T>
	inline constexpr bool is_optional_v = is_optional<T>::value;


	template<typename T>
	struct _optionalize {
		using type = Optional<T>;
	};
	template<typename T>
	struct _optionalize<Optional<T>> {
		using type = Optional<T>;
	};
	template<typename T>
	struct _optionalize<std::optional<T>> {
		using type = Optional<T>;
	};
	template<>
	struct _optionalize<void> {
		using type = Optional<std::nullptr_t>;
	};
	template<typename T>
	using Optionalized = typename _optionalize<T>::type;


	template<typename T>
	struct optional_or_void {
		using type = Optionalized<T>;
	};
	template<>
	struct optional_or_void<void> {
		using type = void;
	};
	template<typename T>
	using OptionalOrVoid = typename optional_or_void<T>::type;



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
		template<typename U,
			typename std::enable_if<std::is_convertible_v<T,U>,std::nullptr_t>::type = nullptr>
		inline explicit operator std::optional<U>() const;
		template<typename U,
			typename std::enable_if<std::is_convertible_v<T,U>,std::nullptr_t>::type = nullptr>
		inline explicit operator Optional<U>() const;
		
		inline bool hasValue() const;
		template<typename U>
		constexpr T valueOr(U&& defaultValue) const&;
		template<typename U>
		constexpr T valueOr(U&& defaultValue) &&;
		template<typename U>
		constexpr Optional<T> valueOrMaybe(U&& defaultValue) const&;
		template<typename U>
		constexpr Optional<T> valueOrMaybe(U&& defaultValue) &&;
		template<typename Else>
		constexpr T valueElse(Else&& fallback) const&;
		template<typename Else>
		constexpr T valueElse(Else&& fallback) &&;
		template<typename Else>
		constexpr Optional<T> valueElseMaybe(Else&& fallback) const&;
		template<typename Else>
		constexpr Optional<T> valueElseMaybe(Else&& fallback) &&;
		template<typename Error>
		constexpr T& valueOrThrow(Error&& error) &;
		template<typename Error>
		constexpr const T& valueOrThrow(Error&& error) const&;
		template<typename Error>
		constexpr T&& valueOrThrow(Error&& error) &&;
		template<typename Error>
		constexpr const T&& valueOrThrow(Error&& error) const&&;
		
		template<typename Transform>
		inline auto map(Transform transform);
		template<typename Transform>
		inline auto map(Transform transform) const;
		
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
	inline Optionalized<T> maybe(T value);
	template<typename Func>
	auto maybeTry(Func func) -> Optionalized<decltype(func())>;
	template<typename Func, typename DefaultValue>
	auto maybeTry(Func func, DefaultValue defaultValue) -> decltype(func());



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
	template<typename U,
		typename std::enable_if<std::is_convertible_v<T,U>,std::nullptr_t>::type>
	Optional<T>::operator std::optional<U>() const {
		return static_cast<std::optional<U>>((const std::optional<T>&)*this);
	}

	template<typename T>
	template<typename U,
		typename std::enable_if<std::is_convertible_v<T,U>,std::nullptr_t>::type>
	Optional<T>::operator Optional<U>() const {
		return Optional<U>(static_cast<std::optional<U>>((const std::optional<T>&)*this));
	}
	
	template<typename T>
	bool Optional<T>::hasValue() const {
		return has_value();
	}

	template<typename T>
	template<typename U>
	constexpr T Optional<T>::valueOr(U&& defaultValue) const& {
		return value_or(defaultValue);
	}

	template<typename T>
	template<typename U>
	constexpr T Optional<T>::valueOr(U&& defaultValue) && {
		return value_or(defaultValue);
	}

	template<typename T>
	template<typename U>
	constexpr Optional<T> Optional<T>::valueOrMaybe(U&& defaultValue) const& {
		if(has_value()) {
			return value();
		}
		return defaultValue;
	}

	template<typename T>
	template<typename U>
	constexpr Optional<T> Optional<T>::valueOrMaybe(U&& defaultValue) && {
		if(has_value()) {
			return value();
		}
		return defaultValue;
	}

	template<typename T>
	template<typename Else>
	constexpr T Optional<T>::valueElse(Else&& fallback) const& {
		if(has_value()) {
			return value();
		}
		return fallback();
	}

	template<typename T>
	template<typename Else>
	constexpr T Optional<T>::valueElse(Else&& fallback) && {
		if(has_value()) {
			return value();
		}
		return fallback();
	}

	template<typename T>
	template<typename Else>
	constexpr Optional<T> Optional<T>::valueElseMaybe(Else&& fallback) const& {
		if(has_value()) {
			return value();
		}
		return fallback();
	}

	template<typename T>
	template<typename Else>
	constexpr Optional<T> Optional<T>::valueElseMaybe(Else&& fallback) && {
		if(has_value()) {
			return value();
		}
		return fallback();
	}

	template<typename T>
	template<typename Error>
	constexpr T& Optional<T>::valueOrThrow(Error&& error) & {
		if(has_value()) {
			return std::forward<T>(value());
		}
		throw error;
	}

	template<typename T>
	template<typename Error>
	constexpr T&& Optional<T>::valueOrThrow(Error&& error) && {
		if(has_value()) {
			return std::forward<T>(value());
		}
		throw error;
	}

	template<typename T>
	template<typename Error>
	constexpr const T& Optional<T>::valueOrThrow(Error&& error) const& {
		if(has_value()) {
			return std::forward<T>(value());
		}
		throw error;
	}

	template<typename T>
	template<typename Error>
	constexpr const T&& Optional<T>::valueOrThrow(Error&& error) const&& {
		if(has_value()) {
			return std::forward<T>(value());
		}
		throw error;
	}



	template<typename T>
	template<typename Transform>
	inline auto Optional<T>::map(Transform transform) {
		using ReturnType = decltype(transform(value()));
		return has_value() ? Optionalized<ReturnType>(transform(value())) : std::nullopt;
	}
	
	template<typename T>
	template<typename Transform>
	inline auto Optional<T>::map(Transform transform) const {
		using ReturnType = decltype(transform(value()));
		return has_value() ? Optionalized<ReturnType>(transform(value())) : std::nullopt;
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
			if constexpr(std::is_same<void,decltype(func())>::value) {
				func();
				return nullptr;
			} else {
				return func();
			}
		} catch(...) {
			return std::nullopt;
		}
	}

	template<typename Func, typename DefaultValue>
	auto maybeTry(Func func, DefaultValue defaultValue) -> decltype(func()) {
		try {
			return func();
		} catch(...) {
			return defaultValue;
		}
	}



	template<typename T>
	bool operator==(const Optional<T>& left, const Optional<T>& right) {
		const std::optional<T>& left1 = left;
		const std::optional<T>& right1 = right;
		if(static_cast<bool>(left1) != static_cast<bool>(right1)) {
			return false;
		}
		if(!static_cast<bool>(left1)) {
			return true;
		}
		return left1.value() == right1.value();
	}

	template<typename T>
	bool operator!=(const Optional<T>& left, const Optional<T>& right) {
		const std::optional<T>& left1 = left;
		const std::optional<T>& right1 = right;
		if(static_cast<bool>(left1) != static_cast<bool>(right1)) {
			return true;
		}
		if(!static_cast<bool>(left1)) {
			return false;
		}
		return left1.value() != right1.value();
	}

	template<typename T>
	bool operator<(const Optional<T>& left, const Optional<T>& right) {
		const std::optional<T>& left1 = left;
		const std::optional<T>& right1 = right;
		return left1 < right1;
	}

	template<typename T>
	bool operator<=(const Optional<T>& left, const Optional<T>& right) {
		const std::optional<T>& left1 = left;
		const std::optional<T>& right1 = right;
		return left1 <= right1;
	}

	template<typename T>
	bool operator>(const Optional<T>& left, const Optional<T>& right) {
		const std::optional<T>& left1 = left;
		const std::optional<T>& right1 = right;
		return left1 > right1;
	}

	template<typename T>
	bool operator>=(const Optional<T>& left, const Optional<T>& right) {
		const std::optional<T>& left1 = left;
		const std::optional<T>& right1 = right;
		return left1 >= right1;
	}

	template<typename T>
	bool operator==(const Optional<T>& left, std::nullopt_t right) {
		const std::optional<T>& left1 = left;
		return left1 == right;
	}

	template<typename T>
	bool operator==(std::nullopt_t left, const Optional<T>& right) {
		const std::optional<T>& right1 = right;
		return left == right1;
	}

	template<typename T>
	bool operator!=(const Optional<T>& left, std::nullopt_t right) {
		const std::optional<T>& left1 = left;
		return left1 != right;
	}

	template<typename T>
	bool operator!=(std::nullopt_t left, const Optional<T>& right) {
		const std::optional<T>& right1 = right;
		return left != right1;
	}

	template<typename T>
	bool operator<(const Optional<T>& left, std::nullopt_t right) {
		const std::optional<T>& left1 = left;
		return left1 < right;
	}

	template<typename T>
	bool operator<(std::nullopt_t left, const Optional<T>& right) {
		const std::optional<T>& right1 = right;
		return left < right1;
	}

	template<typename T>
	bool operator<=(const Optional<T>& left, std::nullopt_t right) {
		const std::optional<T>& left1 = left;
		return left1 <= right;
	}

	template<typename T>
	bool operator<=(std::nullopt_t left, const Optional<T>& right) {
		const std::optional<T>& right1 = right;
		return left <= right1;
	}

	template<typename T>
	bool operator>(const Optional<T>& left, std::nullopt_t right) {
		const std::optional<T>& left1 = left;
		return left1 > right;
	}

	template<typename T>
	bool operator>(std::nullopt_t left, const Optional<T>& right) {
		const std::optional<T>& right1 = right;
		return left > right1;
	}

	template<typename T>
	bool operator>=(const Optional<T>& left, std::nullopt_t right) {
		const std::optional<T>& left1 = left;
		return left1 >= right;
	}

	template<typename T>
	bool operator>=(std::nullopt_t left, const Optional<T>& right) {
		const std::optional<T>& right1 = right;
		return left >= right1;
	}

	template<typename T, typename U>
	bool operator==(const Optional<T>& left, const U& right) {
		const std::optional<T>& left1 = left;
		return left1 == right;
	}

	template<typename T, typename U>
	bool operator==(const U& left, const Optional<T>& right) {
		const std::optional<T>& right1 = right;
		return left == right1;
	}

	template<typename T, typename U>
	bool operator!=(const Optional<T>& left, const U& right) {
		const std::optional<T>& left1 = left;
		return left1 != right;
	}

	template<typename T, typename U>
	bool operator!=(const U& left, const Optional<T>& right) {
		const std::optional<T>& right1 = right;
		return left != right1;
	}

	template<typename T, typename U>
	bool operator<(const Optional<T>& left, const U& right) {
		const std::optional<T>& left1 = left;
		return left1 < right;
	}

	template<typename T, typename U>
	bool operator<(const U& left, const Optional<T>& right) {
		const std::optional<T>& right1 = right;
		return left < right1;
	}

	template<typename T, typename U>
	bool operator<=(const Optional<T>& left, const U& right) {
		const std::optional<T>& left1 = left;
		return left1 <= right;
	}

	template<typename T, typename U>
	bool operator<=(const U& left, const Optional<T>& right) {
		const std::optional<T>& right1 = right;
		return left <= right1;
	}

	template<typename T, typename U>
	bool operator>(const Optional<T>& left, const U& right) {
		const std::optional<T>& left1 = left;
		return left1 > right;
	}

	template<typename T, typename U>
	bool operator>(const U& left, const Optional<T>& right) {
		const std::optional<T>& right1 = right;
		return left > right1;
	}

	template<typename T, typename U>
	bool operator>=(const Optional<T>& left, const U& right) {
		const std::optional<T>& left1 = left;
		return left1 >= right;
	}

	template<typename T, typename U>
	bool operator>=(const U& left, const Optional<T>& right) {
		const std::optional<T>& right1 = right;
		return left >= right1;
	}
}
