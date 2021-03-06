//
//  BasicList.hpp
//  DataCpp
//
//  Created by Luis Finke on 8/30/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#include <fgl/data/Common.hpp>
#include <fgl/data/Optional.hpp>
#include <fgl/data/Traits.hpp>
#include <algorithm>
#include <initializer_list>
#include <stdexcept>
#include <type_traits>
#ifdef __OBJC__
#import <Foundation/Foundation.h>
#endif

namespace fgl {
	template<typename Key,typename T,typename Compare,typename Allocator>
	class Map;


	template<typename Storage>
	class BasicList {
	public:
		using ValueType = typename Storage::value_type;
		using StorageType = Storage;
		
		using value_type = typename Storage::value_type;
		using allocator_type = typename Storage::allocator_type;
		using size_type = typename Storage::size_type;
		
		using iterator = typename Storage::iterator;
		using const_iterator = typename Storage::const_iterator;
		using reverse_iterator = typename Storage::reverse_iterator;
		using const_reverse_iterator = typename Storage::const_reverse_iterator;
		
		Storage storage;
		
		inline BasicList();
		inline BasicList(const BasicList<Storage>&);
		inline BasicList(BasicList<Storage>&&);
		inline BasicList(const Storage&);
		inline BasicList(Storage&&);
		inline BasicList(std::initializer_list<ValueType>);
		inline BasicList(const ValueType* data, size_t size);
		template<typename InputIterator, typename = IsInputIterator<InputIterator>>
		inline BasicList(InputIterator begin, InputIterator end);
		
		template<typename Container, typename = IsTypeContainer<ValueType,Container>>
		inline BasicList(const Container&);
		template<typename Container, typename = IsTypeContainer<ValueType,Container>>
		inline BasicList(Container&&);
		
		inline BasicList<Storage>& operator=(const BasicList<Storage>&);
		inline BasicList<Storage>& operator=(BasicList<Storage>&&);
		template<typename Container, typename = IsTypeContainer<ValueType,Container>>
		inline BasicList<Storage>& operator=(const Container&);
		template<typename Container, typename = IsTypeContainer<ValueType,Container>>
		inline BasicList<Storage>& operator=(Container&&);
		inline BasicList<Storage>& operator=(std::initializer_list<ValueType> list);
		
		operator Storage&() & noexcept;
		operator Storage&&() && noexcept;
		operator const Storage&() const& noexcept;
		
		inline iterator begin();
		inline const_iterator begin() const;
		inline const_iterator cbegin() const;
		inline iterator end();
		inline const_iterator end() const;
		inline const_iterator cend() const;
		inline reverse_iterator rbegin();
		inline const_reverse_iterator rbegin() const;
		inline const_reverse_iterator crbegin() const;
		inline reverse_iterator rend();
		inline const_reverse_iterator rend() const;
		inline const_reverse_iterator crend() const;
		
		inline bool empty() const noexcept;
		inline size_type size() const noexcept;
		inline size_type maxSize() const noexcept;
		
		inline void swap(BasicList<Storage>&) noexcept;
		inline void swap(Storage&) noexcept;
		
		inline ValueType& front();
		inline const ValueType& front() const;
		inline ValueType& back();
		inline const ValueType& back() const;
		
		inline Optional<ValueType> first() const;
		inline OptionalRef<ValueType> firstRef();
		inline OptionalRef<const ValueType> firstRef() const;
		
		inline Optional<ValueType> last() const;
		inline OptionalRef<ValueType> lastRef();
		inline OptionalRef<const ValueType> lastRef() const;
		
		inline iterator findEqual(const ValueType& value);
		inline const_iterator findEqual(const ValueType& value) const;
		inline iterator findLastEqual(const ValueType& value);
		inline const_iterator findLastEqual(const ValueType& value) const;
		template<typename Predicate>
		inline iterator findWhere(Predicate predicate);
		template<typename Predicate>
		inline const_iterator findWhere(Predicate predicate) const;
		template<typename Predicate>
		inline iterator findLastWhere(Predicate predicate);
		template<typename Predicate>
		inline const_iterator findLastWhere(Predicate predicate) const;
		
		inline size_type count(const ValueType& value) const;
		template<typename Predicate>
		inline size_type countWhere(Predicate predicate) const;
		
		template<typename Predicate>
		inline ValueType& firstWhere(Predicate predicate, ValueType& defaultValue);
		template<typename Predicate>
		inline const ValueType& firstWhere(Predicate predicate, const ValueType& defaultValue) const;
		template<typename Predicate>
		inline ValueType& lastWhere(Predicate predicate, ValueType& defaultValue);
		template<typename Predicate>
		inline const ValueType& lastWhere(Predicate predicate, const ValueType& defaultValue) const;
		
