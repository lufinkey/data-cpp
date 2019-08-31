//
//  BasicList.hpp
//  DataCpp
//
//  Created by Luis Finke on 8/30/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#include <algorithm>
#include <initializer_list>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <fgl/data/Common.hpp>
#include <fgl/data/Traits.hpp>

namespace fgl {
	template<typename Storage>
	class BasicList {
	public:
		typedef typename Storage::value_type ValueType;
		typedef Storage StorageType;
		
		typedef typename Storage::value_type value_type;
		typedef typename Storage::allocator_type allocator_type;
		typedef typename Storage::size_type size_type;
		
		typedef typename Storage::iterator iterator;
		typedef typename Storage::const_iterator const_iterator;
		typedef typename Storage::reverse_iterator reverse_iterator;
		typedef typename Storage::const_reverse_iterator const_reverse_iterator;
		
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
		
		inline iterator findEqual(const ValueType& value);
		inline const_iterator findEqual(const ValueType& value) const;
		inline iterator findLastEqual(const ValueType& value);
		inline const_iterator findLastEqual(const ValueType& value) const;
		inline iterator findWhere(const Function<bool(const ValueType&)>& predicate);
		inline const_iterator findWhere(const Function<bool(const ValueType&)>& predicate) const;
		inline iterator findLastWhere(const Function<bool(const ValueType&)>& predicate);
		inline const_iterator findLastWhere(const Function<bool(const ValueType&)>& predicate) const;
		#ifdef __OBJC__
		inline iterator findWhere(BOOL(^predicate)(const ValueType&));
		inline const_iterator findWhere(BOOL(^predicate)(const ValueType&)) const;
		inline iterator findLastWhere(BOOL(^predicate)(const ValueType&));
		inline const_iterator findLastWhere(BOOL(^predicate)(const ValueType&)) const;
		#endif
		
		inline size_type count(const ValueType& value) const;
		inline size_type countWhere(const Function<bool(const ValueType&)>& predicate) const;
		#ifdef __OBJC__
		inline size_type countWhere(BOOL(^predicate)(const ValueType&)) const;
		#endif
		
		inline ValueType& firstWhere(const Function<bool(const ValueType&)>& predicate, const ValueType& defaultValue);
		inline const ValueType& firstWhere(const Function<bool(const ValueType&)>& predicate, const ValueType& defaultValue) const;
		inline ValueType& lastWhere(const Function<bool(const ValueType&)>& predicate, const ValueType& defaultValue);
		inline const ValueType& lastWhere(const Function<bool(const ValueType&)>& predicate, const ValueType& defaultValue) const;
		#ifdef __OBJC__
		inline ValueType& firstWhere(BOOL(^predicate)(const ValueType&), const ValueType& defaultValue);
		inline const ValueType& firstWhere(BOOL(^predicate)(const ValueType&), const ValueType& defaultValue) const;
		inline ValueType& lastWhere(BOOL(^predicate)(const ValueType&), const ValueType& defaultValue);
		inline const ValueType& lastWhere(BOOL(^predicate)(const ValueType&), const ValueType& defaultValue) const;
		#endif
		
		inline bool contains(const ValueType& value) const;
		inline bool containsWhere(const Function<bool(const ValueType&)>& predicate) const;
		#ifdef __OBJC__
		inline bool containsWhere(BOOL(^predicate)(const ValueType&)) const;
		#endif
		
		inline void sort();
		inline void sort(const Function<bool(const ValueType&,const ValueType&)>& predicate);
		#ifdef __OBJC__
		inline void sort(BOOL(^predicate)(const ValueType&));
		#endif
		inline void stableSort();
		inline void stableSort(const Function<bool(const ValueType&,const ValueType&)>& predicate);
		#ifdef __OBJC__
		inline void stableSort(BOOL(^predicate)(const ValueType&));
		#endif
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
		return it.base() - 1;
	}
	
	template<typename Storage>
	typename BasicList<Storage>::const_iterator BasicList<Storage>::findLastEqual(const ValueType& value) const {
		auto it = std::find(rbegin(), rend(), value);
		if(it == rend()) {
			return end();
		}
		return it.base() - 1;
	}
	
	template<typename Storage>
	typename BasicList<Storage>::iterator BasicList<Storage>::findWhere(const Function<bool(const ValueType&)>& predicate) {
		return std::find_if(begin(), end(), predicate);
	}
	
	template<typename Storage>
	typename BasicList<Storage>::const_iterator BasicList<Storage>::findWhere(const Function<bool(const ValueType&)>& predicate) const {
		return std::find_if(begin(), end(), predicate);
	}
	
	template<typename Storage>
	typename BasicList<Storage>::iterator BasicList<Storage>::findLastWhere(const Function<bool(const ValueType&)>& predicate) {
		auto it = std::find_if(rbegin(), rend(), predicate);
		if(it == rend()) {
			return end();
		}
		return it.base() - 1;
	}
	
	template<typename Storage>
	typename BasicList<Storage>::const_iterator BasicList<Storage>::findLastWhere(const Function<bool(const ValueType&)>& predicate) const {
		auto it = std::find_if(rbegin(), rend(), predicate);
		if(it == rend()) {
			return end();
		}
		return it.base() - 1;
	}
	
	#ifdef __OBJC__
	
	template<typename Storage>
	typename BasicList<Storage>::iterator BasicList<Storage>::findWhere(BOOL(^predicate)(const ValueType&)) {
		return std::find_if(begin(), end(), predicate);
	}
	
	template<typename Storage>
	typename BasicList<Storage>::const_iterator BasicList<Storage>::findWhere(BOOL(^predicate)(const ValueType&)) const {
		return std::find_if(begin(), end(), predicate);
	}
	
