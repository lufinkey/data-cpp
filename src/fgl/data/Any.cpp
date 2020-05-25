//
//  Any.cpp
//  DataCpp
//
//  Created by Luis Finke on 5/25/20.
//  Copyright © 2020 Luis Finke. All rights reserved.
//

#include <fgl/data/Any.hpp>

namespace fgl {
	Any::Base* Any::cloneBase() const {
		if(_ptr != nullptr) {
			return _ptr->clone();
		}
		return nullptr;
	}
	
	Any::Any(): _ptr(nullptr) {
		//
	}
	
	Any::Any(std::nullptr_t): _ptr(nullptr) {
		//
	}
	
	Any::Any(Any&& any): _ptr(any._ptr) {
		any._ptr = nullptr;
	}
	
	Any::Any(const Any& any): _ptr(any.cloneBase()) {
		//
	}
	
	Any::~Any() {
		if(_ptr != nullptr) {
			delete _ptr;
		}
	}
	
	Any& Any::operator=(std::nullptr_t) {
		Base* old_ptr = _ptr;
		_ptr = nullptr;
		if(old_ptr != nullptr) {
			delete old_ptr;
		}
		return *this;
	}
	
	Any& Any::operator=(const Any& any) {
		if(_ptr == any._ptr) {
			return *this;
		}
		Base* old_ptr = _ptr;
		_ptr = any.cloneBase();
		if(old_ptr != nullptr) {
			delete old_ptr;
		}
		return *this;
	}
	
	Any& Any::operator=(Any&& any) {
		if(_ptr == any._ptr) {
			return *this;
		}
		Base* old_ptr = _ptr;
		Base* new_ptr = any._ptr;
		any._ptr = nullptr;
		_ptr = new_ptr;
		if(old_ptr != nullptr) {
			delete old_ptr;
		}
		return *this;
	}

	Any::operator std::any() const {
		if(_ptr == nullptr) {
			return std::any();
		}
		return _ptr->toStdAny();
	}

	std::any Any::toStdAny() const {
		if(_ptr == nullptr) {
			return std::any();
		}
		return _ptr->toStdAny();
	}
	
	bool Any::empty() const {
		return (_ptr == nullptr);
	}

	bool Any::hasValue() const {
		return (_ptr != nullptr);
	}

	bool Any::has_value() const {
		return (_ptr != nullptr);
	}

	void Any::reset() {
		if(_ptr != nullptr) {
			delete _ptr;
			_ptr = nullptr;
		}
	}

	void Any::swap(Any& any) noexcept {
		Base* tmpPtr = any._ptr;
		any._ptr = _ptr;
		_ptr = tmpPtr;
	}
	
	void* Any::ptr() const {
		if(_ptr == nullptr) {
			return nullptr;
		}
		return _ptr->ptr();
	}
	
	String Any::toString() const {
		if(_ptr == nullptr) {
			return "";
		}
		return _ptr->toString();
	}
	
	const std::type_info& Any::type() const noexcept {
		if(_ptr == nullptr) {
			return typeid(void);
		}
		return _ptr->type();
	}
}
