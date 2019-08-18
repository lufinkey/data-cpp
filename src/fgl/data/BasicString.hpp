//
//  BasicString.hpp
//  DataCpp
//
//  Created by Luis Finke on 8/14/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#include <locale>
#include <regex>
#include <string>
#ifdef __OBJC__
	#import <Foundation/Foundation.h>
#endif
#include <fgl/data/Common.hpp>
#include <fgl/data/BasicStringUtils.hpp>

namespace fgl {
	template<typename Char>
	class BasicString {
		template<typename OtherChar>
		friend class BasicString;
		friend class BasicStringUtils;
	public:
		typedef Char CharType;
		typedef BasicString<Char> StringType;
		static constexpr size_t NOT_FOUND = (size_t)-1;
		
		BasicString();
		BasicString(const Char* str, size_t length);
		BasicString(const Char* str);
		BasicString(const BasicString<Char>& str);
		BasicString(BasicString<Char>&& str);
		BasicString(const std::basic_string<Char>& str);
		BasicString(Char c);
		
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
		
		template<typename OtherChar,
			typename BasicStringUtils::same_size_convertable_with_char_type<Char,OtherChar>::null_type = nullptr>
		BasicString(OtherChar c);
		template<typename OtherChar,
			typename BasicStringUtils::diff_size_convertable_with_char_type<Char,OtherChar>::null_type = nullptr>
		BasicString(OtherChar c);
		
		template<typename OtherChar,
			typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		BasicString(const OtherChar* str, size_t length);
		template<typename OtherChar,
			typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		BasicString(const OtherChar* str, size_t length);
		
		template<typename OtherChar,
			typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		explicit BasicString(const OtherChar* str);
		template<typename OtherChar,
			typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		explicit BasicString(const OtherChar* str);
		
		template<typename OtherChar,
			typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		explicit BasicString(const BasicString<OtherChar>& str);
		template<typename OtherChar,
			typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		explicit BasicString(BasicString<OtherChar>&& str);
		template<typename OtherChar,
			typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		explicit BasicString(const BasicString<OtherChar>& str);
		
		template<typename OtherChar,
			typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		explicit BasicString(const std::basic_string<OtherChar>& str);
		template<typename OtherChar,
			typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		explicit BasicString(const std::basic_string<OtherChar>& str);
		
		~BasicString();
		
		
		
		operator const Char*() const;
		operator std::basic_string<Char>() const;
		
