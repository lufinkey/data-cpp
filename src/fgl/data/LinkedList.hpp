//
//  LinkedList.hpp
//  DataCpp
//
//  Created by Luis Finke on 8/30/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#include <fgl/data/Common.hpp>
#include <fgl/data/BasicList.hpp>
#include <fgl/data/BasicString.hpp>
#include <fgl/data/Stringify.hpp>
#include <fgl/data/Traits.hpp>
#include <initializer_list>
#include <list>

namespace fgl {
	template<typename T>
	class LinkedList: public BasicList<std::list<T>> {
	public:
		using BaseType = std::list<T>;
		using typename BasicList<BaseType>::ValueType;
		
		using typename BasicList<BaseType>::value_type;
		using typename BasicList<BaseType>::allocator_type;
		using typename BasicList<BaseType>::size_type;
		using typename BasicList<BaseType>::reference;
		using typename BasicList<BaseType>::const_reference;
		
		using typename BasicList<BaseType>::iterator;
		using typename BasicList<BaseType>::const_iterator;
		using typename BasicList<BaseType>::reverse_iterator;
		using typename BasicList<BaseType>::const_reverse_iterator;
		
		using BasicList<BaseType>::BasicList;
		using BasicList<BaseType>::operator=;
		using BasicList<BaseType>::operator BaseType&;
		using BasicList<BaseType>::operator BaseType&&;
		using BasicList<BaseType>::operator const BaseType&;
		
		using BasicList<BaseType>::size;
		using BasicList<BaseType>::front;
		using BasicList<BaseType>::back;
		
		using BasicList<BaseType>::begin;
		using BasicList<BaseType>::cbegin;
		using BasicList<BaseType>::rbegin;
		using BasicList<BaseType>::crbegin;
		using BasicList<BaseType>::end;
		using BasicList<BaseType>::cend;
		using BasicList<BaseType>::rend;
		using BasicList<BaseType>::crend;
		
		using BasicList<BaseType>::findEqual;
		using BasicList<BaseType>::findLastEqual;
		
		using BasicList<BaseType>::toMap;
		
		using BaseType::insert;
		using BaseType::push_front;
		using BaseType::push_back;
		using BaseType::pop_front;
		using BaseType::pop_back;
		using BaseType::remove;
		using BaseType::erase;
		using BaseType::clear;
		using BaseType::splice;
		using BaseType::sort;
		
		static constexpr size_type npos = (size_type)-1;
		
		LinkedList(const BaseType&);
		LinkedList(BaseType&&);
		template<typename Collection, typename Transform, typename = IsCollection<std::remove_reference_t<Collection>>>
		LinkedList(Collection&& collection, Transform transform);
		
		#ifdef __OBJC__
		template<typename Transform>
		LinkedList(NSArray* objcArray, Transform transform);
		#endif

		#ifdef JNIEXPORT
		template<typename Transform>
		LinkedList(JNIEnv* env, jobjectArray javaArray, Transform transform);
		#endif
		
		LinkedList& operator=(const BaseType&);
		LinkedList& operator=(BaseType&&);
		
		LinkedList<T> slice(size_type offset, size_type count = npos) const;
		
		inline iterator insert(const_iterator pos, const std::list<T>& list);
		inline iterator insert(const_iterator pos, std::list<T>&& list);
		
		inline void pushFront(const T& value);
		inline void pushFront(T&& value);
		inline void pushFrontList(const std::list<T>& list);
		inline void pushFrontList(std::list<T>&& list);
		template<typename Collection, typename = IsCollectionOf<T,std::remove_reference_t<Collection>>>
		inline void pushFrontList(Collection&& collection);
		inline void popFront();
		inline T extractFront();
		inline LinkedList<T> extractListFront(size_t count = 1);
		inline LinkedList<T> extractListFront(const_iterator end);
		
		inline void pushBack(const T& value);
		inline void pushBack(T&& value);
		inline void pushBackList(const std::list<T>& list);
		inline void pushBackList(std::list<T>&& list);
		template<typename Collection, typename = IsCollectionOf<T,std::remove_reference_t<Collection>>>
		inline void pushBackList(Collection&& list);
		inline void popBack();
		inline void pop_back();
		inline T extractBack();
		inline LinkedList<T> extractListBack(size_t count = 1);
		inline LinkedList<T> extractListBack(const_iterator start);
		
