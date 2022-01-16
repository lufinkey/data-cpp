//
//  Map.hpp
//  DataCpp
//
//  Created by Luis Finke on 1/13/21.
//  Copyright © 2021 Luis Finke. All rights reserved.
//

#pragma once

#include <fgl/data/Common.hpp>
#include <fgl/data/BasicList.hpp>
#include <fgl/data/Optional.hpp>
#include <map>

namespace fgl {
	template<typename Key, typename T, typename Compare = typename std::map<Key,T>::key_compare, typename Allocator = typename std::map<Key,T,Compare>::allocator_type>
	class Map: public std::map<Key,T,Compare,Allocator> {
	public:
		using BaseType = std::map<Key,T,Compare,Allocator>;
		using typename BaseType::key_type;
		using typename BaseType::mapped_type;
		using typename BaseType::value_type;
		using typename BaseType::size_type;
		using typename BaseType::difference_type;
		using typename BaseType::key_compare;
		using typename BaseType::value_compare;
		using typename BaseType::allocator_type;
		using typename BaseType::reference;
		using typename BaseType::const_reference;
		using typename BaseType::pointer;
		using typename BaseType::const_pointer;
		using typename BaseType::iterator;
		using typename BaseType::const_iterator;
		using typename BaseType::reverse_iterator;
		using typename BaseType::const_reverse_iterator;
		using typename BaseType::node_type;
		using typename BaseType::insert_return_type;
		
		using BaseType::BaseType;
		using BaseType::operator=;
		using BaseType::operator[];
		
		using BaseType::begin;
		using BaseType::cbegin;
		using BaseType::rbegin;
		using BaseType::crbegin;
		using BaseType::end;
		using BaseType::cend;
		using BaseType::rend;
		using BaseType::crend;
		
		using BaseType::find;
		using BaseType::max_size;
		
		using BaseType::insert;
		using BaseType::insert_or_assign;
		
		Map(const BaseType&);
		Map(BaseType&&);
		
		operator BaseType&() & noexcept;
		operator BaseType&&() && noexcept;
		operator const BaseType&() const& noexcept;
		
		Map& operator=(const BaseType&);
		Map& operator=(BaseType&&) noexcept;
		
		template<typename Predicate>
		inline iterator findWhere(Predicate predicate);
		template<typename Predicate>
		inline const_iterator findWhere(Predicate predicate) const;
		template<typename Predicate>
		inline iterator findLastWhere(Predicate predicate);
		template<typename Predicate>
		inline const_iterator findLastWhere(Predicate predicate) const;
		
		template<typename Predicate>
		inline bool containsWhere(Predicate predicate) const;
		
		inline T& get(const Key& key, T& val);
		inline const T& get(const Key& key, const T& val) const;
		
		inline Optional<T> maybeAt(const Key& key) const;
		inline OptionalRef<T> maybeRefAt(const Key& key);
		inline OptionalRef<const T> maybeRefAt(const Key& key) const;
		
		inline size_type maxSize() const noexcept;
		
		template<typename M>
		inline iterator put(const Key& k, M&& obj);
		template<typename M>
		inline iterator put(Key&& k, M&& obj);
		
		template<typename Mapper>
		auto mapValues(Mapper mapper) const;
		
		template<typename Mapper>
		auto map(Mapper mapper) const;
	};



	#pragma mark Map implementation

	template<typename K,typename T,typename C,typename A>
	Map<K,T,C,A>::Map(const BaseType& map): BaseType(map) {}
	template<typename K,typename T,typename C,typename A>
	Map<K,T,C,A>::Map(BaseType&& map): BaseType(map) {}



	template<typename K,typename T,typename C,typename A>
	Map<K,T,C,A>::operator BaseType&() & noexcept {
		return *this;
	}

	template<typename K,typename T,typename C,typename A>
	Map<K,T,C,A>::operator BaseType&&() && noexcept {
		return *this;
	}

	template<typename K,typename T,typename C,typename A>
	Map<K,T,C,A>::operator const BaseType&() const& noexcept {
		return *this;
	}



	template<typename K,typename T,typename C,typename A>
	Map<K,T,C,A>& Map<K,T,C,A>::operator=(const BaseType& other) {
		BaseType::operator=(other);
		return *this;
	}

	template<typename K,typename T,typename C,typename A>
	Map<K,T,C,A>& Map<K,T,C,A>::operator=(BaseType&& other) noexcept {
		BaseType::operator=(other);
		return *this;
	}



	template<typename K,typename T,typename C,typename A>
	template<typename Predicate>
	typename Map<K,T,C,A>::iterator Map<K,T,C,A>::findWhere(Predicate predicate) {
		return std::find_if(begin(), end(), predicate);
	}

