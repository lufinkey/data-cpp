//
//  BasicStringUtils.hpp
//  DataCpp
//
//  Created by Luis Finke on 8/14/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#include <fgl/data/Common.hpp>
#include <codecvt>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#ifdef __OBJC__
	#import <Foundation/Foundation.h>
#endif

namespace fgl {
	#define NULLCHAR 0
	
	template<typename Char>
	class BasicString;
	
	class BasicStringUtils {
	public:
		BasicStringUtils() = delete;
		BasicStringUtils(const BasicStringUtils&) = delete;
		BasicStringUtils& operator=(const BasicStringUtils&) = delete;
		
		// same_size
		template<typename T1, typename T2,
			bool Test = (sizeof(T1)==sizeof(T2))>
		struct same_size {
			static constexpr bool value = false;
		};
		template<typename T1, typename T2>
		struct same_size<T1, T2, true> {
			static constexpr bool value = true;
			typedef T1 type1;
			typedef T2 type2;
			typedef std::nullptr_t null_type;
		};
		
		// is_same
		template<typename T1, typename T2>
		struct is_same {
			static constexpr bool value = false;
		};
		template<typename T>
		struct is_same<T, T> {
			static constexpr bool value = true;
			typedef T type;
			typedef std::nullptr_t null_type;
		};
		
		// can_convert_string_type
		template<typename T,
			bool Test = (std::is_integral<T>::value && (sizeof(T)<=4))>
		struct can_convert_string_type {
			static constexpr bool value = false;
		};
		template<typename T>
		struct can_convert_string_type<T, true> {
			static constexpr bool value = true;
			typedef T type;
			typedef std::nullptr_t null_type;
		};
		
		// cant_convert_string_type
		template<typename T,
			bool Test = !can_convert_string_type<T>::value>
		struct cant_convert_string_type {
			static constexpr bool value = false;
		};
		template<typename T>
		struct cant_convert_string_type<T, true> {
			static constexpr bool value = true;
			typedef T type;
			typedef std::nullptr_t null_type;
		};
		
		// can_convert_string_types
		template<typename T1, typename T2,
			bool Test = (is_same<T1,T2>::value || (can_convert_string_type<T1>::value && can_convert_string_type<T2>::value))>
		struct can_convert_string_types {
			static constexpr bool value = false;
		};
		template<typename T1, typename T2>
		struct can_convert_string_types<T1, T2, true> {
			static constexpr bool value = true;
			typedef T1 type1;
			typedef T2 type2;
			typedef std::nullptr_t null_type;
		};
		
		// is_char_type
		template<typename T>
		struct is_char_type {
			static constexpr bool value = false;
		};
		
		// is_not_char_type
		template<typename T, bool Test = !is_char_type<T>::value>
		struct is_not_char_type {
			static constexpr bool value = false;
		};
		template<typename T>
		struct is_not_char_type<T, true> {
			static constexpr bool value = true;
			typedef T type;
			typedef std::nullptr_t null_type;
		};
		
		// same_size_convertable_strings
		template<typename T1, typename T2,
			bool Test = (!is_same<T1,T2>::value && same_size<T1,T2>::value && can_convert_string_types<T1,T2>::value)>
		struct same_size_convertable_strings {
			static constexpr bool value = false;
		};
		template<typename T1, typename T2>
		struct same_size_convertable_strings<T1, T2, true> {
			static constexpr bool value = true;
			typedef T1 type1;
			typedef T2 type2;
			typedef std::nullptr_t null_type;
		};
		
		// diff_size_convertable_strings
		template<typename T1, typename T2,
			bool Test = (!is_same<T1,T2>::value && !same_size<T1,T2>::value && can_convert_string_types<T1,T2>::value)>
		struct diff_size_convertable_strings {
			static constexpr bool value = false;
		};
		template<typename T1, typename T2>
		struct diff_size_convertable_strings<T1, T2, true> {
			static constexpr bool value = true;
			typedef T1 type1;
			typedef T2 type2;
			typedef std::nullptr_t null_type;
		};
		
		// same_size_convertable_with_char_type
		template<typename T, typename Char,
			bool Test = (same_size_convertable_strings<T,Char>::value && is_char_type<Char>::value)>
		struct same_size_convertable_with_char_type {
			static constexpr bool value = false;
		};
		template<typename T, typename Char>
		struct same_size_convertable_with_char_type<T, Char, true> {
			static constexpr bool value = true;
			typedef T type;
			typedef Char char_type;
			typedef std::nullptr_t null_type;
		};
		
		// diff_size_convertable_with_char_type
		template<typename T, typename Char,
			bool Test = (diff_size_convertable_strings<T, Char>::value && is_char_type<Char>::value)>
		struct diff_size_convertable_with_char_type {
			static constexpr bool value = false;
		};
		template<typename T, typename Char>
		struct diff_size_convertable_with_char_type<T, Char, true> {
			static constexpr bool value = true;
			typedef T type;
			typedef Char char_type;
			typedef std::nullptr_t null_type;
		};
		
