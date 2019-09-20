//
//  ArrayList.hpp
//  DataCpp
//
//  Created by Luis Finke on 8/30/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#include <initializer_list>
#include <vector>
#include <fgl/data/Common.hpp>
#include <fgl/data/BasicList.hpp>
#include <fgl/data/Traits.hpp>

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
		
		inline ValueType& operator[](size_type index);
		inline const ValueType& operator[](size_type) const;
		
		inline ValueType& get(size_type index);
		inline const ValueType& get(size_type) const;
		
		inline const ValueType* data() const;
		inline size_type capacity() const;
		
		inline void reserve(size_type size);
		inline void shrinkToFit();
		
		inline void resize(size_type count);
		inline void resize(size_type count, const ValueType& value);
		
		inline iterator insert(const_iterator pos, const ValueType& value);
		inline iterator insert(const_iterator pos, ValueType&& value);
		inline iterator insert(const_iterator pos, size_type count, const ValueType& value);
		template<typename InputIterator, typename = IsInputIterator<InputIterator>>
		inline iterator insert(const_iterator pos, InputIterator begin, InputIterator end);
		inline iterator insert(const_iterator pos, std::initializer_list<ValueType> list);
		inline iterator insert(const_iterator pos, const ArrayList<T,Storage>& list);
		inline iterator insert(const_iterator pos, ArrayList<T,Storage>&& list);
		
		inline iterator insert(size_type pos, const ValueType& value);
		inline iterator insert(size_type pos, ValueType&& value);
		inline iterator insert(size_type pos, size_type count, const ValueType& value);
		template<typename InputIterator, typename = IsInputIterator<InputIterator>>
		inline iterator insert(size_type pos, InputIterator begin, InputIterator end);
		inline iterator insert(size_type pos, std::initializer_list<ValueType> list);
		inline iterator insert(size_type pos, const ArrayList<T,Storage>& list);
		inline iterator insert(size_type pos, ArrayList<T,Storage>&& list);
		
		inline void pushBack(const ValueType& value);
		inline void pushBack(ValueType&& value);
		inline void pushBack(const ArrayList<T,Storage>& list);
		inline void pushBack(ArrayList<T,Storage>&& list);
		inline void popBack();
		inline ValueType extractBack();
		
		inline iterator remove(const_iterator pos);
		inline iterator remove(const_iterator begin, const_iterator end);
		inline iterator removeAt(size_type pos);
		inline iterator removeAt(size_type pos, size_type count);
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
		
		inline size_type indexOf(const ValueType& value) const;
		inline size_type lastIndexOf(const ValueType& value) const;
		inline size_type indexWhere(const Function<bool(const ValueType&)>& predicate) const;
		inline size_type lastIndexWhere(const Function<bool(const ValueType&)>& predicate) const;
		#ifdef __OBJC__
		inline size_type indexWhere(BOOL(^predicate)(const ValueType&)) const;
		inline size_type lastIndexWhere(BOOL(^predicate)(const ValueType&)) const;
		#endif
		
		template<typename NewT, template<typename...> typename NewStorage = Storage>
		inline ArrayList<NewT,NewStorage> map(const Function<NewT(T&)>&);
		template<typename NewT, template<typename...> typename NewStorage = Storage>
		inline ArrayList<NewT,NewStorage> map(const Function<NewT(const T&)>&) const;
	};
	
	
	
