//
//  ArrayList.hpp
//  DataCpp
//
//  Created by Luis Finke on 8/30/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#include <fgl/data/Common.hpp>
#include <fgl/data/BasicList.hpp>
#include <fgl/data/BasicString.hpp>
#include <fgl/data/Optional.hpp>
#include <fgl/data/Stringify.hpp>
#include <fgl/data/Traits.hpp>
#include <initializer_list>
#include <vector>
#ifdef __OBJC__
#import <Foundation/Foundation.h>
#endif

namespace fgl {
	template<typename T, template<typename...> typename Storage = std::vector>
	class ArrayList: public BasicList<Storage<T>> {
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
		using BasicList<Storage<T>>::operator Storage<T>&;
		using BasicList<Storage<T>>::operator Storage<T>&&;
		using BasicList<Storage<T>>::operator const Storage<T>&;
		
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
		ArrayList(const ListType& list, Transform transform);
		template<typename ListType, typename Transform>
		ArrayList(ListType&& list, Transform transform);
		
		#ifdef __OBJC__
		ArrayList(NSArray* objcArray, Function<T(NSObject*)> transform);
		#endif

		#ifdef JNIEXPORT
		ArrayList(JNIEnv* env, jobjectArray javaArray, Function<T(JNIEnv*,jobject)> transform);
		#endif
		
		inline T& operator[](size_type index);
		inline const T& operator[](size_type) const;
		
		inline T& at(size_type index);
		inline const T& at(size_type) const;
		
		inline Optional<T> maybeAt(size_type index) const;
		inline Optional<std::reference_wrapper<T>> maybeRefAt(size_type index);
		inline Optional<std::reference_wrapper<const T>> maybeRefAt(size_type index) const;
		
		inline const T* data() const;
		inline size_type capacity() const;
		
		inline void reserve(size_type size);
		inline void shrinkToFit();
		
		inline void resize(size_type count);
		inline void resize(size_type count, const T& value);
		
		inline iterator insert(const_iterator pos, const T& value);
		inline iterator insert(const_iterator pos, T&& value);
		inline iterator insert(const_iterator pos, size_type count, const T& value);
		template<typename InputIterator, typename = IsInputIterator<InputIterator>>
		inline iterator insert(const_iterator pos, InputIterator begin, InputIterator end);
		inline iterator insert(const_iterator pos, std::initializer_list<T> list);
		inline iterator insert(const_iterator pos, const ArrayList<T,Storage>& list);
		inline iterator insert(const_iterator pos, ArrayList<T,Storage>&& list);
		
		inline iterator insert(size_type pos, const T& value);
		inline iterator insert(size_type pos, T&& value);
		inline iterator insert(size_type pos, size_type count, const T& value);
		template<typename InputIterator, typename = IsInputIterator<InputIterator>>
		inline iterator insert(size_type pos, InputIterator begin, InputIterator end);
		inline iterator insert(size_type pos, std::initializer_list<T> list);
		inline iterator insert(size_type pos, const ArrayList<T,Storage>& list);
		inline iterator insert(size_type pos, ArrayList<T,Storage>&& list);
		
		inline void pushBack(const T& value);
		inline void pushBack(T&& value);
		inline void push_back(const T& value);
		inline void push_back(T&& value);
		inline void pushBackList(const ArrayList<T,Storage>& list);
		inline void pushBackList(ArrayList<T,Storage>&& list);
		inline void popBack();
		inline void pop_back();
		inline T extractBack();
		
		inline iterator remove(const_iterator pos);
		inline iterator remove(const_iterator begin, const_iterator end);
		inline iterator erase(const_iterator pos);
		inline iterator erase(const_iterator begin, const_iterator end);
		inline iterator removeAt(size_type pos);
		inline iterator removeAt(size_type pos, size_type count);
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
		