		size_type removeEqual(const T& value);
		inline bool removeFirstEqual(const T& value);
		inline bool removeLastEqual(const T& value);
		template<typename Predicate>
		size_type removeWhere(Predicate predicate);
		template<typename Predicate>
		inline bool removeFirstWhere(Predicate predicate);
		template<typename Predicate>
		inline bool removeLastWhere(Predicate predicate);

		template<typename Predicate>
		inline LinkedList<T> where(Predicate predicate) const;
		
		template<typename Transform>
		inline auto map(Transform transform);
		template<typename Transform>
		inline auto map(Transform transform) const;
		
		String toString() const;
	};

	template<typename T, typename Collection, typename = IsCollection<std::remove_reference_t<Collection>>>
	LinkedList<T> operator+(const LinkedList<T>& left, Collection&& right);
	template<typename T, typename Collection, typename = IsCollection<std::remove_reference_t<Collection>>>
	LinkedList<T> operator+(LinkedList<T>&& left, Collection&& right);
	template<typename T>
	LinkedList<T> operator+(LinkedList<T>&& left, std::list<T>&& right);
	template<typename T>
	LinkedList<T> operator+(const LinkedList<T>& left, std::list<T>&& right);
	template<typename T>
	LinkedList<T> operator+(LinkedList<T>&& left, const std::list<T>& right);
	
	
	
#pragma mark LinkedList implementation

	template<typename T>
	LinkedList<T>::LinkedList(const BaseType& list): BasicList<BaseType>(list) {
		//
	}

	template<typename T>
	LinkedList<T>::LinkedList(BaseType&& list): BasicList<BaseType>(list) {
		//
	}

	template<typename T>
	template<typename Collection, typename Transform, typename _>
	LinkedList<T>::LinkedList(Collection&& collection, Transform transform) {
		if constexpr(std::is_reference_v<decltype(*collection.begin())>) {
			for(auto& item : collection) {
				pushBack(transform(item));
			}
		} else {
			for(auto item : collection) {
				pushBack(transform(item));
			}
		}
	}

	#ifdef __OBJC__

	template<typename T>
	template<typename Transform>
	LinkedList<T>::LinkedList(NSArray* objcArray, Transform transform) {
		for(NSObject* obj in objcArray) {
			pushBack(transform(obj));
		}
	}

	#endif

	#ifdef JNIEXPORT

	template<typename T>
	template<typename Transform>
	LinkedList<T>::LinkedList(JNIEnv* env, jobjectArray javaArray, Transform transform) {
		jsize javaArraySize = env->GetArrayLength(javaArray);
		for(jsize i=0; i<javaArraySize; i++) {
			pushBack(transform(env, env->GetObjectArrayElement(javaArray, i)));
		}
	}

	#endif



	template<typename T>
	LinkedList<T>& LinkedList<T>::operator=(const BaseType& list) {
		BaseType::operator=(list);
		return *this;
	}

	template<typename T>
	LinkedList<T>& LinkedList<T>::operator=(BaseType&& list) {
		BaseType::operator=(list);
		return *this;
	}



	template<typename T>
	LinkedList<T> LinkedList<T>::slice(size_type offset, size_type count) const {
		if(offset >= size()) {
			return LinkedList<T>();
		}
		const_iterator it;
		size_t offsetFromEnd = (size() - offset);
		if(offset < offsetFromEnd) {
			it = std::next(begin(), offset);
		} else {
			it = std::prev(end(), offsetFromEnd);
		}
		auto endIt = end();
		LinkedList<T> sublist;
		while(it != endIt && sublist.size() < count) {
			sublist.pushBack(*it);
			it++;
		}
		return sublist;
	}



	template<typename T>
	typename LinkedList<T>::iterator LinkedList<T>::insert(const_iterator pos, const std::list<T>& list) {
		return insert(pos, list.begin(), list.end());
	}
	
