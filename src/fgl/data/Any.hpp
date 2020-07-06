//
//  Any.hpp
//  DataCpp
//
//  Created by Luis Finke on 5/25/20.
//  Copyright © 2020 Luis Finke. All rights reserved.
//

#pragma once

#include <fgl/data/Common.hpp>
#include <fgl/data/Optional.hpp>
#include <fgl/data/String.hpp>
#include <fgl/data/Stringify.hpp>
#include <any>
#include <typeinfo>

namespace fgl {
	class Any {
	private:
		class Base {
		public:
			virtual ~Base() {}
			virtual Base* clone() const = 0;
			virtual void* ptr() const = 0;
			virtual String toString() const = 0;
			virtual std::any toStdAny() const = 0;
			virtual const std::type_info& type() const = 0;
			virtual String typeName() const = 0;
		};
		
		template<typename T>
		class Derived : public Base {
		public:
			T value;
			
			Derived(const T& val);
			Derived(T&& val);
			virtual Base* clone() const override;
			virtual void* ptr() const override;
			virtual String toString() const override;
			virtual std::any toStdAny() const override;
			virtual const std::type_info& type() const override;
			virtual String typeName() const override;
		};
		
		Base* cloneBase() const;
		
		Base* _ptr;
		
	public:
		Any();
		Any(std::nullptr_t);
		Any(Any&&);
		Any(const Any&);
		template<typename U>
		Any(U&&);
		~Any();
		
		Any& operator=(std::nullptr_t);
		Any& operator=(const Any&);
		Any& operator=(Any&&);
		
		template<typename U>
		U& as();
		template<typename U>
		const U& as() const;
		
		template<typename U>
		Optional<U> maybeAs() const;
		template<typename U>
		OptionalRef<U> maybeRefAs();
		template<typename U>
		OptionalRef<const U> maybeRefAs() const;
		
		template<typename U>
		bool is() const;
		
		operator std::any() const;
		std::any toStdAny() const;
		
		template<typename U>
		explicit operator U&();
		template<typename U>
		explicit operator const U&() const;
		
		bool empty() const;
		bool hasValue() const;
		bool has_value() const;
		void reset();
		void swap(Any& any) noexcept;
		
		void* ptr() const;
		
		String toString() const;
		
		const std::type_info& type() const noexcept;
		String typeName() const;
	};



#pragma mark Any implementation

	template<typename T>
	Any::Derived<T>::Derived(const T& val): value(val) {
		//
	}

	template<typename T>
	Any::Derived<T>::Derived(T&& val): value(val) {
		//
	}

	template<typename T>
	Any::Base* Any::Derived<T>::clone() const {
		return new Derived<T>(value);
	}

	template<typename T>
	void* Any::Derived<T>::ptr() const {
		return (void*)(&value);
	}

	template<typename T>
	String Any::Derived<T>::toString() const {
		return fgl::stringify<T>(value);
	}

	template<typename T>
	std::any Any::Derived<T>::toStdAny() const {
		return std::make_any<T>(value);
	}

	template<typename T>
	const std::type_info& Any::Derived<T>::type() const {
		return typeid(T);
	}

	template<typename T>
	String Any::Derived<T>::typeName() const {
		return fgl::stringify_type<T>();
	}
	
	template<typename U>
	Any::Any(U&& value): _ptr(new Derived<typename std::decay<U>::type>(value)) {
		//
	}
	
	template<typename U>
	U& Any::as() {
		using T = typename std::decay<U>::type;
		if(_ptr == nullptr || typeid(T) != _ptr->type()) {
			throw std::bad_any_cast();
		}
		auto derived = static_cast<Derived<T>*>(_ptr);
		return derived->value;
	}
	
	template<typename U>
	const U& Any::as() const {
		using T = typename std::decay<U>::type;
		if(_ptr == nullptr || typeid(T) != _ptr->type()) {
			throw std::bad_any_cast();
		}
		auto derived = static_cast<Derived<T>*>(_ptr);
		return derived->value;
	}

	template<typename U>
	Optional<U> Any::maybeAs() const {
		using T = typename std::decay<U>::type;
		if(_ptr == nullptr) {
			return std::nullopt;
		}
		if(typeid(T) == _ptr->type()) {
			auto derived = static_cast<Derived<T>*>(_ptr);
			return derived->value;
		}
		if constexpr(!std::is_same<Optionalized<T>,T>::value) {
			if(typeid(Optionalized<T>) == _ptr->type()) {
				auto derived = static_cast<Derived<Optionalized<T>>*>(_ptr);
				return derived->value;
			}
		}
		return std::nullopt;
	}

	template<typename U>
	OptionalRef<U> Any::maybeRefAs() {
		using T = typename std::decay<U>::type;
		if(_ptr == nullptr || typeid(T) != _ptr->type()) {
			return std::nullopt;
		}
		auto derived = static_cast<Derived<T>*>(_ptr);
		return std::ref(derived->value);
	}

	template<typename U>
	OptionalRef<const U> Any::maybeRefAs() const {
		using T = typename std::decay<U>::type;
		if(_ptr == nullptr || typeid(T) != _ptr->type()) {
			return std::nullopt;
		}
		auto derived = static_cast<Derived<T>*>(_ptr);
		return std::ref(derived->value);
	}
	
	template<typename U>
	bool Any::is() const {
		using T = typename std::decay<U>::type;
		return (_ptr != nullptr && typeid(T) == _ptr->type());
	}

	template<typename U>
	Any::operator U&() {
		return as<U>();
	}
	
	template<typename U>
	Any::operator const U&() const {
		return as<U>();
	}



#pragma mark Optional implementation

	template<typename T>
	Any Optional<T>::toAny() const {
		if(has_value()) {
			return Any(value());
		}
		return Any();
	}
}
