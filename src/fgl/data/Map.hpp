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
	template<typename Key, typename T, typename Compare = std::less<Key>, typename Allocator = std::allocator<std::pair<const Key,T>>>
	class Map {
	public:
		using StorageType = std::map<Key,T,Compare,Allocator>;
		
		using key_type = typename StorageType::key_type;
		using mapped_type = typename StorageType::mapped_type;
		using value_type = typename StorageType::value_type;
		using size_type = typename StorageType::size_type;
		using difference_type = typename StorageType::difference_type;
		using key_compare = typename StorageType::key_compare;
		using value_compare = typename StorageType::value_compare;
		using allocator_type = typename StorageType::allocator_type;
		using reference = typename StorageType::reference;
		using const_reference = typename StorageType::const_reference;
		using pointer = typename StorageType::pointer;
		using const_pointer = typename StorageType::const_pointer;
		using iterator = typename StorageType::iterator;
		using const_iterator = typename StorageType::const_iterator;
		using reverse_iterator = typename StorageType::reverse_iterator;
		using const_reverse_iterator = typename StorageType::const_reverse_iterator;
		using node_type = typename StorageType::node_type;
		using insert_return_type = typename StorageType::insert_return_type;
		
		StorageType storage;
		
		inline Map();
		inline explicit Map(const Compare& comp, const Allocator& alloc = Allocator());
		inline explicit Map(const Allocator& alloc);
		
		template<typename InputIt>
		inline Map(InputIt first, InputIt last, const Compare& comp = Compare(), const Allocator& alloc = Allocator());
		template<typename InputIt>
		inline Map(InputIt first, InputIt last, const Allocator& alloc);
		
		inline Map(const Map& other);
		inline Map(const Map& other, const Allocator& alloc);
		inline Map(Map&& other);
		inline Map(Map&& other, const Allocator& alloc);
		
		inline Map(const StorageType& other);
		inline Map(const StorageType& other, const Allocator& alloc);
		inline Map(StorageType&& other);
		inline Map(StorageType&& other, const Allocator& alloc);
		
		inline Map(std::initializer_list<value_type> ilist, const Compare& comp = Compare(), const Allocator& alloc = Allocator());
		inline Map(std::initializer_list<value_type> ilist, const Allocator& alloc);
		
		inline operator StorageType&() & noexcept;
		inline operator StorageType&&() && noexcept;
		inline operator const StorageType&() const& noexcept;
		
		inline Map& operator=(const Map& other);
		inline Map& operator=(Map&& other) noexcept;
		inline Map& operator=(std::initializer_list<value_type> ilist);
		
		inline allocator_type get_allocator() const noexcept;
		inline allocator_type getAllocator() const noexcept;
		
		inline T& at(const key_type& key);
		inline const T& at(const key_type& key) const;
		inline Optional<T> maybeAt(const key_type& key) const;
		inline OptionalRef<T> maybeRefAt(const key_type& key);
		inline OptionalRef<const T> maybeRefAt(const key_type& key) const;
		
		inline T& operator[](const key_type& key);
		inline T& operator[](key_type&& key);
		
		inline iterator begin() noexcept;
		inline const_iterator begin() const noexcept;
		inline const_iterator cbegin() const noexcept;
		
		inline iterator end() noexcept;
		inline const_iterator end() const noexcept;
		inline const_iterator cend() const noexcept;
		
		inline reverse_iterator rbegin() noexcept;
		inline const_reverse_iterator rbegin() const noexcept;
		inline const_reverse_iterator crbegin() const noexcept;
		
		inline reverse_iterator rend() noexcept;
		inline const_reverse_iterator rend() const noexcept;
		inline const_reverse_iterator crend() const noexcept;
		
		inline bool empty() const noexcept;
		inline size_type size() const noexcept;
		inline size_type max_size() const noexcept;
		inline size_type maxSize() const noexcept;
		
		inline void clear() noexcept;
		
		template<typename M>
		inline std::pair<iterator,bool> put(const key_type& k, M&& obj);
		template<typename M>
		inline std::pair<iterator,bool> put(key_type&& k, M&& obj);
		
		inline std::pair<iterator,bool> insert(const value_type& value);
		template<typename P>
		inline std::pair<iterator,bool> insert(P&& value);
		inline std::pair<iterator,bool> insert(value_type&& value);
		inline iterator insert(const_iterator hint, const value_type& value);
		inline iterator insert(const_iterator hint, value_type&& value);
		template<typename InputIt>
		inline void insert(InputIt first, InputIt last);
		inline void insert(std::initializer_list<value_type> ilist);
		inline insert_return_type insert(node_type&& nh);
		inline iterator insert(const_iterator hint, node_type&& nh);
		
		template<typename M>
		inline std::pair<iterator,bool> insert_or_assign(const key_type& k, M&& obj);
		template<typename M>
		inline std::pair<iterator,bool> insert_or_assign(key_type&& k, M&& obj);
		template<typename M>
		inline iterator insert_or_assign(const_iterator hint, const key_type& k, M&& obj);
		template<typename M>
		inline iterator insert_or_assign(const_iterator hint, key_type&& k, M&& obj);
		
		template<typename... Args>
		inline std::pair<iterator,bool> emplace(Args&&... args);
		template<typename... Args>
		inline iterator emplace_hint(const_iterator hint, Args&&... args);
		
		template<typename... Args>
		inline std::pair<iterator,bool> try_emplace(const key_type& k, Args&&... args);
		template<typename... Args>
		inline std::pair<iterator,bool> try_emplace(key_type&& k, Args&&... args);
		template<typename... Args>
		inline iterator try_emplace(const_iterator hint, const key_type& k, Args&&... args);
		template<typename... Args>
		inline iterator try_emplace(const_iterator hint, key_type&& k, Args&&... args);
		
		inline iterator erase(const_iterator pos);
		inline iterator erase(iterator pos);
		inline iterator erase(const_iterator first, const_iterator last);
		inline size_type erase(const key_type& key);
		
		inline void swap(Map& other) noexcept;
		inline void swap(StorageType& other) noexcept;
		
		inline node_type extract(const_iterator position);
		inline node_type extract(const key_type& k);
		
		template<typename C2>
		inline void merge(Map<Key,T,C2,Allocator>& source);
		template<typename C2>
		inline void merge(Map<Key,T,C2,Allocator>&& source);
		template<typename C2>
		inline void merge(std::map<Key,T,C2,Allocator>& source);
		template<typename C2>
		inline void merge(std::map<Key,T,C2,Allocator>&& source);
		template<typename C2>
		inline void merge(std::multimap<Key,T,C2,Allocator>& source);
		template<typename C2>
		inline void merge(std::multimap<Key,T,C2,Allocator>&& source);
		
		inline size_type count(const key_type& key) const;
		template<typename K>
		inline size_type count(const K& x) const;
		
		inline iterator find(const key_type& key);
		inline const_iterator find(const key_type& key) const;
		template<typename K2>
		inline iterator find(const K2& key);
		template<typename K2>
		inline const_iterator find(const K2& key) const;
		
		inline bool contains(const key_type& key) const;
		template<typename K2>
		inline bool contains(const K2& key) const;
		
		inline std::pair<iterator,iterator> equal_range(const key_type& key);
		inline std::pair<const_iterator,const_iterator> equal_range(const key_type& key) const;
		template<typename K2>
		inline std::pair<iterator,iterator> equal_range(const K2& key);
		template<typename K2>
		inline std::pair<const_iterator,const_iterator> equal_range(const K2& key) const;
		
		inline iterator lower_bound(const key_type& key);
		inline const_iterator lower_bound(const key_type& key) const;
		template<typename K2>
		inline iterator lower_bound(const K2& key);
		template<typename K2>
		inline const_iterator lower_bound(const K2& key) const;
		
		inline iterator upper_bound(const key_type& key);
		inline const_iterator upper_bound(const key_type& key) const;
		template<typename K2>
		inline iterator upper_bound(const K2& key);
		template<typename K2>
		inline const_iterator upper_bound(const K2& key) const;
		
		inline key_compare key_comp() const;
		inline value_compare value_comp() const;
		
		template<typename Mapper>
		auto mapValues(Mapper mapper) const;
		
		template<typename Mapper>
		auto map(Mapper mapper) const;
	};



	#pragma mark Map implementation

	template<typename K,typename T,typename C,typename A>
	Map<K,T,C,A>::Map() {}

	template<typename K,typename T,typename C,typename A>
	Map<K,T,C,A>::Map(const C& comp, const A& alloc): storage(comp,alloc) {}

	template<typename K,typename T,typename C,typename A>
	Map<K,T,C,A>::Map(const A& alloc): storage(alloc) {}


	template<typename K,typename T,typename C,typename A>
	template<typename InputIt>
	Map<K,T,C,A>::Map(InputIt first, InputIt last, const C& comp, const A& alloc)
	: storage(first,last,comp,alloc) {}

	template<typename K,typename T,typename C,typename A>
	template<typename InputIt>
	Map<K,T,C,A>::Map(InputIt first, InputIt last, const A& alloc)
	: storage(first,last,alloc) {}

	template<typename K,typename T,typename C,typename A>
	Map<K,T,C,A>::Map(const Map& other): storage(other.storage) {}

	template<typename K,typename T,typename C,typename A>
	Map<K,T,C,A>::Map(const Map& other, const A& alloc): storage(other.storage,alloc) {}

	template<typename K,typename T,typename C,typename A>
	Map<K,T,C,A>::Map(Map&& other): storage(other.storage) {}

	template<typename K,typename T,typename C,typename A>
	Map<K,T,C,A>::Map(Map&& other, const A& alloc): storage(other.storage,alloc) {}


	template<typename K,typename T,typename C,typename A>
	Map<K,T,C,A>::Map(const StorageType& other): storage(other) {}

	template<typename K,typename T,typename C,typename A>
	Map<K,T,C,A>::Map(const StorageType& other, const A& alloc): storage(other,alloc) {}

	template<typename K,typename T,typename C,typename A>
	Map<K,T,C,A>::Map(StorageType&& other): storage(other) {}

	template<typename K,typename T,typename C,typename A>
	Map<K,T,C,A>::Map(StorageType&& other, const A& alloc): storage(other,alloc) {}


	template<typename K,typename T,typename C,typename A>
	Map<K,T,C,A>::Map(std::initializer_list<value_type> ilist, const C& comp, const A& alloc)
	: storage(ilist,comp,alloc) {}

	template<typename K,typename T,typename C,typename A>
	Map<K,T,C,A>::Map(std::initializer_list<value_type> ilist, const A& alloc)
	: storage(ilist,alloc) {}



	template<typename K,typename T,typename C,typename A>
	Map<K,T,C,A>::operator StorageType&() & noexcept {
		return storage;
	}

	template<typename K,typename T,typename C,typename A>
	Map<K,T,C,A>::operator StorageType&&() && noexcept {
		return storage;
	}

	template<typename K,typename T,typename C,typename A>
	Map<K,T,C,A>::operator const StorageType&() const& noexcept {
		return storage;
	}



	template<typename K,typename T,typename C,typename A>
	Map<K,T,C,A>& Map<K,T,C,A>::operator=(const Map& other) {
		storage.operator=(other.storage);
		return *this;
	}

	template<typename K,typename T,typename C,typename A>
	Map<K,T,C,A>& Map<K,T,C,A>::operator=(Map&& other) noexcept {
		storage.operator=(other.storage);
		return *this;
	}

	template<typename K,typename T,typename C,typename A>
	Map<K,T,C,A>& Map<K,T,C,A>::operator=(std::initializer_list<value_type> ilist) {
		storage.operator=(ilist);
		return *this;
	}



	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::allocator_type Map<K,T,C,A>::get_allocator() const noexcept {
		return storage.get_allocator();
	}

	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::allocator_type Map<K,T,C,A>::getAllocator() const noexcept {
		return storage.get_allocator();
	}



	template<typename K,typename T,typename C,typename A>
	T& Map<K,T,C,A>::at(const key_type& key) {
		return storage.at(key);
	}

	template<typename K,typename T,typename C,typename A>
	const T& Map<K,T,C,A>::at(const key_type& key) const {
		return storage.at(key);
	}

	template<typename K,typename T,typename C,typename A>
	Optional<T> Map<K,T,C,A>::maybeAt(const key_type& key) const {
		auto it = storage.find(key);
		if(it == storage.end()) {
			return std::nullopt;
		}
		return *it;
	}

	template<typename K,typename T,typename C,typename A>
	OptionalRef<T> Map<K,T,C,A>::maybeRefAt(const key_type& key) {
		auto it = storage.find(key);
		if(it == storage.end()) {
			return std::nullopt;
		}
		return std::ref(*it);
	}

	template<typename K,typename T,typename C,typename A>
	OptionalRef<const T> Map<K,T,C,A>::maybeRefAt(const key_type& key) const {
		auto it = storage.find(key);
		if(it == storage.end()) {
			return std::nullopt;
		}
		return std::ref(*it);
	}



	template<typename K,typename T,typename C,typename A>
	T& Map<K,T,C,A>::operator[](const key_type& key) {
		return storage.operator[](key);
	}
	template<typename K,typename T,typename C,typename A>
	T& Map<K,T,C,A>::operator[](key_type&& key) {
		return storage.operator[](key);
	}



	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::iterator Map<K,T,C,A>::begin() noexcept {
		return storage.begin();
	}

	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::const_iterator Map<K,T,C,A>::begin() const noexcept {
		return storage.begin();
	}

	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::const_iterator Map<K,T,C,A>::cbegin() const noexcept {
		return storage.cbegin();
	}



	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::iterator Map<K,T,C,A>::end() noexcept {
		return storage.end();
	}

	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::const_iterator Map<K,T,C,A>::end() const noexcept {
		return storage.end();
	}

	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::const_iterator Map<K,T,C,A>::cend() const noexcept {
		return storage.cend();
	}



	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::reverse_iterator Map<K,T,C,A>::rbegin() noexcept {
		return storage.rbegin();
	}

	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::const_reverse_iterator Map<K,T,C,A>::rbegin() const noexcept {
		return storage.rbegin();
	}

	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::const_reverse_iterator Map<K,T,C,A>::crbegin() const noexcept {
		return storage.crbegin();
	}



	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::reverse_iterator Map<K,T,C,A>::rend() noexcept {
		return storage.rend();
	}

	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::const_reverse_iterator Map<K,T,C,A>::rend() const noexcept {
		return storage.rend();
	}

	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::const_reverse_iterator Map<K,T,C,A>::crend() const noexcept {
		return storage.crend();
	}



	template<typename K,typename T,typename C,typename A>
	bool Map<K,T,C,A>::empty() const noexcept {
		return storage.empty();
	}

	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::size_type Map<K,T,C,A>::size() const noexcept {
		return storage.size();
	}

	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::size_type Map<K,T,C,A>::max_size() const noexcept {
		return storage.max_size();
	}

	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::size_type Map<K,T,C,A>::maxSize() const noexcept {
		return storage.maxSize();
	}


	template<typename K,typename T,typename C,typename A>
	void Map<K,T,C,A>::clear() noexcept {
		storage.clear();
	}



	template<typename K,typename T,typename C,typename A>
	template<typename M>
	std::pair<typename Map<K,T,C,A>::iterator,bool> Map<K,T,C,A>::put(const key_type& k, M&& obj) {
		return storage.template insert_or_assign<M>(k,std::forward<M>(obj));
	}

	template<typename K,typename T,typename C,typename A>
	template<typename M>
	std::pair<typename Map<K,T,C,A>::iterator,bool> Map<K,T,C,A>::put(key_type&& k, M&& obj) {
		return storage.template insert_or_assign<M>(k,std::forward<M>(obj));
	}



	template<typename K,typename T,typename C,typename A>
	std::pair<typename Map<K,T,C,A>::iterator,bool> Map<K,T,C,A>::insert(const value_type& value) {
		return storage.insert(value);
	}

	template<typename K,typename T,typename C,typename A>
	template<typename P>
	std::pair<typename Map<K,T,C,A>::iterator,bool> Map<K,T,C,A>::insert(P&& value) {
		return storage.template insert<P>(std::forward<P>(value));
	}

	template<typename K,typename T,typename C,typename A>
	std::pair<typename Map<K,T,C,A>::iterator,bool> Map<K,T,C,A>::insert(value_type&& value) {
		return storage.insert(value);
	}

	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::iterator Map<K,T,C,A>::insert(const_iterator hint, const value_type& value) {
		return storage.insert(hint,value);
	}

	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::iterator Map<K,T,C,A>::insert(const_iterator hint, value_type&& value) {
		return storage.insert(hint,value);
	}

	template<typename K,typename T,typename C,typename A>
	template<typename InputIt>
	void Map<K,T,C,A>::insert(InputIt first, InputIt last) {
		return storage.template insert<InputIt>(first,last);
	}

	template<typename K,typename T,typename C,typename A>
	void Map<K,T,C,A>::insert(std::initializer_list<value_type> ilist) {
		return storage.insert(ilist);
	}

	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::insert_return_type Map<K,T,C,A>::insert(node_type&& nh) {
		return storage.insert(std::forward<node_type>(nh));
	}

	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::iterator Map<K,T,C,A>::insert(const_iterator hint, node_type&& nh) {
		return storage.insert(hint,std::forward<node_type>(nh));
	}



	template<typename K,typename T,typename C,typename A>
	template<typename M>
	std::pair<typename Map<K,T,C,A>::iterator,bool> Map<K,T,C,A>::insert_or_assign(const key_type& k, M&& obj) {
		return storage.template insert_or_assign<M>(k,std::forward<M>(obj));
	}

	template<typename K,typename T,typename C,typename A>
	template<typename M>
	std::pair<typename Map<K,T,C,A>::iterator,bool> Map<K,T,C,A>::insert_or_assign(key_type&& k, M&& obj) {
		return storage.template insert_or_assign<M>(k,std::forward<M>(obj));
	}

	template<typename K,typename T,typename C,typename A>
	template<typename M>
	typename Map<K,T,C,A>::iterator Map<K,T,C,A>::insert_or_assign(const_iterator hint, const key_type& k, M&& obj) {
		return storage.template insert_or_assign<M>(hint,k,std::forward<M>(obj));
	}

	template<typename K,typename T,typename C,typename A>
	template<typename M>
	typename Map<K,T,C,A>::iterator Map<K,T,C,A>::insert_or_assign(const_iterator hint, key_type&& k, M&& obj) {
		return storage.template insert_or_assign<M>(hint,k,std::forward<M>(obj));
	}


	template<typename K,typename T,typename C,typename A>
	template<typename... Args>
	std::pair<typename Map<K,T,C,A>::iterator,bool> Map<K,T,C,A>::emplace(Args&&... args) {
		return storage.template emplace<Args...>(std::forward<Args>(args)...);
	}

	template<typename K,typename T,typename C,typename A>
	template<typename... Args>
	typename Map<K,T,C,A>::iterator Map<K,T,C,A>::emplace_hint(const_iterator hint, Args&&... args) {
		return storage.template emplace_hint<Args...>(hint,std::forward<Args>(args)...);
	}

	template<typename K,typename T,typename C,typename A>
	template<typename... Args>
	std::pair<typename Map<K,T,C,A>::iterator,bool> Map<K,T,C,A>::try_emplace(const key_type& k, Args&&... args) {
		return storage.template try_emplace<Args...>(k,std::forward<Args>(args)...);
	}

	template<typename K,typename T,typename C,typename A>
	template<typename... Args>
	std::pair<typename Map<K,T,C,A>::iterator,bool> Map<K,T,C,A>::try_emplace(key_type&& k, Args&&... args) {
		return storage.template try_emplace<Args...>(k,std::forward<Args>(args)...);
	}

	template<typename K,typename T,typename C,typename A>
	template<typename... Args>
	typename Map<K,T,C,A>::iterator Map<K,T,C,A>::try_emplace(const_iterator hint, const key_type& k, Args&&... args) {
		return storage.template try_emplace<Args...>(hint,k,std::forward<Args>(args)...);
	}

	template<typename K,typename T,typename C,typename A>
	template<typename... Args>
	typename Map<K,T,C,A>::iterator Map<K,T,C,A>::try_emplace(const_iterator hint, key_type&& k, Args&&... args) {
		return storage.template try_emplace<Args...>(hint,k,std::forward<Args>(args)...);
	}



	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::iterator Map<K,T,C,A>::erase(const_iterator pos) {
		return storage.erase(pos);
	}

	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::iterator Map<K,T,C,A>::erase(iterator pos) {
		return storage.erase(pos);
	}

	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::iterator Map<K,T,C,A>::erase(const_iterator first, const_iterator last) {
		return storage.erase(first,last);
	}

	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::size_type Map<K,T,C,A>::erase(const key_type& key) {
		return storage.erase(key);
	}



	template<typename K,typename T,typename C,typename A>
	void Map<K,T,C,A>::swap(Map& other) noexcept {
		storage.swap(other.storage);
	}

	template<typename K,typename T,typename C,typename A>
	void Map<K,T,C,A>::swap(StorageType& other) noexcept {
		return storage.swap(other);
	}



	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::node_type Map<K,T,C,A>::extract(const_iterator position) {
		return storage.extract(position);
	}

	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::node_type Map<K,T,C,A>::extract(const key_type& k) {
		return storage.extract(k);
	}



	template<typename K,typename T,typename C,typename A>
	template<typename C2>
	void Map<K,T,C,A>::merge(Map<K,T,C2,A>& source) {
		storage.template merge<C2>(source.storage);
	}

	template<typename K,typename T,typename C,typename A>
	template<typename C2>
	void Map<K,T,C,A>::merge(Map<K,T,C2,A>&& source) {
		storage.template merge<C2>(source.storage);
	}

	template<typename K,typename T,typename C,typename A>
	template<typename C2>
	void Map<K,T,C,A>::merge(std::map<K,T,C2,A>& source) {
		storage.template merge<C2>(source);
	}

	template<typename K,typename T,typename C,typename A>
	template<typename C2>
	void Map<K,T,C,A>::merge(std::map<K,T,C2,A>&& source) {
		storage.template merge<C2>(source);
	}

	template<typename K,typename T,typename C,typename A>
	template<typename C2>
	void Map<K,T,C,A>::merge(std::multimap<K,T,C2,A>& source) {
		return storage.template merge<C2>(source);
	}

	template<typename K,typename T,typename C,typename A>
	template<typename C2>
	void Map<K,T,C,A>::merge(std::multimap<K,T,C2,A>&& source) {
		return storage.template merge<C2>(source);
	}


	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::size_type Map<K,T,C,A>::count(const key_type& key) const {
		return storage.count(key);
	}

	template<typename K,typename T,typename C,typename A>
	template<typename K2>
	typename Map<K,T,C,A>::size_type Map<K,T,C,A>::count(const K2& key) const {
		return storage.template count<K2>(key);
	}



	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::iterator Map<K,T,C,A>::find(const key_type& key) {
		return storage.find(key);
	}

	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::const_iterator Map<K,T,C,A>::find(const key_type& key) const {
		return storage.find(key);
	}

	template<typename K,typename T,typename C,typename A>
	template<typename K2>
	typename Map<K,T,C,A>::iterator Map<K,T,C,A>::find(const K2& key) {
		return storage.template find<K2>(key);
	}

	template<typename K,typename T,typename C,typename A>
	template<typename K2>
	typename Map<K,T,C,A>::const_iterator Map<K,T,C,A>::find(const K2& key) const {
		return storage.template find<K2>(key);
	}



	template<typename K,typename T,typename C,typename A>
	bool Map<K,T,C,A>::contains(const key_type& key) const {
		return storage.find(key) != storage.end();
	}

	template<typename K,typename T,typename C,typename A>
	template<typename K2>
	bool Map<K,T,C,A>::contains(const K2& key) const {
		return storage.template find<K2>(key) != storage.end();
	}



	template<typename K,typename T,typename C,typename A>
	std::pair<typename Map<K,T,C,A>::iterator,typename Map<K,T,C,A>::iterator> Map<K,T,C,A>::equal_range(const key_type& key) {
		return storage.equal_range(key);
	}

	template<typename K,typename T,typename C,typename A>
	std::pair<typename Map<K,T,C,A>::const_iterator,typename Map<K,T,C,A>::const_iterator> Map<K,T,C,A>::equal_range(const key_type& key) const {
		return storage.equal_range(key);
	}

	template<typename K,typename T,typename C,typename A>
	template<typename K2>
	std::pair<typename Map<K,T,C,A>::iterator,typename Map<K,T,C,A>::iterator> Map<K,T,C,A>::equal_range(const K2& key) {
		return storage.template equal_range<K2>(key);
	}

	template<typename K,typename T,typename C,typename A>
	template<typename K2>
	std::pair<typename Map<K,T,C,A>::const_iterator,typename Map<K,T,C,A>::const_iterator> Map<K,T,C,A>::equal_range(const K2& key) const {
		return storage.template equal_range<K2>(key);
	}



	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::iterator Map<K,T,C,A>::lower_bound(const key_type& key) {
		return storage.lower_bound(key);
	}

	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::const_iterator Map<K,T,C,A>::lower_bound(const key_type& key) const {
		return storage.lower_bound(key);
	}

	template<typename K,typename T,typename C,typename A>
	template<typename K2>
	typename Map<K,T,C,A>::iterator Map<K,T,C,A>::lower_bound(const K2& x) {
		return storage.template lower_bound<K2>(x);
	}

	template<typename K,typename T,typename C,typename A>
	template<typename K2>
	typename Map<K,T,C,A>::const_iterator Map<K,T,C,A>::lower_bound(const K2& x) const {
		return storage.template lower_bound<K2>(x);
	}



	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::iterator Map<K,T,C,A>::upper_bound(const key_type& key) {
		return storage.upper_bound(key);
	}

	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::const_iterator Map<K,T,C,A>::upper_bound(const key_type& key) const {
		return storage.upper_bound(key);
	}

	template<typename K,typename T,typename C,typename A>
	template<typename K2>
	typename Map<K,T,C,A>::iterator Map<K,T,C,A>::upper_bound(const K2& key) {
		return storage.template upper_bound<K>(key);
	}

	template<typename K,typename T,typename C,typename A>
	template<typename K2>
	typename Map<K,T,C,A>::const_iterator Map<K,T,C,A>::upper_bound(const K2& key) const {
		return storage.template upper_bound<K>(key);
	}



	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::key_compare Map<K,T,C,A>::key_comp() const {
		return storage.key_comp();
	}

	template<typename K,typename T,typename C,typename A>
	typename Map<K,T,C,A>::value_compare Map<K,T,C,A>::value_comp() const {
		return storage.value_comp();
	}



	template<typename K,typename T,typename C,typename A>
	template<typename Mapper>
	auto Map<K,T,C,A>::mapValues(Mapper mapper) const {
		using MappedType = decltype(mapper(storage.begin()->first, storage.begin()->second));
		using NewMap = Map<K,MappedType>;
		NewMap newMap;
		auto it = newMap.end();
		for(auto& pair : storage) {
			it = newMap.insert(it, std::pair<K,MappedType>(pair.first, mapper(pair.first, pair.second)));
			it++;
		}
		return newMap;
	}

	template<typename K,typename T,typename C,typename A>
	template<typename Mapper>
	auto Map<K,T,C,A>::map(Mapper mapper) const {
		using PairType = decltype(mapper(storage.begin()->first, storage.begin()->second));
		using NewMap = Map<typename PairType::first_type, typename PairType::second_type>;
		NewMap newMap;
		auto it = newMap.end();
		for(auto& pair : storage) {
			it = newMap.insert(it, mapper(pair.first, pair.second));
			it++;
		}
		return newMap;
	}



	#pragma mark BasicList implementation

	template<typename Storage>
	template<typename Mapper>
	auto BasicList<Storage>::toMap(Mapper mapper) const {
		using PairType = decltype(mapper(*storage.begin(),std::declval<size_t>()));
		using MapType = Map<typename PairType::first_type, typename PairType::second_type>;
		MapType map;
		size_t i=0;
		for(auto& item : storage) {
			map.insert(mapper(item,i));
			i++;
		}
		return map;
	}
}