		template<typename Predicate>
		inline Optional<ValueType> firstWhere(Predicate predicate) const;
		template<typename Predicate>
		inline OptionalRef<ValueType> firstRefWhere(Predicate predicate);
		template<typename Predicate>
		inline OptionalRef<const ValueType> firstRefWhere(Predicate predicate) const;
		template<typename Predicate>
		inline Optional<ValueType> lastWhere(Predicate predicate) const;
		template<typename Predicate>
		inline OptionalRef<ValueType> lastRefWhere(Predicate predicate);
		template<typename Predicate>
		inline OptionalRef<const ValueType> lastRefWhere(Predicate predicate) const;
		
		inline bool contains(const ValueType& value) const;
		template<typename Predicate>
		inline bool containsWhere(Predicate predicate) const;
		
		template<typename T, typename Predicate>
		T reduce(T initialValue, Predicate callback);
		template<typename T, typename Predicate>
		T reduce(T initialValue, Predicate callback) const;
		
		#ifdef __OBJC__
		template<typename Transform>
		inline NSMutableArray* toNSArray(Transform transform);
		template<typename Transform>
		inline NSMutableArray* toNSArray(Transform transform) const;
		#endif
		
		#ifdef JNIEXPORT
		template<typename Transform>
		jobjectArray toJavaObjectArray(JNIEnv* env, jclass objectClass, Transform transform);
		template<typename Transform>
		jobjectArray toJavaObjectArray(JNIEnv* env, jclass objectClass, Transform transform) const;
		#endif
		
		
		template<typename Mapper>
		auto toMap(Mapper mapper) const;
	};
	
	
	
#pragma mark BasicList implementation
	
	template<typename Storage>
	BasicList<Storage>::BasicList() {
		//
	}
	
	template<typename Storage>
	BasicList<Storage>::BasicList(const BasicList<Storage>& list)
	: storage(list.storage) {
		//
	}
	
	template<typename Storage>
	BasicList<Storage>::BasicList(BasicList<Storage>&& list)
	: storage(list.storage) {
		//
	}
	
	template<typename Storage>
	BasicList<Storage>::BasicList(const Storage& storage)
	: storage(storage) {
		//
	}
	
	template<typename Storage>
	BasicList<Storage>::BasicList(Storage&& storage)
	: storage(storage) {
		//
	}
	
	template<typename Storage>
	BasicList<Storage>::BasicList(std::initializer_list<ValueType> list)
	: storage(list) {
		//
	}
	
	template<typename Storage>
	BasicList<Storage>::BasicList(const ValueType* data, size_t size)
	: storage(data, data+size) {
		//
	}
	
	template<typename Storage>
	template<typename InputIterator, typename _>
	BasicList<Storage>::BasicList(InputIterator begin, InputIterator end)
	: storage(begin, end) {
		//
	}
	
	template<typename Storage>
	template<typename Container, typename _>
	BasicList<Storage>::BasicList(const Container& container)
	: storage(container.begin(), container.end()) {
		//
	}
	
	template<typename Storage>
	template<typename Container, typename _>
	BasicList<Storage>::BasicList(Container&& container)
	: storage(std::make_move_iterator(container.begin()), std::make_move_iterator(container.end())) {
		//
	}
	
	template<typename Storage>
	BasicList<Storage>& BasicList<Storage>::operator=(const BasicList<Storage>& list) {
		storage = list.storage;
		return *this;
	}
	
	template<typename Storage>
	BasicList<Storage>& BasicList<Storage>::operator=(BasicList<Storage>&& list) {
		storage = list.storage;
		return *this;
	}
	
	template<typename Storage>
	template<typename Container, typename _>
	BasicList<Storage>& BasicList<Storage>::operator=(const Container& container) {
		storage.assign(container.begin(), container.end());
		return *this;
	}
	
	template<typename Storage>
	template<typename Container, typename _>
	BasicList<Storage>& BasicList<Storage>::operator=(Container&& container) {
		storage.assign(std::make_move_iterator(container.begin()), std::make_move_iterator(container.end()));
		return *this;
	}
	
	template<typename Storage>
	BasicList<Storage>& BasicList<Storage>::operator=(std::initializer_list<ValueType> list) {
		storage.assign(list);
		return *this;
	}



	template<typename Storage>
	BasicList<Storage>::operator Storage&() & noexcept {
		return storage;
	}

