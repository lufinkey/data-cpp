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
	template<typename T, template<typename...> typename Storage = std::list>
	class LinkedList: public BasicList<Storage<T>> {
	public:
		using typename BasicList<Storage<T>>::ValueType;
		using typename BasicList<Storage<T>>::StorageType;
		
		using typename BasicList<Storage<T>>::value_type;
		using typename BasicList<Storage<T>>::allocator_type;
		using typename BasicList<Storage<T>>::size_type;
		
		using typename BasicList<Storage<T>>::iterator;
		using typename BasicList<Storage<T>>::const_iterator;
		using typename BasicList<Storage<T>>::reverse_iterator;
		using typename BasicList<Storage<T>>::const_reverse_iterator;
		
		using BasicList<Storage<T>>::BasicList;
		using BasicList<Storage<T>>::operator=;
		
		using BasicList<Storage<T>>::size;
		using BasicList<Storage<T>>::front;
		using BasicList<Storage<T>>::back;
		
		using BasicList<Storage<T>>::begin;
		using BasicList<Storage<T>>::cbegin;
		using BasicList<Storage<T>>::rbegin;
		using BasicList<Storage<T>>::crbegin;
		using BasicList<Storage<T>>::end;
		using BasicList<Storage<T>>::cend;
		using BasicList<Storage<T>>::rend;
		using BasicList<Storage<T>>::crend;
		
		using BasicList<Storage<T>>::storage;
		
		using BasicList<Storage<T>>::findEqual;
		using BasicList<Storage<T>>::findLastEqual;
		
		template<typename ListType, typename Transform>
		LinkedList(const ListType& list, Transform transform);
		template<typename ListType, typename Transform>
		LinkedList(ListType&& list, Transform transform);
		
		#ifdef __OBJC__
		LinkedList(NSArray* objcArray, Function<T(NSObject*)> transform);
		#endif

		#ifdef JNIEXPORT
		LinkedList(JNIEnv* env, jobjectArray javaArray, Function<T(JNIEnv*,jobject)> transform);
		#endif
		
		inline iterator insert(const_iterator pos, const T& value);
		inline iterator insert(const_iterator pos, T&& value);
		inline iterator insert(const_iterator pos, size_type count, const T& value);
		template<typename InputIterator, typename = IsInputIterator<InputIterator>>
		inline iterator insert(const_iterator pos, InputIterator begin, InputIterator end);
		inline iterator insert(const_iterator pos, std::initializer_list<T> list);
		inline iterator insert(const_iterator pos, const LinkedList<T,Storage>& list);
		inline iterator insert(const_iterator pos, LinkedList<T,Storage>&& list);
		
		inline void splice(const_iterator pos, LinkedList<T,Storage>& list);
		inline void splice(const_iterator pos, LinkedList<T,Storage>&& list);
		inline void splice(const_iterator pos, LinkedList<T,Storage>& list, const_iterator list_pos);
		inline void splice(const_iterator pos, LinkedList<T,Storage>&& list, const_iterator list_pos);
		inline void splice(const_iterator pos, LinkedList<T,Storage>& list, const_iterator begin, const_iterator end);
		inline void splice(const_iterator pos, LinkedList<T,Storage>&& list, const_iterator begin, const_iterator end);
		
		inline void pushFront(const T& value);
		inline void pushFront(T&& value);
		inline void push_front(const T& value);
		inline void push_front(T&& value);
		inline void pushFrontList(const LinkedList<T,Storage>& list);
		inline void pushFrontList(LinkedList<T,Storage>&& list);
		inline void popFront();
		inline void pop_front();
		inline T extractFront();
		inline LinkedList<T,Storage> extractListFront(size_t count = 1);
		
		inline void pushBack(const T& value);
		inline void pushBack(T&& value);
		inline void push_back(const T& value);
		inline void push_back(T&& value);
		inline void pushBackList(const LinkedList<T,Storage>& list);
		inline void pushBackList(LinkedList<T,Storage>&& list);
		inline void popBack();
		inline void pop_back();
		inline T extractBack();
		inline LinkedList<T,Storage> extractListBack(size_t count = 1);
		
		inline iterator remove(const_iterator pos);
		inline iterator remove(const_iterator begin, const_iterator end);
		inline iterator erase(const_iterator pos);
		inline iterator erase(const_iterator begin, const_iterator end);
		size_type removeEqual(const T& value);
		inline bool removeFirstEqual(const T& value);
		inline bool removeLastEqual(const T& value);
		template<typename Predicate>
		size_type removeWhere(Predicate predicate);
		template<typename Predicate>
		inline bool removeFirstWhere(Predicate predicate);
		template<typename Predicate>
		inline bool removeLastWhere(Predicate predicate);
		inline void clear();

		template<typename Predicate>
		inline LinkedList<T,Storage> where(Predicate predicate) const;
		
		template<typename NewT, template<typename...> typename NewStorage = Storage>
		inline LinkedList<NewT,NewStorage> map(const Function<NewT(T&)>&);
		template<typename NewT, template<typename...> typename NewStorage = Storage>
		inline LinkedList<NewT,NewStorage> map(const Function<NewT(const T&)>&) const;
		
		inline void sort();
		template<typename Predicate>
		inline void sort(Predicate predicate);
		
		String toString() const;
	};

	template<typename T, template<typename...> typename Storage, typename ListType, typename = IsContainer<ListType>>
	LinkedList<T,Storage> operator+(const LinkedList<T,Storage>& left, const ListType& right);
	template<typename T, template<typename...> typename Storage>
	LinkedList<T,Storage> operator+(LinkedList<T,Storage>&& left, LinkedList<T,Storage>&& right);
	template<typename T, template<typename...> typename Storage>
	LinkedList<T,Storage> operator+(const LinkedList<T,Storage>& left, LinkedList<T,Storage>&& right);
	template<typename T, template<typename...> typename Storage>
	LinkedList<T,Storage> operator+(LinkedList<T,Storage>&& left, const LinkedList<T,Storage>& right);
	
	
	