		// string_type_convertable_with_char_type
		template<typename T, typename Char,
			bool Test = (can_convert_string_types<T,Char>::value && (is_char_type<Char>::value || is_same<T,Char>::value))>
		struct string_type_convertable_with_char_type {
			static constexpr bool value = false;
		};
		template<typename T, typename Char>
		struct string_type_convertable_with_char_type<T, Char, true> {
			static constexpr bool value = true;
			typedef T type;
			typedef Char char_type;
			typedef std::nullptr_t null_type;
		};
		
		// string_type_convertable_with_bool
		template<typename T, typename Bool,
			bool Test = (is_same<Bool,bool>::value && !is_same<T,Bool>::value && is_char_type<T>::value)>
		struct string_type_convertable_with_bool {
			static constexpr bool value = false;
		};
		template<typename T, typename Bool>
		struct string_type_convertable_with_bool<T, Bool, true> {
			static constexpr bool value = true;
			typedef T type;
			typedef Bool bool_type;
			typedef std::nullptr_t null_type;
		};
		
		// string_type_convertable_with_number
		template<typename T, typename Num,
			bool Test = (std::is_arithmetic<Num>::value
				&& !is_same<Num,bool>::value && !is_same<T,Num>::value && std::is_pod<T>::value
				&& is_char_type<T>::value && !is_char_type<Num>::value && !std::is_pointer<Num>::value)>
		struct string_type_convertable_with_number {
			static constexpr bool value = false;
		};
		template<typename T, typename Num>
		struct string_type_convertable_with_number<T, Num, true> {
			static constexpr bool value = true;
			typedef T type;
			typedef Num number_type;
			typedef std::nullptr_t null_type;
		};
		
		// string_type_convertable_with_number_or_enum
		template<typename T, typename Num,
			bool Test = ((std::is_arithmetic<Num>::value || std::is_enum<Num>::value)
				&& !is_same<Num,bool>::value && !is_same<T,Num>::value && std::is_pod<T>::value
				&& is_char_type<T>::value && !is_char_type<Num>::value && !std::is_pointer<Num>::value)>
		struct string_type_convertable_with_number_or_enum {
			static constexpr bool value = false;
		};
		template<typename T, typename Num>
		struct string_type_convertable_with_number_or_enum<T, Num, true> {
			static constexpr bool value = true;
			typedef T type;
			typedef Num number_type;
			typedef std::nullptr_t null_type;
		};
		
		// utf_eqv
		template<typename T, size_t T_SIZE=sizeof(T)>
		struct utf_eqv {};
		template<typename T>
		struct utf_eqv<T, 1> {
			typedef char type;
		};
		template<typename T>
		struct utf_eqv<T, 2> {
			typedef char16_t type;
		};
		template<typename T>
		struct utf_eqv<T, 4> {
			typedef char32_t type;
		};
		
		// functions
		
		template<typename Char>
		static size_t strlen(const Char* str) noexcept;
		template<typename Char>
		static bool streq(const Char* left, const Char* right) noexcept;
		
		template<typename Char,
			typename InputChar,
			typename std::enable_if<(sizeof(Char) == sizeof(InputChar)), std::nullptr_t>::type = nullptr>
		static std::basic_string<Char>&& cast(std::basic_string<InputChar>&& str);
		template<typename Char>
		static BasicString<Char> fromBool(bool);
		template<typename Char, typename Num,
			typename is_char_type<Char>::null_type = nullptr>
		static BasicString<Char> fromNumber(Num);
		template<typename Char, typename Num,
			typename is_not_char_type<Char>::null_type = nullptr>
		static BasicString<Char> fromNumber(Num);
		template<typename Char,
			typename InputChar,
			typename std::enable_if<(sizeof(Char) == sizeof(InputChar) && BasicStringUtils::can_convert_string_types<Char,InputChar>::value), std::nullptr_t>::type = nullptr>
		static std::basic_string<Char> convert(const InputChar* str, size_t length);
		template<typename Char,
			typename InputChar,
			typename std::enable_if<(sizeof(Char) == 1 && sizeof(InputChar) > 1
				&& BasicStringUtils::can_convert_string_types<Char,InputChar>::value), std::nullptr_t>::type = nullptr>
		static std::basic_string<Char> convert(const InputChar* str, size_t length);
		template<typename Char,
			typename InputChar,
			typename std::enable_if<(sizeof(Char) > 1 && sizeof(InputChar) > 1
				&& BasicStringUtils::can_convert_string_types<Char,InputChar>::value), std::nullptr_t>::type = nullptr>
		static std::basic_string<Char> convert(const InputChar* str, size_t length);
		template<typename Char,
			typename InputChar,
			typename std::enable_if<(sizeof(Char) > 1 && sizeof(InputChar) == 1
				&& BasicStringUtils::can_convert_string_types<Char,InputChar>::value), std::nullptr_t>::type = nullptr>
		static std::basic_string<Char> convert(const InputChar* str, size_t length);
		
		template<typename Char>
		inline static size_t get_safe_resize(size_t len1, size_t len2);
		
