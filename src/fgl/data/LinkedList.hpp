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
		inline void pushFront(const LinkedList<T,Storage>& list);
		inline void pushFront(LinkedList<T,Storage>&& list);
		inline void popFront();
		inline T extractFront();
		
		inline void pushBack(const T& value);
		inline void pushBack(T&& value);
		inline void pushBack(const LinkedList<T,Storage>& list);
		inline void pushBack(LinkedList<T,Storage>&& list);
		inline void popBack();
		inline T extractBack();
		
		inline iterator remove(const_iterator pos);
		inline iterator remove(const_iterator begin, const_iterator end);
		size_type removeEqual(const T& value);
		inline bool removeFirstEqual(const T& value);
		inline bool removeLastEqual(const T& value);
		size_type removeWhere(const Function<bool(const T&)>& predicate);
		inline bool removeFirstWhere(const Function<bool(const T&)>& predicate);
		inline bool removeLastWhere(const Function<bool(const T&)>& predicate);
		#ifdef __OBJC__
		size_type removeWhere(BOOL(^predicate)(const T&));
		inline bool removeFirstWhere(BOOL(^predicate)(const T&));
		inline bool removeLastWhere(BOOL(^predicate)(const T&));
		#endif
		inline void clear();

		inline LinkedList<T,Storage> where(const Function<bool(const T&)>& predicate) const;
		
		template<typename NewT, template<typename...> typename NewStorage = Storage>
		inline LinkedList<NewT,NewStorage> map(const Function<NewT(T&)>&);
		template<typename NewT, template<typename...> typename NewStorage = Storage>
		inline LinkedList<NewT,NewStorage> map(const Function<NewT(const T&)>&) const;

		#ifdef JNIEXPORT
		jobjectArray toJavaObjectArray(JNIEnv* env, jclass objectClass, Function<jobject(JNIEnv*,const T&)> transform) const;
		#endif
	};
	
	
	
