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
	template<typename T>
	class ArrayList: public BasicList<std::vector<T>> {
	public:
		using BaseType = std::vector<T>;
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
		
		using BaseType::at;
		using BaseType::capacity;
		using BaseType::reserve;
		using BaseType::resize;
		using BaseType::shrink_to_fit;
		using BaseType::insert;
		using BaseType::push_back;
		using BaseType::pop_back;
		using BaseType::erase;
		using BaseType::clear;
		
		ArrayList(const BaseType&);
		ArrayList(BaseType&&);
		template<typename Collection, typename Transform, typename = IsCollection<std::remove_reference_t<Collection>>>
		ArrayList(Collection&& collection, Transform transform);
		
		#ifdef __OBJC__
		template<typename Transform>
		ArrayList(NSArray* objcArray, Transform transform);
		#endif

		#ifdef JNIEXPORT
		template<typename Transform>
		ArrayList(JNIEnv* env, jobjectArray javaArray, Transform transform);
		#endif
		
		ArrayList& operator=(const BaseType&);
		ArrayList& operator=(BaseType&&);
		
		constexpr reference operator[](size_type index) noexcept;
		constexpr const_reference operator[](size_type index) const noexcept;
		
		inline Optional<T> maybeAt(size_type index) const;
		inline OptionalRef<T> maybeRefAt(size_type index);
		inline OptionalRef<const T> maybeRefAt(size_type index) const;
		
		inline void shrinkToFit();
		
		inline void pushBack(const T& value);
		inline void pushBack(T&& value);
		inline void pushBackList(const ArrayList& list);
		inline void pushBackList(ArrayList&& list);
		template<typename Collection, typename = IsCollectionOf<T,std::remove_reference_t<Collection>>>
		inline void pushBackList(Collection&& list);
		inline void popBack();
		inline T extractBack();
		
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
		
		inline size_type indexOf(const T& value) const;
		inline size_type lastIndexOf(const T& value) const;
		template<typename Predicate>
		inline size_type indexWhere(Predicate predicate) const;
		template<typename Predicate>
		inline size_type lastIndexWhere(Predicate predicate) const;

		template<typename Predicate>
		inline ArrayList where(Predicate predicate) const;
		
		template<typename Transform>
		inline auto map(Transform transform);
		template<typename Transform>
		inline auto map(Transform transform) const;
		
		inline void sort();
		template<typename Predicate>
		inline void sort(Predicate predicate);
		inline void unstableSort();
		template<typename Predicate>
		inline void unstableSort(Predicate predicate);
		
		String toString() const;
	};

	template<typename T, typename Collection, typename = IsCollection<std::remove_reference_t<Collection>>>
	ArrayList<T> operator+(const ArrayList<T>& left, Collection&& right);
	template<typename T, typename Collection, typename = IsCollection<std::remove_reference_t<Collection>>>
	ArrayList<T> operator+(ArrayList<T>&& left, Collection&& right);
	template<typename T>
	ArrayList<T> operator+(ArrayList<T>&& left, std::vector<T>&& right);
	template<typename T>
	ArrayList<T> operator+(const ArrayList<T>& left, std::vector<T>&& right);
	template<typename T>
	ArrayList<T> operator+(ArrayList<T>&& left, const std::vector<T>& right);
	
	
	