	template<typename T>
	typename LinkedList<T>::iterator LinkedList<T>::insert(const_iterator pos, std::list<T>&& list) {
		auto it = list.begin();
		splice(pos, list);
		return it;
	}
	
	
	
	template<typename T>
	void LinkedList<T>::pushFront(const T& value) {
		push_front(value);
	}
	
	template<typename T>
	void LinkedList<T>::pushFront(T&& value) {
		push_front(value);
	}
	
	template<typename T>
	void LinkedList<T>::pushFrontList(const std::list<T>& list) {
		insert(begin(), list.begin(), list.end());
	}
	
	template<typename T>
	void LinkedList<T>::pushFrontList(std::list<T>&& list) {
		splice(begin(), list);
	}

	template<typename T>
	template<typename Collection, typename _>
	void LinkedList<T>::pushFrontList(Collection&& list) {
		insert(end(), list.begin(), list.end());
	}
	
	template<typename T>
	void LinkedList<T>::popFront() {
		FGL_ASSERT(size() > 0, "cannot call popFront on empty array");
		pop_front();
	}

	template<typename T>
	T LinkedList<T>::extractFront() {
		FGL_ASSERT(size() > 0, "cannot call extractFront on empty array");
		auto value = std::move(front());
		pop_front();
		return value;
	}

	template<typename T>
	LinkedList<T> LinkedList<T>::extractListFront(size_t count) {
		LinkedList<T> extracted;
		while(extracted.size() < count && size() > 0) {
			auto value = std::move(front());
			pop_front();
			extracted.pushBack(value);
		}
		return extracted;
	}

	template<typename T>
	LinkedList<T> LinkedList<T>::extractListFront(const_iterator end) {
		LinkedList<T> extracted;
		while(begin() != end && size() > 0) {
			auto value = std::move(front());
			pop_front();
			extracted.pushBack(value);
		}
		return extracted;
	}
	
	
	
	template<typename T>
	void LinkedList<T>::pushBack(const T& value) {
		push_back(value);
	}
	
	template<typename T>
	void LinkedList<T>::pushBack(T&& value) {
		push_back(value);
	}
	
	template<typename T>
	void LinkedList<T>::pushBackList(const std::list<T>& list) {
		insert(end(), list.begin(), list.end());
	}
	
	template<typename T>
	void LinkedList<T>::pushBackList(std::list<T>&& list) {
		splice(end(), list);
	}

	template<typename T>
	template<typename Collection, typename _>
	void LinkedList<T>::pushBackList(Collection&& list) {
		insert(end(), list.begin(), list.end());
	}
	
	template<typename T>
	void LinkedList<T>::popBack() {
		FGL_ASSERT(size() > 0, "cannot call popBack on empty array");
		pop_back();
	}
	
	template<typename T>
	T LinkedList<T>::extractBack() {
		FGL_ASSERT(size() > 0, "cannot call extractBack on empty array");
		auto value = std::move(back());
		pop_back();
		return value;
	}

	template<typename T>
	LinkedList<T> LinkedList<T>::extractListBack(size_t count) {
		LinkedList<T> extracted;
		while(extracted.size() < count && size() > 0) {
			auto value = std::move(back());
			pop_back();
			extracted.pushFront(value);
		}
		return extracted;
	}

	template<typename T>
	LinkedList<T> LinkedList<T>::extractListBack(const_iterator start) {
		LinkedList<T> extracted;
		while(size() > 0) {
			bool lastItem = (std::prev(end(), 1) == start);
			auto value = std::move(back());
			pop_back();
			extracted.pushFront(value);
			if(lastItem) {
				break;
			}
		}
		return extracted;
	}
	
	
	
	template<typename T>
	typename LinkedList<T>::size_type LinkedList<T>::removeEqual(const T& value) {
		size_type count = 0;
		auto last = end();
		auto firstDelete = last;
		for(auto it=begin(); it!=last; it++) {
			if(value == *it) {
				count++;
				if(firstDelete == last) {
					firstDelete = it;
				}
			}
			else if(firstDelete != last) {
				erase(firstDelete, it);
			}
		}
		if(firstDelete != last) {
			erase(firstDelete, last);
		}
		return count;
	}
	