	template<typename Storage>
	BasicList<Storage>::operator Storage&&() && noexcept {
		return storage;
	}

	template<typename Storage>
	BasicList<Storage>::operator const Storage&() const& noexcept {
		return storage;
	}
	
	
	
	template<typename Storage>
	typename BasicList<Storage>::iterator BasicList<Storage>::begin() {
		return storage.begin();
	}
	
	template<typename Storage>
	typename BasicList<Storage>::const_iterator BasicList<Storage>::begin() const {
		return storage.begin();
	}
	
	template<typename Storage>
	typename BasicList<Storage>::const_iterator BasicList<Storage>::cbegin() const {
		return storage.cbegin();
	}
	
	template<typename Storage>
	typename BasicList<Storage>::iterator BasicList<Storage>::end() {
		return storage.end();
	}
	
	template<typename Storage>
	typename BasicList<Storage>::const_iterator BasicList<Storage>::end() const {
		return storage.end();
	}
	
	template<typename Storage>
	typename BasicList<Storage>::const_iterator BasicList<Storage>::cend() const {
		return storage.cend();
	}
	
	template<typename Storage>
	typename BasicList<Storage>::reverse_iterator BasicList<Storage>::rbegin() {
		return storage.rbegin();
	}
	
	template<typename Storage>
	typename BasicList<Storage>::const_reverse_iterator BasicList<Storage>::rbegin() const {
		return storage.rbegin();
	}
	
	template<typename Storage>
	typename BasicList<Storage>::const_reverse_iterator BasicList<Storage>::crbegin() const {
		return storage.crbegin();
	}
	
	template<typename Storage>
	typename BasicList<Storage>::reverse_iterator BasicList<Storage>::rend() {
		return storage.rend();
	}
	
	template<typename Storage>
	typename BasicList<Storage>::const_reverse_iterator BasicList<Storage>::rend() const {
		return storage.rend();
	}
	
	template<typename Storage>
	typename BasicList<Storage>::const_reverse_iterator BasicList<Storage>::crend() const {
		return storage.crend();
	}
	
	
	
	template<typename Storage>
	bool BasicList<Storage>::empty() const noexcept {
		return storage.empty();
	}
	
	template<typename Storage>
	typename BasicList<Storage>::size_type BasicList<Storage>::size() const noexcept {
		return storage.size();
	}
	
	template<typename Storage>
	typename BasicList<Storage>::size_type BasicList<Storage>::maxSize() const noexcept {
		return storage.max_size();
	}
	
	
	
	template<typename Storage>
	void BasicList<Storage>::swap(BasicList<Storage>& list) noexcept {
		storage.swap(list.storage);
	}
	
	template<typename Storage>
	void BasicList<Storage>::swap(Storage& otherStorage) noexcept {
		storage.swap(otherStorage);
	}
	
	
	
	template<typename Storage>
	typename BasicList<Storage>::ValueType& BasicList<Storage>::front() {
		FGL_ASSERT(size() > 0, "Cannot get front of empty list");
		return storage.front();
	}
	
	template<typename Storage>
	const typename BasicList<Storage>::ValueType& BasicList<Storage>::front() const {
		FGL_ASSERT(size() > 0, "Cannot get front of empty list");
		return storage.front();
	}
	
	template<typename Storage>
	typename BasicList<Storage>::ValueType& BasicList<Storage>::back() {
		FGL_ASSERT(size() > 0, "Cannot get front of empty list");
		return storage.back();
	}
	
	template<typename Storage>
	const typename BasicList<Storage>::ValueType& BasicList<Storage>::back() const {
		FGL_ASSERT(size() > 0, "Cannot get front of empty list");
		return storage.back();
	}



	template<typename Storage>
	Optional<typename BasicList<Storage>::ValueType> BasicList<Storage>::first() const {
		if(this->storage.size() == 0) {
			return std::nullopt;
		}
		return this->storage.front();
	}
	
	template<typename Storage>
	OptionalRef<typename BasicList<Storage>::ValueType> BasicList<Storage>::firstRef() {
		if(this->storage.size() == 0) {
			return std::nullopt;
		}
		return std::ref<ValueType>(this->storage.front());
	}
	
	template<typename Storage>
	OptionalRef<const typename BasicList<Storage>::ValueType> BasicList<Storage>::firstRef() const {
		if(this->storage.size() == 0) {
			return std::nullopt;
		}
		return std::ref<const ValueType>(this->storage.front());
	}
	
