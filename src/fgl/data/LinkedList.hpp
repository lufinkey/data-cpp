//
//  LinkedList.hpp
//  DataCpp
//
//  Created by Luis Finke on 8/30/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#include <initializer_list>
#include <list>
#include <fgl/data/Common.hpp>
#include <fgl/data/BasicList.hpp>
#include <fgl/data/Traits.hpp>

namespace fgl {
	template<typename Storage>
	class BasicLinkedList: public BasicList<Storage> {
	public:
		using typename BasicList<Storage>::ValueType;
		using typename BasicList<Storage>::StorageType;
		
		using typename BasicList<Storage>::value_type;
		using typename BasicList<Storage>::allocator_type;
		using typename BasicList<Storage>::size_type;
		
		using typename BasicList<Storage>::iterator;
		using typename BasicList<Storage>::const_iterator;
		using typename BasicList<Storage>::reverse_iterator;
		using typename BasicList<Storage>::const_reverse_iterator;
		
		using BasicList<Storage>::BasicList;
		using BasicList<Storage>::operator=;
		
		inline iterator insert(const_iterator pos, const ValueType& value);
		inline iterator insert(const_iterator pos, ValueType&& value);
		inline iterator insert(const_iterator pos, size_type count, const ValueType& value);
		template<typename InputIterator, typename = IsInputIterator<InputIterator>>
		inline iterator insert(const_iterator pos, InputIterator begin, InputIterator end);
		inline iterator insert(const_iterator pos, std::initializer_list<ValueType> list);
		inline iterator insert(const_iterator pos, const BasicLinkedList<Storage>& list);
		inline iterator insert(const_iterator pos, BasicLinkedList<Storage>&& list);
		
		inline void splice(const_iterator pos, BasicLinkedList<Storage>& list);
		inline void splice(const_iterator pos, BasicLinkedList<Storage>&& list);
		inline void splice(const_iterator pos, BasicLinkedList<Storage>& list, const_iterator list_pos);
		inline void splice(const_iterator pos, BasicLinkedList<Storage>&& list, const_iterator list_pos);
		inline void splice(const_iterator pos, BasicLinkedList<Storage>& list, const_iterator begin, const_iterator end);
		inline void splice(const_iterator pos, BasicLinkedList<Storage>&& list, const_iterator begin, const_iterator end);
		
		inline void pushFront(const ValueType& value);
		inline void pushFront(ValueType&& value);
		inline void pushFront(const BasicLinkedList<Storage>& list);
		inline void pushFront(BasicLinkedList<Storage>&& list);
		inline void popFront();
		inline ValueType extractFront();
		
		inline void pushBack(const ValueType& value);
		inline void pushBack(ValueType&& value);
		inline void pushBack(const BasicLinkedList<Storage>& list);
		inline void pushBack(BasicLinkedList<Storage>&& list);
		inline void popBack();
		inline ValueType extractBack();
		
		inline iterator remove(const_iterator pos);
		inline iterator remove(const_iterator begin, const_iterator end);
		size_type removeEqual(const ValueType& value);
		inline bool removeFirstEqual(const ValueType& value);
		inline bool removeLastEqual(const ValueType& value);
		size_type removeWhere(const Function<bool(const ValueType&)>& predicate);
		inline bool removeFirstWhere(const Function<bool(const ValueType&)>& predicate);
		inline bool removeLastWhere(const Function<bool(const ValueType&)>& predicate);
		#ifdef __OBJC__
		size_type removeWhere(BOOL(^predicate)(const ValueType&));
		inline bool removeFirstWhere(BOOL(^predicate)(const ValueType&));
		inline bool removeLastWhere(BOOL(^predicate)(const ValueType&));
		#endif
		inline void clear();
	};
	
	template<typename T>
	using LinkedList = BasicLinkedList<std::list<T>>;
	
	
	
#pragma mark LinkedList implementation
	
	template<typename Storage>
	typename BasicLinkedList<Storage>::iterator BasicLinkedList<Storage>::insert(const_iterator pos, const ValueType& value) {
		return this->storage.insert(pos, value);
	}
	
	template<typename Storage>
	typename BasicLinkedList<Storage>::iterator BasicLinkedList<Storage>::insert(const_iterator pos, ValueType&& value) {
		return this->storage.insert(pos, value);
	}
	
	template<typename Storage>
	typename BasicLinkedList<Storage>::iterator BasicLinkedList<Storage>::insert(const_iterator pos, size_type count, const ValueType& value) {
		return this->storage.insert(pos, count, value);
	}
	
