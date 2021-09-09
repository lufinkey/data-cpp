//
//  Traits.hpp
//  DataCpp
//
//  Created by Luis Finke on 8/30/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#include <iterator>
#include <type_traits>

namespace fgl {
	template<typename T>
	struct has_const_iterator {
	private:
		typedef char yes;
		typedef struct {
			char array[2];
		} no;
		
		template<typename C>
		static yes test(typename C::const_iterator*);
		template<typename C>
		static no test(...);
		
	public:
		static const bool value = sizeof(test<T>(0)) == sizeof(yes);
		typedef T type;
	};
	
	template <typename T>
	struct has_begin_end {
		template<typename C>
		static char (&f(typename std::enable_if<
			std::is_same<decltype(static_cast<typename C::const_iterator (C::*)() const>(&C::begin)),
			typename C::const_iterator(C::*)() const>::value, void>::type*))[1];
		
		template<typename C>
		static char (&f(...))[2];
		
		template<typename C> static char (&g(typename std::enable_if<
			std::is_same<decltype(static_cast<typename C::const_iterator (C::*)() const>(&C::end)),
			typename C::const_iterator(C::*)() const>::value, void>::type*))[1];
		
		template<typename C> static char (&g(...))[2];
		
		static bool const beg_value = sizeof(f<T>(0)) == 1;
		static bool const end_value = sizeof(g<T>(0)) == 1;
	};
	
	template<typename T>
	struct is_collection : std::integral_constant<bool, has_const_iterator<T>::value && has_begin_end<T>::beg_value && has_begin_end<T>::end_value>
	{ };

	template<typename T>
	constexpr auto is_collection_v = is_collection<T>::value;
	
	
	template<typename InputIterator>
	using IsInputIterator = typename std::enable_if<
		(std::is_convertible_v<typename std::iterator_traits<InputIterator>::iterator_category, std::input_iterator_tag>), InputIterator>::type;
	
	template<typename Collection>
	using IsCollection = typename std::enable_if<(is_collection_v<Collection>), Collection>::type;
	
	template<typename T, typename Collection>
	using IsCollectionOf = typename std::enable_if<
		(is_collection_v<Collection> && std::is_same_v<T,typename Collection::value_type>), Collection>::type;

	template<typename T, typename Collection>
	using IsCollectionNotOf = typename std::enable_if<
		(is_collection_v<Collection> && !std::is_same_v<T,typename Collection::value_type>), Collection>::type;



	template<typename T>
	struct is_pair: std::false_type {};
	template<typename T, typename U>
	struct is_pair<std::pair<T,U>>: std::true_type {
		using first_type = T;
		using second_type = U;
	};



	template<bool Condition, typename T>
	using member_if = typename std::conditional<Condition,T,empty[0]>::type;

	template<typename T>
	using NullifyVoid = typename std::conditional<std::is_void_v<T>,std::nullptr_t,T>::type;



	#define CREATE_HAS_CONST_MEMBER_FUNC(memberName) \
	template<typename T, typename ReturnType, class... Args> \
	struct has_const_memberfunc_##memberName { \
	private: \
		typedef char yes[1]; \
		typedef char no[2]; \
		template <typename U, U> struct type_check; \
		template <typename Q> static yes &chk(type_check<ReturnType(T::*)(Args...)const,&Q::memberName>*); \
		template <typename  > static no  &chk(...); \
	public: \
		static constexpr bool value = sizeof(chk<T>(0)) == sizeof(yes); \
	};



	#define CREATE_HAS_STATIC_FUNC(memberName) \
	template<typename T, typename ReturnType, class... Args> \
	struct has_staticfunc_##memberName { \
	private: \
		typedef char yes[1]; \
		typedef char no[2]; \
		template <typename U, U> struct type_check; \
		template <typename Q> static yes &chk(type_check<ReturnType(*)(Args...),&Q::memberName>*); \
		template <typename  > static no  &chk(...); \
	public: \
		static constexpr bool value = sizeof(chk<T>(0)) == sizeof(yes); \
	};



	template<typename T>
	using add_lref_if_nonref = typename std::conditional<std::is_reference_v<T>,T,T&>::type;

	template<typename T>
	using make_lref_if_rref_or_nonref = typename std::conditional<std::is_rvalue_reference_v<T>,
		std::add_lvalue_reference_t<std::remove_const_t<std::remove_reference_t<T>>>,
		add_lref_if_nonref<T>>::type;


	CREATE_HAS_CONST_MEMBER_FUNC(size)
}