#pragma mark LinkedList implementation

	#ifdef JNIEXPORT

	template<typename T, template<typename...> typename Storage>
	LinkedList<T,Storage>::LinkedList(JNIEnv* env, jobjectArray javaArray, Function<T(JNIEnv*,jobject)> transform) {
		jsize javaArraySize = env->GetArrayLength(javaArray);
		reserve((size_type)javaArraySize);
		for(jsize i=0; i<javaArraySize; i++) {
			pushBack(transform(env, env->GetObjectArrayElement(javaArray, i)));
		}
	}

	#endif
	
	template<typename T, template<typename...> typename Storage>
	typename LinkedList<T,Storage>::iterator LinkedList<T,Storage>::insert(const_iterator pos, const T& value) {
		return this->storage.insert(pos, value);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename LinkedList<T,Storage>::iterator LinkedList<T,Storage>::insert(const_iterator pos, T&& value) {
		return this->storage.insert(pos, value);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename LinkedList<T,Storage>::iterator LinkedList<T,Storage>::insert(const_iterator pos, size_type count, const T& value) {
		return this->storage.insert(pos, count, value);
	}
	
	template<typename T, template<typename...> typename Storage>
	template<typename InputIterator, typename _>
	typename LinkedList<T,Storage>::iterator LinkedList<T,Storage>::insert(const_iterator pos, InputIterator first, InputIterator last) {
		return this->storage.insert(pos, first, last);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename LinkedList<T,Storage>::iterator LinkedList<T,Storage>::insert(const_iterator pos, std::initializer_list<T> list) {
		return this->storage.insert(pos, list);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename LinkedList<T,Storage>::iterator LinkedList<T,Storage>::insert(const_iterator pos, const LinkedList<T,Storage>& list) {
		return this->storage.insert(pos, list.begin(), list.end());
	}
	
	template<typename T, template<typename...> typename Storage>
	typename LinkedList<T,Storage>::iterator LinkedList<T,Storage>::insert(const_iterator pos, LinkedList<T,Storage>&& list) {
		auto it = list.begin();
		this->storage.splice(pos, list.storage);
		return it;
	}
	
	
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::splice(const_iterator pos, LinkedList<T,Storage>& list) {
		this->storage.splice(pos, list.storage);
	}
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::splice(const_iterator pos, LinkedList<T,Storage>&& list) {
		this->storage.splice(pos, list.storage);
	}
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::splice(const_iterator pos, LinkedList<T,Storage>& list, const_iterator list_pos) {
		this->storage.splice(pos, list.storage, list_pos);
	}
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::splice(const_iterator pos, LinkedList<T,Storage>&& list, const_iterator list_pos) {
		this->storage.splice(pos, list.storage, list_pos);
	}
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::splice(const_iterator pos, LinkedList<T,Storage>& list, const_iterator first, const_iterator last) {
		this->storage.splice(pos, list, first, last);
	}
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::splice(const_iterator pos, LinkedList<T,Storage>&& list, const_iterator first, const_iterator last) {
		this->storage.splice(pos, list, first, last);
	}
	
	
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::pushFront(const T& value) {
		this->storage.push_front(value);
	}
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::pushFront(T&& value) {
		this->storage.push_front(value);
	}
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::pushFront(const LinkedList<T,Storage>& list) {
		this->storage.insert(this->begin(), list);
	}
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::pushFront(LinkedList<T,Storage>&& list) {
		this->storage.insert(this->begin(), list);
	}
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::popFront() {
		FGL_ASSERT(this->size() > 0, "cannot call popFront on empty array");
		this->storage.pop_front();
	}
	
	template<typename T, template<typename...> typename Storage>
	T LinkedList<T,Storage>::extractFront() {
		FGL_ASSERT(this->size() > 0, "cannot call extractFront on empty array");
		auto value = std::move(this->front());
		popFront();
		return value;
	}
	
	
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::pushBack(const T& value) {
		this->storage.push_back(value);
	}
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::pushBack(T&& value) {
		this->storage.push_back(value);
	}
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::pushBack(const LinkedList<T,Storage>& list) {
		this->storage.insert(this->end(), list);
	}
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::pushBack(LinkedList<T,Storage>&& list) {
		this->storage.insert(this->end(), list);
	}
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::popBack() {
		FGL_ASSERT(this->size() > 0, "cannot call popBack on empty array");
		this->storage.pop_back();
	}
	
	template<typename T, template<typename...> typename Storage>
	T LinkedList<T,Storage>::extractBack() {
		FGL_ASSERT(this->size() > 0, "cannot call extractBack on empty array");
		auto value = std::move(this->back());
		popBack();
		return value;
	}
	
	
	
	template<typename T, template<typename...> typename Storage>
	typename LinkedList<T,Storage>::iterator LinkedList<T,Storage>::remove(const_iterator pos) {
		return this->storage.erase(pos);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename LinkedList<T,Storage>::iterator LinkedList<T,Storage>::remove(const_iterator first, const_iterator last) {
		return this->storage.erase(first, last);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename LinkedList<T,Storage>::size_type LinkedList<T,Storage>::removeEqual(const T& value) {
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
	
	template<typename T, template<typename...> typename Storage>
	bool LinkedList<T,Storage>::removeFirstEqual(const T& value) {
		auto last = this->end();
		for(auto it=this->begin(); it!=last; it++) {
			if(value == *it) {
				this->storage.erase(it);
				return true;
			}
		}
		return false;
	}
	
	template<typename T, template<typename...> typename Storage>
	bool LinkedList<T,Storage>::removeLastEqual(const T& value) {
		auto last = this->rend();
		for(auto it=this->rbegin(); it!=last; it++) {
			if(value == *it) {
				this->storage.erase(it.base() - 1);
				return true;
			}
		}
		return false;
	}
	
	template<typename T, template<typename...> typename Storage>
	typename LinkedList<T,Storage>::size_type LinkedList<T,Storage>::removeWhere(const Function<bool(const T&)>& predicate) {
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
	
	template<typename T, template<typename...> typename Storage>
	bool LinkedList<T,Storage>::removeFirstWhere(const Function<bool(const T&)>& predicate) {
		auto last = this->end();
		for(auto it=this->begin(); it!=last; it++) {
			if(predicate(*it)) {
				this->storage.erase(it);
				return true;
			}
		}
		return false;
	}
	
	template<typename T, template<typename...> typename Storage>
	bool LinkedList<T,Storage>::removeLastWhere(const Function<bool(const T&)>& predicate) {
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
	
	template<typename T, template<typename...> typename Storage>
	typename LinkedList<T,Storage>::size_type LinkedList<T,Storage>::removeWhere(BOOL(^predicate)(const T&)) {
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
	
	template<typename T, template<typename...> typename Storage>
	bool LinkedList<T,Storage>::removeFirstWhere(BOOL(^predicate)(const T&)) {
		auto last = this->end();
		for(auto it=this->begin(); it!=last; it++) {
			if(predicate(*it)) {
				this->storage.erase(it);
				return true;
			}
		}
		return false;
	}
	
	template<typename T, template<typename...> typename Storage>
	bool LinkedList<T,Storage>::removeLastWhere(BOOL(^predicate)(const T&)) {
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
	
	template<typename T, template<typename...> typename Storage>
	void LinkedList<T,Storage>::clear() {
		this->storage.clear();
	}



	template<typename T, template<typename...> typename Storage>
	LinkedList<T,Storage> LinkedList<T,Storage>::where(const Function<bool(const T&)>& predicate) const {
		LinkedList<T,Storage> newList;
		for(auto& item : this->storage) {
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
		newList.reserve(this->size());
		for(auto& item : this->storage) {
			newList.pushBack(transform(item));
		}
		return newList;
	}
	
	template<typename T, template<typename...> typename Storage>
	template<typename NewT, template<typename...> typename NewStorage>
	LinkedList<NewT,NewStorage> LinkedList<T,Storage>::map(const Function<NewT(const T&)>& transform) const {
		LinkedList<NewT,NewStorage> newList;
		newList.reserve(this->size());
		for(auto& item : this->storage) {
			newList.pushBack(transform(item));
		}
		return newList;
	}



	#ifdef JNIEXPORT

	template<typename T, template<typename...> typename Storage>
	jobjectArray LinkedList<T,Storage>::toJavaObjectArray(JNIEnv* env, jclass objectClass, Function<jobject(JNIEnv*,const T&)> transform) const {
		jobjectArray javaArray = env->NewObjectArray((jsize)storage.size(), objectClass, nullptr);
		jsize i=0;
		for(auto& item : storage) {
			env->SetObjectArrayElement(javaArray, i, transform(env, item));
			i++;
		}
		return javaArray;
	}

	#endif
}