#pragma mark ArrayList implementation
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::ValueType& ArrayList<T,Storage>::operator[](size_type index) {
		FGL_ASSERT(index >= 0 && index < this->size(), "index out of bounds");
		return this->storage[index];
	}
	
	template<typename T, template<typename...> typename Storage>
	const typename ArrayList<T,Storage>::ValueType& ArrayList<T,Storage>::operator[](size_type index) const {
		FGL_ASSERT(index >= 0 && index < this->size(), "index out of bounds");
		return this->storage[index];
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::ValueType& ArrayList<T,Storage>::get(size_type index) {
		FGL_ASSERT(index >= 0 && index < this->size(), "index out of bounds");
		return this->storage[index];
	}
	
	template<typename T, template<typename...> typename Storage>
	const typename ArrayList<T,Storage>::ValueType& ArrayList<T,Storage>::get(size_type index) const {
		FGL_ASSERT(index >= 0 && index < this->size(), "index out of bounds");
		return this->storage[index];
	}
	
	
	
	template<typename T, template<typename...> typename Storage>
	const typename ArrayList<T,Storage>::ValueType* ArrayList<T,Storage>::data() const {
		return this->storage.data();
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::size_type ArrayList<T,Storage>::capacity() const {
		return this->storage.capacity();
	}
	
	template<typename T, template<typename...> typename Storage>
	void ArrayList<T,Storage>::reserve(size_type size) {
		this->storage.reserve(size);
	}
	
	template<typename T, template<typename...> typename Storage>
	void ArrayList<T,Storage>::shrinkToFit() {
		this->storage.shrink_to_fit();
	}
	
	template<typename T, template<typename...> typename Storage>
	void ArrayList<T,Storage>::resize(size_type count) {
		this->storage.resize(count);
	}
	
	template<typename T, template<typename...> typename Storage>
	void ArrayList<T,Storage>::resize(size_type count, const ValueType& value) {
		this->storage.resize(count, value);
	}
	
	
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::insert(const_iterator pos, const ValueType& value) {
		return this->storage.insert(pos, value);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::insert(const_iterator pos, ValueType&& value) {
		return this->storage.insert(pos, value);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::insert(const_iterator pos, size_type count, const ValueType& value) {
		return this->storage.insert(pos, count, value);
	}
	
	template<typename T, template<typename...> typename Storage>
	template<typename InputIterator, typename _>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::insert(const_iterator pos, InputIterator first, InputIterator last) {
		return this->storage.insert(pos, first, last);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::insert(const_iterator pos, std::initializer_list<ValueType> list) {
		return this->storage.insert(pos, list);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::insert(const_iterator pos, const ArrayList<T,Storage>& list) {
		return this->storage.insert(pos, list.begin(), list.end());
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::insert(const_iterator pos, ArrayList<T,Storage>&& list) {
		return this->storage.insert(pos, std::make_move_iterator(list.begin()), std::make_move_iterator(list.end()));
	}
	
	
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::insert(size_type pos, const ValueType& value) {
		return this->storage.insert(this->begin()+pos, value);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::insert(size_type pos, ValueType&& value) {
		return this->storage.insert(this->begin()+pos, value);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::insert(size_type pos, size_type count, const ValueType& value) {
		return this->storage.insert(this->begin()+pos, count, value);
	}
	
	template<typename T, template<typename...> typename Storage>
	template<typename InputIterator, typename _>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::insert(size_type pos, InputIterator first, InputIterator last) {
		return this->storage.insert(this->begin()+pos, first, last);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::insert(size_type pos, std::initializer_list<ValueType> list) {
		return this->storage.insert(this->begin()+pos, list);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::insert(size_type pos, const ArrayList<T,Storage>& list) {
		return this->storage.insert(this->begin()+pos, list.begin(), list.end());
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::insert(size_type pos, ArrayList<T,Storage>&& list) {
		return this->storage.insert(this->begin()+pos, std::make_move_iterator(list.begin()), std::make_move_iterator(list.end()));
	}
	
	
	
	template<typename T, template<typename...> typename Storage>
	void ArrayList<T,Storage>::pushBack(const ValueType& value) {
		this->storage.push_back(value);
	}
	
	template<typename T, template<typename...> typename Storage>
	void ArrayList<T,Storage>::pushBack(ValueType&& value) {
		this->storage.push_back(value);
	}
	
	template<typename T, template<typename...> typename Storage>
	void ArrayList<T,Storage>::pushBack(const ArrayList<T,Storage>& list) {
		this->storage.insert(this->end(), list);
	}
	template<typename T, template<typename...> typename Storage>
	void ArrayList<T,Storage>::pushBack(ArrayList<T,Storage>&& list) {
		this->storage.insert(this->end(), list);
	}
	
	template<typename T, template<typename...> typename Storage>
	void ArrayList<T,Storage>::popBack() {
		FGL_ASSERT(this->size() > 0, "cannot call popBack on empty array");
		this->storage.pop_back();
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::ValueType ArrayList<T,Storage>::extractBack() {
		FGL_ASSERT(this->size() > 0, "cannot call extractBack on empty array");
		auto value = std::move(this->back());
		popBack();
		return value;
	}
	
	
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::remove(const_iterator pos) {
		return this->storage.erase(pos);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::remove(const_iterator first, const_iterator last) {
		return this->storage.erase(first, last);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::removeAt(size_type pos) {
		return this->storage.erase(this->begin()+pos);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::iterator ArrayList<T,Storage>::removeAt(size_type pos, size_type count) {
		auto start = this->begin() + pos;
		return this->storage.erase(start, start+count);
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::size_type ArrayList<T,Storage>::removeEqual(const ValueType& value) {
		size_type removeCount = 0;
		size_t firstRemoveIndex = -1;
		for(size_t i=(this->size()-1); i!=-1; i--) {
			if(this->storage[i] == value) {
				removeCount++;
				if(firstRemoveIndex == -1) {
					firstRemoveIndex = i;
				}
			}
			else {
				if(firstRemoveIndex != -1) {
					this->storage.erase(this->begin()+i+1, this->begin()+firstRemoveIndex+1);
					firstRemoveIndex = -1;
				}
			}
		}
		if(firstRemoveIndex != -1) {
			this->storage.erase(this->begin(), this->begin()+firstRemoveIndex+1);
		}
		return removeCount;
	}
	
	template<typename T, template<typename...> typename Storage>
	bool ArrayList<T,Storage>::removeFirstEqual(const ValueType& value) {
		auto it = findEqual(value);
		if(it == this->end()) {
			return false;
		}
		this->storage.erase(it);
		return true;
	}
	
	template<typename T, template<typename...> typename Storage>
	bool ArrayList<T,Storage>::removeLastEqual(const ValueType& value) {
		auto it = findLastEqual(value);
		if(it == this->end()) {
			return false;
		}
		this->storage.erase(it);
		return true;
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::size_type ArrayList<T,Storage>::removeWhere(const Function<bool(const ValueType&)>& predicate) {
		size_type removeCount = 0;
		size_t firstRemoveIndex = -1;
		for(size_t i=(this->size()-1); i!=-1; i--) {
			if(predicate(this->storage[i])) {
				removeCount++;
				if(firstRemoveIndex == -1) {
					firstRemoveIndex = i;
				}
			}
			else {
				if(firstRemoveIndex != -1) {
					this->storage.erase(this->begin()+i+1, this->begin()+firstRemoveIndex+1);
					firstRemoveIndex = -1;
				}
			}
		}
		if(firstRemoveIndex != -1) {
			this->storage.erase(this->begin(), this->begin()+firstRemoveIndex+1);
		}
		return removeCount;
	}
	
	template<typename T, template<typename...> typename Storage>
	bool ArrayList<T,Storage>::removeFirstWhere(const Function<bool(const ValueType&)>& predicate) {
		auto it = findWhere(predicate);
		if(it == this->end()) {
			return false;
		}
		this->storage.erase(it);
		return true;
	}
	
	template<typename T, template<typename...> typename Storage>
	bool ArrayList<T,Storage>::removeLastWhere(const Function<bool(const ValueType&)>& predicate) {
		auto it = findLastWhere(predicate);
		if(it == this->end()) {
			return false;
		}
		this->storage.erase(it);
		return true;
	}
	
	#ifdef __OBJC__
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::size_type ArrayList<T,Storage>::removeWhere(BOOL(^predicate)(const ValueType&)) {
		size_type removeCount = 0;
		size_t firstRemoveIndex = -1;
		for(size_t i=(this->size()-1); i!=-1; i--) {
			if(predicate(this->storage[i])) {
				removeCount++;
				if(firstRemoveIndex == -1) {
					firstRemoveIndex = i;
				}
			}
			else {
				if(firstRemoveIndex != -1) {
					this->storage.erase(this->begin()+i+1, this->begin()+firstRemoveIndex+1);
					firstRemoveIndex = -1;
				}
			}
		}
		if(firstRemoveIndex != -1) {
			this->storage.erase(this->begin(), this->begin()+firstRemoveIndex+1);
		}
		return removeCount;
	}
	
	template<typename T, template<typename...> typename Storage>
	bool ArrayList<T,Storage>::removeFirstWhere(BOOL(^predicate)(const ValueType&)) {
		auto it = findWhere(predicate);
		if(it == this->end()) {
			return false;
		}
		this->storage.erase(it);
		return true;
	}
	
	template<typename T, template<typename...> typename Storage>
	bool ArrayList<T,Storage>::removeLastWhere(BOOL(^predicate)(const ValueType&)) {
		auto it = findLastWhere(predicate);
		if(it == this->end()) {
			return false;
		}
		this->storage.erase(it);
		return true;
	}
	
	#endif
	
	template<typename T, template<typename...> typename Storage>
	void ArrayList<T,Storage>::clear() {
		this->storage.clear();
	}
	
	
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::size_type ArrayList<T,Storage>::indexOf(const ValueType& value) const {
		return findEqual(value) - this->begin();
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::size_type ArrayList<T,Storage>::lastIndexOf(const ValueType& value) const {
		return findLastEqual(value) - this->begin();
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::size_type ArrayList<T,Storage>::indexWhere(const Function<bool(const ValueType&)>& predicate) const {
		return findWhere(predicate) - this->begin();
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::size_type ArrayList<T,Storage>::lastIndexWhere(const Function<bool(const ValueType&)>& predicate) const {
		return findLastWhere(predicate) - this->begin();
	}
	
	#ifdef __OBJC__
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::size_type ArrayList<T,Storage>::indexWhere(BOOL(^predicate)(const ValueType&)) const {
		return findWhere(predicate) - this->begin();
	}
	
	template<typename T, template<typename...> typename Storage>
	typename ArrayList<T,Storage>::size_type ArrayList<T,Storage>::lastIndexWhere(BOOL(^predicate)(const ValueType&)) const {
		return findLastWhere(predicate) - this->begin();
	}
	
	#endif
	
	
	
	template<typename T, template<typename...> typename Storage>
	template<typename NewT, template<typename...> typename NewStorage>
	ArrayList<NewT,NewStorage> ArrayList<T,Storage>::map(const Function<NewT(T&)>& transform) {
		ArrayList<NewT,NewStorage> newArray;
		newArray.reserve(this->size());
		for(auto& item : this->storage) {
			newArray.pushBack(transform(item));
		}
		return newArray;
	}
	
	template<typename T, template<typename...> typename Storage>
	template<typename NewT, template<typename...> typename NewStorage>
	ArrayList<NewT,NewStorage> ArrayList<T,Storage>::map(const Function<NewT(const T&)>& transform) const {
		ArrayList<NewT,NewStorage> newArray;
		newArray.reserve(this->size());
		for(auto& item : this->storage) {
			newArray.pushBack(transform(item));
		}
		return newArray;
	}
}
