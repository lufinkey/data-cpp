//
//  BasicString.hpp
//  DataCpp
//
//  Created by Luis Finke on 8/14/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#include <fgl/data/Common.hpp>
#include <fgl/data/BasicStringUtils.hpp>
#include <fgl/data/Traits.hpp>
#include <initializer_list>
#include <list>
#include <locale>
#include <regex>
#include <string>
#include <string_view>
#ifdef __OBJC__
	#import <Foundation/Foundation.h>
#endif

namespace fgl {
	template<typename T>
	class LinkedList;

	template<typename Char>
	using BasicStringView = std::basic_string_view<Char>;

	template<typename Char>
	class BasicString: public std::basic_string<Char> {
		template<typename OtherChar>
		friend class BasicString;
		friend class BasicStringUtils;
	public:
		using CharType = Char;
		using BaseType = std::basic_string<Char>;
		
		using typename BaseType::size_type;
		using typename BaseType::reference;
		using typename BaseType::const_reference;
		
		using BaseType::npos;
		static const size_type NOT_FOUND = BaseType::npos;
		
		using BaseType::BaseType;
		using BaseType::assign;
		using BaseType::operator=;
		using BaseType::append;
		using BaseType::operator+=;
		using BaseType::compare;
		using BaseType::data;
		using BaseType::c_str;
		using BaseType::size;
		using BaseType::length;
		using BaseType::max_size;
		using BaseType::empty;
		using BaseType::clear;
		using BaseType::resize;
		using BaseType::reserve;
		using BaseType::at;
		using BaseType::substr;
		using BaseType::find;
		using BaseType::replace;
		using BaseType::operator std::basic_string_view<Char>;
		
		BasicString(const BaseType&);
		BasicString(BaseType&&);
		
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
		inline operator std::basic_string<Char>&() & noexcept;
		inline operator std::basic_string<Char>&&() && noexcept;
		inline operator const std::basic_string<Char>&() const& noexcept;
		
		
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
			typename BasicStringUtils::can_convert_string_types<Char,char>::null_type = nullptr>
		inline BasicString<char> toString() const;
		
		
		
		BasicString<Char>& operator=(const BaseType&);
		BasicString<Char>& operator=(BaseType&&);
		
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
		
		
		
		inline int compare(const Char* cmp, size_type length, const std::locale& locale) const;
		inline int compare(const Char* cmp, const std::locale& locale) const;
		inline int compare(const std::basic_string<Char>& cmp, const std::locale& locale) const;
		
		constexpr bool equals(const Char* str, size_type length) const;
		inline bool equals(const Char* str) const;
		inline bool equals(const std::basic_string<Char>& str) const;
		
		
		
		inline size_type maxSize() const noexcept;
		inline reference operator[](size_type index);
		inline const_reference operator[](size_type index) const;
		inline reference charAt(size_type index);
		inline const_reference charAt(size_type index) const;
		
		inline size_type indexOf(Char find, size_type startIndex=0) const noexcept;
		inline size_type indexOf(const std::basic_string<Char>& find, size_type startIndex=0) const noexcept;
		inline size_type indexOf(const Char* find, size_type startIndex=0) const;
		inline size_type lastIndexOf(Char find, size_type startIndex = npos) const noexcept;
		inline size_type lastIndexOf(const std::basic_string<Char>& find, size_type startIndex = npos) const noexcept;
		inline size_type lastIndexOf(const Char* find, size_type startIndex = npos) const;
		
		bool startsWith(const Char* str, size_type length) const;
		inline bool startsWith(const Char* str) const;
		inline bool startsWith(const std::basic_string<Char>& str) const;
		bool endsWith(const Char* str, size_type length) const;
		inline bool endsWith(const Char* str) const;
		inline bool endsWith(const std::basic_string<Char>& str) const;
		
		
		