	template<typename K,typename T,typename C,typename A>
	template<typename Predicate>
	typename Map<K,T,C,A>::const_iterator Map<K,T,C,A>::findWhere(Predicate predicate) const {
		return std::find_if(begin(), end(), predicate);
	}

	template<typename K,typename T,typename C,typename A>
	template<typename Predicate>
	typename Map<K,T,C,A>::iterator Map<K,T,C,A>::findLastWhere(Predicate predicate) {
		auto rendVal = rend();
		auto it = std::find_if(rbegin(), rendVal, predicate);
		if(it == rendVal) {
			return end();
		}
		auto retIt = it.base();
		retIt--;
		return retIt;
	}

	template<typename K,typename T,typename C,typename A>
	template<typename Predicate>
	typename Map<K,T,C,A>::const_iterator Map<K,T,C,A>::findLastWhere(Predicate predicate) const {
		auto rendVal = rend();
		auto it = std::find_if(rbegin(), rendVal, predicate);
		if(it == rendVal) {
			return end();
		}
		auto retIt = it.base();
		retIt--;
		return retIt;
	}

	template<typename K,typename T,typename C,typename A>
	template<typename Predicate>
	bool Map<K,T,C,A>::containsWhere(Predicate predicate) const {
		return findWhere(predicate) != end();
	}



	template<typename K,typename T,typename C,typename A>
	T& Map<K,T,C,A>::get(const K& key, T& val) {
		auto it = find(key);
		if(it == end()) {
			return val;
		}
		return it->second;
	}

	template<typename K,typename T,typename C,typename A>
	const T& Map<K,T,C,A>::get(const K& key, const T& val) const {
		auto it = find(key);
		if(it == end()) {
			return val;
		}
		return it->second;
	}

	template<typename K,typename T,typename C,typename A>
	Optional<T> Map<K,T,C,A>::maybeAt(const K& key) const {
		auto it = find(key);
		if(it == end()) {
			return std::nullopt;
		}
		return it->second;
	}

	template<typename K,typename T,typename C,typename A>
	OptionalRef<T> Map<K,T,C,A>::maybeRefAt(const K& key) {
		auto it = find(key);
		if(it == end()) {
			return std::nullopt;
		}
		return std::ref(it->second);
	}

	template<typename K,typename T,typename C,typename A>
	OptionalRef<const T> Map<K,T,C,A>::maybeRefAt(const K& key) const {
		auto it = find(key);
		if(it == end()) {
			return std::nullopt;
		}
		return std::ref(it->second);
	}



	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::size_type Map<K,T,C,A>::maxSize() const noexcept {
		return max_size();
	}



	template<typename K,typename T,typename C,typename A>
	template<typename M>
	typename Map<K,T,C,A>::iterator Map<K,T,C,A>::put(const K& k, M&& obj) {
		return BaseType::template insert_or_assign<M>(k,std::forward<M>(obj)).first;
	}

	template<typename K,typename T,typename C,typename A>
	template<typename M>
	typename Map<K,T,C,A>::iterator Map<K,T,C,A>::put(K&& k, M&& obj) {
		return BaseType::template insert_or_assign<M>(k,std::forward<M>(obj)).first;
	}



	template<typename K,typename T,typename C,typename A>
	template<typename Mapper>
	auto Map<K,T,C,A>::mapValues(Mapper mapper) const {
		using MappedType = decltype(mapper(begin()->first, begin()->second));
		using NewMap = Map<K,MappedType>;
		NewMap newMap;
		auto it = newMap.end();
		for(auto& pair : *this) {
			it = newMap.insert(it, std::pair<K,MappedType>(pair.first, mapper(pair.first, pair.second)));
			it++;
		}
		return newMap;
	}

	template<typename K,typename T,typename C,typename A>
	template<typename Mapper>
	auto Map<K,T,C,A>::map(Mapper mapper) const {
		using PairType = decltype(mapper(*begin()));
		using NewMap = Map<typename PairType::first_type, typename PairType::second_type>;
		NewMap newMap;
		auto it = newMap.end();
		for(auto& pair : *this) {
			it = newMap.insert(it, mapper(pair));
			it++;
		}
		return newMap;
	}



	#pragma mark BasicList implementation

	template<typename BaseClass>
	template<typename Mapper>
	auto BasicList<BaseClass>::toMap(Mapper mapper) const {
		using PairType = decltype(mapper(*begin(),std::declval<size_t>()));
		using MapType = Map<typename std::tuple_element<0,PairType>::type, typename std::tuple_element<1,PairType>::type>;
		MapType map;
		size_t i=0;
		for(auto& item : *this) {
			map.insert(mapper(item,i));
			i++;
		}
		return map;
	}
}