		inline size_type indexOf(const T& value) const;
		inline size_type lastIndexOf(const T& value) const;
		template<typename Predicate>
		inline size_type indexWhere(Predicate predicate) const;
		template<typename Predicate>
		inline size_type lastIndexWhere(Predicate predicate) const;

		template<typename Predicate>
		inline ArrayList<T,Storage> where(Predicate predicate) const;
		
		template<typename NewT, template<typename...> typename NewStorage = Storage>
		inline ArrayList<NewT,NewStorage> map(const Function<NewT(T&)>&);
		template<typename NewT, template<typename...> typename NewStorage = Storage>
		inline ArrayList<NewT,NewStorage> map(const Function<NewT(const T&)>&) const;
		
		inline void sort();
		template<typename Predicate>
		inline void sort(Predicate predicate);
		inline void unstableSort();
		template<typename Predicate>
		inline void unstableSort(Predicate predicate);
		
		String toString() const;
	};

	template<typename T, template<typename...> typename Storage, typename ListType, typename = IsContainer<ListType>>
	ArrayList<T,Storage> operator+(const ArrayList<T,Storage>& left, const ListType& right);
	
	
	
#pragma mark ArrayList implementation

	template<typename T, template<typename...> typename Storage>
	template<typename ListType, typename Transform>
	ArrayList<T,Storage>::ArrayList(const ListType& list, Transform transform) {
		reserve(list.size());
		for(auto& item : list) {
			pushBack(transform(item));
		}
	}

	template<typename T, template<typename...> typename Storage>
	template<typename ListType, typename Transform>
	ArrayList<T,Storage>::ArrayList(ListType&& list, Transform transform) {
		reserve(list.size());
		for(auto& item : list) {
			pushBack(transform(item));
		}
	}

	#ifdef __OBJC__

	template<typename T, template<typename...> typename Storage>
	ArrayList<T,Storage>::ArrayList(NSArray* objcArray, Function<T(NSObject*)> transform) {
		reserve((size_type)objcArray.count);
		for(NSObject* obj in objcArray) {
			pushBack(transform(obj));
		}
	}

	#endif

	#ifdef JNIEXPORT

	template<typename T, template<typename...> typename Storage>
	ArrayList<T,Storage>::ArrayList(JNIEnv* env, jobjectArray javaArray, Function<T(JNIEnv*,jobject)> transform) {
		jsize javaArraySize = env->GetArrayLength(javaArray);
		reserve((size_type)javaArraySize);
		for(jsize i=0; i<javaArraySize; i++) {
			pushBack(transform(env, env->GetObjectArrayElement(javaArray, i)));
		}
	}

	#endif
	
	template<typename T, template<typename...> typename Storage>
	T& ArrayList<T,Storage>::operator[](size_type index) {
		FGL_ASSERT(index >= 0 && index < size(), "index out of bounds");
		return storage[index];
	}
	
	template<typename T, template<typename...> typename Storage>
	const T& ArrayList<T,Storage>::operator[](size_type index) const {
		FGL_ASSERT(index >= 0 && index < size(), "index out of bounds");
		return storage[index];
	}
	
	template<typename T, template<typename...> typename Storage>
	T& ArrayList<T,Storage>::at(size_type index) {
		FGL_ASSERT(index >= 0 && index < size(), "index out of bounds");
		return storage.at(index);
	}
	
	template<typename T, template<typename...> typename Storage>
	const T& ArrayList<T,Storage>::at(size_type index) const {
		FGL_ASSERT(index >= 0 && index < size(), "index out of bounds");
		return storage.at(index);
	}
	


	template<typename T, template<typename...> typename Storage>
	Optional<T> ArrayList<T,Storage>::maybeAt(size_type index) const {
		if(index >= storage.size()) {
			return std::nullopt;
		}
		return storage[index];
	}
	
	template<typename T, template<typename...> typename Storage>
	Optional<std::reference_wrapper<T>> ArrayList<T,Storage>::maybeRefAt(size_type index) {
		if(index >= storage.size()) {
			return std::nullopt;
		}
		return std::ref<T>(storage[index]);
	}
	