		BasicString<Char> replacing(Char find, Char replace) const;
		BasicString<Char> replacing(const std::basic_string<Char>& find, const std::basic_string<Char>& replace) const;
		inline BasicString<Char> replacing(const std::basic_regex<Char>& find, const std::basic_string<Char>& replace, std::regex_constants::match_flag_type flags = std::regex_constants::match_default) const;
		template<typename InputIt>
		BasicString<Char> replacing(size_type startIndex, size_type count, InputIt first, InputIt last) const;
		inline BasicString<Char> replacing(size_type startIndex, size_type count, const std::basic_string<Char>& replace) const;

		inline BasicString<Char> substring(size_type startIndex = 0, size_type count = npos) const;
		inline BasicStringView<Char> viewSubstring(size_type startIndex, size_type count = npos) const;
		
		LinkedList<BasicString<Char>> split(Char delim) const;
		LinkedList<BasicString<Char>> split(const Char* delim) const;
		LinkedList<BasicString<Char>> split(const std::basic_string<Char>& delim) const;
		
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
		
		bool isDigits(std::locale locale = std::locale()) const;
		
		template<typename Num,
			typename BasicStringUtils::string_type_convertable_with_number<Char,Num>::null_type = nullptr>
		Num toArithmeticValue(const std::locale& locale = std::locale()) const;
		
		
		
		template<typename Collection, typename = IsCollectionOf<BasicString<Char>,std::remove_reference_t<Collection>>>
		static BasicString<Char> join(Collection&& collection, const BasicString<Char>& separator = BasicString<Char>());
		static BasicString<Char> join(std::initializer_list<BasicString<Char>> list, const BasicString<Char>& separator = BasicString<Char>());
		
		static BasicString<Char> random(size_t length, const std::vector<Char>& charSet);
		static BasicString<Char> random(size_t length, const std::string& charSet);

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
	inline BasicString<Char> operator+(const BasicString<Char>& left, const BasicString<OtherChar>& right);
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type = nullptr>
	inline BasicString<Char> operator+(BasicString<Char>&& left, const BasicString<OtherChar>& right);
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type = nullptr>
	inline BasicString<Char> operator+(const BasicString<Char>& left, BasicString<OtherChar>&& right);
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type = nullptr>
	inline BasicString<Char> operator+(BasicString<Char>&& left, BasicString<OtherChar>&& right);
	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type = nullptr>
	inline BasicString<Char> operator+(const BasicString<Char>& left, const OtherChar* right);
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type = nullptr>
	inline BasicString<Char> operator+(const Char* left, const BasicString<OtherChar>& right);
	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type = nullptr>
	inline BasicString<Char> operator+(const BasicString<Char>& left, const std::basic_string<OtherChar>& right);
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type = nullptr>
	inline BasicString<Char> operator+(BasicString<Char>&& left, const std::basic_string<OtherChar>& right);
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type = nullptr>
	inline BasicString<Char> operator+(const BasicString<Char>& left, std::basic_string<OtherChar>&& right);
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type = nullptr>
	inline BasicString<Char> operator+(BasicString<Char>&& left, std::basic_string<OtherChar>&& right);

	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type = nullptr>
	inline BasicString<Char> operator+(const std::basic_string<Char>& left, const BasicString<OtherChar>& right);
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type = nullptr>
	inline BasicString<Char> operator+(std::basic_string<Char>&& left, const BasicString<OtherChar>& right);
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type = nullptr>
	inline BasicString<Char> operator+(const std::basic_string<Char>& left, BasicString<OtherChar>&& right);
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type = nullptr>
	inline BasicString<Char> operator+(std::basic_string<Char>&& left, BasicString<OtherChar>&& right);
	
	#ifdef __OBJC__
	template<typename Char,
		typename BasicStringUtils::can_convert_string_types<Char,unichar>::null_type = nullptr>
	inline BasicString<Char> operator+(const BasicString<Char>& left, NSString* right);
	template<typename Char,
		typename BasicStringUtils::can_convert_string_types<Char,unichar>::null_type = nullptr>
	inline BasicString<Char> operator+(NSString* left, const BasicString<Char>& right);
	#endif
	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::string_type_convertable_with_char_type<Char,OtherChar>::null_type = nullptr>
	inline BasicString<Char> operator+(const BasicString<Char>& left, OtherChar right);
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::string_type_convertable_with_char_type<Char,OtherChar>::null_type = nullptr>
	inline BasicString<Char> operator+(Char left, const BasicString<OtherChar>& right);
	