		template<typename Char>
		static BasicString<Char> concat(const BasicString<Char>& left, const BasicString<Char>& right);
		template<typename Char>
		static BasicString<Char> concat(const BasicString<Char>& left, const Char* right);
		template<typename Char>
		static BasicString<Char> concat(const Char* left, const BasicString<Char>& right);
		template<typename Char>
		static BasicString<Char> concat(const BasicString<Char>& left, const std::basic_string<Char>& right);
		template<typename Char>
		static BasicString<Char> concat(const std::basic_string<Char>& left, const BasicString<Char>& right);
		template<typename Char>
		static BasicString<Char> concat(const BasicString<Char>& left, Char right);
		template<typename Char>
		static BasicString<Char> concat(Char left, const BasicString<Char>& right);
		
		#ifdef __OBJC__
		template<typename Char,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<Char>::value
				&& sizeof(unichar)==sizeof(Char) && sizeof(Char)!=sizeof(char)), std::nullptr_t>::type = nullptr>
		static BasicString<Char> concat(const BasicString<Char>& left, NSString* right);
		template<typename Char,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<Char>::value
				&& sizeof(Char)==sizeof(char)), std::nullptr_t>::type = nullptr>
		static BasicString<Char> concat(const BasicString<Char>& left, NSString* right);
		template<typename Char,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<Char>::value
				&& sizeof(unichar)!=sizeof(Char) && sizeof(Char)!=sizeof(char)), std::nullptr_t>::type = nullptr>
		static BasicString<Char> concat(const BasicString<Char>& left, NSString* right);
		
		template<typename Char,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<Char>::value
				&& sizeof(unichar)==sizeof(Char) && sizeof(Char)!=sizeof(char)), std::nullptr_t>::type = nullptr>
		static BasicString<Char> concat(NSString* left, const BasicString<Char>& right);
		template<typename Char,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<Char>::value
				&& sizeof(Char)==sizeof(char)), std::nullptr_t>::type = nullptr>
		static BasicString<Char> concat(NSString* left, const BasicString<Char>& right);
		template<typename Char,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<Char>::value
				&& sizeof(unichar)!=sizeof(Char) && sizeof(Char)!=sizeof(char)), std::nullptr_t>::type = nullptr>
		static BasicString<Char> concat(NSString* left, const BasicString<Char>& right);
		#endif
		
		template<typename Char, typename OtherChar,
			typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		static BasicString<Char> concat(const BasicString<Char>& left, const OtherChar* right);
		template<typename Char, typename OtherChar,
			typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		static BasicString<Char> concat(const BasicString<Char>& left, const OtherChar* right);
		
		template<typename Char, typename OtherChar,
			typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		static BasicString<Char> concat(const Char* left, const BasicString<OtherChar>& right);
		template<typename Char, typename OtherChar,
			typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		static BasicString<Char> concat(const Char* left, const BasicString<OtherChar>& right);
		
		template<typename Char, typename OtherChar,
			typename BasicStringUtils::same_size_convertable_with_char_type<Char,OtherChar>::null_type = nullptr>
		static BasicString<Char> concat(const BasicString<Char>& left, OtherChar right);
		template<typename Char, typename OtherChar,
			typename BasicStringUtils::diff_size_convertable_with_char_type<Char,OtherChar>::null_type = nullptr>
		static BasicString<Char> concat(const BasicString<Char>& left, OtherChar right);
		
		template<typename Char, typename OtherChar,
			typename BasicStringUtils::same_size_convertable_with_char_type<Char, OtherChar>::null_type = nullptr>
		static BasicString<Char> concat(Char left, const BasicString<OtherChar>& right);
		template<typename Char, typename OtherChar,
			typename BasicStringUtils::diff_size_convertable_with_char_type<Char, OtherChar>::null_type = nullptr>
		static BasicString<Char> concat(Char left, const BasicString<OtherChar>& right);
		
		template<typename Char, typename Bool,
			typename BasicStringUtils::string_type_convertable_with_bool<Char,Bool>::null_type = nullptr>
		static BasicString<Char> concat(const BasicString<Char>& left, Bool right);
		template<typename Char, typename Bool,
			typename BasicStringUtils::string_type_convertable_with_bool<Char,Bool>::null_type = nullptr>
		static BasicString<Char> concat(Bool left, const BasicString<Char>& right);
		
		template<typename Char, typename Num,
			typename BasicStringUtils::string_type_convertable_with_number_or_enum<Char,Num>::null_type = nullptr>
		static BasicString<Char> concat(const BasicString<Char>& left, Num right);
		template<typename Char, typename Num,
			typename BasicStringUtils::string_type_convertable_with_number_or_enum<Char,Num>::null_type = nullptr>
		static BasicString<Char> concat(Num left, const BasicString<Char>& right);
		
		template<typename Char, typename OtherChar,
			typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		static BasicString<Char> concat(const BasicString<Char>& left, const BasicString<OtherChar>& right);
		template<typename Char, typename OtherChar,
			typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		static BasicString<Char> concat(const BasicString<Char>& left, const BasicString<OtherChar>& right);
		
		template<typename Char, typename OtherChar,
			typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		static BasicString<Char> concat(const BasicString<Char>& left, const std::basic_string<OtherChar>& right);
		template<typename Char, typename OtherChar,
			typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		static BasicString<Char> concat(const BasicString<Char>& left, const std::basic_string<OtherChar>& right);
		template<typename Char, typename OtherChar,
			typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		static BasicString<Char> concat(const std::basic_string<Char>& left, const BasicString<OtherChar>& right);
		template<typename Char, typename OtherChar,
			typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		static BasicString<Char> concat(const std::basic_string<Char>& left, const BasicString<OtherChar>& right);
	};
	
	// char
	template<>
	struct BasicStringUtils::is_char_type<char> {
		static constexpr bool value = true;
		typedef char type;
		typedef std::nullptr_t null_type;
	};
	// wchar_t
	template<>
	struct BasicStringUtils::is_char_type<wchar_t> {
		static constexpr bool value = true;
		typedef wchar_t type;
		typedef std::nullptr_t null_type;
	};
	// char16_t
	template<>
	struct BasicStringUtils::is_char_type<char16_t> {
		static constexpr bool value = true;
		typedef char16_t type;
		typedef std::nullptr_t null_type;
	};
	template<>
	struct BasicStringUtils::is_char_type<char32_t> {
		static constexpr bool value = true;
		typedef char32_t type;
		typedef std::nullptr_t null_type;
	};
	#ifdef __OBJC__
	template<>
	struct BasicStringUtils::is_char_type<unichar> {
		static constexpr bool value = true;
		typedef unichar type;
		typedef std::nullptr_t null_type;
	};
	#endif
	
	
	
	