	template<typename Storage>
	Optional<typename BasicList<Storage>::ValueType> BasicList<Storage>::last() const {
		if(this->storage.size() == 0) {
			return std::nullopt;
		}
		return this->storage.back();
	}
	
	template<typename Storage>
	OptionalRef<typename BasicList<Storage>::ValueType> BasicList<Storage>::lastRef() {
		if(this->storage.size() == 0) {
			return std::nullopt;
		}
		return std::ref<ValueType>(this->storage.back());
	}
	
	template<typename Storage>
	OptionalRef<const typename BasicList<Storage>::ValueType> BasicList<Storage>::lastRef() const {
		if(this->storage.size() == 0) {
			return std::nullopt;
		}
		return std::ref<const ValueType>(this->storage.back());
	}
	
	
	
	template<typename Storage>
	typename BasicList<Storage>::iterator BasicList<Storage>::findEqual(const ValueType& value) {
		return std::find(begin(), end(), value);
	}
	
	template<typename Storage>
	typename BasicList<Storage>::const_iterator BasicList<Storage>::findEqual(const ValueType& value) const {
		return std::find(begin(), end(), value);
	}
	
	template<typename Storage>
	typename BasicList<Storage>::iterator BasicList<Storage>::findLastEqual(const ValueType& value) {
		auto it = std::find(rbegin(), rend(), value);
		if(it == rend()) {
			return end();
		}
		auto retIt = it.base();
		retIt--;
		return retIt;
	}
	
	template<typename Storage>
	typename BasicList<Storage>::const_iterator BasicList<Storage>::findLastEqual(const ValueType& value) const {
		auto it = std::find(rbegin(), rend(), value);
		if(it == rend()) {
			return end();
		}
		auto retIt = it.base();
		retIt--;
		return retIt;
	}
	
	template<typename Storage>
	template<typename Predicate>
	typename BasicList<Storage>::iterator BasicList<Storage>::findWhere(Predicate predicate) {
		return std::find_if(begin(), end(), predicate);
	}
	
	template<typename Storage>
	template<typename Predicate>
	typename BasicList<Storage>::const_iterator BasicList<Storage>::findWhere(Predicate predicate) const {
		return std::find_if(begin(), end(), predicate);
	}
	
	template<typename Storage>
	template<typename Predicate>
	typename BasicList<Storage>::iterator BasicList<Storage>::findLastWhere(Predicate predicate) {
		auto it = std::find_if(rbegin(), rend(), predicate);
		if(it == rend()) {
			return end();
		}
		auto retIt = it.base();
		retIt--;
		return retIt;
	}
	
	template<typename Storage>
	template<typename Predicate>
	typename BasicList<Storage>::const_iterator BasicList<Storage>::findLastWhere(Predicate predicate) const {
		auto it = std::find_if(rbegin(), rend(), predicate);
		if(it == rend()) {
			return end();
		}
		auto retIt = it.base();
		retIt--;
		return retIt;
	}
	
	
	
	template<typename Storage>
	typename BasicList<Storage>::size_type BasicList<Storage>::count(const ValueType& value) const {
		return std::count(begin(), end(), value);
	}
	
	template<typename Storage>
	template<typename Predicate>
	typename BasicList<Storage>::size_type BasicList<Storage>::countWhere(Predicate predicate) const {
		return std::count_if(begin(), end(), predicate);
	}
	
	
	
	template<typename Storage>
	template<typename Predicate>
	typename BasicList<Storage>::ValueType& BasicList<Storage>::firstWhere(Predicate predicate, ValueType& defaultValue) {
		auto it = findWhere(predicate);
		if(it == end()) {
			return defaultValue;
		}
		return *it;
	}
	
	template<typename Storage>
	template<typename Predicate>
	const typename BasicList<Storage>::ValueType& BasicList<Storage>::firstWhere(Predicate predicate, const ValueType& defaultValue) const {
		auto it = findWhere(predicate);
		if(it == end()) {
			return defaultValue;
		}
		return *it;
	}
	
	template<typename Storage>
	template<typename Predicate>
	typename BasicList<Storage>::ValueType& BasicList<Storage>::lastWhere(Predicate predicate, ValueType& defaultValue) {
		auto it = findLastWhere(predicate);
		if(it == end()) {
			return defaultValue;
		}
		return *it;
	}
	
	template<typename Storage>
	template<typename Predicate>
	const typename BasicList<Storage>::ValueType& BasicList<Storage>::lastWhere(Predicate predicate, const ValueType& defaultValue) const {
		auto it = findLastWhere(predicate);
		if(it == end()) {
			return defaultValue;
		}
		return *it;
	}



