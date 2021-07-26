//
//  BasicList.hpp
//  DataCpp
//
//  Created by Luis Finke on 8/30/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#include <fgl/data/Common.hpp>
#include <fgl/data/Optional.hpp>
#include <fgl/data/Traits.hpp>
#include <algorithm>
#include <initializer_list>
#include <stdexcept>
#include <type_traits>
#ifdef __OBJC__
#import <Foundation/Foundation.h>
#endif

namespace fgl {
	template<typename Key,typename T,typename Compare,typename Allocator>
	class Map;


	template<typename BaseClass>
	class BasicList: public BaseClass {
	public:
		using BaseType = BaseClass;
		
		using typename BaseClass::value_type;
		using typename BaseClass::allocator_type;
		using typename BaseClass::size_type;
		using typename BaseClass::difference_type;
		using typename BaseClass::reference;
		using typename BaseClass::const_reference;
		using typename BaseClass::pointer;
		using typename BaseClass::const_pointer;
		using typename BaseClass::iterator;
		using typename BaseClass::const_iterator;
		using typename BaseClass::reverse_iterator;
		using typename BaseClass::const_reverse_iterator;
		
		using ValueType = typename BaseClass::value_type;
		
		using BaseClass::BaseClass;
		using BaseClass::operator=;
		using BaseClass::begin;
		using BaseClass::cbegin;
		using BaseClass::end;
		using BaseClass::cend;
		using BaseClass::rbegin;
		using BaseClass::crbegin;
		using BaseClass::rend;
		using BaseClass::crend;
		using BaseClass::empty;
		using BaseClass::size;
		using BaseClass::max_size;
		using BaseClass::swap;
		
		template<typename Collection, typename = IsCollectionOf<ValueType,std::remove_reference_t<Collection>>>
		inline BasicList(Collection&&);
		template<typename Collection, typename = IsCollectionOf<ValueType,std::remove_reference_t<Collection>>>
		inline BasicList& operator=(Collection&&);
		
		inline operator BaseClass&() & noexcept;
		inline operator BaseClass&&() && noexcept;
		inline operator const BaseClass&() const& noexcept;
		
		inline size_type maxSize() const noexcept;
		
		inline reference front();
		inline const_reference front() const;
		inline reference back();
		inline const_reference back() const;
		
		inline Optional<ValueType> first() const;
		inline OptionalRef<ValueType> firstRef();
		inline OptionalRef<const ValueType> firstRef() const;
		
		inline Optional<ValueType> last() const;
		inline OptionalRef<ValueType> lastRef();
		inline OptionalRef<const ValueType> lastRef() const;
		
		inline iterator findEqual(const ValueType& value);
		inline const_iterator findEqual(const ValueType& value) const;
		inline iterator findLastEqual(const ValueType& value);
		inline const_iterator findLastEqual(const ValueType& value) const;
		template<typename Predicate>
		inline iterator findWhere(Predicate predicate);
		template<typename Predicate>
		inline const_iterator findWhere(Predicate predicate) const;
		template<typename Predicate>
		inline iterator findLastWhere(Predicate predicate);
		template<typename Predicate>
		inline const_iterator findLastWhere(Predicate predicate) const;
		
		inline size_type count(const ValueType& value) const;
		template<typename Predicate>
		inline size_type countWhere(Predicate predicate) const;
		
		template<typename Predicate>
		inline reference firstWhere(Predicate predicate, ValueType& defaultValue);
		template<typename Predicate>
		inline const_reference firstWhere(Predicate predicate, const ValueType& defaultValue) const;
		template<typename Predicate>
		inline reference lastWhere(Predicate predicate, ValueType& defaultValue);
		template<typename Predicate>
		inline const_reference lastWhere(Predicate predicate, const ValueType& defaultValue) const;
		
		template<typename Predicate>
		inline Optional<ValueType> firstWhere(Predicate predicate) const;
		template<typename Predicate>
		inline OptionalRef<ValueType> firstRefWhere(Predicate predicate);
		template<typename Predicate>
		inline OptionalRef<const ValueType> firstRefWhere(Predicate predicate) const;
		template<typename Predicate>
		inline Optional<ValueType> lastWhere(Predicate predicate) const;
		template<typename Predicate>
		inline OptionalRef<ValueType> lastRefWhere(Predicate predicate);
		template<typename Predicate>
		inline OptionalRef<const ValueType> lastRefWhere(Predicate predicate) const;
		
		inline bool contains(const ValueType& value) const;
		template<typename Predicate>
		inline bool containsWhere(Predicate predicate) const;
		