	template<typename T>
	bool LinkedList<T>::removeFirstEqual(const T& value) {
		auto it = findEqual(value);
		if(it == end()) {
			return false;
		}
		erase(it);
		return true;
	}
	
	template<typename T>
	bool LinkedList<T>::removeLastEqual(const T& value) {
		auto it = findLastEqual(value);
		if(it == end()) {
			return false;
		}
		erase(it);
		return true;
	}
	
	template<typename T>
	template<typename Predicate>
	typename LinkedList<T>::size_type LinkedList<T>::removeWhere(Predicate predicate) {
		size_type count = 0;
		auto last = end();
		auto firstDelete = last;
		for(auto it=begin(); it!=last; it++) {
			if(predicate(*it)) {
				count++;
				if(firstDelete == last) {
					firstDelete = it;
				}
			}
			else if(firstDelete != last) {
				erase(firstDelete, it);
			}
		}
		if(firstDelete != last) {
			erase(firstDelete, last);
		}
		return count;
	}
	
	template<typename T>
	template<typename Predicate>
	bool LinkedList<T>::removeFirstWhere(Predicate predicate) {
		auto last = end();
		for(auto it=begin(); it!=last; it++) {
			if(predicate(*it)) {
				erase(it);
				return true;
			}
		}
		return false;
	}
	
	template<typename T>
	template<typename Predicate>
	bool LinkedList<T>::removeLastWhere(Predicate predicate) {
		auto last = rend();
		for(auto it=rbegin(); it!=last; it++) {
			if(predicate(*it)) {
				erase(std::prev(it.base(), 1));
				return true;
			}
		}
		return false;
	}



	template<typename T>
	template<typename Predicate>
	LinkedList<T> LinkedList<T>::where(Predicate predicate) const {
		LinkedList<T> newList;
		for(const_reference item : *this) {
			if(predicate(item)) {
				newList.pushBack(item);
			}
		}
		return newList;
	}
	
	
	
	template<typename T>
	template<typename Transform>
	auto LinkedList<T>::map(Transform transform) {
		using ReturnType = decltype(transform(front()));
		LinkedList<ReturnType> newList;
		for(reference item : *this) {
			newList.pushBack(transform(item));
		}
		return newList;
	}
	
	template<typename T>
	template<typename Transform>
	auto LinkedList<T>::map(Transform transform) const {
		using ReturnType = decltype(transform(front()));
		LinkedList<ReturnType> newList;
		for(const_reference item : *this) {
			newList.pushBack(transform(item));
		}
		return newList;
	}



	template<typename T>
	String LinkedList<T>::toString() const {
		if(size() == 0) {
			return String::join({"LinkedList<", stringify_type<T>(), ">[]"});
		}
		return String::join({
			"LinkedList<", stringify_type<T>(), ">[\n\t",
			String::join(map([](const_reference item) -> String {
				return stringify<T>(item);
			}), ",\n\t"), "\n]" });
	}



	template<typename T, typename Collection, typename _>
	LinkedList<T> operator+(const LinkedList<T>& left, Collection&& right) {
		LinkedList<T> newList;
		newList.pushBackList(left);
		newList.pushBackList(std::forward<Collection>(right));
		return newList;
	}

	template<typename T, typename Collection, typename _>
	LinkedList<T> operator+(LinkedList<T>&& left, Collection&& right) {
		auto newList = std::move(left);
		newList.pushBackList(std::forward<Collection>(right));
		return newList;
	}

	template<typename T>
	LinkedList<T> operator+(LinkedList<T>&& left, std::list<T>&& right) {
		auto newList = std::move(left);
		newList.splice(newList.end(), right);
		return newList;
	}

	template<typename T>
	LinkedList<T> operator+(const LinkedList<T>& left, std::list<T>&& right) {
		LinkedList<T> newList;
		newList.pushBackList(left);
		newList.splice(newList.end(), right);
		return newList;
	}

	template<typename T>
	LinkedList<T> operator+(LinkedList<T>&& left, const std::list<T>& right) {
		auto newList = std::move(left);
		newList.pushBackList(right);
		return newList;
	}
}