	template<typename Storage>
	template<typename InputIterator, typename _>
	typename BasicLinkedList<Storage>::iterator BasicLinkedList<Storage>::insert(const_iterator pos, InputIterator first, InputIterator last) {
		return this->storage.insert(pos, first, last);
	}
	
	template<typename Storage>
	typename BasicLinkedList<Storage>::iterator BasicLinkedList<Storage>::insert(const_iterator pos, std::initializer_list<ValueType> list) {
		return this->storage.insert(pos, list);
	}
	
	template<typename Storage>
	typename BasicLinkedList<Storage>::iterator BasicLinkedList<Storage>::insert(const_iterator pos, const BasicLinkedList<Storage>& list) {
		return this->storage.insert(pos, list.begin(), list.end());
	}
	
	template<typename Storage>
	typename BasicLinkedList<Storage>::iterator BasicLinkedList<Storage>::insert(const_iterator pos, BasicLinkedList<Storage>&& list) {
		auto it = list.begin();
		this->storage.splice(pos, list.storage);
		return it;
	}
	
	
	
	template<typename Storage>
	void BasicLinkedList<Storage>::splice(const_iterator pos, BasicLinkedList<Storage>& list) {
		this->storage.splice(pos, list.storage);
	}
	
	template<typename Storage>
	void BasicLinkedList<Storage>::splice(const_iterator pos, BasicLinkedList<Storage>&& list) {
		this->storage.splice(pos, list.storage);
	}
	
	template<typename Storage>
	void BasicLinkedList<Storage>::splice(const_iterator pos, BasicLinkedList<Storage>& list, const_iterator list_pos) {
		this->storage.splice(pos, list.storage, list_pos);
	}
	
	template<typename Storage>
	void BasicLinkedList<Storage>::splice(const_iterator pos, BasicLinkedList<Storage>&& list, const_iterator list_pos) {
		this->storage.splice(pos, list.storage, list_pos);
	}
	
	template<typename Storage>
	void BasicLinkedList<Storage>::splice(const_iterator pos, BasicLinkedList<Storage>& list, const_iterator first, const_iterator last) {
		this->storage.splice(pos, list, first, last);
	}
	
	template<typename Storage>
	void BasicLinkedList<Storage>::splice(const_iterator pos, BasicLinkedList<Storage>&& list, const_iterator first, const_iterator last) {
		this->storage.splice(pos, list, first, last);
	}
	
	
	
	template<typename Storage>
	void BasicLinkedList<Storage>::pushFront(const ValueType& value) {
		this->storage.push_front(value);
	}
	
	template<typename Storage>
	void BasicLinkedList<Storage>::pushFront(ValueType&& value) {
		this->storage.push_front(value);
	}
	
	template<typename Storage>
	void BasicLinkedList<Storage>::pushFront(const BasicLinkedList<Storage>& list) {
		this->storage.insert(this->begin(), list);
	}
	template<typename Storage>
	void BasicLinkedList<Storage>::pushFront(BasicLinkedList<Storage>&& list) {
		this->storage.insert(this->begin(), list);
	}
	
	template<typename Storage>
	void BasicLinkedList<Storage>::popFront() {
		FGL_ASSERT(this->size() > 0, "cannot call popFront on empty array");
		this->storage.pop_front();
	}
	
	template<typename Storage>
	typename BasicLinkedList<Storage>::ValueType BasicLinkedList<Storage>::extractFront() {
		FGL_ASSERT(this->size() > 0, "cannot call extractFront on empty array");
		auto value = std::move(this->front());
		popFront();
		return value;
	}
	
	
	
	template<typename Storage>
	void BasicLinkedList<Storage>::pushBack(const ValueType& value) {
		this->storage.push_back(value);
	}
	
	template<typename Storage>
	void BasicLinkedList<Storage>::pushBack(ValueType&& value) {
		this->storage.push_back(value);
	}
	
	template<typename Storage>
	void BasicLinkedList<Storage>::pushBack(const BasicLinkedList<Storage>& list) {
		this->storage.insert(this->end(), list);
	}
	template<typename Storage>
	void BasicLinkedList<Storage>::pushBack(BasicLinkedList<Storage>&& list) {
		this->storage.insert(this->end(), list);
	}
	
	template<typename Storage>
	void BasicLinkedList<Storage>::popBack() {
		FGL_ASSERT(this->size() > 0, "cannot call popBack on empty array");
		this->storage.pop_back();
	}
	
	template<typename Storage>
	typename BasicLinkedList<Storage>::ValueType BasicLinkedList<Storage>::extractBack() {
		FGL_ASSERT(this->size() > 0, "cannot call extractBack on empty array");
		auto value = std::move(this->back());
		popBack();
		return value;
	}
	
	
	