	template<typename T, template<typename...> typename Storage>
	Optional<std::reference_wrapper<const T>> ArrayList<T,Storage>::maybeRefAt(size_type index) const {
		if(index >= storage.size()) {
			return std::nullopt;
		}
		return std::ref<const T>(storage[index]);
	}
	
	
	
	template<typename T, template<typename...> typename Storage>
	const T* ArrayList<T,Storage>::data() const {
		return storage.data();
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::size_type ArrayList<T,Storage>::capacity() const {
		return storage.capacity();
	}
	
	template<typename T, template<typename...> typename Storage>
	void ArrayList<T,Storage>::reserve(size_type size) {
		storage.reserve(size);
	}
	
	template<typename T, template<typename...> typename Storage>
	void ArrayList<T,Storage>::shrinkToFit() {
		storage.shrink_to_fit();
	}
	
	template<typename T, template<typename...> typename Storage>
	void ArrayList<T,Storage>::resize(size_type count) {
		storage.resize(count);
	}
	
	template<typename T, template<typename...> typename Storage>
	void ArrayList<T,Storage>::resize(size_type count, const T& value) {
		storage.resize(count, value);
	}
	
	
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::insert(const_iterator pos, const T& value) {
		return storage.insert(pos, value);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::insert(const_iterator pos, T&& value) {
		return storage.insert(pos, value);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::insert(const_iterator pos, size_type count, const T& value) {
		return storage.insert(pos, count, value);
	}
	
	template<typename T, template<typename...> typename Storage>
	template<typename InputIterator, typename _>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::insert(const_iterator pos, InputIterator first, InputIterator last) {
		return storage.insert(pos, first, last);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::insert(const_iterator pos, std::initializer_list<T> list) {
		return storage.insert(pos, list);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::insert(const_iterator pos, const ArrayList<T,Storage>& list) {
		return storage.insert(pos, list.begin(), list.end());
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::insert(const_iterator pos, ArrayList<T,Storage>&& list) {
		return storage.insert(pos, std::make_move_iterator(list.begin()), std::make_move_iterator(list.end()));
	}
	
	
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::insert(size_type pos, const T& value) {
		return storage.insert(begin()+pos, value);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::insert(size_type pos, T&& value) {
		return storage.insert(begin()+pos, value);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::insert(size_type pos, size_type count, const T& value) {
		return storage.insert(begin()+pos, count, value);
	}
	
	template<typename T, template<typename...> typename Storage>
	template<typename InputIterator, typename _>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::insert(size_type pos, InputIterator first, InputIterator last) {
		return storage.insert(begin()+pos, first, last);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::insert(size_type pos, std::initializer_list<T> list) {
		return storage.insert(begin()+pos, list);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::insert(size_type pos, const ArrayList<T,Storage>& list) {
		return storage.insert(begin()+pos, list.begin(), list.end());
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::insert(size_type pos, ArrayList<T,Storage>&& list) {
		return storage.insert(begin()+pos, std::make_move_iterator(list.begin()), std::make_move_iterator(list.end()));
	}
	
	
	
	template<typename T, template<typename...> typename Storage>
	void ArrayList<T,Storage>::pushBack(const T& value) {
		storage.push_back(value);
	}
	
	template<typename T, template<typename...> typename Storage>
	void ArrayList<T,Storage>::pushBack(T&& value) {
		storage.push_back(value);
	}

	template<typename T, template<typename...> typename Storage>
	void ArrayList<T,Storage>::push_back(const T& value) {
		storage.push_back(value);
	}
	
	template<typename T, template<typename...> typename Storage>
	void ArrayList<T,Storage>::push_back(T&& value) {
		storage.push_back(value);
	}
	
	template<typename T, template<typename...> typename Storage>
	void ArrayList<T,Storage>::pushBackList(const ArrayList<T,Storage>& list) {
		storage.insert(end(), list.begin(), list.end());
	}
	template<typename T, template<typename...> typename Storage>
	void ArrayList<T,Storage>::pushBackList(ArrayList<T,Storage>&& list) {
		storage.insert(end(), std::make_move_iterator(list.begin()), std::make_move_iterator(list.end()));
	}
	
	template<typename T, template<typename...> typename Storage>
	void ArrayList<T,Storage>::popBack() {
		FGL_ASSERT(size() > 0, "cannot call popBack on empty array");
		storage.pop_back();
	}

	template<typename T, template<typename...> typename Storage>
	void ArrayList<T,Storage>::pop_back() {
		storage.pop_back();
	}
	
	template<typename T, template<typename...> typename Storage>
	T ArrayList<T,Storage>::extractBack() {
		FGL_ASSERT(size() > 0, "cannot call extractBack on empty array");
		auto value = std::move(back());
		popBack();
		return value;
	}
	
	
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::remove(const_iterator pos) {
		return storage.erase(pos);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::remove(const_iterator first, const_iterator last) {
		return storage.erase(first, last);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::erase(const_iterator pos) {
		return storage.erase(pos);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::erase(const_iterator first, const_iterator last) {
		return storage.erase(first, last);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::removeAt(size_type pos) {
		return storage.erase(begin()+pos);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::removeAt(size_type pos, size_type count) {
		auto start = begin() + pos;
		return storage.erase(start, start+count);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::size_type ArrayList<T,Storage>::removeEqual(const T& value) {
		size_type removeCount = 0;
		size_t firstRemoveIndex = -1;
		for(size_t i=(size()-1); i!=-1; i--) {
			if(storage[i] == value) {
				removeCount++;
				if(firstRemoveIndex == -1) {
					firstRemoveIndex = i;
				}
			}
			else {
				if(firstRemoveIndex != -1) {
					storage.erase(begin()+i+1, begin()+firstRemoveIndex+1);
					firstRemoveIndex = -1;
				}
			}
		}
		if(firstRemoveIndex != -1) {
			storage.erase(begin(), begin()+firstRemoveIndex+1);
		}
		return removeCount;
	}
	
	template<typename T, template<typename...> typename Storage>
	bool ArrayList<T,Storage>::removeFirstEqual(const T& value) {
		auto it = findEqual(value);
		if(it == end()) {
			return false;
		}
		storage.erase(it);
		return true;
	}
	
	template<typename T, template<typename...> typename Storage>
	bool ArrayList<T,Storage>::removeLastEqual(const T& value) {
		auto it = findLastEqual(value);
		if(it == end()) {
			return false;
		}
		storage.erase(it);
		return true;
	}
	
	template<typename T, template<typename...> typename Storage>
	template<typename Predicate>
	typename ArrayList<T,Storage>::size_type ArrayList<T,Storage>::removeWhere(Predicate predicate) {
		size_type removeCount = 0;
		size_t firstRemoveIndex = -1;
		for(size_t i=(size()-1); i!=-1; i--) {
			if(predicate(storage[i])) {
				removeCount++;
				if(firstRemoveIndex == -1) {
					firstRemoveIndex = i;
				}
			}
			else {
				if(firstRemoveIndex != -1) {
					storage.erase(begin()+i+1, begin()+firstRemoveIndex+1);
					firstRemoveIndex = -1;
				}
			}
		}
		if(firstRemoveIndex != -1) {
			storage.erase(begin(), begin()+firstRemoveIndex+1);
		}
		return removeCount;
	}
	
	template<typename T, template<typename...> typename Storage>
	template<typename Predicate>
	bool ArrayList<T,Storage>::removeFirstWhere(Predicate predicate) {
		auto it = findWhere(predicate);
		if(it == end()) {
			return false;
		}
		storage.erase(it);
		return true;
	}
	
	template<typename T, template<typename...> typename Storage>
	template<typename Predicate>
	bool ArrayList<T,Storage>::removeLastWhere(Predicate predicate) {
		auto it = findLastWhere(predicate);
		if(it == end()) {
			return false;
		}
		storage.erase(it);
		return true;
	}
	
	template<typename T, template<typename...> typename Storage>
	void ArrayList<T,Storage>::clear() {
		storage.clear();
	}
	
	
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::size_type ArrayList<T,Storage>::indexOf(const T& value) const {
		return findEqual(value) - begin();
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::size_type ArrayList<T,Storage>::lastIndexOf(const T& value) const {
		return findLastEqual(value) - begin();
	}
	
	template<typename T, template<typename...> typename Storage>
	template<typename Predicate>
	typename ArrayList<T,Storage>::size_type ArrayList<T,Storage>::indexWhere(Predicate predicate) const {
		return findWhere(predicate) - begin();
	}
	
	template<typename T, template<typename...> typename Storage>
	template<typename Predicate>
	typename ArrayList<T,Storage>::size_type ArrayList<T,Storage>::lastIndexWhere(Predicate predicate) const {
		return findLastWhere(predicate) - begin();
	}



	template<typename T, template<typename...> typename Storage>
	template<typename Predicate>
	ArrayList<T,Storage> ArrayList<T,Storage>::where(Predicate predicate) const {
		std::list<T> newList;
		for(auto& item : storage) {
			if(predicate(item)) {
				newList.push_back(item);
			}
		}
		return newList;
	}
	
	
	
	template<typename T, template<typename...> typename Storage>
	template<typename NewT, template<typename...> typename NewStorage>
	ArrayList<NewT,NewStorage> ArrayList<T,Storage>::map(const Function<NewT(T&)>& transform) {
		ArrayList<NewT,NewStorage> newArray;
		newArray.reserve(size());
		for(auto& item : storage) {
			newArray.pushBack(transform(item));
		}
		return newArray;
	}
	
	template<typename T, template<typename...> typename Storage>
	template<typename NewT, template<typename...> typename NewStorage>
	ArrayList<NewT,NewStorage> ArrayList<T,Storage>::map(const Function<NewT(const T&)>& transform) const {
		ArrayList<NewT,NewStorage> newArray;
		newArray.reserve(size());
		for(auto& item : storage) {
			newArray.pushBack(transform(item));
		}
		return newArray;
	}



	template<typename T, template<typename...> typename Storage>
	void ArrayList<T,Storage>::sort() {
		std::stable_sort(begin(), end());
	}
	
	template<typename T, template<typename...> typename Storage>
	template<typename Predicate>
	void ArrayList<T,Storage>::sort(Predicate predicate) {
		std::stable_sort(begin(), end(), predicate);
	}
	
	template<typename T, template<typename...> typename Storage>
	void ArrayList<T,Storage>::unstableSort() {
		std::sort(begin(), end());
	}
	
	template<typename T, template<typename...> typename Storage>
	template<typename Predicate>
	void ArrayList<T,Storage>::unstableSort(Predicate predicate) {
		std::sort(begin(), end(), predicate);
	}



	template<typename T, template<typename...> typename Storage>
	String ArrayList<T,Storage>::toString() const {
		if(storage.size() == 0) {
			return String::join({"ArrayList<", stringify_type<T>(), ">[]"});
		}
		return String::join({
			"ArrayList<", stringify_type<T>(), ">[\n\t",
			String::join(map<String>([](const T& item) {
				return stringify(item);
			}), ",\n\t"), "\n]" });
	}
	
	
	
	template<typename T, template<typename...> typename Storage, typename ListType, typename _>
	ArrayList<T,Storage> operator+(const ArrayList<T,Storage>& left, const ListType& right) {
		ArrayList<T,Storage> newList;
		newList.reserve(left.size() + right.size());
		for(auto& item : left) {
			newList.pushBack(item);
		}
		for(auto& item : right) {
			newList.pushBack(item);
		}
		return newList;
	}
}