		template<typename T, typename Transform>
		T reduce(T initialValue, Transform transform);
		template<typename T, typename Transform>
		T reduce(T initialValue, Transform transform) const;
		
		#ifdef __OBJC__
		template<typename Transform>
		inline NSMutableArray* toNSArray(Transform transform);
		template<typename Transform>
		inline NSMutableArray* toNSArray(Transform transform) const;
		#endif
		
		#ifdef JNIEXPORT
		template<typename Transform>
		jobjectArray toJavaObjectArray(JNIEnv* env, jclass objectClass, Transform transform);
		template<typename Transform>
		jobjectArray toJavaObjectArray(JNIEnv* env, jclass objectClass, Transform transform) const;
		#endif
		
		
		template<typename Mapper>
		auto toMap(Mapper mapper) const;
	};
	
	
	
#pragma mark BasicList implementation
	
	template<typename BaseClass>
	template<typename Collection, typename _>
	BasicList<BaseClass>::BasicList(Collection&& collection)
	: BaseClass(collection.begin(), collection.end()) {
		//
	}
	
	template<typename BaseClass>
	template<typename Collection, typename _>
	BasicList<BaseClass>& BasicList<BaseClass>::operator=(Collection&& collection) {
		assign(collection.begin(), collection.end());
		return *this;
	}


	template<typename BaseClass>
	BasicList<BaseClass>::operator BaseClass&() & noexcept {
		return *this;
	}

	template<typename BaseClass>
	BasicList<BaseClass>::operator BaseClass&&() && noexcept {
		return *this;
	}

	template<typename BaseClass>
	BasicList<BaseClass>::operator const BaseClass&() const& noexcept {
		return *this;
	}
	
	
	template<typename BaseClass>
	typename BasicList<BaseClass>::size_type BasicList<BaseClass>::maxSize() const noexcept {
		return max_size();
	}
	
	
	
	template<typename BaseClass>
	typename BasicList<BaseClass>::reference BasicList<BaseClass>::front() {
		FGL_ASSERT(size() > 0, "Cannot get front of empty list");
		return BaseClass::front();
	}
	
	template<typename BaseClass>
	typename BasicList<BaseClass>::const_reference BasicList<BaseClass>::front() const {
		FGL_ASSERT(size() > 0, "Cannot get front of empty list");
		return BaseClass::front();
	}
	
	template<typename BaseClass>
	typename BasicList<BaseClass>::reference BasicList<BaseClass>::back() {
		FGL_ASSERT(size() > 0, "Cannot get back of empty list");
		return BaseClass::back();
	}
	
	template<typename BaseClass>
	typename BasicList<BaseClass>::const_reference BasicList<BaseClass>::back() const {
		FGL_ASSERT(size() > 0, "Cannot get back of empty list");
		return BaseClass::back();
	}



	template<typename BaseClass>
	Optional<typename BasicList<BaseClass>::ValueType> BasicList<BaseClass>::first() const {
		if(size() == 0) {
			return std::nullopt;
		}
		return BaseClass::front();
	}
	
	template<typename BaseClass>
	OptionalRef<typename BasicList<BaseClass>::ValueType> BasicList<BaseClass>::firstRef() {
		if(size() == 0) {
			return std::nullopt;
		}
		return std::ref<ValueType>(BaseClass::front());
	}
	
	template<typename BaseClass>
	OptionalRef<const typename BasicList<BaseClass>::ValueType> BasicList<BaseClass>::firstRef() const {
		if(size() == 0) {
			return std::nullopt;
		}
		return std::ref<const ValueType>(BaseClass::front());
	}
	
	template<typename BaseClass>
	Optional<typename BasicList<BaseClass>::ValueType> BasicList<BaseClass>::last() const {
		if(size() == 0) {
			return std::nullopt;
		}
		return BaseClass::back();
	}
	
	template<typename BaseClass>
	OptionalRef<typename BasicList<BaseClass>::ValueType> BasicList<BaseClass>::lastRef() {
		if(size() == 0) {
			return std::nullopt;
		}
		return BaseClass::back();
	}
	
	template<typename BaseClass>
	OptionalRef<const typename BasicList<BaseClass>::ValueType> BasicList<BaseClass>::lastRef() const {
		if(size() == 0) {
			return std::nullopt;
		}
		return std::ref<const ValueType>(BaseClass::back());
	}
	
	
	
	template<typename BaseClass>
	typename BasicList<BaseClass>::iterator BasicList<BaseClass>::findEqual(const ValueType& value) {
		return std::find(begin(), end(), value);
	}
	
	template<typename BaseClass>
	typename BasicList<BaseClass>::const_iterator BasicList<BaseClass>::findEqual(const ValueType& value) const {
		return std::find(begin(), end(), value);
	}
	