#pragma mark LinkedList implementation

	template<typename T, template<typename...> typename Storage>
	template<typename ListType, typename Transform>
	LinkedList<T,Storage>::LinkedList(const ListType& list, Transform transform) {
		for(auto& item : list) {
			pushBack(transform(item));
		}
	}

	template<typename T, template<typename...> typename Storage>
	template<typename ListType, typename Transform>
	LinkedList<T,Storage>::LinkedList(ListType&& list, Transform transform) {
		for(auto& item : list) {
			pushBack(transform(item));
		}
	}

	#ifdef __OBJC__

	template<typename T, template<typename...> typename Storage>
	LinkedList<T,Storage>::LinkedList(NSArray* objcArray, Function<T(NSObject*)> transform) {
		for(NSObject* obj in objcArray) {
			pushBack(transform(obj));
		}
	}

	#endif

	#ifdef JNIEXPORT

	template<typename T, template<typename...> typename Storage>
	LinkedList<T,Storage>::LinkedList(JNIEnv* env, jobjectArray javaArray, Function<T(JNIEnv*,jobject)> transform) {
		jsize javaArraySize = env->GetArrayLength(javaArray);
		for(jsize i=0; i<javaArraySize; i++) {
			pushBack(transform(env, env->GetObjectArrayElement(javaArray, i)));
		}
	}

	#endif
	
	template<typename T, template<typename...> typename Storage>
	typename LinkedList<T,Storage>::iterator LinkedList<T,Storage>::insert(const_iterator pos, const T& value) {
		return storage.insert(pos, value);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename LinkedList<T,Storage>::iterator LinkedList<T,Storage>::insert(const_iterator pos, T&& value) {
		return storage.insert(pos, value);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename LinkedList<T,Storage>::iterator LinkedList<T,Storage>::insert(const_iterator pos, size_type count, const T& value) {
		return storage.insert(pos, count, value);
	}
	
	template<typename T, template<typename...> typename Storage>
	template<typename InputIterator, typename _>
	typename LinkedList<T,Storage>::iterator LinkedList<T,Storage>::insert(const_iterator pos, InputIterator first, InputIterator last) {
		return storage.insert(pos, first, last);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename LinkedList<T,Storage>::iterator LinkedList<T,Storage>::insert(const_iterator pos, std::initializer_list<T> list) {
		return storage.insert(pos, list);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename LinkedList<T,Storage>::iterator LinkedList<T,Storage>::insert(const_iterator pos, const LinkedList<T,Storage>& list) {
		return storage.insert(pos, list.begin(), list.end());
	}
	
	template<typename T, template<typename...> typename Storage>
	typename LinkedList<T,Storage>::iterator LinkedList<T,Storage>::insert(const_iterator pos, LinkedList<T,Storage>&& list) {
		auto it = list.begin();
		storage.splice(pos, list.storage);
		return it;
	}
	
	
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::splice(const_iterator pos, LinkedList<T,Storage>& list) {
		storage.splice(pos, list.storage);
	}
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::splice(const_iterator pos, LinkedList<T,Storage>&& list) {
		storage.splice(pos, list.storage);
	}
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::splice(const_iterator pos, LinkedList<T,Storage>& list, const_iterator list_pos) {
		storage.splice(pos, list.storage, list_pos);
	}
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::splice(const_iterator pos, LinkedList<T,Storage>&& list, const_iterator list_pos) {
		storage.splice(pos, list.storage, list_pos);
	}
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::splice(const_iterator pos, LinkedList<T,Storage>& list, const_iterator first, const_iterator last) {
		storage.splice(pos, list, first, last);
	}
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::splice(const_iterator pos, LinkedList<T,Storage>&& list, const_iterator first, const_iterator last) {
		storage.splice(pos, list, first, last);
	}
	
	
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::pushFront(const T& value) {
		storage.push_front(value);
	}
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::pushFront(T&& value) {
		storage.push_front(value);
	}

	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::push_front(const T& value) {
		storage.push_front(value);
	}
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::push_front(T&& value) {
		storage.push_front(value);
	}
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::pushFrontList(const LinkedList<T,Storage>& list) {
		storage.insert(begin(), list.begin(), list.end());
	}
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::pushFrontList(LinkedList<T,Storage>&& list) {
		storage.insert(begin(), std::make_move_iterator(list.begin()), std::make_move_iterator(list.end()));
	}
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::popFront() {
		FGL_ASSERT(size() > 0, "cannot call popFront on empty array");
		storage.pop_front();
	}

	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::pop_front() {
		storage.pop_front();
	}
	
	template<typename T, template<typename...> typename Storage>
	T LinkedList<T,Storage>::extractFront() {
		FGL_ASSERT(size() > 0, "cannot call extractFront on empty array");
		auto value = std::move(front());
		popFront();
		return value;
	}

	template<typename T, template<typename...> typename Storage>
	LinkedList<T,Storage> LinkedList<T,Storage>::extractListFront(size_t count) {
		LinkedList<T,Storage> extracted;
		while(extracted.size() < count && storage.size() > 0) {
			auto value = std::move(front());
			popFront();
			extracted.pushBack(value);
		}
		return extracted;
	}
	
	
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::pushBack(const T& value) {
		storage.push_back(value);
	}
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::pushBack(T&& value) {
		storage.push_back(value);
	}

	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::push_back(const T& value) {
		storage.push_back(value);
	}
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::push_back(T&& value) {
		storage.push_back(value);
	}
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::pushBackList(const LinkedList<T,Storage>& list) {
		storage.insert(end(), list.begin(), list.end());
	}
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::pushBackList(LinkedList<T,Storage>&& list) {
		storage.insert(end(), std::make_move_iterator(list.begin()), std::make_move_iterator(list.end()));
	}
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::popBack() {
		FGL_ASSERT(size() > 0, "cannot call popBack on empty array");
		storage.pop_back();
	}

	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::pop_back() {
		storage.pop_back();
	}
	
	template<typename T, template<typename...> typename Storage>
	T LinkedList<T,Storage>::extractBack() {
		FGL_ASSERT(size() > 0, "cannot call extractBack on empty array");
		auto value = std::move(back());
		popBack();
		return value;
	}

	template<typename T, template<typename...> typename Storage>
	LinkedList<T,Storage> LinkedList<T,Storage>::extractListBack(size_t count) {
		LinkedList<T,Storage> extracted;
		while(extracted.size() < count && storage.size() > 0) {
			auto value = std::move(back());
			popBack();
			extracted.pushFront(value);
		}
		return extracted;
	}
	
	
	
	template<typename T, template<typename...> typename Storage>
	typename LinkedList<T,Storage>::iterator LinkedList<T,Storage>::remove(const_iterator pos) {
		return storage.erase(pos);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename LinkedList<T,Storage>::iterator LinkedList<T,Storage>::remove(const_iterator first, const_iterator last) {
		return storage.erase(first, last);
	}

	template<typename T, template<typename...> typename Storage>
	typename LinkedList<T,Storage>::iterator LinkedList<T,Storage>::erase(const_iterator pos) {
		return storage.erase(pos);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename LinkedList<T,Storage>::iterator LinkedList<T,Storage>::erase(const_iterator first, const_iterator last) {
		return storage.erase(first, last);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename LinkedList<T,Storage>::size_type LinkedList<T,Storage>::removeEqual(const T& value) {
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
				storage.erase(firstDelete, it);
			}
		}
		if(firstDelete != last) {
			storage.erase(firstDelete, last);
		}
		return count;
	}
	
	template<typename T, template<typename...> typename Storage>
	bool LinkedList<T,Storage>::removeFirstEqual(const T& value) {
		auto it = findEqual(value);
		if(it == end()) {
			return false;
		}
		storage.erase(it);
		return true;
	}
	
	template<typename T, template<typename...> typename Storage>
	bool LinkedList<T,Storage>::removeLastEqual(const T& value) {
		auto it = findLastEqual(value);
		if(it == end()) {
			return false;
		}
		storage.erase(it);
		return true;
	}
	
	template<typename T, template<typename...> typename Storage>
	template<typename Predicate>
	typename LinkedList<T,Storage>::size_type LinkedList<T,Storage>::removeWhere(Predicate predicate) {
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
				storage.erase(firstDelete, it);
			}
		}
		if(firstDelete != last) {
			storage.erase(firstDelete, last);
		}
		return count;
	}
	
	template<typename T, template<typename...> typename Storage>
	template<typename Predicate>
	bool LinkedList<T,Storage>::removeFirstWhere(Predicate predicate) {
		auto last = end();
		for(auto it=begin(); it!=last; it++) {
			if(predicate(*it)) {
				storage.erase(it);
				return true;
			}
		}
		return false;
	}
	
	template<typename T, template<typename...> typename Storage>
	template<typename Predicate>
	bool LinkedList<T,Storage>::removeLastWhere(Predicate predicate) {
		auto last = rend();
		for(auto it=rbegin(); it!=last; it++) {
			if(predicate(*it)) {
				storage.erase(std::prev(it.base(), 1));
				return true;
			}
		}
		return false;
	}
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::clear() {
		storage.clear();
	}



	template<typename T, template<typename...> typename Storage>
	template<typename Predicate>
	LinkedList<T,Storage> LinkedList<T,Storage>::where(Predicate predicate) const {
		LinkedList<T,Storage> newList;
		for(auto& item : storage) {
			if(predicate(item)) {
				newList.pushBack(item);
			}
		}
		return newList;
	}
	
	
	
	template<typename T, template<typename...> typename Storage>
	template<typename NewT, template<typename...> typename NewStorage>
	LinkedList<NewT,NewStorage> LinkedList<T,Storage>::map(const Function<NewT(T&)>& transform) {
		LinkedList<NewT,NewStorage> newList;
		for(auto& item : storage) {
			newList.pushBack(transform(item));
		}
		return newList;
	}
	
	template<typename T, template<typename...> typename Storage>
	template<typename NewT, template<typename...> typename NewStorage>
	LinkedList<NewT,NewStorage> LinkedList<T,Storage>::map(const Function<NewT(const T&)>& transform) const {
		LinkedList<NewT,NewStorage> newList;
		for(auto& item : storage) {
			newList.pushBack(transform(item));
		}
		return newList;
	}



	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::sort() {
		storage.sort();
	}
	
	template<typename T, template<typename...> typename Storage>
	template<typename Predicate>
	void LinkedList<T,Storage>::sort(Predicate predicate) {
		storage.sort(predicate);
	}



	template<typename T, template<typename...> typename Storage>
	String LinkedList<T,Storage>::toString() const {
		if(storage.size() == 0) {
			return String::join({"LinkedList<", stringify_type<T>(), ">[]"});
		}
		return String::join({
			"LinkedList<", stringify_type<T>(), ">[\n\t",
			String::join(map<String>([](const T& item) {
				return stringify(item);
			}), ",\n\t"), "\n]" });
	}



	template<typename T, template<typename...> typename Storage, typename ListType, typename _>
	LinkedList<T,Storage> operator+(const LinkedList<T,Storage>& left, const ListType& right) {
		LinkedList<T,Storage> newList;
		for(auto& item : left) {
			newList.pushBack(item);
		}
		for(auto& item : right) {
			newList.pushBack(item);
		}
		return newList;
	}

	template<typename T, template<typename...> typename Storage>
	LinkedList<T,Storage> operator+(LinkedList<T,Storage>&& left, LinkedList<T,Storage>&& right) {
		LinkedList<T,Storage> newList;
		newList.splice(newList.end(), left);
		newList.splice(newList.end(), right);
		return newList;
	}

	template<typename T, template<typename...> typename Storage>
	LinkedList<T,Storage> operator+(const LinkedList<T,Storage>& left, LinkedList<T,Storage>&& right) {
		LinkedList<T,Storage> newList;
		for(auto& item : left) {
			newList.pushBack(item);
		}
		newList.splice(newList.end(), right);
		return newList;
	}

	template<typename T, template<typename...> typename Storage>
	LinkedList<T,Storage> operator+(LinkedList<T,Storage>&& left, const LinkedList<T,Storage>& right) {
		LinkedList<T,Storage> newList;
		newList.splice(newList.end(), left);
		for(auto& item : right) {
			newList.pushBack(item);
		}
		return newList;
	}
}
