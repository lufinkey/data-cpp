//
//  SharedPtr.hpp
//  DataCpp
//
//  Created by Luis Finke on 9/8/21.
//  Copyright © 2021 Luis Finke. All rights reserved.
//

#pragma once

#include <fgl/data/Common.hpp>
#include <memory>

namespace fgl {
	template<typename T>
	class SharedPtr: public std::shared_ptr<T> {
	public:
		using std::shared_ptr<T>::shared_ptr;
		
		SharedPtr(std::shared_ptr<T>&&);
		SharedPtr(const std::shared_ptr<T>&);
		
		operator std::shared_ptr<T>&() &;
		operator std::shared_ptr<T>&&() &&;
		operator const std::shared_ptr<T>&() const&;
		
		template<typename U>
		SharedPtr<U> as() const;
		template<typename U>
		SharedPtr<U> staticAs() const;
	};


	template<typename T>
	class WeakPtr: public std::weak_ptr<T> {
	public:
		using std::weak_ptr<T>::weak_ptr;
		
		WeakPtr(std::weak_ptr<T>&&);
		WeakPtr(const std::weak_ptr<T>&);
		
		operator std::weak_ptr<T>&() &;
		operator std::weak_ptr<T>&&() &&;
		operator const std::weak_ptr<T>&() const&;
		
		SharedPtr<T> lock() const noexcept;
	};


	template<typename T>
	using $ = SharedPtr<T>;
	template<typename T>
	using w$ = WeakPtr<T>;
	template<typename T, typename... Args>
	$<T> new$(Args&&... args) {
		return std::make_shared<T>(args...);
	}



	#pragma mark SharedPtr implementation

	template<typename T>
	SharedPtr<T>::SharedPtr(std::shared_ptr<T>&& ptr): std::shared_ptr<T>(ptr) {
		//
	}

	template<typename T>
	SharedPtr<T>::SharedPtr(const std::shared_ptr<T>& ptr): std::shared_ptr<T>(ptr) {
		//
	}

	template<typename T>
	SharedPtr<T>::operator std::shared_ptr<T>&() & {
		return *this;
	}

	template<typename T>
	SharedPtr<T>::operator std::shared_ptr<T>&&() && {
		return *this;
	}

	template<typename T>
	SharedPtr<T>::operator const std::shared_ptr<T>&() const& {
		return *this;
	}

	template<typename T>
	template<typename U>
	SharedPtr<U> SharedPtr<T>::as() const {
		return std::dynamic_pointer_cast<U>(*this);
	}

	template<typename T>
	template<typename U>
	SharedPtr<U> SharedPtr<T>::staticAs() const {
		return std::static_pointer_cast<U>(*this);
	}



	#pragma mark WeakPtr implementation

	template<typename T>
	WeakPtr<T>::WeakPtr(std::weak_ptr<T>&& ptr): std::weak_ptr<T>(ptr) {
		//
	}

	template<typename T>
	WeakPtr<T>::WeakPtr(const std::weak_ptr<T>& ptr): std::weak_ptr<T>(ptr) {
		//
	}

	template<typename T>
	WeakPtr<T>::operator std::weak_ptr<T>&() & {
		return *this;
	}

	template<typename T>
	WeakPtr<T>::operator std::weak_ptr<T>&&() && {
		return *this;
	}

	template<typename T>
	WeakPtr<T>::operator const std::weak_ptr<T>&() const& {
		return *this;
	}

	template<typename T>
	SharedPtr<T> WeakPtr<T>::lock() const noexcept {
		return std::weak_ptr<T>::lock();
	}
}