	template<typename BaseClass>
	typename BasicList<BaseClass>::iterator BasicList<BaseClass>::findLastEqual(const ValueType& value) {
		auto it = std::find(rbegin(), rend(), value);
		if(it == rend()) {
			return end();
		}
		auto retIt = it.base();
		retIt--;
		return retIt;
	}
	
	template<typename BaseClass>
	typename BasicList<BaseClass>::const_iterator BasicList<BaseClass>::findLastEqual(const ValueType& value) const {
		auto it = std::find(rbegin(), rend(), value);
		if(it == rend()) {
			return end();
		}
		auto retIt = it.base();
		retIt--;
		return retIt;
	}
	
	template<typename BaseClass>
	template<typename Predicate>
	typename BasicList<BaseClass>::iterator BasicList<BaseClass>::findWhere(Predicate predicate) {
		return std::find_if(begin(), end(), predicate);
	}
	
	template<typename BaseClass>
	template<typename Predicate>
	typename BasicList<BaseClass>::const_iterator BasicList<BaseClass>::findWhere(Predicate predicate) const {
		return std::find_if(begin(), end(), predicate);
	}
	
	template<typename BaseClass>
	template<typename Predicate>
	typename BasicList<BaseClass>::iterator BasicList<BaseClass>::findLastWhere(Predicate predicate) {
		auto it = std::find_if(rbegin(), rend(), predicate);
		if(it == rend()) {
			return end();
		}
		auto retIt = it.base();
		retIt--;
		return retIt;
	}
	
	template<typename BaseClass>
	template<typename Predicate>
	typename BasicList<BaseClass>::const_iterator BasicList<BaseClass>::findLastWhere(Predicate predicate) const {
		auto it = std::find_if(rbegin(), rend(), predicate);
		if(it == rend()) {
			return end();
		}
		auto retIt = it.base();
		retIt--;
		return retIt;
	}
	
	
	
	template<typename BaseClass>
	typename BasicList<BaseClass>::size_type BasicList<BaseClass>::count(const ValueType& value) const {
		return std::count(begin(), end(), value);
	}
	
	template<typename BaseClass>
	template<typename Predicate>
	typename BasicList<BaseClass>::size_type BasicList<BaseClass>::countWhere(Predicate predicate) const {
		return std::count_if(begin(), end(), predicate);
	}
	
	
	
	template<typename BaseClass>
	template<typename Predicate>
	typename BasicList<BaseClass>::reference BasicList<BaseClass>::firstWhere(Predicate predicate, ValueType& defaultValue) {
		auto it = findWhere(predicate);
		if(it == end()) {
			return defaultValue;
		}
		return *it;
	}
	
	template<typename BaseClass>
	template<typename Predicate>
	typename BasicList<BaseClass>::const_reference BasicList<BaseClass>::firstWhere(Predicate predicate, const ValueType& defaultValue) const {
		auto it = findWhere(predicate);
		if(it == end()) {
			return defaultValue;
		}
		return *it;
	}
	
	template<typename BaseClass>
	template<typename Predicate>
	typename BasicList<BaseClass>::reference BasicList<BaseClass>::lastWhere(Predicate predicate, ValueType& defaultValue) {
		auto it = findLastWhere(predicate);
		if(it == end()) {
			return defaultValue;
		}
		return *it;
	}
	
	template<typename BaseClass>
	template<typename Predicate>
	typename BasicList<BaseClass>::const_reference BasicList<BaseClass>::lastWhere(Predicate predicate, const ValueType& defaultValue) const {
		auto it = findLastWhere(predicate);
		if(it == end()) {
			return defaultValue;
		}
		return *it;
	}



	template<typename BaseClass>
	template<typename Predicate>
	Optional<typename BasicList<BaseClass>::ValueType> BasicList<BaseClass>::firstWhere(Predicate predicate) const {
		auto it = findWhere(predicate);
		if(it == end()) {
			return std::nullopt;
		}
		return *it;
	}

	template<typename BaseClass>
	template<typename Predicate>
	OptionalRef<typename BasicList<BaseClass>::ValueType> BasicList<BaseClass>::firstRefWhere(Predicate predicate) {
		auto it = findWhere(predicate);
		if(it == end()) {
			return std::nullopt;
		}
		return std::ref(*it);
	}

	template<typename BaseClass>
	template<typename Predicate>
	OptionalRef<const typename BasicList<BaseClass>::ValueType> BasicList<BaseClass>::firstRefWhere(Predicate predicate) const {
		auto it = findWhere(predicate);
		if(it == end()) {
			return std::nullopt;
		}
		return std::ref(*it);
	}

