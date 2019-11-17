//
//  Optional.hpp
//  DataCpp
//
//  Created by Luis Finke on 11/17/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#include <optional>
#include <variant>
#include <tuple>

namespace fgl {
	template<typename T>
	using Optional = std::optional<T>;

	namespace _chain_access {
		struct noaccess {
			bool padding = false;
		};
		
		template<typename T>
		struct OptObject {
			T& val;
			
			inline auto operator->() {
				if(!val) {
					throw noaccess();
				}
				if constexpr(std::is_pointer<T>::value) {
					return val;
				} else {
					return val.operator->();
				}
			}
			
			template<typename ...Args>
			inline auto operator()(Args... args) {
				if(!val) {
					throw noaccess();
				}
				return val(args...);
			}
			
			template<typename Type>
			inline auto get() {
				try {
					return std::get<Type>(val);
				} catch(std::bad_variant_access&) {
					throw noaccess();
				}
			}
			
			template<size_t index>
			inline auto get() {
				try {
					return std::get<index>(val);
				} catch(std::bad_variant_access&) {
					throw noaccess();
				}
			}
			
			inline auto value() {
				if constexpr(std::is_same<typename std::remove_cv<T>::type,Optional<typename T::value_type>>::value) {
					return val.value();
				} else {
					return val;
				}
			}
		};
	
		struct Opter {
			template<typename T>
			inline OptObject<T> operator()(T& obj) {
				return OptObject<T>{ obj };
			}
		};
	}

	template<typename T, typename Func>
	inline Optional<T> chain(Func func, Optional<T> defaultValue = std::nullopt) {
		using namespace _chain_access;
		try {
			return func(Opter());
		} catch(noaccess&) {
			return defaultValue;
		}
	}

	template<typename Func>
	inline bool chain(Func func) {
		using namespace _chain_access;
		try {
			func(Opter());
			return true;
		} catch(noaccess&) {
			return false;
		}
	}
}