	template<typename Storage>
	template<typename Predicate>
	Optional<typename BasicList<Storage>::ValueType> BasicList<Storage>::firstWhere(Predicate predicate) const {
		auto it = findWhere(predicate);
		if(it == end()) {
			return std::nullopt;
		}
		return *it;
	}

	template<typename Storage>
	template<typename Predicate>
	OptionalRef<typename BasicList<Storage>::ValueType> BasicList<Storage>::firstRefWhere(Predicate predicate) {
		auto it = findWhere(predicate);
		if(it == end()) {
			return std::nullopt;
		}
		return std::ref(*it);
	}

	template<typename Storage>
	template<typename Predicate>
	OptionalRef<const typename BasicList<Storage>::ValueType> BasicList<Storage>::firstRefWhere(Predicate predicate) const {
		auto it = findWhere(predicate);
		if(it == end()) {
			return std::nullopt;
		}
		return std::ref(*it);
	}

	template<typename Storage>
	template<typename Predicate>
	Optional<typename BasicList<Storage>::ValueType> BasicList<Storage>::lastWhere(Predicate predicate) const {
		auto it = findLastWhere(predicate);
		if(it == end()) {
			return std::nullopt;
		}
		return *it;
	}

	template<typename Storage>
	template<typename Predicate>
	OptionalRef<typename BasicList<Storage>::ValueType> BasicList<Storage>::lastRefWhere(Predicate predicate) {
		auto it = findLastWhere(predicate);
		if(it == end()) {
			return std::nullopt;
		}
		return std::ref(*it);
	}

	template<typename Storage>
	template<typename Predicate>
	OptionalRef<const typename BasicList<Storage>::ValueType> BasicList<Storage>::lastRefWhere(Predicate predicate) const {
		auto it = findLastWhere(predicate);
		if(it == end()) {
			return std::nullopt;
		}
		return std::ref(*it);
	}
	
	
	
	template<typename Storage>
	bool BasicList<Storage>::contains(const ValueType& value) const {
		return findEqual(value) != end();
	}
	
	template<typename Storage>
	template<typename Predicate>
	bool BasicList<Storage>::containsWhere(Predicate predicate) const {
		return findWhere(predicate) != end();
	}



	template<typename Storage>
	template<typename T, typename Predicate>
	T BasicList<Storage>::reduce(T value, Predicate callback) {
		size_t i=0;
		for(auto& item : storage) {
			value = std::move(callback(std::move(value), item, i, this));
			i++;
		}
		return value;
	}

	template<typename Storage>
	template<typename T, typename Predicate>
	T BasicList<Storage>::reduce(T value, Predicate callback) const {
		size_t i=0;
		for(auto& item : storage) {
			value = std::move(callback(std::move(value), item, i, this));
			i++;
		}
		return value;
	}



	#ifdef __OBJC__

	template<typename Storage>
	template<typename Transform>
	NSMutableArray* BasicList<Storage>::toNSArray(Transform transform) {
		NSMutableArray* nsArray = [[NSMutableArray alloc] initWithCapacity:(NSUInteger)size()];
		for(auto& item : storage) {
			[nsArray addObject:transform(item)];
		}
		return nsArray;
	}

	template<typename Storage>
	template<typename Transform>
	NSMutableArray* BasicList<Storage>::toNSArray(Transform transform) const {
		NSMutableArray* nsArray = [[NSMutableArray alloc] initWithCapacity:(NSUInteger)size()];
		for(auto& item : storage) {
			[nsArray addObject:transform(item)];
		}
		return nsArray;
	}

	#endif



	#ifdef JNIEXPORT

	template<typename Storage>
	template<typename Transform>
	jobjectArray BasicList<Storage>::toJavaObjectArray(JNIEnv* env, jclass objectClass, Transform transform) {
		jobjectArray javaArray = env->NewObjectArray((jsize)size(), objectClass, nullptr);
		jsize i=0;
		for(auto& item : storage) {
			env->SetObjectArrayElement(javaArray, i, transform(env, item));
			i++;
		}
		return javaArray;
	}

	template<typename Storage>
	template<typename Transform>
	jobjectArray BasicList<Storage>::toJavaObjectArray(JNIEnv* env, jclass objectClass, Transform transform) const {
		jobjectArray javaArray = env->NewObjectArray((jsize)size(), objectClass, nullptr);
		jsize i=0;
		for(auto& item : storage) {
			env->SetObjectArrayElement(javaArray, i, transform(env, item));
			i++;
		}
		return javaArray;
	}

	#endif
}