		#ifdef __OBJC__
		template<typename _Char=Char,
			typename BasicStringUtils::is_same<_Char,Char>::null_type = nullptr,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
				&& sizeof(unichar)==sizeof(_Char) && sizeof(_Char)!=sizeof(char)), std::nullptr_t>::type = nullptr>
		NSString* toNSString() const;
		template<typename _Char=Char,
			typename BasicStringUtils::is_same<_Char,Char>::null_type = nullptr,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
				&& sizeof(_Char)==sizeof(char)), std::nullptr_t>::type = nullptr>
		NSString* toNSString() const;
		template<typename _Char=Char,
			typename BasicStringUtils::is_same<_Char,Char>::null_type = nullptr,
			typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
				&& sizeof(unichar)!=sizeof(_Char) && sizeof(_Char)!=sizeof(char)), std::nullptr_t>::type = nullptr>
		NSString* toNSString() const;
		
		template<typename _Char=Char,
			typename BasicStringUtils::is_same<_Char,Char>::null_type = nullptr,
			typename BasicStringUtils::can_convert_string_type<_Char>::null_type = nullptr>
		explicit operator NSString*() const;
		#endif
		
		template<typename OtherChar,
			typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		std::basic_string<OtherChar> toStdString() const;
		template<typename OtherChar,
			typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		std::basic_string<OtherChar> toStdString() const;
		template<typename SameChar,
			typename BasicStringUtils::is_same<Char,SameChar>::null_type = nullptr>
		std::basic_string<SameChar> toStdString() const;
		
		template<typename SomeChar,
			typename BasicStringUtils::can_convert_string_types<Char,SomeChar>::null_type = nullptr>
		BasicString<SomeChar> toBasicString() const;
		template<typename _Char=Char,
			typename BasicStringUtils::is_same<_Char,Char>::null_type = nullptr,
			typename BasicStringUtils::can_convert_string_type<Char>::null_type = nullptr>
		BasicString<char> toString() const;
		
		
		
		void assign(const Char* str, size_t length);
		void assign(const Char* str);
		
		BasicString<Char>& operator=(const Char* str);
		BasicString<Char>& operator=(const BasicString<Char>& str);
		BasicString<Char>& operator=(BasicString<Char>&& str);
		BasicString<Char>& operator=(const std::basic_string<Char>& str);
		BasicString<Char>& operator=(Char c);
		
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
		BasicString<Char>& operator=(OtherChar c);
		template<typename OtherChar,
			typename BasicStringUtils::diff_size_convertable_with_char_type<Char, OtherChar>::null_type = nullptr>
		BasicString<Char>& operator=(OtherChar c);
		
		template<typename OtherChar,
			typename BasicStringUtils::same_size_convertable_strings<Char, OtherChar>::null_type = nullptr>
		BasicString<Char>& operator=(const OtherChar* str);
		template<typename OtherChar,
			typename BasicStringUtils::diff_size_convertable_strings<Char, OtherChar>::null_type = nullptr>
		BasicString<Char>& operator=(const OtherChar* str);
		
		template<typename OtherChar, typename BasicStringUtils::same_size_convertable_strings<Char, OtherChar>::null_type = nullptr>
		BasicString<Char>& operator=(const BasicString<OtherChar>& str);
		template<typename OtherChar, typename BasicStringUtils::diff_size_convertable_strings<Char, OtherChar>::null_type = nullptr>
		BasicString<Char>& operator=(const BasicString<OtherChar>& str);
		template<typename OtherChar, typename BasicStringUtils::same_size_convertable_strings<Char, OtherChar>::null_type = nullptr>
		BasicString<Char>& operator=(const std::basic_string<OtherChar>& str);
		template<typename OtherChar, typename BasicStringUtils::diff_size_convertable_strings<Char, OtherChar>::null_type = nullptr>
		BasicString<Char>& operator=(const std::basic_string<OtherChar>& str);
		
		
		
		void append(const Char* str, size_t length);
		void append(const Char* str);
		void append(Char c);
		
		BasicString<Char>& operator+=(const Char* str);
		BasicString<Char>& operator+=(const BasicString<Char>& str);
		BasicString<Char>& operator+=(const std::basic_string<Char>& str);
		BasicString<Char>& operator+=(Char c);
		
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
		BasicString<Char>& operator+=(OtherChar c);
		template<typename OtherChar,
			typename BasicStringUtils::diff_size_convertable_with_char_type<Char,OtherChar>::null_type = nullptr>
		BasicString<Char>& operator+=(OtherChar c);
		
		template<typename OtherChar,
			typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		BasicString<Char>& operator+=(const OtherChar* str);
		template<typename OtherChar,
			typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		BasicString<Char>& operator+=(const OtherChar* str);
		
		template<typename OtherChar,
			typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		BasicString<Char>& operator+=(const BasicString<OtherChar>& str);
		template<typename OtherChar,
			typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		BasicString<Char>& operator+=(const BasicString<OtherChar>& str);
		template<typename OtherChar,
			typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		BasicString<Char>& operator+=(const std::basic_string<OtherChar>& str);
		template<typename OtherChar,
			typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type = nullptr>
		BasicString<Char>& operator+=(const std::basic_string<OtherChar>& str);
		
		template<typename Bool,
			typename BasicStringUtils::string_type_convertable_with_bool<Char,Bool>::null_type = nullptr>
		BasicString<Char>& operator+=(Bool);
		template<typename Num,
			typename BasicStringUtils::string_type_convertable_with_number_or_enum<Char,Num>::null_type = nullptr>
		BasicString<Char>& operator+=(Num);
		
		
		
		int compare(const Char* cmp, size_t length, const std::locale& locale = std::locale()) const;
		int compare(const Char* cmp, const std::locale& locale = std::locale()) const;
		int compare(const BasicString<Char>& cmp, const std::locale& locale = std::locale()) const;
		
		bool equals(const Char* str, size_t length) const;
		bool equals(const Char* str) const;
		bool equals(const BasicString<Char>& str) const;
		
		
		
		const Char* data() const;
		size_t length() const;
		void clear();
		void resize(size_t size);
		Char& charAt(size_t index);
		Char& operator[](size_t index);
		const Char& charAt(size_t index) const;
		const Char& operator[](size_t index) const;
		
		size_t indexOf(Char find, size_t startIndex=0) const;
		size_t indexOf(const BasicString<Char>& find, size_t startIndex=0) const;
		size_t indexOf(const Char* find, size_t startIndex=0) const;
		size_t lastIndexOf(Char find, size_t startIndex) const;
		size_t lastIndexOf(Char find) const;
		size_t lastIndexOf(const BasicString<Char>& find, size_t startIndex) const;
		size_t lastIndexOf(const BasicString<Char>& find) const;
		size_t lastIndexOf(const Char* find, size_t startIndex) const;
		size_t lastIndexOf(const Char* find) const;
		
		bool startsWith(const Char* str) const;
		bool startsWith(const Char* str, size_t length) const;
		bool startsWith(const BasicString<Char>& str) const;
		bool endsWith(const Char* str) const;
		bool endsWith(const Char* str, size_t length) const;
		bool endsWith(const BasicString<Char>& str) const;
		
		
		
		BasicString<Char> replace(const Char& find, const Char& replace) const;
		BasicString<Char> replace(const BasicString<Char>& find, const BasicString<Char>& replace) const;
		BasicString<Char> replace(const std::basic_regex<Char>& find, const BasicString<Char>& replace) const;
		BasicString<Char> replace(size_t startIndex, size_t endIndex, const Char* replace) const;
		BasicString<Char> replace(size_t startIndex, size_t endIndex, const BasicString<Char>& replace) const;
		
		BasicString<Char> substring(size_t startIndex, size_t endIndex) const;
		BasicString<Char> substring(size_t startIndex) const;
		
		LinkedList<BasicString<Char>> split(Char delim) const;
		LinkedList<BasicString<Char>> split(const Char* delim) const;
		LinkedList<BasicString<Char>> split(const BasicString<Char>& delim) const;
		
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
		
		
		
	private:
		Char* characters;
		size_t size;
	};
	
	
	
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