#pragma mark BasicStringUtils implementation
	
	template<typename Char>
	size_t BasicStringUtils::strlen(const Char* str) noexcept {
		FGL_ASSERT(str != nullptr, "str cannot be null");
		size_t size = 0;
		while(str[size] != NULLCHAR) {
			size++;
		}
		return size;
	}
	
	template<typename Char>
	bool BasicStringUtils::streq(const Char* left, const Char* right) noexcept {
		FGL_ASSERT(left != nullptr, "left cannot be null");
		FGL_ASSERT(right != nullptr, "right cannot be null");
		size_t counter = 0;
		do {
			Char c1 = left[counter];
			Char c2 = right[counter];
			if(c1 != c2) {
				return false;
			}
			if(c1 == NULLCHAR) {
				return true;
			}
			counter++;
		}
		while(true);
	}
	
	template<typename Char,
		typename InputChar,
		typename std::enable_if<(sizeof(Char) == sizeof(InputChar)), std::nullptr_t>::type>
	std::basic_string<Char>&& BasicStringUtils::cast(std::basic_string<InputChar>&& str) {
		auto& str_left = str;
		return std::move(*((std::basic_string<Char>*)&str_left));
	}
	
	template<typename Char>
	BasicString<Char> BasicStringUtils::fromBool(bool b) {
		if(b) {
			char str[5];
			str[0] = (Char)'t';
			str[1] = (Char)'r';
			str[2] = (Char)'u';
			str[3] = (Char)'e';
			str[4] = NULLCHAR;
			return BasicString<Char>(str, 5);
		}
		else {
			char str[6];
			str[0] = (Char)'f';
			str[1] = (Char)'a';
			str[2] = (Char)'l';
			str[3] = (Char)'s';
			str[4] = (Char)'e';
			str[5] = NULLCHAR;
			return BasicString<Char>(str, 6);
		}
	}
	
	template<typename Char, typename Num,
		typename BasicStringUtils::is_char_type<Char>::null_type>
	BasicString<Char> BasicStringUtils::fromNumber(Num num) {
		std::basic_ostringstream<Char> ss;
		ss << std::setprecision(10);
		if constexpr(sizeof(Num) == 1) {
			ss << (int)num;
		}
		else {
			ss << num;
		}
		return ss.str();
	}
	
	template<typename Char, typename Num,
		typename BasicStringUtils::is_not_char_type<Char>::null_type>
	BasicString<Char> BasicStringUtils::fromNumber(Num num) {
		std::basic_ostringstream<char> ss;
		ss << std::setprecision(10);
		if constexpr(sizeof(Num) == 1) {
			ss << (int)num;
		}
		else {
			ss << num;
		}
		auto str = ss.str();
		if constexpr(same_size_convertable_strings<Char,char>::value) {
			return cast<Char>(str);
		}
		return convert<Char,char>(str.c_str(), str.length());
	}
	
	// same-size chars
	template<typename Char,
		typename InputChar,
		typename std::enable_if<(sizeof(Char) == sizeof(InputChar)
			&& BasicStringUtils::can_convert_string_types<Char,InputChar>::value), std::nullptr_t>::type>
	std::basic_string<Char> BasicStringUtils::convert(const InputChar* str, size_t length) {
		FGL_ASSERT(str != nullptr, "str cannot be null");
		return std::basic_string<Char>((const Char*)str, length);
	}
	
	// multibyte to byte
	template<typename Char,
		typename InputChar,
		typename std::enable_if<(sizeof(Char) == 1 && sizeof(InputChar) > 1
			&& BasicStringUtils::can_convert_string_types<Char,InputChar>::value), std::nullptr_t>::type>
	std::basic_string<Char> BasicStringUtils::convert(const InputChar* str, size_t length) {
		typedef typename utf_eqv<InputChar>::type UTFInputChar;
		if constexpr(sizeof(InputChar) == 2) {
			std::wstring_convert<std::codecvt_utf8_utf16<UTFInputChar>,UTFInputChar> convert;
			return cast<Char>(convert.to_bytes((const UTFInputChar*)str, (const UTFInputChar*)(str+length)));
		}
		else if constexpr(sizeof(InputChar) == 4) {
			std::wstring_convert<std::codecvt_utf8<UTFInputChar>,UTFInputChar> convert;
			return cast<Char>(convert.to_bytes((const UTFInputChar*)str, (const UTFInputChar*)(str+length)));
		}
		else {
			static_assert((sizeof(InputChar) == 2 || sizeof(InputChar) == 4), "Unimplemented code path");
		}
	}
	
	// multibyte to multibyte
	template<typename Char,
		typename InputChar,
		typename std::enable_if<(sizeof(Char) > 1 && sizeof(InputChar) > 1
			&& BasicStringUtils::can_convert_string_types<Char,InputChar>::value), std::nullptr_t>::type>
	std::basic_string<Char> BasicStringUtils::convert(const InputChar* str, size_t length) {
		bool little_endian = false;
		int n = 1;
		if(*(char*)&n == 1) {
			little_endian = true;
		}
		if(little_endian) {
			if constexpr(sizeof(Char) < sizeof(InputChar)) {
				typedef typename utf_eqv<InputChar>::type UTFInputChar;
				std::wstring_convert<std::codecvt_utf16<UTFInputChar, 0x10ffff, std::codecvt_mode::little_endian>, UTFInputChar> convert;
				std::string&& buffer = convert.to_bytes((const UTFInputChar*)str, (const UTFInputChar*)(str+length));
				return std::basic_string<Char>((const Char*)buffer.c_str(), (buffer.length()*sizeof(std::string::value_type))/sizeof(Char));
			}
			else {
				typedef typename utf_eqv<Char>::type UTFChar;
				std::wstring_convert<std::codecvt_utf16<UTFChar, 0x10ffff, std::codecvt_mode::little_endian>, UTFChar> convert;
				return cast<Char>(convert.from_bytes((const char*)str, (const char*)(str+length)));
			}
		}
		else {
			if constexpr(sizeof(Char) < sizeof(InputChar)) {
				typedef typename utf_eqv<InputChar>::type UTFInputChar;
				std::wstring_convert<std::codecvt_utf16<UTFInputChar>, UTFInputChar> convert;
				std::string&& buffer = convert.to_bytes((const UTFInputChar*)str, (const UTFInputChar*)(str+length));
				return std::basic_string<Char>((const Char*)buffer.c_str(), (buffer.length()*sizeof(std::string::value_type))/sizeof(Char));
			}
			else {
				typedef typename utf_eqv<Char>::type UTFChar;
				std::wstring_convert<std::codecvt_utf16<UTFChar>, UTFChar> convert;
				return cast<Char>(convert.from_bytes((const char*)str, (const char*)(str+length)));
			}
		}
	}
	
	// byte to multibyte
	template<typename Char,
		typename InputChar,
		typename std::enable_if<(sizeof(Char) > 1 && sizeof(InputChar) == 1
			&& BasicStringUtils::can_convert_string_types<Char,InputChar>::value), std::nullptr_t>::type>
	std::basic_string<Char> BasicStringUtils::convert(const InputChar* str, size_t length) {
		FGL_ASSERT(str != nullptr, "str cannot be null");
		typedef typename utf_eqv<Char>::type UTFChar;
		if constexpr(sizeof(Char) == 2) {
			std::wstring_convert<std::codecvt_utf8_utf16<UTFChar>, UTFChar> convert;
			return cast<Char>(convert.from_bytes((const char*)str, (const char*)(str+length)));
		}
		else if constexpr(sizeof(Char) == 4) {
			std::wstring_convert<std::codecvt_utf8<UTFChar>, UTFChar> convert;
			return cast<Char>(convert.from_bytes((const char*)str, (const char*)(str+length)));
		}
		else {
			static_assert((sizeof(InputChar) == 2 || sizeof(InputChar) == 4), "Unimplemented code path");
		}
	}
	
	template<typename Char>
	size_t BasicStringUtils::get_safe_resize(size_t len1, size_t len2) {
		size_t maxLength = (((size_t)-1) / sizeof(Char)) - 1;
		if(len1 >= maxLength || len2 >= maxLength) {
			throw std::bad_alloc();
		}
		size_t remainingLength = maxLength - len1;
		if(remainingLength <= len2) {
			throw std::bad_alloc();
		}
		return len1 + len2;
	}
	
	template<typename Char>
	BasicString<Char> BasicStringUtils::concat(const BasicString<Char>& left, const BasicString<Char>& right) {
		size_t size_new = get_safe_resize<Char>(left.size(), right.size());
		BasicString<Char> newStr;
		newStr.reserve(size_new);
		newStr.append(left);
		newStr.append(right);
		return newStr;
	}
	
	template<typename Char>
	BasicString<Char> BasicStringUtils::concat(const BasicString<Char>& left, const Char* right) {
		FGL_ASSERT(right != nullptr, "right cannot be null");
		size_t right_size = strlen<Char>(right);
		size_t size_new = get_safe_resize<Char>(left.size(), right_size);
		BasicString<Char> newStr;
		newStr.reserve(size_new);
		newStr.append(left);
		newStr.append(right, right_size);
		return newStr;
	}
	
	template<typename Char>
	BasicString<Char> BasicStringUtils::concat(const Char* left, const BasicString<Char>& right) {
		FGL_ASSERT(left != nullptr, "left cannot be null");
		size_t left_size = strlen<Char>(left);
		size_t size_new = get_safe_resize<Char>(left_size, right.size());
		BasicString<Char> newStr;
		newStr.reserve(size_new);
		newStr.append(left, left_size);
		newStr.append(right);
		return newStr;
	}
	
	template<typename Char>
	BasicString<Char> BasicStringUtils::concat(const BasicString<Char>& left, const std::basic_string<Char>& right) {
		size_t size_new = get_safe_resize<Char>(left.size(), right.size());
		BasicString<Char> newStr;
		newStr.reserve(size_new);
		newStr.append(left);
		newStr.append(right);
		return newStr;
	}
	
	template<typename Char>
	BasicString<Char> BasicStringUtils::concat(const std::basic_string<Char>& left, const BasicString<Char>& right) {
		size_t size_new = get_safe_resize<Char>(left.size(), right.size());
		BasicString<Char> newStr;
		newStr.reserve(size_new);
		newStr.append(left);
		newStr.append(right);
		return newStr;
	}
	
	template<typename Char>
	BasicString<Char> BasicStringUtils::concat(const BasicString<Char>& left, Char right) {
		size_t size_new = get_safe_resize<Char>(left.size(), 1);
		BasicString<Char> newStr;
		newStr.reserve(size_new);
		newStr.append(left);
		newStr.append(1, right);
		return newStr;
	}
	
	template<typename Char>
	BasicString<Char> BasicStringUtils::concat(Char left, const BasicString<Char>& right) {
		size_t size_new = get_safe_resize<Char>(1, right.size());
		BasicString<Char> newStr;
		newStr.reserve(size_new);
		newStr.append(1, left);
		newStr.append(right);
		return newStr;
	}
	
	#ifdef __OBJC__
	
	template<typename Char,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<Char>::value
			&& sizeof(unichar)==sizeof(Char) && sizeof(Char)!=sizeof(char)), std::nullptr_t>::type>
	BasicString<Char> BasicStringUtils::concat(const BasicString<Char>& left, NSString* right) {
		if(right == nil) {
			return left;
		}
		NSUInteger right_length = right.length;
		size_t right_size = (size_t)right_length;
		size_t size_new = get_safe_resize<Char>(left.size(), right_size);
		BasicString<Char> newStr;
		newStr.reserve(size_new);
		newStr.append(left);
		newStr.resize(size_new);
		NSRange range = NSMakeRange(0, right_length);
		[right getCharacters:(unichar*)(newStr.data()+left.size()) range:range];
		return newStr;
	}
	
	template<typename Char,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<Char>::value
			&& sizeof(Char)==sizeof(char)), std::nullptr_t>::type>
	BasicString<Char> BasicStringUtils::concat(const BasicString<Char>& left, NSString* right) {
		if(right == nil) {
			return left;
		}
		NSUInteger right_length = [right lengthOfBytesUsingEncoding:NSUTF8StringEncoding];
		size_t right_size = (size_t)right_length;
		size_t size_new = get_safe_resize<Char>(left.size(), right_size);
		BasicString<Char> newStr;
		newStr.reserve(size_new);
		newStr.append(left);
		const Char* right_chars = (const Char*)[right UTF8String];
		newStr.append(right_chars, right_size);
		return newStr;
	}
	
	template<typename Char,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<Char>::value
			&& sizeof(unichar)!=sizeof(Char) && sizeof(Char)!=sizeof(char)), std::nullptr_t>::type>
	BasicString<Char> BasicStringUtils::concat(const BasicString<Char>& left, NSString* right) {
		if(right == nil) {
			return left;
		}
		NSUInteger right_length = right.length;
		auto buffer = std::make_unique<unichar[]>((size_t)right.length);
		NSRange range = NSMakeRange(0, right_length);
		[right getCharacters:buffer.get() range:range];
		auto right_str = convert<Char,unichar>(buffer.get(), (size_t)right_length);
		buffer = nullptr;
		size_t size_new = get_safe_resize<Char>(left.size(), right_str.length());
		BasicString<Char> newStr;
		newStr.reserve(size_new);
		newStr.append(left);
		newStr.append(right_str);
		return newStr;
	}
	
	template<typename Char,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<Char>::value
			&& sizeof(unichar)==sizeof(Char) && sizeof(Char)!=sizeof(char)), std::nullptr_t>::type>
	BasicString<Char> BasicStringUtils::concat(NSString* left, const BasicString<Char>& right) {
		if(left == nil) {
			return right;
		}
		size_t left_size = (size_t)left.length;
		size_t size_new = get_safe_resize<Char>(left_size, right.size());
		BasicString<Char> newStr;
		newStr.reserve(size_new);
		newStr.resize(left_size);
		NSRange range = NSMakeRange(0, (NSUInteger)left_size);
		[left getCharacters:(unichar*)newStr.data() range:range];
		newStr.append(right);
		return newStr;
	}
	
	template<typename Char,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<Char>::value
			&& sizeof(Char)==sizeof(char)), std::nullptr_t>::type>
	BasicString<Char> BasicStringUtils::concat(NSString* left, const BasicString<Char>& right) {
		if(left == nil) {
			return right;
		}
		NSUInteger left_length = [left lengthOfBytesUsingEncoding:NSUTF8StringEncoding];
		size_t left_size = (size_t)left_length;
		size_t size_new = get_safe_resize<Char>(left_size, right.size());
		BasicString<Char> newStr;
		newStr.reserve(size_new);
		newStr.append((const Char*)[left UTF8String], left_size);
		newStr.append(right);
		return newStr;
	}
	
	template<typename Char,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<Char>::value
			&& sizeof(unichar)!=sizeof(Char) && sizeof(Char)!=sizeof(char)), std::nullptr_t>::type>
	BasicString<Char> BasicStringUtils::concat(NSString* left, const BasicString<Char>& right) {
		if(left == nil) {
			return right;
		}
		NSUInteger left_length = left.length;
		auto buffer = std::make_unique<unichar[]>((size_t)left.length);
		NSRange range = NSMakeRange(0, left_length);
		[left getCharacters:buffer.get() range:range];
		auto left_str = convert<Char,unichar>(buffer.get(), (size_t)left_length);
		buffer = nullptr;
		size_t left_size = left_str.length();
		size_t size_new = get_safe_resize<Char>(left_size, right.size());
		BasicString<Char> newStr;
		newStr.reserve(size_new);
		newStr.append(left_str);
		newStr.append(right);
		return newStr;
	}
	
	#endif
	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char> BasicStringUtils::concat(const BasicString<Char>& left, const OtherChar* right) {
		FGL_ASSERT(right != nullptr, "right cannot be null");
		const Char* right_chars = (const Char*)right;
		size_t right_size = strlen<Char>(right_chars);
		size_t size_new = get_safe_resize<Char>(left.size(), right_size);
		BasicString<Char> newStr;
		newStr.reserve(size_new);
		newStr.append(left);
		newStr.append(right_chars, right_size);
		return newStr;
	}
	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char> BasicStringUtils::concat(const BasicString<Char>& left, const OtherChar* right) {
		FGL_ASSERT(right != nullptr, "right cannot be null");
		auto right_str = convert<Char,OtherChar>(right, strlen<OtherChar>(right));
		size_t size_new = get_safe_resize<Char>(left.size(), right_str.size());
		BasicString<Char> newStr;
		newStr.reserve(size_new);
		newStr.append(left);
		newStr.append(right_str);
		return newStr;
	}
	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char> BasicStringUtils::concat(const Char* left, const BasicString<OtherChar>& right) {
		FGL_ASSERT(left != nullptr, "left cannot be null");
		size_t left_size = strlen<Char>(left);
		size_t size_new = get_safe_resize<Char>(left_size, right.size());
		BasicString<Char> newStr;
		newStr.reserve(size_new);
		newStr.append(left, left_size);
		newStr.append(right);
		return newStr;
	}
	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char> BasicStringUtils::concat(const Char* left, const BasicString<OtherChar>& right) {
		FGL_ASSERT(left != nullptr, "left cannot be null");
		size_t left_size = strlen<Char>(left);
		auto right_str = convert<Char,OtherChar>(right.data(), right.size());\
		size_t size_new = get_safe_resize<Char>(left_size, right_str.size());
		BasicString<Char> newStr;
		newStr.reserve(size_new);
		newStr.append(left, left_size);
		newStr.append(right);
		return newStr;
	}
	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::same_size_convertable_with_char_type<Char,OtherChar>::null_type>
	BasicString<Char> BasicStringUtils::concat(const BasicString<Char>& left, OtherChar right) {
		size_t size_new = get_safe_resize<Char>(left.size(), 1);
		BasicString<Char> newStr;
		newStr.reserve(size_new);
		newStr.append(left);
		newStr.append(1,(Char)right);
		return newStr;
	}
	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::diff_size_convertable_with_char_type<Char,OtherChar>::null_type>
	BasicString<Char> BasicStringUtils::concat(const BasicString<Char>& left, OtherChar right) {
		auto right_str = convert<Char,OtherChar>(std::basic_string<OtherChar>(1,right).data(), 1);
		size_t right_size = right_str.length();
		size_t size_new = get_safe_resize<Char>(left.size(), right_size);
		BasicString<Char> newStr;
		newStr.reserve(size_new);
		newStr.append(left);
		newStr.append(right_str);
		return newStr;
	}
	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::same_size_convertable_with_char_type<Char,OtherChar>::null_type>
	BasicString<Char> BasicStringUtils::concat(Char left, const BasicString<OtherChar>& right) {
		size_t size_new = get_safe_resize<Char>(1, right.size());
		BasicString<Char> newStr;
		newStr.reserve(size_new);
		newStr.append(1,left);
		newStr.append(right);
		return newStr;
	}
	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::diff_size_convertable_with_char_type<Char,OtherChar>::null_type>
	BasicString<Char> BasicStringUtils::concat(Char left, const BasicString<OtherChar>& right) {
		auto right_str = convert<Char,OtherChar>(right.data(), right.size());
		size_t size_new = get_safe_resize<Char>(1, right_str.size());
		BasicString<Char> newStr;
		newStr.reserve(size_new);
		newStr.append(1,left);
		newStr.append(right);
		return newStr;
	}
	
	template<typename Char, typename Bool,
		typename BasicStringUtils::string_type_convertable_with_bool<Char,Bool>::null_type>
	BasicString<Char> BasicStringUtils::concat(const BasicString<Char>& left, Bool right) {
		auto right_str = fromBool<Char>(right);
		size_t size_new = get_safe_resize<Char>(left.size(), right_str.size());
		BasicString<Char> newStr;
		newStr.reserve(size_new);
		newStr.append(left);
		newStr.append(right_str);
		return newStr;
	}
	
	template<typename Char, typename Bool,
		typename BasicStringUtils::string_type_convertable_with_bool<Char,Bool>::null_type>
	BasicString<Char> BasicStringUtils::concat(Bool left, const BasicString<Char>& right) {
		auto left_str = fromBool<Char>(left);
		size_t size_new = get_safe_resize<Char>(left_str.size(), right.size());
		BasicString<Char> newStr;
		newStr.reserve(size_new);
		newStr.append(left_str);
		newStr.append(right);
		return newStr;
	}
	
	template<typename Char, typename Num,
		typename BasicStringUtils::string_type_convertable_with_number_or_enum<Char,Num>::null_type>
	BasicString<Char> BasicStringUtils::concat(const BasicString<Char>& left, Num right) {
		auto right_str = fromNumber<Char,Num>(right);
		size_t size_new = get_safe_resize<Char>(left.size(), right_str.size());
		BasicString<Char> newStr;
		newStr.reserve(size_new);
		newStr.append(left);
		newStr.append(right_str);
		return newStr;
	}
	
	template<typename Char, typename Num,
		typename BasicStringUtils::string_type_convertable_with_number_or_enum<Char,Num>::null_type>
	BasicString<Char> BasicStringUtils::concat(Num left, const BasicString<Char>& right) {
		auto left_str = fromNumber<Char,Num>(left);
		size_t size_new = get_safe_resize<Char>(left_str.size(), right.size());
		BasicString<Char> newStr;
		newStr.reserve(size_new);
		newStr.append(left_str);
		newStr.append(right);
		return newStr;
	}
	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::same_size_convertable_strings<Char, OtherChar>::null_type>
	BasicString<Char> BasicStringUtils::concat(const BasicString<Char>& left, const BasicString<OtherChar>& right) {
		size_t size_new = get_safe_resize<Char>(left.size(), right.size());
		BasicString<Char> newStr;
		newStr.reserve(size_new);
		newStr.append(left);
		newStr.append(right);
		return newStr;
	}
	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::diff_size_convertable_strings<Char, OtherChar>::null_type>
	BasicString<Char> BasicStringUtils::concat(const BasicString<Char>& left, const BasicString<OtherChar>& right) {
		auto right_str = convert<Char,OtherChar>(right.characters, right.size);
		size_t size_new = get_safe_resize<Char>(left.size(), right_str.size());
		BasicString<Char> newStr;
		newStr.reserve(size_new);
		newStr.append(left);
		newStr.append(right_str);
		return newStr;
	}
	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::same_size_convertable_strings<Char, OtherChar>::null_type>
	BasicString<Char> BasicStringUtils::concat(const BasicString<Char>& left, const std::basic_string<OtherChar>& right) {
		size_t right_size = right.size();
		size_t size_new = get_safe_resize<Char>(left.size(), right_size);
		BasicString<Char> newStr;
		newStr.reserve(size_new);
		newStr.append(left);
		newStr.append((const Char*)right.data(), right_size);
		return newStr;
	}
	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::diff_size_convertable_strings<Char, OtherChar>::null_type>
	BasicString<Char> BasicStringUtils::concat(const BasicString<Char>& left, const std::basic_string<OtherChar>& right) {
		auto right_str = convert<Char,OtherChar>(right.data(), right.length());
		size_t size_new = get_safe_resize<Char>(left.size(), right_str.size());
		BasicString<Char> newStr;
		newStr.reserve(size_new);
		newStr.append(left);
		newStr.append(right_str);
		return newStr;
	}
	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::same_size_convertable_strings<Char, OtherChar>::null_type>
	BasicString<Char> BasicStringUtils::concat(const std::basic_string<Char>& left, const BasicString<OtherChar>& right) {
		size_t size_new = get_safe_resize<Char>(left.size(), right.size());
		BasicString<Char> newStr;
		newStr.reserve(size_new);
		newStr.append(left);
		newStr.append((const Char*)right.data(), right.size());
		return newStr;
	}
	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::diff_size_convertable_strings<Char, OtherChar>::null_type>
	BasicString<Char> BasicStringUtils::concat(const std::basic_string<Char>& left, const BasicString<OtherChar>& right) {
		auto right_str = convert<Char,OtherChar>(right.data(), right.size());
		size_t size_new = get_safe_resize<Char>(left.size(), right_str.size());
		BasicString<Char> newStr;
		newStr.reserve(size_new);
		newStr.append(left);
		newStr.append(right_str);
		return newStr;
	}
}
