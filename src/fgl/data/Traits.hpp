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
	struct is_container : std::integral_constant<bool, has_const_iterator<T>::value && has_begin_end<T>::beg_value && has_begin_end<T>::end_value>
	{ };
	
	
	
	
	template<typename InputIterator>
	using IsInputIterator = typename std::enable_if<
		(std::is_convertible<typename std::iterator_traits<InputIterator>::iterator_category, std::input_iterator_tag>::value)>::type;
	
	template<typename Container>
	using IsContainer = typename std::enable_if<
		(is_container<Container>::value)>::type;
	
	template<typename T, typename Container>
	using IsTypeContainer = typename std::enable_if<
	(is_container<Container>::value && std::is_same<T,typename Container::value_type>::value)>::type;




	template<typename T>
	struct is_ptr_container: std::false_type {};
	template<typename T>
	struct is_ptr_container<std::shared_ptr<T>>: std::true_type {
		using content_type = T;
	};
	template<typename T>
	struct is_ptr_container<std::unique_ptr<T>>: std::true_type {
		using content_type = T;
	};
	
	template<typename T>
	struct is_weak_ptr: std::false_type {};
	template<typename T>
	struct is_weak_ptr<std::weak_ptr<T>>: std::true_type {
		using content_type = T;
	};

	template<typename T>
	struct is_pair: std::false_type {};
	template<typename T, typename U>
	struct is_pair<std::pair<T,U>>: std::true_type {
		using first_type = T;
		using second_type = U;
	};



	#define CREATE_HAS_MEMBER_FUNC(memberName) \
	template<typename T, typename ReturnType, typename std::enable_if<std::is_class<T>::value, std::nullptr_t>::type = nullptr> \
	struct has_memberfunc_##memberName { \
	private: \
		typedef char yes[1]; \
		typedef char no[2]; \
		template <typename U, U> struct type_check; \
		template <typename Q> static yes &chk(type_check<ReturnType(T::*)()const,&Q::memberName>*); \
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




	CREATE_HAS_MEMBER_FUNC(size)
}