	template<typename BaseClass>
	template<typename Predicate>
	Optional<typename BasicList<BaseClass>::ValueType> BasicList<BaseClass>::lastWhere(Predicate predicate) const {
		auto it = findLastWhere(predicate);
		if(it == end()) {
			return std::nullopt;
		}
		return *it;
	}

	template<typename BaseClass>
	template<typename Predicate>
	OptionalRef<typename BasicList<BaseClass>::ValueType> BasicList<BaseClass>::lastRefWhere(Predicate predicate) {
		auto it = findLastWhere(predicate);
		if(it == end()) {
			return std::nullopt;
		}
		return std::ref(*it);
	}

	template<typename BaseClass>
	template<typename Predicate>
	OptionalRef<const typename BasicList<BaseClass>::ValueType> BasicList<BaseClass>::lastRefWhere(Predicate predicate) const {
		auto it = findLastWhere(predicate);
		if(it == end()) {
			return std::nullopt;
		}
		return std::ref(*it);
	}
	
	
	
	template<typename BaseClass>
	bool BasicList<BaseClass>::contains(const ValueType& value) const {
		return findEqual(value) != end();
	}
	
	template<typename BaseClass>
	template<typename Predicate>
	bool BasicList<BaseClass>::containsWhere(Predicate predicate) const {
		return findWhere(predicate) != end();
	}



	template<typename BaseClass>
	template<typename T, typename Transform>
	T BasicList<BaseClass>::reduce(T value, Transform transform) {
		for(auto& item : *this) {
			value = transform(value, item);
		}
		return value;
	}

	template<typename BaseClass>
	template<typename T, typename Transform>
	T BasicList<BaseClass>::reduce(T value, Transform transform) const {
		for(auto& item : *this) {
			value = transform(value, item);
		}
		return value;
	}



	#ifdef __OBJC__

	template<typename BaseClass>
	template<typename Transform>
	NSMutableArray* BasicList<BaseClass>::toNSArray(Transform transform) {
		constexpr auto arg_count = get_arity_v<Transform>;
		static_assert(arg_count >= 1 && arg_count <= 3, "toNSArray callback must contain between 1 and 3 arguments");
		NSMutableArray* nsArray = [[NSMutableArray alloc] initWithCapacity:(NSUInteger)size()];
		if constexpr(arg_count == 1) {
			for(auto& item : *this) {
				[nsArray addObject:transform(item)];
			}
		} else {
			size_t i=0;
			for(auto& item : *this) {
				if constexpr(arg_count == 2) {
					[nsArray addObject:transform(item, i)];
				} else {
					[nsArray addObject:transform(item, i, this)];
				}
				i++;
			}
		}
		return nsArray;
	}

	template<typename BaseClass>
	template<typename Transform>
	NSMutableArray* BasicList<BaseClass>::toNSArray(Transform transform) const {
		NSMutableArray* nsArray = [[NSMutableArray alloc] initWithCapacity:(NSUInteger)size()];
		for(auto& item : *this) {
			[nsArray addObject:transform(item)];
		}
		return nsArray;
	}

	#endif



	#ifdef JNIEXPORT

	template<typename BaseClass>
	template<typename Transform>
	jobjectArray BasicList<BaseClass>::toJavaObjectArray(JNIEnv* env, jclass objectClass, Transform transform) {
		jobjectArray javaArray = env->NewObjectArray((jsize)size(), objectClass, nullptr);
		jsize i=0;
		for(auto& item : *this) {
			env->SetObjectArrayElement(javaArray, i, transform(env, item));
			i++;
		}
		return javaArray;
	}

	template<typename BaseClass>
	template<typename Transform>
	jobjectArray BasicList<BaseClass>::toJavaObjectArray(JNIEnv* env, jclass objectClass, Transform transform) const {
		constexpr auto arg_count = get_arity_v<Transform>;
		static_assert(arg_count >= 2 && arg_count <= 4, "toJavaObjectArray callback must contain between 2 and 4 arguments");
		jobjectArray javaArray = env->NewObjectArray((jsize)size(), objectClass, nullptr);
		jsize i=0;
		for(auto& item : *this) {
			if constexpr(arg_count == 2) {
				env->SetObjectArrayElement(javaArray, i, transform(env, item));
			} else if constexpr(arg_count == 3) {
				env->SetObjectArrayElement(javaArray, i, transform(env, item, (size_type)i));
			} else {
				env->SetObjectArrayElement(javaArray, i, transform(env, item, (size_type)i, this));
			}
			i++;
		}
		return javaArray;
	}

	#endif
}