	template<typename Storage>
	typename BasicList<Storage>::iterator BasicList<Storage>::findLastWhere(BOOL(^predicate)(const ValueType&)) {
		auto it = std::find_if(rbegin(), rend(), predicate);
		if(it == rend()) {
			return end();
		}
		return it.base() - 1;
	}
	
	template<typename Storage>
	typename BasicList<Storage>::const_iterator BasicList<Storage>::findLastWhere(BOOL(^predicate)(const ValueType&)) const {
		auto it = std::find_if(rbegin(), rend(), predicate);
		if(it == rend()) {
			return end();
		}
		return it.base() - 1;
	}
	
	#endif
	
	
	
	template<typename Storage>
	typename BasicList<Storage>::size_type BasicList<Storage>::count(const ValueType& value) const {
		return std::count(begin(), end(), value);
	}
	
	template<typename Storage>
	typename BasicList<Storage>::size_type BasicList<Storage>::countWhere(const Function<bool(const ValueType&)>& predicate) const {
		return std::count_if(begin(), end(), predicate);
	}
	
	#ifdef __OBJC__
	
	template<typename Storage>
	typename BasicList<Storage>::size_type BasicList<Storage>::countWhere(BOOL(^predicate)(const ValueType&)) const {
		return std::count_if(begin(), end(), predicate);
	}
	
	#endif
	
	
	
	template<typename Storage>
	typename BasicList<Storage>::ValueType& BasicList<Storage>::firstWhere(const Function<bool(const ValueType&)>& predicate, const ValueType& defaultValue) {
		auto it = findWhere(predicate);
		if(it == end()) {
			return defaultValue;
		}
		return *it;
	}
	
	template<typename Storage>
	const typename BasicList<Storage>::ValueType& BasicList<Storage>::firstWhere(const Function<bool(const ValueType&)>& predicate, const ValueType& defaultValue) const {
		auto it = findWhere(predicate);
		if(it == end()) {
			return defaultValue;
		}
		return *it;
	}
	
	template<typename Storage>
	typename BasicList<Storage>::ValueType& BasicList<Storage>::lastWhere(const Function<bool(const ValueType&)>& predicate, const ValueType& defaultValue) {
		auto it = findLastWhere(predicate);
		if(it == end()) {
			return defaultValue;
		}
		return *it;
	}
	
	template<typename Storage>
	const typename BasicList<Storage>::ValueType& BasicList<Storage>::lastWhere(const Function<bool(const ValueType&)>& predicate, const ValueType& defaultValue) const {
		auto it = findLastWhere(predicate);
		if(it == end()) {
			return defaultValue;
		}
		return *it;
	}
	
	#ifdef __OBJC__
	
	template<typename Storage>
	typename BasicList<Storage>::ValueType& BasicList<Storage>::firstWhere(BOOL(^predicate)(const ValueType&), const ValueType& defaultValue) {
		auto it = findWhere(predicate);
		if(it == end()) {
			return defaultValue;
		}
		return *it;
	}
	
	template<typename Storage>
	const typename BasicList<Storage>::ValueType& BasicList<Storage>::firstWhere(BOOL(^predicate)(const ValueType&), const ValueType& defaultValue) const {
		auto it = findWhere(predicate);
		if(it == end()) {
			return defaultValue;
		}
		return *it;
	}
	
	template<typename Storage>
	typename BasicList<Storage>::ValueType& BasicList<Storage>::lastWhere(BOOL(^predicate)(const ValueType&), const ValueType& defaultValue) {
		auto it = findLastWhere(predicate);
		if(it == end()) {
			return defaultValue;
		}
		return *it;
	}
	
	template<typename Storage>
	const typename BasicList<Storage>::ValueType& BasicList<Storage>::lastWhere(BOOL(^predicate)(const ValueType&), const ValueType& defaultValue) const {
		auto it = findLastWhere(predicate);
		if(it == end()) {
			return defaultValue;
		}
		return *it;
	}
	
	#endif
	
	
	
	template<typename Storage>
	bool BasicList<Storage>::contains(const ValueType& value) const {
		return findEqual(value) != end();
	}
	
	template<typename Storage>
	bool BasicList<Storage>::containsWhere(const Function<bool(const ValueType&)>& predicate) const {
		return findWhere(predicate) != end();
	}
	
	#ifdef __OBJC__
	
	template<typename Storage>
	bool BasicList<Storage>::containsWhere(BOOL(^predicate)(const ValueType&)) const {
		return findWhere(predicate) != end();
	}
	
	#endif
	
	
	
	template<typename Storage>
	void BasicList<Storage>::sort() {
		std::sort(begin(), end());
	}
	
	template<typename Storage>
	void BasicList<Storage>::sort(const Function<bool(const ValueType&,const ValueType&)>& predicate) {
		std::sort(begin(), end(), predicate);
	}
	
	#ifdef __OBJC__
	
	template<typename Storage>
	void BasicList<Storage>::sort(BOOL(^predicate)(const ValueType&)) {
		std::sort(begin(), end(), predicate);
	}
	
	#endif
	
	template<typename Storage>
	void BasicList<Storage>::stableSort() {
		std::stable_sort(begin(), end());
	}
	
	template<typename Storage>
	void BasicList<Storage>::stableSort(const Function<bool(const ValueType&,const ValueType&)>& predicate) {
		std::stable_sort(begin(), end(), predicate);
	}
	
	#ifdef __OBJC__
	
	template<typename Storage>
	void BasicList<Storage>::stableSort(BOOL(^predicate)(const ValueType&)) {
		std::stable_sort(begin(), end(), predicate);
	}
	
	#endif
}
