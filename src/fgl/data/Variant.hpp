//
//  Variant.hpp
//  DataCpp
//
//  Created by Luis Finke on 9/1/21.
//  Copyright © 2021 Luis Finke. All rights reserved.
//

#pragma once

#include <fgl/data/Common.hpp>
#include <variant>
#include <typeinfo>

namespace fgl {
	template<typename... Types>
	class Variant;

	template<typename T>
	struct is_variant: std::false_type {};
	template<typename... T>
	struct is_variant<std::variant<T...>>: std::true_type {};
	template<typename... T>
	struct is_variant<Variant<T...>>: std::true_type {};

	String stringify_type(const std::type_info&);

	template<typename... Types>
	class Variant: public std::variant<Types...> {
	public:
		using BaseType = std::variant<Types...>;
		
		using BaseType::BaseType;
		
		Variant(BaseType&&);
		Variant(const BaseType&);
		
		operator BaseType&() &;
		operator BaseType&&() &&;
		operator const BaseType&() const&;
		
		template<typename Visitor>
		inline decltype(auto) visit(Visitor&& visitor);
		template<typename Visitor>
		inline decltype(auto) visit(Visitor&& visitor) const;
		
		template<typename T>
		inline bool is() const;
		
		template<typename T>
		inline T& get() &;
		template<typename T>
		inline T&& get() &&;
		template<typename T>
		inline const T& get() const&;
		
		template<typename T, typename U>
		inline T& getOr(U&&);
		template<typename T, typename U>
		inline const T& getOr(U&&) const;
		
		template<typename T>
		inline Optional<T> maybeGet() const;
		
		template<typename T>
		inline OptionalRef<T> maybeGetRef();
		template<typename T>
		inline OptionalRef<const T> maybeGetRef() const;
		
		const std::type_info& type() const;
		String typeName() const;
	};



	#pragma mark Variant implementation

	template<typename... Types>
	Variant<Types...>::Variant(BaseType&& v): BaseType(v) {
		//
	}

	template<typename... Types>
	Variant<Types...>::Variant(const BaseType& v): BaseType(v) {
		//
	}

	template<typename... Types>
	Variant<Types...>::operator BaseType&() & {
		return (BaseType&)*this;
	}

	template<typename... Types>
	Variant<Types...>::operator BaseType&&() && {
		return (BaseType&&)*this;
	}

	template<typename... Types>
	Variant<Types...>::operator const BaseType&() const & {
		return (const BaseType&)*this;
	}

	template<typename... Types>
	template<typename Visitor>
	decltype(auto) Variant<Types...>::visit(Visitor&& visitor) {
		return std::visit(visitor, *this);
	}

	template<typename... Types>
	template<typename Visitor>
	decltype(auto) Variant<Types...>::visit(Visitor&& visitor) const {
		return std::visit(visitor, *this);
	}

	template<typename... Types>
	template<typename T>
	bool Variant<Types...>::is() const {
		return std::holds_alternative<T>(*this);
	}

	template<typename... Types>
	template<typename T>
	T& Variant<Types...>::get() & {
		return std::get<T>(*this);
	}

	template<typename... Types>
	template<typename T>
	T&& Variant<Types...>::get() && {
		return (T&&)std::get<T>(*this);
	}

	template<typename... Types>
	template<typename T>
	const T& Variant<Types...>::get() const& {
		return std::get<T>(*this);
	}

	template<typename... Types>
	template<typename T, typename U>
	T& Variant<Types...>::getOr(U&& defaultVal) {
		if(std::holds_alternative<T>(*this)) {
			return std::get<T>(*this);
		}
		return defaultVal;
	}

	template<typename... Types>
	template<typename T, typename U>
	const T& Variant<Types...>::getOr(U&& defaultVal) const {
		if(std::holds_alternative<T>(*this)) {
			return std::get<T>(*this);
		}
		return defaultVal;
	}

	template<typename... Types>
	template<typename T>
	Optional<T> Variant<Types...>::maybeGet() const {
		if(std::holds_alternative<T>(*this)) {
			return std::get<T>(*this);
		}
		return std::nullopt;
	}

	template<typename... Types>
	template<typename T>
	OptionalRef<T> Variant<Types...>::maybeGetRef() {
		if(std::holds_alternative<T>(*this)) {
			return std::ref(std::get<T>(*this));
		}
		return std::nullopt;
	}

	template<typename... Types>
	template<typename T>
	OptionalRef<const T> Variant<Types...>::maybeGetRef() const {
		if(std::holds_alternative<T>(*this)) {
			return std::ref(std::get<T>(*this));
		}
		return std::nullopt;
	}

	template<typename... Types>
	const std::type_info& Variant<Types...>::type() const {
		return std::visit([](auto& val) {
			return typeid(val);
		});
	}

	template<typename... Types>
	String Variant<Types...>::typeName() const {
		return stringify_type(type());
	}
}