	template<typename Storage>
	typename BasicLinkedList<Storage>::iterator BasicLinkedList<Storage>::remove(const_iterator pos) {
		return this->storage.erase(pos);
	}
	
	template<typename Storage>
	typename BasicLinkedList<Storage>::iterator BasicLinkedList<Storage>::remove(const_iterator first, const_iterator last) {
		return this->storage.erase(first, last);
	}
	
	template<typename Storage>
	typename BasicLinkedList<Storage>::size_type BasicLinkedList<Storage>::removeEqual(const ValueType& value) {
		size_type count = 0;
		auto last = this->end();
		auto firstDelete = last;
		for(auto it=this->begin(); it!=last; it++) {
			if(value == *it) {
				count++;
				if(firstDelete == last) {
					firstDelete = it;
				}
			}
			else if(firstDelete != last) {
				this->storage.erase(firstDelete, it);
			}
		}
		if(firstDelete != last) {
			this->storage.erase(firstDelete, last);
		}
		return count;
	}
	
	template<typename Storage>
	bool BasicLinkedList<Storage>::removeFirstEqual(const ValueType& value) {
		auto last = this->end();
		for(auto it=this->begin(); it!=last; it++) {
			if(value == *it) {
				this->storage.erase(it);
				return true;
			}
		}
		return false;
	}
	
	template<typename Storage>
	bool BasicLinkedList<Storage>::removeLastEqual(const ValueType& value) {
		auto last = this->rend();
		for(auto it=this->rbegin(); it!=last; it++) {
			if(value == *it) {
				this->storage.erase(it.base() - 1);
				return true;
			}
		}
		return false;
	}
	
	template<typename Storage>
	typename BasicLinkedList<Storage>::size_type BasicLinkedList<Storage>::removeWhere(const Function<bool(const ValueType&)>& predicate) {
		size_type count = 0;
		auto last = this->end();
		auto firstDelete = last;
		for(auto it=this->begin(); it!=last; it++) {
			if(predicate(*it)) {
				count++;
				if(firstDelete == last) {
					firstDelete = it;
				}
			}
			else if(firstDelete != last) {
				this->storage.erase(firstDelete, it);
			}
		}
		if(firstDelete != last) {
			this->storage.erase(firstDelete, last);
		}
		return count;
	}
	
	template<typename Storage>
	bool BasicLinkedList<Storage>::removeFirstWhere(const Function<bool(const ValueType&)>& predicate) {
		auto last = this->end();
		for(auto it=this->begin(); it!=last; it++) {
			if(predicate(*it)) {
				this->storage.erase(it);
				return true;
			}
		}
		return false;
	}
	
	template<typename Storage>
	bool BasicLinkedList<Storage>::removeLastWhere(const Function<bool(const ValueType&)>& predicate) {
		auto last = this->rend();
		for(auto it=this->rbegin(); it!=last; it++) {
			if(predicate(*it)) {
				this->storage.erase(it.base() - 1);
				return true;
			}
		}
		return false;
	}
	
	#ifdef __OBJC__
	
	template<typename Storage>
	typename BasicLinkedList<Storage>::size_type BasicLinkedList<Storage>::removeWhere(BOOL(^predicate)(const ValueType&)) {
		size_type count = 0;
		auto last = this->end();
		auto firstDelete = last;
		for(auto it=this->begin(); it!=last; it++) {
			if(predicate(*it)) {
				count++;
				if(firstDelete == last) {
					firstDelete = it;
				}
			}
			else if(firstDelete != last) {
				this->storage.erase(firstDelete, it);
			}
		}
		if(firstDelete != last) {
			this->storage.erase(firstDelete, last);
		}
		return count;
	}
	
	template<typename Storage>
	bool BasicLinkedList<Storage>::removeFirstWhere(BOOL(^predicate)(const ValueType&)) {
		auto last = this->end();
		for(auto it=this->begin(); it!=last; it++) {
			if(predicate(*it)) {
				this->storage.erase(it);
				return true;
			}
		}
		return false;
	}
	
	template<typename Storage>
	bool BasicLinkedList<Storage>::removeLastWhere(BOOL(^predicate)(const ValueType&)) {
		auto last = this->rend();
		for(auto it=this->rbegin(); it!=last; it++) {
			if(predicate(*it)) {
				this->storage.erase(it.base() - 1);
				return true;
			}
		}
		return false;
	}
	
	#endif
	
	template<typename Storage>
	void BasicLinkedList<Storage>::clear() {
		this->storage.clear();
	}
}
