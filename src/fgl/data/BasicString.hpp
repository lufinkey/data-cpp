//
//  BasicString.hpp
//  DataCpp
//
//  Created by Luis Finke on 8/14/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#include <initializer_list>
#include <list>
#include <locale>
#include <regex>
#include <string>
#ifdef __OBJC__
	#import <Foundation/Foundation.h>
#endif
#include <fgl/data/Common.hpp>
#include <fgl/data/BasicStringUtils.hpp>

namespace fgl {
	template<typename T, template<typename...> typename Storage>
	class LinkedList;
	
	template<typename Char>
	class BasicString {
		template<typename OtherChar>
		friend class BasicString;
		friend class BasicStringUtils;
	public:
		typedef Char CharType;
		typedef BasicString<Char> StringType;
		typedef std::basic_string<Char> StorageType;
		
		typedef Char char_type;
		typedef typename StorageType::size_type size_type;
		
		static const size_type npos = StorageType::npos;
		static const size_type NOT_FOUND = StorageType::npos;
		
		std::basic_string<Char> storage;
		
		inline BasicString() noexcept;
		inline BasicString(size_type count, Char c);
		inline BasicString(const Char* str, size_type length);
		inline BasicString(const Char* str);
		inline BasicString(const std::basic_string<Char>& str);
		inline BasicString(std::basic_string<Char>&& str) noexcept;
		inline BasicString(Char c);
		inline BasicString(std::initializer_list<Char>);
		template<typename InputIt>
		inline BasicString(InputIt first, InputIt last);
		
