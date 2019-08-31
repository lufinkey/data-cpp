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
}
