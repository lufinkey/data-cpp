//
//  Any.hpp
//  DataCpp
//
//  Created by Luis Finke on 5/25/20.
//  Copyright © 2020 Luis Finke. All rights reserved.
//

#pragma once

#include <any>
#include <typeinfo>
#include <fgl/data/Common.hpp>
#include <fgl/data/String.hpp>
#include <fgl/data/Stringify.hpp>
#include <fgl/data/Optional.hpp>

namespace fgl {
	class Any {
	private:
		class Base {
		public:
			virtual ~Base() {}
			virtual Base* clone() const = 0;
			virtual void* getPtr() const = 0;
			virtual String toString() const = 0;
			virtual std::any toStdAny() const = 0;
			virtual const std::type_info& type() const = 0;
		};
		
		template<typename T>
		class Derived : public Base {
		public:
			T value;
			
			Derived(const T& val);
			Derived(T&& val);
			virtual Base* clone() const override;
			virtual void* getPtr() const override;
			virtual String toString() const override;
			virtual std::any toStdAny() const override;
			virtual const std::type_info& type() const override;
		};
		
		Base* cloneBase() const;
		
		Base* ptr;
		
	public:
		Any();
		Any(std::nullptr_t);
		Any(Any& any);
		Any(Any&& any);
		Any(const Any& any);
		Any(const Any&& any);
		template<typename U>
		Any(U&& value);
		~Any();
		
		Any& operator=(std::nullptr_t);
		Any& operator=(const Any& any);
		Any& operator=(Any&& any);
		
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
		
		void* getPtr() const;
		
		String toString() const;
		
		const std::type_info& type() const noexcept;
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
	void* Any::Derived<T>::getPtr() const {
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

	Any::Base* Any::cloneBase() const {
		if(ptr != nullptr) {
			return ptr->clone();
		}
		return nullptr;
	}
	
	Any::Any() : ptr(nullptr) {
		//
	}
	
	Any::Any(std::nullptr_t) : ptr(nullptr) {
		//
	}
	
	Any::Any(Any& any) : ptr(any.cloneBase()) {
		//
	}
	
	Any::Any(Any&& any) : ptr(any.ptr) {
		any.ptr = nullptr;
	}
	
	Any::Any(const Any& any) : ptr(any.cloneBase()) {
		//
	}
	
	Any::Any(const Any&& any) : ptr(any.cloneBase()) {
		//
	}
	
	template<typename U>
	Any::Any(U&& value) : ptr(new Derived<typename std::decay<U>::type>(value)) {
		//
	}
	
	Any::~Any() {
		if(ptr != nullptr) {
			delete ptr;
		}
	}
	
	Any& Any::operator=(std::nullptr_t) {
		Base* old_ptr = ptr;
		ptr = nullptr;
		if(old_ptr != nullptr) {
			delete old_ptr;
		}
		return *this;
	}
	
	Any& Any::operator=(const Any& any) {
		if(ptr == any.ptr) {
			return *this;
		}
		Base* old_ptr = ptr;
		ptr = any.cloneBase();
		if(old_ptr != nullptr) {
			delete old_ptr;
		}
		return *this;
	}
	
	Any& Any::operator=(Any&& any) {
		if(ptr == any.ptr) {
			return *this;
		}
		Base* old_ptr = ptr;
		Base* new_ptr = any.ptr;
		any.ptr = nullptr;
		ptr = new_ptr;
		if(old_ptr != nullptr) {
			delete old_ptr;
		}
		return *this;
	}
	
	template<typename U>
	U& Any::as() {
		using T = typename std::decay<U>::type;
		if(ptr == nullptr || typeid(T) != ptr->type()) {
			throw std::bad_any_cast();
		}
		auto derived = static_cast<Derived<T>*>(ptr);
		return derived->value;
	}
	
	template<typename U>
	const U& Any::as() const {
		using T = typename std::decay<U>::type;
		if(ptr == nullptr || typeid(T) != ptr->type()) {
			throw std::bad_any_cast();
		}
		auto derived = static_cast<Derived<T>*>(ptr);
		return derived->value;
	}

	template<typename U>
	Optional<U> Any::maybeAs() const {
		using T = typename std::decay<U>::type;
		if(ptr == nullptr) {
			return std::nullopt;
		}
		if(typeid(T) == ptr->type()) {
			auto derived = static_cast<Derived<T>*>(ptr);
			return derived->value;
		}
		if(typeid(Optionalized<T>) == ptr->type()) {
			auto derived = static_cast<Derived<Optionalized<T>>*>(ptr);
			return derived->value;
		}
		return std::nullopt;
	}

	template<typename U>
	OptionalRef<U> Any::maybeRefAs() {
		using T = typename std::decay<U>::type;
		if(ptr == nullptr || typeid(T) != ptr->type()) {
			return std::nullopt;
		}
		auto derived = static_cast<Derived<T>*>(ptr);
		return std::ref(derived->value);
	}

	template<typename U>
	OptionalRef<const U> Any::maybeRefAs() const {
		using T = typename std::decay<U>::type;
		if(ptr == nullptr || typeid(T) != ptr->type()) {
			return std::nullopt;
		}
		auto derived = static_cast<Derived<T>*>(ptr);
		return std::ref(derived->value);
	}
	
	template<typename U>
	bool Any::is() const {
		using T = typename std::decay<U>::type;
		return (ptr != nullptr && typeid(T) == ptr->type());
	}

	Any::operator std::any() const {
		if(ptr == nullptr) {
			return std::any();
		}
		return ptr->toStdAny();
	}

	std::any Any::toStdAny() const {
		if(ptr == nullptr) {
			return std::any();
		}
		return ptr->toStdAny();
	}
	
	template<typename U>
	Any::operator U&() {
		return as<U>();
	}
	
	template<typename U>
	Any::operator const U&() const {
		return as<U>();
	}
	
	bool Any::empty() const {
		return (ptr == nullptr);
	}

	bool Any::hasValue() const {
		return (ptr != nullptr);
	}

	bool Any::has_value() const {
		return (ptr != nullptr);
	}

	void Any::reset() {
		if(ptr != nullptr) {
			delete ptr;
			ptr = nullptr;
		}
	}

	void Any::swap(Any& any) noexcept {
		Base* tmpPtr = any.ptr;
		any.ptr = ptr;
		ptr = tmpPtr;
	}
	
	void* Any::getPtr() const {
		if(ptr == nullptr) {
			return nullptr;
		}
		return ptr->getPtr();
	}
	
	String Any::toString() const {
		if(ptr == nullptr) {
			return "";
		}
		return ptr->toString();
	}
	
	const std::type_info& Any::type() const noexcept {
		if(ptr == nullptr) {
			return typeid(void);
		}
		return ptr->type();
	}
}