		#ifdef __OBJC__
		template<typename _Char=Char,
			typename BasicStringUtils::is_same<_Char,Char>::null_type = nullptr,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
				&& sizeof(unichar)==sizeof(_Char) && sizeof(_Char)!=sizeof(char)), std::nullptr_t>::type = nullptr>
		explicit BasicString(NSString* nsString);
		template<typename _Char=Char,
			typename BasicStringUtils::is_same<_Char,Char>::null_type = nullptr,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
				&& sizeof(_Char)==sizeof(char)), std::nullptr_t>::type = nullptr>
		explicit BasicString(NSString* nsString);
		template<typename _Char=Char,
			typename BasicStringUtils::is_same<_Char,Char>::null_type = nullptr,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
				&& sizeof(unichar)!=sizeof(_Char) && sizeof(_Char)!=sizeof(char)), std::nullptr_t>::type = nullptr>
		explicit BasicString(NSString* nsString);
		#endif

		#ifdef JNIEXPORT
		BasicString(JNIEnv* env, jstring javaString);
		#endif
		
		#ifdef NODE_API_MODULE
		BasicString(Napi::String napiString);
		#endif
		#ifdef NAPI_MODULE
		BasicString(napi_env env, napi_value napiString);
		#endif
		
		template<typename OtherChar,
			typename BasicStringUtils::same_size_convertable_with_char_type<Char,OtherChar>::null_type = nullptr>
		inline explicit BasicString(OtherChar c);
		template<typename OtherChar,
			typename BasicStringUtils::diff_size_convertable_with_char_type<Char,OtherChar>::null_type = nullptr>
		inline explicit BasicString(OtherChar c);
		
		template<typename OtherChar,
			typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		inline BasicString(const OtherChar* str, size_type length);
		template<typename OtherChar,
			typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		inline BasicString(const OtherChar* str, size_type length);
		
		template<typename OtherChar,
			typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		inline explicit BasicString(const OtherChar* str);
		template<typename OtherChar,
			typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		inline explicit BasicString(const OtherChar* str);
		
		template<typename OtherChar,
			typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		inline explicit BasicString(const BasicString<OtherChar>& str);
		template<typename OtherChar,
			typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		inline explicit BasicString(BasicString<OtherChar>&& str);
		template<typename OtherChar,
			typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		inline explicit BasicString(const BasicString<OtherChar>& str);
		
		template<typename OtherChar,
			typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		inline explicit BasicString(const std::basic_string<OtherChar>& str);
		template<typename OtherChar,
			typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		inline explicit BasicString(std::basic_string<OtherChar>&& str);
		template<typename OtherChar,
			typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		inline explicit BasicString(const std::basic_string<OtherChar>& str);
		
		
		
		inline explicit operator const Char*() const noexcept;
		inline operator std::basic_string<Char>&() noexcept;
		inline operator const std::basic_string<Char>&() const noexcept;
		inline operator const std::basic_string_view<Char>() const noexcept;
		
		
		#ifdef __OBJC__
		template<typename _Char=Char,
			typename BasicStringUtils::is_same<_Char,Char>::null_type = nullptr,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
				&& sizeof(unichar)==sizeof(_Char) && sizeof(_Char)!=sizeof(char)), std::nullptr_t>::type = nullptr>
		inline NSString* toNSString() const;
		template<typename _Char=Char,
			typename BasicStringUtils::is_same<_Char,Char>::null_type = nullptr,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
				&& sizeof(_Char)==sizeof(char)), std::nullptr_t>::type = nullptr>
		inline NSString* toNSString() const;
		template<typename _Char=Char,
			typename BasicStringUtils::is_same<_Char,Char>::null_type = nullptr,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
				&& sizeof(unichar)!=sizeof(_Char) && sizeof(_Char)!=sizeof(char)), std::nullptr_t>::type = nullptr>
		inline NSString* toNSString() const;
		
		template<typename _Char=Char,
			typename BasicStringUtils::is_same<_Char,Char>::null_type = nullptr,
			typename BasicStringUtils::can_convert_string_type<_Char>::null_type = nullptr>
		inline explicit operator NSString*() const;
		#endif

		#ifdef JNIEXPORT
		inline jstring toJavaString(JNIEnv* env) const;
		#endif
		
		#ifdef NODE_API_MODULE
		Napi::String toNapiString(napi_env) const;
		#endif
		#ifdef NAPI_MODULE
		napi_value toNodeJSValue(napi_env) const;
		#endif
		
		template<typename OtherChar,
			typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		inline std::basic_string<OtherChar> toStdString() const;
		template<typename OtherChar,
			typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		inline std::basic_string<OtherChar> toStdString() const;
		template<typename SameChar,
			typename BasicStringUtils::is_same<Char,SameChar>::null_type = nullptr>
		inline std::basic_string<SameChar> toStdString() const;
		
		template<typename SomeChar,
			typename BasicStringUtils::can_convert_string_types<Char,SomeChar>::null_type = nullptr>
		inline BasicString<SomeChar> toBasicString() const;
		
		template<typename _Char=Char,
			typename BasicStringUtils::is_same<_Char,Char>::null_type = nullptr,
			typename BasicStringUtils::can_convert_string_type<Char>::null_type = nullptr>
		inline BasicString<char> toString() const;
		
		
		
		inline BasicString<Char>& assign(const BasicString<Char>& str);
		inline BasicString<Char>& assign(BasicString<Char>&& str);
		inline BasicString<Char>& assign(const std::basic_string<Char>& str);
		inline BasicString<Char>& assign(std::basic_string<Char>&& str) noexcept;
		inline BasicString<Char>& assign(const Char* str, size_type length);
		inline BasicString<Char>& assign(const Char* str);
		inline BasicString<Char>& assign(size_type count, Char c);
		template<typename InputIt>
		inline BasicString<Char>& assign(InputIt first, InputIt last);
		inline BasicString<Char>& assign(std::initializer_list<Char>);
		
		inline BasicString<Char>& operator=(const std::basic_string<Char>& str);
		inline BasicString<Char>& operator=(std::basic_string<Char>&& str) noexcept;
		inline BasicString<Char>& operator=(const Char* str);
		inline BasicString<Char>& operator=(Char c);
		inline BasicString<Char>& operator=(std::initializer_list<Char>);
		
		#ifdef __OBJC__
		template<typename _Char=Char,
			typename BasicStringUtils::is_same<_Char, Char>::null_type = nullptr,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
				&& sizeof(unichar)==sizeof(_Char) && sizeof(_Char)!=sizeof(char)), std::nullptr_t>::type = nullptr>
		BasicString<Char>& operator=(NSString* nsString);
		template<typename _Char=Char,
			typename BasicStringUtils::is_same<_Char, Char>::null_type = nullptr,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
				&& sizeof(_Char)==sizeof(char)), std::nullptr_t>::type = nullptr>
		BasicString<Char>& operator=(NSString* nsString);
		template<typename _Char=Char,
			typename BasicStringUtils::is_same<_Char, Char>::null_type = nullptr,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
				&& sizeof(unichar)!=sizeof(_Char) && sizeof(_Char)!=sizeof(char)), std::nullptr_t>::type = nullptr>
		BasicString<Char>& operator=(NSString* nsString);
		#endif
		
		template<typename OtherChar,
			typename BasicStringUtils::same_size_convertable_with_char_type<Char, OtherChar>::null_type = nullptr>
		inline BasicString<Char>& operator=(OtherChar c);
		template<typename OtherChar,
			typename BasicStringUtils::diff_size_convertable_with_char_type<Char, OtherChar>::null_type = nullptr>
		inline BasicString<Char>& operator=(OtherChar c);
		
		template<typename OtherChar,
			typename BasicStringUtils::same_size_convertable_strings<Char, OtherChar>::null_type = nullptr>
		inline BasicString<Char>& operator=(const OtherChar* str);
		template<typename OtherChar,
			typename BasicStringUtils::diff_size_convertable_strings<Char, OtherChar>::null_type = nullptr>
		inline BasicString<Char>& operator=(const OtherChar* str);
		
		template<typename OtherChar, typename BasicStringUtils::same_size_convertable_strings<Char, OtherChar>::null_type = nullptr>
		inline BasicString<Char>& operator=(const BasicString<OtherChar>& str);
		template<typename OtherChar, typename BasicStringUtils::diff_size_convertable_strings<Char, OtherChar>::null_type = nullptr>
		inline BasicString<Char>& operator=(const BasicString<OtherChar>& str);
		template<typename OtherChar, typename BasicStringUtils::same_size_convertable_strings<Char, OtherChar>::null_type = nullptr>
		inline BasicString<Char>& operator=(const std::basic_string<OtherChar>& str);
		template<typename OtherChar, typename BasicStringUtils::diff_size_convertable_strings<Char, OtherChar>::null_type = nullptr>
		inline BasicString<Char>& operator=(const std::basic_string<OtherChar>& str);
		
		
		
		inline BasicString<Char>& append(const BasicString<Char>& str);
		inline BasicString<Char>& append(const std::basic_string<Char>& str);
		inline BasicString<Char>& append(const Char* str, size_type length);
		inline BasicString<Char>& append(const Char* str);
		inline BasicString<Char>& append(size_type count, Char c);
		template<typename InputIt>
		inline BasicString<Char>& append(InputIt first, InputIt last);
		inline BasicString<Char>& append(std::initializer_list<Char>);
		
		inline BasicString<Char>& operator+=(const BasicString<Char>& str);
		inline BasicString<Char>& operator+=(const std::basic_string<Char>& str);
		inline BasicString<Char>& operator+=(const Char* str);
		inline BasicString<Char>& operator+=(Char c);
		inline BasicString<Char>& operator+=(std::initializer_list<Char>);
		
		#ifdef __OBJC__
		template<typename _Char=Char,
			typename BasicStringUtils::is_same<_Char,Char>::null_type = nullptr,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
				&& sizeof(unichar)==sizeof(_Char) && sizeof(_Char)!=sizeof(char)), std::nullptr_t>::type = nullptr>
		BasicString<Char>& operator+=(NSString* nsString);
		template<typename _Char=Char,
			typename BasicStringUtils::is_same<_Char,Char>::null_type = nullptr,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
				&& sizeof(_Char)==sizeof(char)), std::nullptr_t>::type = nullptr>
		BasicString<Char>& operator+=(NSString* nsString);
		template<typename _Char=Char,
			typename BasicStringUtils::is_same<_Char,Char>::null_type = nullptr,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
				&& sizeof(unichar)!=sizeof(_Char) && sizeof(_Char)!=sizeof(char)), std::nullptr_t>::type = nullptr>
		BasicString<Char>& operator+=(NSString* nsString);
		#endif
		
		template<typename OtherChar,
			typename BasicStringUtils::same_size_convertable_with_char_type<Char,OtherChar>::null_type = nullptr>
		inline BasicString<Char>& operator+=(OtherChar c);
		template<typename OtherChar,
			typename BasicStringUtils::diff_size_convertable_with_char_type<Char,OtherChar>::null_type = nullptr>
		inline BasicString<Char>& operator+=(OtherChar c);
		
		template<typename OtherChar,
			typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		inline BasicString<Char>& operator+=(const OtherChar* str);
		template<typename OtherChar,
			typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		inline BasicString<Char>& operator+=(const OtherChar* str);
		
		template<typename OtherChar,
			typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		inline BasicString<Char>& operator+=(const BasicString<OtherChar>& str);
		template<typename OtherChar,
			typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		inline BasicString<Char>& operator+=(const BasicString<OtherChar>& str);
		template<typename OtherChar,
			typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		inline BasicString<Char>& operator+=(const std::basic_string<OtherChar>& str);
		template<typename OtherChar,
			typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		inline BasicString<Char>& operator+=(const std::basic_string<OtherChar>& str);
		
		template<typename Bool,
			typename BasicStringUtils::string_type_convertable_with_bool<Char,Bool>::null_type = nullptr>
		inline BasicString<Char>& operator+=(Bool);
		template<typename Num,
			typename BasicStringUtils::string_type_convertable_with_number_or_enum<Char,Num>::null_type = nullptr>
		inline BasicString<Char>& operator+=(Num);
		
		
		
		inline int compare(const Char* cmp, size_type length) const noexcept;
		inline int compare(const Char* cmp) const noexcept;
		inline int compare(const BasicString<Char>& cmp) const noexcept;
		inline int compare(const std::basic_string<Char>& cmp) const noexcept;
		
		inline int compare(const Char* cmp, size_type length, const std::locale& locale) const;
		inline int compare(const Char* cmp, const std::locale& locale) const;
		inline int compare(const BasicString<Char>& cmp, const std::locale& locale) const;
		inline int compare(const std::basic_string<Char>& cmp, const std::locale& locale) const;
		
		bool equals(const Char* str, size_type length) const;
		inline bool equals(const Char* str) const;
		inline bool equals(const BasicString<Char>& str) const;
		
		
		
		inline Char* data() noexcept;
		inline const Char* data() const noexcept;
		inline const Char* c_str() const noexcept;
		inline size_type size() const noexcept;
		inline size_type length() const noexcept;
		inline size_type maxSize() const noexcept;
		inline bool empty() const noexcept;
		inline void clear();
		inline void resize(size_type size);
		inline void resize(size_type size, Char c);
		inline void reserve(size_type capacity);
		inline Char& charAt(size_type index);
		inline Char& operator[](size_type index);
		inline const Char& charAt(size_type index) const;
		inline const Char& operator[](size_type index) const;
		
		inline size_type indexOf(Char find, size_type startIndex=0) const noexcept;
		inline size_type indexOf(const std::basic_string<Char>& find, size_type startIndex=0) const noexcept;
		inline size_type indexOf(const BasicString<Char>& find, size_type startIndex=0) const noexcept;
		inline size_type indexOf(const Char* find, size_type startIndex=0) const;
		inline size_type lastIndexOf(Char find, size_type startIndex = npos) const noexcept;
		inline size_type lastIndexOf(const std::basic_string<Char>& find, size_type startIndex = npos) const noexcept;
		inline size_type lastIndexOf(const BasicString<Char>& find, size_type startIndex = npos) const noexcept;
		inline size_type lastIndexOf(const Char* find, size_type startIndex = npos) const;
		
		bool startsWith(const Char* str, size_type length) const;
		inline bool startsWith(const Char* str) const;
		inline bool startsWith(const std::basic_string<Char>& str) const;
		inline bool startsWith(const BasicString<Char>& str) const;
		bool endsWith(const Char* str, size_type length) const;
		inline bool endsWith(const Char* str) const;
		inline bool endsWith(const std::basic_string<Char>& str) const;
		inline bool endsWith(const BasicString<Char>& str) const;
		
		
		
		BasicString<Char> replacing(Char find, Char replace) const;
		BasicString<Char> replacing(const std::basic_string<Char>& find, const std::basic_string<Char>& replace) const;
		inline BasicString<Char> replacing(const std::basic_regex<Char>& find, const std::basic_string<Char>& replace, std::regex_constants::match_flag_type flags = std::regex_constants::match_default) const;
		template<typename InputIt>
		BasicString<Char> replacing(size_type startIndex, size_type count, InputIt first, InputIt last) const;
		inline BasicString<Char> replacing(size_type startIndex, size_type count, const std::basic_string<Char>& replace) const;
		
		inline BasicString<Char> substring(size_type startIndex = 0, size_type count = npos) const;
		
		LinkedList<BasicString<Char>,std::list> split(Char delim) const;
		LinkedList<BasicString<Char>,std::list> split(const Char* delim) const;
		LinkedList<BasicString<Char>,std::list> split(const std::basic_string<Char>& delim) const;
		inline LinkedList<BasicString<Char>,std::list> split(const BasicString<Char>& delim) const;
		
		template<typename _Char=Char,
			typename BasicStringUtils::is_same<_Char,Char>::null_type = nullptr,
			typename BasicStringUtils::can_convert_string_type<_Char>::null_type = nullptr>
		BasicString<Char> trim(const std::locale& locale = std::locale()) const;
		
		template<typename _Char=Char,
			typename BasicStringUtils::is_same<_Char,Char>::null_type = nullptr,
			typename BasicStringUtils::can_convert_string_type<_Char>::null_type = nullptr>
		BasicString<Char> toLowerCase(const std::locale& locale = std::locale()) const;
		template<typename _Char=Char,
			typename BasicStringUtils::is_same<_Char,Char>::null_type = nullptr,
			typename BasicStringUtils::can_convert_string_type<_Char>::null_type = nullptr>
		BasicString<Char> toUpperCase(const std::locale& locale = std::locale()) const;
		
		template<typename Num,
			typename BasicStringUtils::string_type_convertable_with_number<Char,Num>::null_type = nullptr>
		Num toArithmeticValue(const std::locale& locale = std::locale()) const;
		
		
		
		template<typename ListType,
			typename std::enable_if<std::is_same<BasicString<Char>,typename ListType::value_type>::value, std::nullptr_t>::type = nullptr>
		static BasicString<Char> join(const ListType& list, const BasicString<Char>& separator = BasicString<Char>());

		template<typename StreamableType>
		static BasicString<Char> stream(StreamableType streamable) {
			std::basic_ostringstream<Char> ss;
			ss << streamable;
			return ss.str();
		}
	};



	template<typename Char>
	std::ostream& operator<<(std::ostream& os, const BasicString<Char>& str);
	
	
	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type = nullptr>
	BasicString<Char> operator+(const BasicString<Char>& left, const BasicString<OtherChar>& right);
	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type = nullptr>
	BasicString<Char> operator+(const BasicString<Char>& left, const OtherChar* right);
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type = nullptr>
	BasicString<Char> operator+(const Char* left, const BasicString<OtherChar>& right);
	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type = nullptr>
	BasicString<Char> operator+(const BasicString<Char>& left, const std::basic_string<OtherChar>& right);
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type = nullptr>
	BasicString<Char> operator+(const std::basic_string<Char>& left, const BasicString<OtherChar>& right);
	
	#ifdef __OBJC__
	template<typename Char,
		typename BasicStringUtils::can_convert_string_types<Char,unichar>::null_type = nullptr>
	BasicString<Char> operator+(const BasicString<Char>& left, NSString* right);
	template<typename Char,
		typename BasicStringUtils::can_convert_string_types<Char,unichar>::null_type = nullptr>
	BasicString<Char> operator+(NSString* left, const BasicString<Char>& right);
	#endif
	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::string_type_convertable_with_char_type<Char,OtherChar>::null_type = nullptr>
	BasicString<Char> operator+(const BasicString<Char>& left, const OtherChar& right);
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::string_type_convertable_with_char_type<Char,OtherChar>::null_type = nullptr>
	BasicString<Char> operator+(const Char& left, const BasicString<OtherChar>& right);
	
	template<typename Char, typename Bool,
		typename BasicStringUtils::string_type_convertable_with_bool<Char,Bool>::null_type = nullptr>
	BasicString<Char> operator+(const BasicString<Char>& left, Bool right);
	template<typename Char, typename Bool,
		typename BasicStringUtils::string_type_convertable_with_bool<Char,Bool>::null_type = nullptr>
	BasicString<Char> operator+(Bool left, const BasicString<Char>& right);
	
	template<typename Char, typename Num,
		typename BasicStringUtils::string_type_convertable_with_number_or_enum<Char,Num>::null_type = nullptr>
	BasicString<Char> operator+(const BasicString<Char>& left, Num right);
	template<typename Char, typename Num,
		typename BasicStringUtils::string_type_convertable_with_number_or_enum<Char,Num>::null_type = nullptr>
	BasicString<Char> operator+(Num left, const BasicString<Char>& right);
	
	template<typename Char>
	bool operator==(const BasicString<Char>& left, const BasicString<Char>& right);
	template<typename Char>
	bool operator==(const BasicString<Char>& left, const std::basic_string<Char>& right);
	template<typename Char>
	bool operator==(const std::basic_string<Char>& left, const BasicString<Char>& right);
	template<typename Char>
	bool operator==(const BasicString<Char>& left, const Char* right);
	template<typename Char>
	bool operator==(const Char* left, const BasicString<Char>& right);
	template<typename Char>
	bool operator==(const BasicString<Char>& left, Char right);
	template<typename Char>
	bool operator==(Char left, const BasicString<Char>& right);
	
	template<typename Char>
	bool operator!=(const BasicString<Char>& left, const BasicString<Char>& right);
	template<typename Char>
	bool operator!=(const BasicString<Char>& left, const Char* right);
	template<typename Char>
	bool operator!=(const Char* left, const BasicString<Char>& right);
	template<typename Char>
	bool operator!=(const BasicString<Char>& left, Char right);
	template<typename Char>
	bool operator!=(Char left, const BasicString<Char>& right);
	
	template<typename Char>
	bool operator<(const BasicString<Char>& left, const BasicString<Char>& right);
	template<typename Char>
	bool operator<(const BasicString<Char>& left, const Char* right);
	template<typename Char>
	bool operator<(const Char* left, const BasicString<Char>& right);
	
	template<typename Char>
	bool operator<=(const BasicString<Char>& left, const BasicString<Char>& right);
	template<typename Char>
	bool operator<=(const BasicString<Char>& left, const Char* right);
	template<typename Char>
	bool operator<=(const Char* left, const BasicString<Char>& right);
	
	template<typename Char>
	bool operator>(const BasicString<Char>& left, const BasicString<Char>& right);
	template<typename Char>
	bool operator>(const BasicString<Char>& left, const Char* right);
	template<typename Char>
	bool operator>(const Char* left, const BasicString<Char>& right);
	
	template<typename Char>
	bool operator>=(const BasicString<Char>& left, const BasicString<Char>& right);
	template<typename Char>
	bool operator>=(const BasicString<Char>& left, const Char* right);
	template<typename Char>
	bool operator>=(const Char* left, const BasicString<Char>& right);
	
	
	
	typedef BasicString<std::string::value_type> String;
	typedef BasicString<std::wstring::value_type> WideString;
}