#pragma mark ArrayList implementation

	template<typename T>
	ArrayList<T>::ArrayList(const BaseType& list): BasicList<BaseType>(list) {
		//
	}

	template<typename T>
	ArrayList<T>::ArrayList(BaseType&& list): BasicList<BaseType>(list) {
		//
	}

	template<typename T>
	template<typename Collection, typename Transform, typename _>
	ArrayList<T>::ArrayList(Collection&& collection, Transform transform) {
		reserve(collection.size());
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
	ArrayList<T>::ArrayList(NSArray* objcArray, Transform transform) {
		reserve((size_type)objcArray.count);
		for(NSObject* obj in objcArray) {
			pushBack(transform(obj));
		}
	}

	#endif

	#ifdef JNIEXPORT

	template<typename T>
	template<typename Transform>
	ArrayList<T>::ArrayList(JNIEnv* env, jobjectArray javaArray, Transform transform) {
		jsize javaArraySize = env->GetArrayLength(javaArray);
		reserve((size_type)javaArraySize);
		for(jsize i=0; i<javaArraySize; i++) {
			pushBack(transform(env, env->GetObjectArrayElement(javaArray, i)));
		}
	}

	#endif



	template<typename T>
	ArrayList<T>& ArrayList<T>::operator=(const BaseType& list) {
		BaseType::operator=(list);
		return *this;
	}

	template<typename T>
	ArrayList<T>& ArrayList<T>::operator=(BaseType&& list) {
		BaseType::operator=(list);
		return *this;
	}


	
	template<typename T>
	constexpr typename ArrayList<T>::reference ArrayList<T>::operator[](size_type index) noexcept {
		FGL_ASSERT(index >= 0 && index < size(), "index out of bounds");
		return BaseType::operator[](index);
	}
	
	template<typename T>
	constexpr typename ArrayList<T>::const_reference ArrayList<T>::operator[](size_type index) const noexcept {
		FGL_ASSERT(index >= 0 && index < size(), "index out of bounds");
		return BaseType::operator[](index);
	}



	template<typename T>
	Optional<T> ArrayList<T>::maybeAt(size_type index) const {
		if(index >= size()) {
			return std::nullopt;
		}
		return BaseType::operator[](index);
	}
	
	template<typename T>
	OptionalRef<T> ArrayList<T>::maybeRefAt(size_type index) {
		if(index >= size()) {
			return std::nullopt;
		}
		return std::ref<T>(BaseType::operator[](index));
	}
	
	template<typename T>
	OptionalRef<const T> ArrayList<T>::maybeRefAt(size_type index) const {
		if(index >= size()) {
			return std::nullopt;
		}
		return std::ref<const T>(BaseType::operator[](index));
	}
	
	
	
	template<typename T>
	void ArrayList<T>::shrinkToFit() {
		shrink_to_fit();
	}
	
	
	
	template<typename T>
	void ArrayList<T>::pushBack(const T& value) {
		push_back(value);
	}
	
	template<typename T>
	void ArrayList<T>::pushBack(T&& value) {
		push_back(value);
	}
	
	template<typename T>
	void ArrayList<T>::pushBackList(const ArrayList<T>& list) {
		insert(end(), list.begin(), list.end());
	}
	template<typename T>
	void ArrayList<T>::pushBackList(ArrayList<T>&& list) {
		insert(end(), std::make_move_iterator(list.begin()), std::make_move_iterator(list.end()));
	}
	
	template<typename T>
	void ArrayList<T>::popBack() {
		FGL_ASSERT(size() > 0, "cannot call popBack on empty array");
		pop_back();
	}
	
	template<typename T>
	T ArrayList<T>::extractBack() {
		FGL_ASSERT(size() > 0, "cannot call extractBack on empty array");
		auto value = std::move(back());
		pop_back();
		return value;
	}
	
	
	
	template<typename T>
	typename ArrayList<T>::iterator ArrayList<T>::removeAt(size_type pos) {
		return erase(begin()+pos);
	}
	
	template<typename T>
	typename ArrayList<T>::iterator ArrayList<T>::removeAt(size_type pos, size_type count) {
		auto start = begin() + pos;
		return erase(start, start+count);
	}
	
	template<typename T>
	typename ArrayList<T>::size_type ArrayList<T>::removeEqual(const T& value) {
		size_type removeCount = 0;
		size_t firstRemoveIndex = -1;
		for(size_t i=(size()-1); i!=-1; i--) {
			if(BaseType::operator[](i) == value) {
				removeCount++;
				if(firstRemoveIndex == -1) {
					firstRemoveIndex = i;
				}
			}
			else {
				if(firstRemoveIndex != -1) {
					erase(begin()+i+1, begin()+firstRemoveIndex+1);
					firstRemoveIndex = -1;
				}
			}
		}
		if(firstRemoveIndex != -1) {
			erase(begin(), begin()+firstRemoveIndex+1);
		}
		return removeCount;
	}
	
	template<typename T>
	bool ArrayList<T>::removeFirstEqual(const T& value) {
		auto it = findEqual(value);
		if(it == end()) {
			return false;
		}
		erase(it);
		return true;
	}
	
	template<typename T>
	bool ArrayList<T>::removeLastEqual(const T& value) {
		auto it = findLastEqual(value);
		if(it == end()) {
			return false;
		}
		erase(it);
		return true;
	}
	
	template<typename T>
	template<typename Predicate>
	typename ArrayList<T>::size_type ArrayList<T>::removeWhere(Predicate predicate) {
		size_type removeCount = 0;
		size_t firstRemoveIndex = -1;
		for(size_t i=(size()-1); i!=-1; i--) {
			if(predicate(BaseType::operator[](i))) {
				removeCount++;
				if(firstRemoveIndex == -1) {
					firstRemoveIndex = i;
				}
			}
			else {
				if(firstRemoveIndex != -1) {
					erase(begin()+i+1, begin()+firstRemoveIndex+1);
					firstRemoveIndex = -1;
				}
			}
		}
		if(firstRemoveIndex != -1) {
			erase(begin(), begin()+firstRemoveIndex+1);
		}
		return removeCount;
	}
	
	template<typename T>
	template<typename Predicate>
	bool ArrayList<T>::removeFirstWhere(Predicate predicate) {
		auto it = findWhere(predicate);
		if(it == end()) {
			return false;
		}
		erase(it);
		return true;
	}
	
	template<typename T>
	template<typename Predicate>
	bool ArrayList<T>::removeLastWhere(Predicate predicate) {
		auto it = findLastWhere(predicate);
		if(it == end()) {
			return false;
		}
		erase(it);
		return true;
	}
	
	
	
	template<typename T>
	typename ArrayList<T>::size_type ArrayList<T>::indexOf(const T& value) const {
		return findEqual(value) - begin();
	}
	
	template<typename T>
	typename ArrayList<T>::size_type ArrayList<T>::lastIndexOf(const T& value) const {
		return findLastEqual(value) - begin();
	}
	
	template<typename T>
	template<typename Predicate>
	typename ArrayList<T>::size_type ArrayList<T>::indexWhere(Predicate predicate) const {
		return findWhere(predicate) - begin();
	}
	
	template<typename T>
	template<typename Predicate>
	typename ArrayList<T>::size_type ArrayList<T>::lastIndexWhere(Predicate predicate) const {
		return findLastWhere(predicate) - begin();
	}



	template<typename T>
	template<typename Predicate>
	ArrayList<T> ArrayList<T>::where(Predicate predicate) const {
		std::list<T> newList;
		for(const_reference item : *this) {
			if(predicate(item)) {
				newList.push_back(item);
			}
		}
		return newList;
	}
	
	
	
	template<typename T>
	template<typename Transform>
	auto ArrayList<T>::map(Transform transform) {
		using ReturnType = decltype(transform(front()));
		ArrayList<ReturnType> newArray;
		newArray.reserve(size());
		for(reference item : *this) {
			newArray.pushBack(transform(item));
		}
		return newArray;
	}
	
	template<typename T>
	template<typename Transform>
	auto ArrayList<T>::map(Transform transform) const {
		using ReturnType = decltype(transform(front()));
		ArrayList<ReturnType> newArray;
		newArray.reserve(size());
		for(const_reference item : *this) {
			newArray.pushBack(transform(item));
		}
		return newArray;
	}



	template<typename T>
	void ArrayList<T>::sort() {
		std::stable_sort(begin(), end());
	}
	
	template<typename T>
	template<typename Predicate>
	void ArrayList<T>::sort(Predicate predicate) {
		std::stable_sort(begin(), end(), predicate);
	}
	
	template<typename T>
	void ArrayList<T>::unstableSort() {
		std::sort(begin(), end());
	}
	
	template<typename T>
	template<typename Predicate>
	void ArrayList<T>::unstableSort(Predicate predicate) {
		std::sort(begin(), end(), predicate);
	}



	template<typename T>
	String ArrayList<T>::toString() const {
		if(size() == 0) {
			return String::join({"ArrayList<", stringify_type<T>(), ">[]"});
		}
		return String::join({
			"ArrayList<", stringify_type<T>(), ">[\n\t",
			String::join(map([](const_reference item) -> String {
				return stringify<T>(item);
			}), ",\n\t"), "\n]" });
	}
	
	
	
	template<typename T, typename Collection, typename _>
	ArrayList<T> operator+(const ArrayList<T>& left, Collection&& right) {
		ArrayList<T> newList;
		newList.reserve(left.size() + right.size());
		newList.pushBackList(left);
		newList.pushBackList(std::forward<Collection>(right));
		return newList;
	}

	template<typename T, typename Collection, typename _>
	ArrayList<T> operator+(ArrayList<T>&& left, Collection&& right) {
		auto newList = std::move(left);
		newList.pushBackList(std::forward<Collection>(right));
		return newList;
	}

	template<typename T>
	ArrayList<T> operator+(ArrayList<T>&& left, std::vector<T>&& right) {
		auto newList = std::move(left);
		newList.pushBackList(right);
		return newList;
	}

	template<typename T>
	ArrayList<T> operator+(const ArrayList<T>& left, std::vector<T>&& right) {
		ArrayList<T> newList;
		newList.reserve(left.size() + right.size());
		newList.pushBackList(left);
		newList.pushBackList(right);
		return newList;
	}

	template<typename T>
	ArrayList<T> operator+(ArrayList<T>&& left, const std::vector<T>& right) {
		auto newList = std::move(left);
		newList.pushBackList(right);
		return newList;
	}
}
