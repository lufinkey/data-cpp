//
//  Stringify.hpp
//  DataCpp
//
//  Created by Luis Finke on 2/26/20.
//  Copyright © 2020 Luis Finke. All rights reserved.
//

#pragma once

#include <fgl/data/Common.hpp>
#include <fgl/data/Optional.hpp>
#include <fgl/data/BasicString.hpp>
#include <fgl/data/Traits.hpp>

namespace fgl {
	namespace StringifyUtils {
		CREATE_HAS_MEMBER_FUNC(toString)
		CREATE_HAS_MEMBER_FUNC(to_string)
		CREATE_HAS_MEMBER_FUNC(string_value)
	
		template<typename T>
		struct has_members {
		public:
			static constexpr bool toString =
				(has_member_toString<T, String>::value
				|| has_member_toString<T, std::string>::value
				|| has_member_toString<T, char*>::value
				|| has_member_toString<T, const char*>::value);
			
			static constexpr bool to_string =
				(has_member_to_string<T, String>::value
				|| has_member_to_string<T, std::string>::value
				|| has_member_to_string<T, char*>::value
				|| has_member_to_string<T, const char*>::value);
			
			static constexpr bool string_value =
				(has_member_string_value<T, String>::value
				|| has_member_string_value<T, std::string>::value
				|| has_member_string_value<T, char*>::value
				|| has_member_string_value<T, const char*>::value);
		};
	}

	std::string demangled_type(std::string const& sym);


	template<typename T>
	String stringify_type() {
		std::stringstream ss;
		ss << demangled_type(typeid(T).name());
		if (std::is_const<typename std::remove_reference<T>::type>::value) {
			ss << " const";
		}
		if (std::is_lvalue_reference<T>::value) {
			ss << " &";
		}
		if (std::is_rvalue_reference<T>::value) {
			ss << " &&";
		}
		return ss.str();
	}


	template<typename T>
	String stringify(const T& obj) {
		using Type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
		if constexpr(std::is_same<String,Type>::value || std::is_same<std::string,Type>::value
		   || std::is_same<char*,Type>::value || std::is_same<const char*,Type>::value || std::is_same<char,Type>::value
		   || std::is_same<std::wstring,Type>::value || std::is_same<const wchar_t*,Type>::value
		   || std::is_same<wchar_t*,Type>::value || std::is_same<wchar_t,Type>::value) {
			return (String)obj;
		}
		else if constexpr(is_optional<Type>::value) {
			return obj ? ("Optional( "+stringify(obj.value())+" )") : "Optional()";
		}
		else if constexpr(is_ptr_container<Type>::value) {
			if(obj == nullptr) {
				return "null";
			}
			return stringify<typename is_ptr_container<Type>::content_type*>(obj.get());
		}
		else if constexpr(is_weak_ptr<Type>::value) {
			return "weak_ptr<"+stringify_type<typename is_weak_ptr<Type>::content_type>()+">(use_count="+std::to_string(obj.use_count())+")";
		}
		else if constexpr(is_pair<Type>::value) {
			return "pair( "+stringify(obj.first)+" , "+stringify(obj.second)+" )";
		}
		else if constexpr(std::is_array<Type>::value) {
			size_t arraySize = sizeof(Type) / sizeof(typename std::remove_extent<Type>::type);
			if(arraySize == 0) {
				return "[]";
			}
			std::stringstream ss;
			ss << "[\n";
			for(size_t i=0; i<arraySize; i++) {
				ss << "\t" << stringify(obj[i]);
				if(i != (arraySize-1)) {
					ss << ",\n";
				}
			}
			ss << "]";
			return ss.str();
		}
		else if constexpr(std::is_pointer<Type>::value) {
			if(obj == nullptr) {
				return "null";
			}
			using U = typename std::remove_pointer<Type>::type;
			if(std::is_class<U>::value || std::is_arithmetic<U>::value) {
				return stringify(*obj);
			} else {
				std::stringstream ss;
				ss << stringify_type<U>() << " @ 0x" << std::setfill('0') << std::setw(sizeof(obj)*2) << std::hex << ((uintptr_t)obj);
				return ss.str();
			}
		}
		else if constexpr(std::is_arithmetic<Type>::value) {
			return std::to_string(obj);
		}
		else if constexpr(StringifyUtils::has_members<Type>::toString) {
			return obj.toString();
		}
		else if constexpr(StringifyUtils::has_members<Type>::to_string) {
			return obj.to_string();
		}
		else if constexpr(StringifyUtils::has_members<Type>::string_value) {
			return obj.string_value();
		}
		else if constexpr(std::is_base_of<std::exception,Type>::value) {
			return obj.what();
		}
		else if constexpr(is_container<Type>::value) {
			std::stringstream ss;
			ss << stringify_type<Type>() << "[";
			for(auto begin=obj.begin(), end=obj.end(), it=begin; it!=end; it++) {
				if(it == begin) {
					ss << "\n";
				} else {
					ss << ",\n";
				}
				ss << "\t" << stringify(*it);
			}
			ss << "]";
			return ss.str();
		}
		else {
			std::stringstream ss;
			ss << stringify_type<T>() << " @ 0x" << std::setfill('0') << std::setw(sizeof(&obj)*2) << std::hex << ((uintptr_t)&obj);
			return ss.str();
		}
	}
}