	template<typename Char, typename Bool,
		typename BasicStringUtils::string_type_convertable_with_bool<Char,Bool>::null_type = nullptr>
	inline BasicString<Char> operator+(const BasicString<Char>& left, Bool right);
	template<typename Char, typename Bool,
		typename BasicStringUtils::string_type_convertable_with_bool<Char,Bool>::null_type = nullptr>
	inline BasicString<Char> operator+(Bool left, const BasicString<Char>& right);
	
	template<typename Char, typename Num,
		typename BasicStringUtils::string_type_convertable_with_number_or_enum<Char,Num>::null_type = nullptr>
	inline BasicString<Char> operator+(const BasicString<Char>& left, Num right);
	template<typename Char, typename Num,
		typename BasicStringUtils::string_type_convertable_with_number_or_enum<Char,Num>::null_type = nullptr>
	inline BasicString<Char> operator+(Num left, const BasicString<Char>& right);

	template<typename Char>
	inline auto operator<=>(const BasicString<Char>& left, const BasicString<Char>& right);
	
	template<typename Char>
	inline bool operator==(const BasicString<Char>& left, const BasicString<Char>& right);
	template<typename Char>
	inline bool operator==(const BasicString<Char>& left, const std::basic_string<Char>& right);
	template<typename Char>
	inline bool operator==(const std::basic_string<Char>& left, const BasicString<Char>& right);
	template<typename Char>
	inline bool operator==(const BasicString<Char>& left, const Char* right);
	template<typename Char>
	inline bool operator==(const Char* left, const BasicString<Char>& right);
	template<typename Char>
	inline bool operator==(const BasicString<Char>& left, Char right);
	template<typename Char>
	inline bool operator==(Char left, const BasicString<Char>& right);
	
	template<typename Char>
	inline bool operator!=(const BasicString<Char>& left, const BasicString<Char>& right);
	template<typename Char>
	inline bool operator!=(const BasicString<Char>& left, const Char* right);
	template<typename Char>
	inline bool operator!=(const Char* left, const BasicString<Char>& right);
	template<typename Char>
	inline bool operator!=(const BasicString<Char>& left, Char right);
	template<typename Char>
	inline bool operator!=(Char left, const BasicString<Char>& right);
	
	template<typename Char>
	inline bool operator<(const BasicString<Char>& left, const BasicString<Char>& right);
	template<typename Char>
	inline bool operator<(const BasicString<Char>& left, const Char* right);
	template<typename Char>
	inline bool operator<(const Char* left, const BasicString<Char>& right);
	
	template<typename Char>
	inline bool operator<=(const BasicString<Char>& left, const BasicString<Char>& right);
	template<typename Char>
	inline bool operator<=(const BasicString<Char>& left, const Char* right);
	template<typename Char>
	inline bool operator<=(const Char* left, const BasicString<Char>& right);
	
	template<typename Char>
	inline bool operator>(const BasicString<Char>& left, const BasicString<Char>& right);
	template<typename Char>
	inline bool operator>(const BasicString<Char>& left, const Char* right);
	template<typename Char>
	inline bool operator>(const Char* left, const BasicString<Char>& right);
	
	template<typename Char>
	inline bool operator>=(const BasicString<Char>& left, const BasicString<Char>& right);
	template<typename Char>
	inline bool operator>=(const BasicString<Char>& left, const Char* right);
	template<typename Char>
	inline bool operator>=(const Char* left, const BasicString<Char>& right);
	
	
	
	typedef BasicString<std::string::value_type> String;
	typedef BasicStringView<std::string_view::value_type> StringView;
	typedef BasicString<std::wstring::value_type> WideString;
	typedef BasicStringView<std::wstring_view::value_type> WideStringView;
}
