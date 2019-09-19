//
//  BasicString.impl.hpp
//  DataCpp
//
//  Created by Luis Finke on 8/17/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#include <fgl/data/BasicString.hpp>
#include <fgl/data/LinkedList.hpp>
#include <memory>

namespace fgl {
	template<typename Char>
	BasicString<Char>::BasicString()
	: characters((Char*)std::calloc(1, sizeof(Char))), size(0) {
		if(characters == nullptr) {
			throw std::bad_alloc();
		}
		characters[0] = NULLCHAR;
	}
	
	template<typename Char>
	BasicString<Char>::BasicString(const Char* str, size_t length)
	: characters((Char*)std::calloc(BasicStringUtils::get_safe_resize<Char>(length,0)+1, sizeof(Char))), size(length) {
		if(characters == nullptr) {
			throw std::bad_alloc();
		}
		for(size_t i=0; i<size; i++) {
			characters[i] = str[i];
		}
		characters[size] = NULLCHAR;
	}
	
	template<typename Char>
	BasicString<Char>::BasicString(const Char* str)
	: BasicString(str, BasicStringUtils::strlen<Char>(str)) {
		//
	}
	
	template<typename Char>
	BasicString<Char>::BasicString(const BasicString<Char>& str)
	: BasicString(str.characters, str.size) {
		//
	}
	
	template<typename Char>
	BasicString<Char>::BasicString(BasicString<Char>&& str)
	: characters(str.characters), size(str.size) {
		str.size = 0;
		str.characters = nullptr;
	}
	
	template<typename Char>
	BasicString<Char>::BasicString(const std::basic_string<Char>& str)
	: BasicString(str.c_str(), str.length()) {
		//
	}
	
	template<typename Char>
	BasicString<Char>::BasicString(Char c)
	: characters((Char*)std::calloc(2, sizeof(Char))), size(1) {
		//same char
		if(characters == nullptr) {
			throw std::bad_alloc();
		}
		characters[0] = c;
		characters[size] = NULLCHAR;
	}
	
	#ifdef __OBJC__
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char,Char>::null_type,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
			&& sizeof(unichar)==sizeof(_Char) && sizeof(_Char)!=sizeof(char)), std::nullptr_t>::type>
	BasicString<Char>::BasicString(NSString* nsString)
	: characters(nullptr), size(0) {
		NSUInteger nsString_length = nsString.length;
		size_t size_new = BasicStringUtils::get_safe_resize<Char>((size_t)nsString_length, 0);
		NSRange range = NSMakeRange(0, nsString_length);
		Char* characters_new = (Char*)std::calloc(size_new+1, sizeof(Char));
		if(characters_new == nullptr) {
			throw std::bad_alloc();
		}
		characters = characters_new;
		size = size_new;
		[nsString getCharacters:(unichar*)characters range:range];
		characters[size_new] = NULLCHAR;
	}
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char,Char>::null_type,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
			&& sizeof(_Char)==sizeof(char)), std::nullptr_t>::type>
	BasicString<Char>::BasicString(NSString* nsString)
	: BasicString((const Char*)[nsString UTF8String]) {
		//
	}
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char,Char>::null_type,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
			&& sizeof(unichar)!=sizeof(_Char) && sizeof(_Char)!=sizeof(char)), std::nullptr_t>::type>
	BasicString<Char>::BasicString(NSString* nsString)
	: characters(nullptr), size(0) {
		NSUInteger nsString_length = nsString.length;
		auto buffer = std::make_unique<unichar[]>((size_t)nsString_length);
		NSRange range = NSMakeRange(0, nsString_length);
		[nsString getCharacters:buffer.get() range:range];
		/*printf("old(");
		for(size_t i=0; i<(size_t)nsLength; i++) {
			int c = (int)buffer[i];
			printf("%i, ", c);
		}
		printf(") ");*/
		auto output = BasicStringUtils::convert<Char,unichar>(buffer.get(), (size_t)nsString_length);
		buffer = nullptr;
		/*printf("new(");
		for(size_t i=0; i<output.length(); i++) {
			int c = (int)output[i];
			printf("%i, ", c);
		}
		printf(") ");*/
		assign(output.c_str(), output.length());
	}
	
	#endif
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::same_size_convertable_with_char_type<Char,OtherChar>::null_type>
	BasicString<Char>::BasicString(OtherChar c)
	: characters((Char*)std::calloc(2, sizeof(Char))), size(1) {
		// same char size
		if(characters == nullptr) {
			throw std::bad_alloc();
		}
		characters[0] = (Char)c;
		characters[size] = NULLCHAR;
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::diff_size_convertable_with_char_type<Char,OtherChar>::null_type>
	BasicString<Char>::BasicString(OtherChar c)
	: characters(nullptr), size(0) {
		// different char size
		OtherChar str[2] = {c, NULLCHAR};
		auto output = BasicStringUtils::convert<Char,OtherChar>(str, 1);
		assign(output.c_str(), output.length());
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>::BasicString(const OtherChar* str, size_t length)
	: BasicString((const Char*)str, length) {
		// same char size
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>::BasicString(const OtherChar* str, size_t length)
	: characters(nullptr), size(0) {
		// different char size
		auto output = BasicStringUtils::convert<Char,OtherChar>(str, length);
		assign(output.c_str(), output.length());
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>::BasicString(const OtherChar* str)
	: BasicString((const Char*)str, BasicStringUtils::strlen<OtherChar>(str)) {
		// same char size
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>::BasicString(const OtherChar* str)
	: BasicString(str, BasicStringUtils::strlen<OtherChar>(str)) {
		// different char size
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>::BasicString(const BasicString<OtherChar>& str)
	: BasicString((const Char*)str.characters, str.size) {
		// same char size
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>::BasicString(BasicString<OtherChar>&& str)
	: characters((const Char*)str.characters), size(str.size) {
		// same char size
		str.size = 0;
		str.characters = nullptr;
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>::BasicString(const BasicString<OtherChar>& str)
	: BasicString(str.characters, str.size) {
		// different char size
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>::BasicString(const std::basic_string<OtherChar>& str)
	: BasicString((const Char*)str.c_str(), str.length()) {
		// same char size
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>::BasicString(const std::basic_string<OtherChar>& str)
	: BasicString(str.c_str(), str.length()) {
		// different char size
	}
	
	template<typename Char>
	BasicString<Char>::~BasicString() {
		if(characters != nullptr) {
			std::free(characters);
		}
	}
	
	
	
	
	template<typename Char>
	BasicString<Char>::operator const Char*() const {
		return characters;
	}
	
	template<typename Char>
	BasicString<Char>::operator std::basic_string<Char>() const {
		return std::basic_string<Char>(characters, size);
	}
	
	#ifdef __OBJC__
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char,Char>::null_type,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
			&& sizeof(unichar)==sizeof(_Char) && sizeof(_Char)!=sizeof(char)), std::nullptr_t>::type>
	NSString* BasicString<Char>::toNSString() const {
		return [NSString stringWithCharacters:(const unichar*)characters length:(NSUInteger)size];
	}
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char,Char>::null_type,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
			&& sizeof(_Char)==sizeof(char)), std::nullptr_t>::type>
	NSString* BasicString<Char>::toNSString() const {
		return [NSString stringWithUTF8String:(const char*)characters];
	}
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char, Char>::null_type,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
			&& sizeof(unichar)!=sizeof(_Char) && sizeof(_Char)!=sizeof(char)), std::nullptr_t>::type>
	NSString* BasicString<Char>::toNSString() const {
		auto output = BasicStringUtils::convert<unichar,Char>(characters, size);
		return [NSString stringWithCharacters:(const unichar*)output.c_str() length:(NSUInteger)output.length()];
	}
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char, Char>::null_type,
		typename BasicStringUtils::can_convert_string_type<_Char>::null_type>
	BasicString<Char>::operator NSString*() const {
		return toNSString();
	}
	
	#endif
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type>
	std::basic_string<OtherChar> BasicString<Char>::toStdString() const {
		// same char size
		return std::basic_string<OtherChar>((const OtherChar*)characters, size);
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type>
	std::basic_string<OtherChar> BasicString<Char>::toStdString() const {
		// different char size
		return BasicStringUtils::convert<OtherChar,Char>(characters, size);
	}
	
	template<typename Char>
	template<typename SameChar,
		typename BasicStringUtils::is_same<Char,SameChar>::null_type>
	std::basic_string<SameChar> BasicString<Char>::toStdString() const {
		// same char
		return std::basic_string<SameChar>(characters, size);
	}
	
	template<typename Char>
	template<typename SomeChar,
		typename BasicStringUtils::can_convert_string_types<Char,SomeChar>::null_type>
	BasicString<SomeChar> BasicString<Char>::toBasicString() const {
		return BasicString<SomeChar>(characters, size);
	}
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char, Char>::null_type,
		typename BasicStringUtils::can_convert_string_type<Char>::null_type>
	BasicString<char> BasicString<Char>::toString() const {
		return BasicString<char>(characters, size);
	}
	
	
	
	
	template<typename Char>
	void BasicString<Char>::assign(const Char* str, size_t length) {
		size_t size_new = BasicStringUtils::get_safe_resize<Char>(length, 0);
		Char* characters_new = (Char*)std::realloc(characters, (size_new+1)*sizeof(Char));
		if(characters_new == nullptr) {
			throw std::bad_alloc();
		}
		characters = characters_new;
		for(size_t i=0; i<size_new; i++) {
			characters[i] = str[i];
		}
		size = size_new;
		characters[size] = NULLCHAR;
	}
	
	template<typename Char>
	void BasicString<Char>::assign(const Char* str) {
		assign(str, BasicStringUtils::strlen<Char>(str));
	}
	
	template<typename Char>
	BasicString<Char>& BasicString<Char>::operator=(const Char* str) {
		assign(str, BasicStringUtils::strlen<Char>(str));
		return *this;
	}
	
	template<typename Char>
	BasicString<Char>& BasicString<Char>::operator=(const BasicString<Char>& str) {
		assign(str.characters, str.size);
		return *this;
	}
	
	template<typename Char>
	BasicString<Char>& BasicString<Char>::operator=(BasicString<Char>&& str) {
		if(characters != nullptr) {
			size = 0;
			std::free(characters);
		}
		characters = str.characters;
		size = str.size;
		str.size = 0;
		str.characters = nullptr;
		return *this;
	}
	
	template<typename Char>
	BasicString<Char>& BasicString<Char>::operator=(const std::basic_string<Char>& str) {
		assign(str.c_str(), str.length());
		return *this;
	}
	
	template<typename Char>
	BasicString<Char>& BasicString<Char>::operator=(Char c) {
		size_t size_new = 1;
		Char* characters_new = (Char*)std::realloc(characters, (size_new+1)*sizeof(Char));
		if(characters_new == nullptr) {
			throw std::bad_alloc();
		}
		characters = characters_new;
		characters[0] = c;
		size = size_new;
		characters[size] = NULLCHAR;
		return *this;
	}
	
	#ifdef __OBJC__
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char,Char>::null_type,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
			&& sizeof(unichar)==sizeof(_Char) && sizeof(_Char)!=sizeof(char)), std::nullptr_t>::type>
	BasicString<Char>& BasicString<Char>::operator=(NSString* nsString) {
		NSUInteger nsString_length = nsString.length;
		size_t size_new = BasicStringUtils::get_safe_resize<Char>((size_t)nsString_length, 0);
		NSRange range = NSMakeRange(0, nsString_length);
		Char* characters_new = (Char*)std::realloc(characters, (size_new+1)*sizeof(Char));
		if(characters_new == nullptr) {
			throw std::bad_alloc();
		}
		characters = characters_new;
		size = size_new;
		[nsString getCharacters:(unichar*)characters range:range];
		characters[size_new] = NULLCHAR;
		return *this;
	}
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char, Char>::null_type,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
			&& sizeof(_Char)==sizeof(char)), std::nullptr_t>::type>
	BasicString<Char>& BasicString<Char>::operator=(NSString* nsString) {
		assign((const Char*)[nsString UTF8String]);
		return *this;
	}
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char, Char>::null_type,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
			&& sizeof(unichar)!=sizeof(_Char) && sizeof(_Char)!=sizeof(char)), std::nullptr_t>::type>
	BasicString<Char>& BasicString<Char>::operator=(NSString* nsString) {
		NSUInteger nsString_length = nsString.length;
		size_t length = BasicStringUtils::get_safe_resize<Char>((size_t)nsString_length, 0);
		auto buffer = std::make_unique<unichar[]>(length);
		auto buffer_data = buffer.get();
		NSRange range = NSMakeRange(0, nsString_length);
		[nsString getCharacters:buffer_data range:range];
		buffer_data[length] = NULLCHAR;
		auto output = BasicStringUtils::convert<Char,unichar>(buffer_data, length);
		buffer = nullptr;
		assign(output.c_str(), output.length());
		return *this;
	}
	
	#endif
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::same_size_convertable_with_char_type<Char, OtherChar>::null_type>
	BasicString<Char>& BasicString<Char>::operator=(OtherChar c) {
		// same char size
		size_t size_new = 1;
		Char* characters_new = (Char*)std::realloc(characters, (size_new+1)*sizeof(Char));
		if(characters_new == nullptr) {
			throw std::bad_alloc();
		}
		characters = characters_new;
		characters[0] = (Char)c;
		size = size_new;
		characters[size] = NULLCHAR;
		return *this;
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::diff_size_convertable_with_char_type<Char, OtherChar>::null_type>
	BasicString<Char>& BasicString<Char>::operator=(OtherChar c) {
		// different char size
		OtherChar str[2] = {c, NULLCHAR};
		auto output = BasicStringUtils::convert<Char,OtherChar>(str, 1);
		assign(output.c_str(), output.length());
		return *this;
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::same_size_convertable_strings<Char, OtherChar>::null_type>
	BasicString<Char>& BasicString<Char>::operator=(const OtherChar* str) {
		// same char size
		assign((const Char*)str, BasicStringUtils::strlen<OtherChar>(str));
		return *this;
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::diff_size_convertable_strings<Char, OtherChar>::null_type>
	BasicString<Char>& BasicString<Char>::operator=(const OtherChar* str) {
		// different char size
		auto output = BasicStringUtils::convert<Char,OtherChar>(str, BasicStringUtils::strlen<OtherChar>(str));
		assign(output.c_str(), output.length());
		return *this;
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::same_size_convertable_strings<Char, OtherChar>::null_type>
	BasicString<Char>& BasicString<Char>::operator=(const BasicString<OtherChar>& str) {
		// same char size
		assign((const Char*)str.characters, str.size);
		return *this;
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::diff_size_convertable_strings<Char, OtherChar>::null_type>
	BasicString<Char>& BasicString<Char>::operator=(const BasicString<OtherChar>& str) {
		// different char size
		auto output = BasicStringUtils::convert<Char,OtherChar>(str.characters, str.size);
		assign(output.c_str(), output.length());
		return *this;
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::same_size_convertable_strings<Char, OtherChar>::null_type>
	BasicString<Char>& BasicString<Char>::operator=(const std::basic_string<OtherChar>& str) {
		// same char size
		assign((const Char*)str.c_str(), str.length());
		return *this;
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::diff_size_convertable_strings<Char, OtherChar>::null_type>
	BasicString<Char>& BasicString<Char>::operator=(const std::basic_string<OtherChar>& str) {
		// different char size
		auto output = BasicStringUtils::convert<Char,OtherChar>(str.c_str(), str.length());
		assign(output.c_str(), output.length());
		return *this;
	}
	
	
	
	
	template<typename Char>
	void BasicString<Char>::append(const Char* str, size_t length) {
		size_t size_new = BasicStringUtils::get_safe_resize<Char>(size, length);
		Char* characters_new = (Char*)std::realloc(characters, (size_new+1)*sizeof(Char));
		if(characters_new == nullptr) {
			throw std::bad_alloc();
		}
		characters = characters_new;
		size_t counter = 0;
		for(size_t i=size; i<size_new; i++) {
			characters[i] = str[counter];
			counter++;
		}
		size = size_new;
		characters[size] = NULLCHAR;
	}
	
	template<typename Char>
	void BasicString<Char>::append(const Char* str) {
		append(str, BasicStringUtils::strlen<Char>(str));
	}
	
	template<typename Char>
	void BasicString<Char>::append(Char c) {
		Char str[2] = {c, NULLCHAR};
		append(str, 1);
	}
	
	template<typename Char>
	BasicString<Char>& BasicString<Char>::operator+=(const Char* str) {
		append(str);
		return *this;
	}
	
	template<typename Char>
	BasicString<Char>& BasicString<Char>::operator+=(const BasicString<Char>& str) {
		append(str.characters, str.size);
		return *this;
	}
	
	template<typename Char>
	BasicString<Char>& BasicString<Char>::operator+=(const std::basic_string<Char>& str) {
		append(str.c_str(), str.length());
		return *this;
	}
	
	template<typename Char>
	BasicString<Char>& BasicString<Char>::operator+=(Char c) {
		append(c);
		return *this;
	}
	
	#ifdef __OBJC__
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char,Char>::null_type,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
			&& sizeof(unichar)==sizeof(_Char) && sizeof(_Char)!=sizeof(char)), std::nullptr_t>::type>
	BasicString<Char>& BasicString<Char>::operator+=(NSString* nsString) {
		NSUInteger nsString_length = nsString.length;
		size_t size_new = BasicStringUtils::get_safe_resize<Char>(size, (size_t)nsString_length);
		Char* characters_new = (Char*)std::realloc(characters, (size_new+1)*sizeof(Char));
		if(characters_new == nullptr) {
			throw std::bad_alloc();
		}
		characters = characters_new;
		size_t size_old = size;
		size = size_new;
		NSRange range = NSMakeRange(0, nsString_length);
		[nsString getCharacters:(unichar*)(characters+size_old) range:range];
		characters[size_new] = NULLCHAR;
		return *this;
	}
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char,Char>::null_type,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
			&& sizeof(_Char)==sizeof(char)), std::nullptr_t>::type>
	BasicString<Char>& BasicString<Char>::operator+=(NSString* nsString) {
		append((const Char*)[nsString UTF8String]);
		return *this;
	}
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char,Char>::null_type,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
			&& sizeof(unichar)!=sizeof(_Char) && sizeof(_Char)!=sizeof(char)), std::nullptr_t>::type>
	BasicString<Char>& BasicString<Char>::operator+=(NSString* nsString) {
		NSUInteger nsString_length = nsString.length;
		auto buffer = std::make_unique<unichar[]>((size_t)nsString_length);
		auto buffer_data = buffer.get();
		NSRange range = NSMakeRange(0, nsString_length);
		[nsString getCharacters:buffer_data range:range];
		buffer_data[nsString_length] = NULLCHAR;
		auto output = BasicStringUtils::convert<Char,unichar>(buffer_data, (size_t)nsString_length);
		buffer = nullptr;
		append(output.c_str(), output.length());
		return *this;
	}
	
	#endif
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::same_size_convertable_with_char_type<Char,OtherChar>::null_type>
	BasicString<Char>& BasicString<Char>::operator+=(OtherChar c) {
		// same char size
		append((Char)c);
		return *this;
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::diff_size_convertable_with_char_type<Char,OtherChar>::null_type>
	BasicString<Char>& BasicString<Char>::operator+=(OtherChar c) {
		// different char size
		OtherChar str[2] = {c, NULLCHAR};
		auto output = BasicStringUtils::convert<Char,OtherChar>(str, 1);
		append(output.c_str(), output.length());
		return *this;
	}
	
	template<typename Char>
	template<typename OtherChar, typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>& BasicString<Char>::operator+=(const OtherChar* str) {
		// same char size
		append((const Char*)str, BasicStringUtils::strlen<OtherChar>(str));
		return *this;
	}
	
	template<typename Char>
	template<typename OtherChar, typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>& BasicString<Char>::operator+=(const OtherChar* str) {
		// different char size
		auto output = BasicStringUtils::convert<Char,OtherChar>(str, BasicStringUtils::strlen<Char>(str));
		append(output.c_str(), output.length());
		return *this;
	}
	
	template<typename Char>
	template<typename OtherChar, typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>& BasicString<Char>::operator+=(const BasicString<OtherChar>& str) {
		// same char size
		append((const Char*)str.characters, str.size);
		return *this;
	}
	
	template<typename Char>
	template<typename OtherChar, typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>& BasicString<Char>::operator+=(const BasicString<OtherChar>& str) {
		// different char size
		auto output = BasicStringUtils::convert<Char,OtherChar>(str.characters, str.size);
		append(output.c_str(), output.length());
		return *this;
	}
	
	template<typename Char>
	template<typename OtherChar, typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>& BasicString<Char>::operator+=(const std::basic_string<OtherChar>& str) {
		// same char size
		append((const Char*)str.c_str(), str.length());
		return *this;
	}
	
	template<typename Char>
	template<typename OtherChar, typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>& BasicString<Char>::operator+=(const std::basic_string<OtherChar>& str) {
		// different char size
		auto output = BasicStringUtils::convert<Char,OtherChar>(str.c_str(), str.length());
		append(output.c_str(), output.length());
		return *this;
	}
	
	template<typename Char>
	template<typename Bool, typename BasicStringUtils::string_type_convertable_with_bool<Char,Bool>::null_type>
	BasicString<Char>& BasicString<Char>::operator+=(Bool b) {
		auto output = BasicStringUtils::fromBool<Char>(b);
		append(output.characters, output.size);
		return *this;
	}
	
	template<typename Char>
	template<typename Num, typename BasicStringUtils::string_type_convertable_with_number_or_enum<Char,Num>::null_type>
	BasicString<Char>& BasicString<Char>::operator+=(Num num) {
		auto output = BasicStringUtils::fromNumber<Char,Num>(num);
		append(output.c_str(), output.length());
		return *this;
	}
	
	
	
	
	template<typename Char>
	int BasicString<Char>::compare(const Char* cmp, size_t length, const std::locale& locale) const {
		return std::use_facet<std::collate<Char>>(locale).compare(characters, characters+size, cmp, cmp+length);
	}
	
	template<typename Char>
	int BasicString<Char>::compare(const Char* cmp, const std::locale& locale) const {
		return compare(cmp, BasicStringUtils::strlen<Char>(cmp), locale);
	}
	
	template<typename Char>
	int BasicString<Char>::compare(const BasicString<Char>& cmp, const std::locale& locale) const {
		return compare(cmp.characters, cmp.size, locale);
	}
	
	template<typename Char>
	bool BasicString<Char>::equals(const Char* str, size_t length) const {
		if(size == length) {
			for(size_t i=0; i<size; i++) {
				if(characters[i] != str[i]) {
					return false;
				}
			}
			return true;
		}
		return false;
	}
	
	template<typename Char>
	bool BasicString<Char>::equals(const Char* str) const {
		return equals(str, BasicStringUtils::strlen<Char>(str));
	}
	
	template<typename Char>
	bool BasicString<Char>::equals(const BasicString<Char>& str) const {
		return equals(str.characters, str.size);
	}
	
	
	
	
	template<typename Char>
	const Char* BasicString<Char>::data() const {
		return characters;
	}
	
	template<typename Char>
	size_t BasicString<Char>::length() const {
		return size;
	}
	
	template<typename Char>
	void BasicString<Char>::clear() {
		Char* characters_new = (Char*)std::realloc(characters, sizeof(Char));
		if(characters_new == nullptr) {
			throw std::bad_alloc();
		}
		characters = characters_new;
		size = 0;
		characters[0] = NULLCHAR;
	}
	
	template<typename Char>
	void BasicString<Char>::resize(size_t size_new) {
		BasicStringUtils::get_safe_resize<Char>(size_new, 0);
		size_t size_old = size;
		Char* characters_new = (Char*)std::realloc(characters, (size_new+1)*sizeof(Char));
		if(characters_new == nullptr) {
			throw std::bad_alloc();
		}
		characters = characters_new;
		size = size_new;
		for(size_t i=size_old; i<size_new; i++) {
			characters[i] = NULLCHAR;
		}
		characters[size_new] = NULLCHAR;
	}
	
	template<typename Char>
	Char& BasicString<Char>::charAt(size_t index) {
		if(index < size) {
			return characters[index];
		}
		throw std::out_of_range("index " + std::to_string(index) + " is out of bounds in BasicString<" + typeid(Char).name() + "> with a length of " + std::to_string(size));
	}
	
	template<typename Char>
	Char& BasicString<Char>::operator[](size_t index) {
		if(index < size) {
			return characters[index];
		}
		throw std::out_of_range("index " + std::to_string(index) + " is out of bounds in BasicString<" + typeid(Char).name() + "> with a length of " + std::to_string(size));
	}
	
	template<typename Char>
	const Char& BasicString<Char>::charAt(size_t index) const {
		if(index < size) {
			return characters[index];
		}
		throw std::out_of_range("index " + std::to_string(index) + " is out of bounds in BasicString<" + typeid(Char).name() + "> with a length of " + std::to_string(size));
	}
	
	template<typename Char>
	const Char& BasicString<Char>::operator[](size_t index) const {
		if(index < size) {
			return characters[index];
		}
		throw std::out_of_range("index " + std::to_string(index) + " is out of bounds in BasicString<" + typeid(Char).name() + "> with a length of " + std::to_string(size));
	}
	
	template<typename Char>
	size_t BasicString<Char>::indexOf(Char find, size_t startIndex) const {
		if(startIndex > size) {
			throw std::out_of_range("index " + std::to_string(startIndex) + " is out of bounds in BasicString<" + typeid(Char).name() + "> with a length of " + std::to_string(size));
		}
		for(size_t i=startIndex; i<size; i++) {
			if(characters[i] == find) {
				return i;
			}
		}
		return BasicString<Char>::NOT_FOUND;
	}
	
	template<typename Char>
	size_t BasicString<Char>::indexOf(const BasicString<Char>& find, size_t startIndex) const {
		if(startIndex > size) {
			throw std::out_of_range("index " + std::to_string(startIndex) + " is out of bounds in BasicString<" + typeid(Char).name() + "> with a length of " + std::to_string(size));
		}
		if(find.size == 0) {
			return BasicString<Char>::NOT_FOUND;
		}
		else if(find.size > size) {
			return BasicString<Char>::NOT_FOUND;
		}
		size_t finish = size - find.size;
		for(size_t i=0; i<=finish; i++) {
			if(characters[i] == find.characters[0]) {
				bool match = true;
				for(size_t j=1; j<find.size; j++) {
					if(characters[i+j] != find.characters[j]) {
						match = false;
						break;
					}
				}
				if(match) {
					return i;
				}
			}
		}
		return BasicString<Char>::NOT_FOUND;
	}
	
	template<typename Char>
	size_t BasicString<Char>::indexOf(const Char* find, size_t startIndex) const {
		if(startIndex > size) {
			throw std::out_of_range("index " + std::to_string(startIndex) + " is out of bounds in BasicString<" + typeid(Char).name() + "> with a length of " + std::to_string(size));
		}
		size_t find_size = BasicStringUtils::strlen<Char>(find);
		if(find_size == 0) {
			return BasicString<Char>::NOT_FOUND;
		}
		else if(find_size > size) {
			return BasicString<Char>::NOT_FOUND;
		}
		size_t finish = size - find_size;
		for(size_t i=0; i<=finish; i++) {
			if(characters[i] == find[0]) {
				bool match = true;
				for(size_t j=1; j<find_size; j++) {
					if(characters[i+j] != find[j]) {
						match = false;
						break;
					}
				}
				if(match) {
					return i;
				}
			}
		}
		return BasicString<Char>::NOT_FOUND;
	}
	
	template<typename Char>
	size_t BasicString<Char>::lastIndexOf(Char find, size_t startIndex) const {
		if(startIndex == (size_t)-1) {
			return BasicString<Char>::NOT_FOUND;
		}
		else if(startIndex >= size) {
			throw std::out_of_range("index " + std::to_string(startIndex) + " is out of bounds in BasicString<" + typeid(Char).name() + "> with a length of " + std::to_string(size));
		}
		for(size_t i=startIndex; i!=(size_t)-1; i--) {
			if(characters[i] == find) {
				return i;
			}
		}
		return BasicString<Char>::NOT_FOUND;
	}
	
	template<typename Char>
	size_t BasicString<Char>::lastIndexOf(Char find) const {
		return lastIndexOf(find, size-1);
	}
	
	template<typename Char>
	size_t BasicString<Char>::lastIndexOf(const BasicString<Char>& find, size_t startIndex) const {
		if(startIndex == (size_t)-1) {
			return BasicString<Char>::NOT_FOUND;
		}
		else if(startIndex >= size) {
			throw std::out_of_range("index " + std::to_string(startIndex) + " is out of bounds in BasicString<" + typeid(Char).name() + "> with a length of " + std::to_string(size));
		}
		else if(find.size == 0) {
			return BasicString<Char>::NOT_FOUND;
		}
		else if(find.size > size) {
			return BasicString<Char>::NOT_FOUND;
		}
		size_t find_last = find.size-1;
		for(size_t i=(startIndex-1); i>=find_last; i--) {
			if(characters[i] == find.characters[find_last]) {
				bool match = true;
				size_t counter = i-1;
				for(size_t j=(find_last-1); j!=(size_t)-1; j--) {
					if(characters[counter] != find.characters[j]) {
						match = false;
						break;
					}
					else {
						counter--;
					}
				}
				if(match) {
					return i;
				}
			}
		}
		return BasicString<Char>::NOT_FOUND;
	}
	
	template<typename Char>
	size_t BasicString<Char>::lastIndexOf(const BasicString<Char>& find) const {
		return lastIndexOf(find, size);
	}
	
	template<typename Char>
	size_t BasicString<Char>::lastIndexOf(const Char* find, size_t startIndex) const {
		size_t find_size = BasicStringUtils::strlen<Char>(find);
		if(startIndex == (size_t)-1) {
			return BasicString<Char>::NOT_FOUND;
		}
		else if(startIndex >= size) {
			throw std::out_of_range("index " + std::to_string(startIndex) + " is out of bounds in BasicString<" + typeid(Char).name() + "> with a length of " + std::to_string(size));
		}
		else if(find_size == 0) {
			return BasicString<Char>::NOT_FOUND;
		}
		else if(find_size > size) {
			return BasicString<Char>::NOT_FOUND;
		}
		size_t find_last = find_size-1;
		for(size_t i=startIndex; i>=find_last; i--) {
			if(characters[i]==find[find_last]) {
				bool match = true;
				size_t counter = i-1;
				for(size_t j=(find_last-1); j!=(size_t)-1; j--) {
					if(characters[counter] != find[j]) {
						match = false;
						break;
					}
					else {
						counter--;
					}
				}
				if(match) {
					return i;
				}
			}
		}
		return BasicString<Char>::NOT_FOUND;
	}
	
	template<typename Char>
	size_t BasicString<Char>::lastIndexOf(const Char* find) const {
		return lastIndexOf(find, size-1);
	}
	
	template<typename Char>
	bool BasicString<Char>::startsWith(const Char* str) const {
		return startsWith(str, BasicStringUtils::strlen<Char>(str));
	}
	
	template<typename Char>
	bool BasicString<Char>::startsWith(const Char* str, size_t length) const {
		if(length > size) {
			return false;
		}
		for(size_t i=0; i<length; i++) {
			if(characters[i]!=str[i]) {
				return false;
			}
		}
		return true;
	}
	
	template<typename Char>
	bool BasicString<Char>::startsWith(const BasicString<Char>& str) const {
		return startsWith(str.characters, str.size);
	}
	
	template<typename Char>
	bool BasicString<Char>::endsWith(const Char* str) const {
		return endsWith(str, BasicStringUtils::strlen<Char>(str));
	}
	
	template<typename Char>
	bool BasicString<Char>::endsWith(const Char* str, size_t length) const {
		if(length > size) {
			return false;
		}
		size_t str_index = length-1;
		size_t str_end = size-length-1;
		for(size_t i=(size-1); i!=str_end; i--) {
			if(characters[i]!=str[str_index]) {
				return false;
			}
			str_index--;
		}
		return true;
	}
	
	template<typename Char>
	bool BasicString<Char>::endsWith(const BasicString<Char>& str) const {
		return endsWith(str.characters, str.size);
	}
	
	
	
	template<typename Char>
	BasicString<Char> BasicString<Char>::replace(const Char& find, const Char& replace) const {
		BasicString<Char> newStr;
		Char* newStr_characters_new = (Char*)std::realloc(newStr.characters, (size+1)*sizeof(Char));
		if(newStr_characters_new == nullptr) {
			throw std::bad_alloc();
		}
		newStr.characters = newStr_characters_new;
		newStr.size = size;
		for(size_t i=0; i<size; i++) {
			if(characters[i] == find) {
				newStr.characters[i] = replace;
			}
			else {
				newStr.characters[i] = characters[i];
			}
		}
		newStr.characters[size] = NULLCHAR;
		return newStr;
	}
	
	template<typename Char>
	BasicString<Char> BasicString<Char>::replace(const BasicString<Char>& find, const BasicString<Char>& replace) const {
		if(find.size == 0) {
			return *this;
		}
		else if(find.size > size) {
			return *this;
		}
		std::list<size_t> indexes;
		size_t finish = size - find.size;
		for(size_t i=0; i<=finish; i++) {
			if(characters[i] == find.characters[0]) {
				bool match = true;
				for(size_t j=1; j<find.size; j++) {
					if(characters[i+j] != find.characters[j]) {
						match = false;
						break;
					}
				}
				if(match) {
					indexes.push_back(i);
					i += (find.size-1);
				}
			}
		}
		size_t indexes_size = indexes.size();
		if(indexes_size > 0) {
			BasicString<Char> newStr;
			size_t size_new = size + (replace.size*indexes_size) - (find.size*indexes_size);
			Char* newStr_characters_new = (Char*)std::realloc(newStr.characters, (size_new+1)*sizeof(Char));
			if(newStr_characters_new == nullptr) {
				throw std::bad_alloc();
			}
			newStr.characters = newStr_characters_new;
			newStr.size = size_new;
			size_t oldStr_counter = 0;
			auto indexes_it = indexes.begin();
			for(size_t i=0; i<size_new; i++) {
				if(oldStr_counter == *indexes_it) {
					for(size_t j=0; j<replace.size; j++) {
						newStr.characters[i] = replace.characters[j];
						i++;
					}
					i--;
					oldStr_counter += find.size;
					indexes_it++;
				}
				else {
					newStr.characters[i] = characters[oldStr_counter];
					oldStr_counter++;
				}
			}
			newStr.characters[size_new] = NULLCHAR;
			return newStr;
		}
		return *this;
	}
	
	template<typename Char>
	BasicString<Char> BasicString<Char>::replace(const std::basic_regex<Char>& find, const BasicString<Char>& replace) const {
		return std::regex_replace(characters, find, replace.characters);
	}
	
	template<typename Char>
	BasicString<Char> BasicString<Char>::replace(size_t startIndex, size_t endIndex, const Char* replace) const {
		if(startIndex >= size) {
			throw std::out_of_range("index " + std::to_string(startIndex) + " is out of bounds in BasicString<" + typeid(Char).name() + "> with a length of " + std::to_string(size));
		}
		else if(endIndex > size && !(endIndex < startIndex && endIndex == (size_t)-1)) {
			throw std::out_of_range("index " + std::to_string(endIndex) + " is out of bounds in BasicString<" + typeid(Char).name() + "> with a length of " + std::to_string(size));
		}
		size_t replace_size = BasicStringUtils::strlen<Char>(replace);
		if(startIndex <= endIndex) {
			size_t find_size = endIndex - startIndex;
			size_t size_new = BasicStringUtils::get_safe_resize<Char>(size - find_size, replace_size);
			BasicString<Char> newStr;
			Char* newStr_characters_new = (Char*)std::realloc(newStr.characters, (size_new+1)*sizeof(Char));
			if(newStr_characters_new == nullptr) {
				throw std::bad_alloc();
			}
			newStr.characters = newStr_characters_new;
			newStr.size = size_new;
			for(size_t i=0; i<startIndex; i++) {
				newStr.characters[i] = characters[i];
			}
			size_t replace_end = startIndex + replace_size;
			size_t counter = 0;
			for(size_t i=startIndex; i<replace_end; i++) {
				newStr.characters[i] = replace[counter];
				counter++;
			}
			counter = endIndex;
			for(size_t i=replace_end; i<size_new; i++) {
				newStr.characters[i] = characters[counter];
				counter++;
			}
			newStr.characters[size_new] = NULLCHAR;
			return newStr;
		}
		else {
			BasicString<Char> newStr;
			size_t find_size = (startIndex+1) - (endIndex+1);
			size_t size_new = BasicStringUtils::get_safe_resize<Char>(size - find_size, replace_size);
			Char* newStr_characters_new = (Char*)std::realloc(newStr.characters, (size_new+1)*sizeof(Char));
			if(newStr_characters_new == nullptr) {
				throw std::bad_alloc();
			}
			newStr.characters = newStr_characters_new;
			newStr.size = size_new;
			if(endIndex != (size_t)-1) {
				for(size_t i=0; i<=endIndex; i++) {
					newStr.characters[i] = characters[i];
				}
			}
			size_t replace_end = (endIndex+1)+replace_size;
			size_t counter = replace_size-1;
			for(size_t i=(endIndex+1); i<replace_end; i++) {
				newStr.characters[i] = replace[counter];
				counter--;
			}
			counter = startIndex+1;
			for(size_t i=replace_end; i<size_new; i++) {
				newStr.characters[i] = characters[counter];
				counter++;
			}
			newStr.characters[size_new] = NULLCHAR;
			return newStr;
		}
	}
	
	template<typename Char>
	BasicString<Char> BasicString<Char>::replace(size_t startIndex, size_t endIndex, const BasicString<Char>& replace) const {
		if(startIndex >= size) {
			throw std::out_of_range("index " + std::to_string(startIndex) + " is out of bounds in BasicString<" + typeid(Char).name() + "> with a length of " + std::to_string(size));
		}
		else if(endIndex>size && !(endIndex<startIndex && endIndex==(size_t)-1)) {
			throw std::out_of_range("index " + std::to_string(endIndex) + " is out of bounds in BasicString<" + typeid(Char).name() + "> with a length of " + std::to_string(size));
		}
		if(startIndex <= endIndex) {
			BasicString<Char> newStr;
			size_t find_size = endIndex - startIndex;
			size_t size_new = BasicStringUtils::get_safe_resize<Char>(size - find_size, replace.size);
			Char* newStr_characters_new = (Char*)std::realloc(newStr.characters, (size_new+1)*sizeof(Char));
			if(newStr_characters_new == nullptr) {
				throw std::bad_alloc();
			}
			newStr.characters = newStr_characters_new;
			newStr.size = size_new;
			for(size_t i=0; i<startIndex; i++) {
				newStr.characters[i] = characters[i];
			}
			size_t replace_end = startIndex + replace.size;
			size_t counter = 0;
			for(size_t i=startIndex; i<replace_end; i++) {
				newStr.characters[i] = replace.characters[counter];
				counter++;
			}
			counter = endIndex;
			for(size_t i=replace_end; i<size_new; i++) {
				newStr.characters[i] = characters[counter];
				counter++;
			}
			newStr.characters[size_new] = NULLCHAR;
			return newStr;
		}
		else {
			BasicString<Char> newStr;
			size_t find_size = (startIndex+1) - (endIndex+1);
			size_t size_new = BasicStringUtils::get_safe_resize<Char>(size - find_size, replace.size);
			Char* newStr_characters_new = (Char*)std::realloc(newStr.characters, (size_new+1)*sizeof(Char));
			if(newStr_characters_new == nullptr) {
				throw std::bad_alloc();
			}
			newStr.characters = newStr_characters_new;
			newStr.size = size_new;
			if(endIndex != (size_t)-1) {
				for(size_t i=0; i<=endIndex; i++) {
					newStr.characters[i] = characters[i];
				}
			}
			size_t replace_end = (endIndex+1)+replace.size;
			size_t counter = replace.size-1;
			for(size_t i=(endIndex+1); i<replace_end; i++) {
				newStr.characters[i] = replace.characters[counter];
				counter--;
			}
			counter = startIndex+1;
			for(size_t i=replace_end; i<size_new; i++) {
				newStr.characters[i] = characters[counter];
				counter++;
			}
			newStr.characters[size_new] = NULLCHAR;
			return newStr;
		}
	}
	
	template<typename Char>
	BasicString<Char> BasicString<Char>::substring(size_t startIndex, size_t endIndex) const {
		if(startIndex > size) {
			throw std::out_of_range("index " + std::to_string(startIndex) + " is out of bounds in BasicString<" + typeid(Char).name() + "> with a length of " + std::to_string(size));
		}
		else if(endIndex > size && !(endIndex<startIndex && endIndex==(size_t)-1)) {
			throw std::out_of_range("index " + std::to_string(endIndex) + " is out of bounds in BasicString<" + typeid(Char).name() + "> with a length of " + std::to_string(size));
		}
		BasicString<Char> newStr;
		if(startIndex > endIndex) {
			size_t size_new = (startIndex+1) - (endIndex+1);
			Char* newStr_characters_new = (Char*)std::realloc(newStr.characters, (size_new+1)*sizeof(Char));
			if(newStr_characters_new == nullptr) {
				throw std::bad_alloc();
			}
			newStr.characters = newStr_characters_new;
			newStr.size = size_new;
			size_t counter = 0;
			for(size_t i=startIndex; i!=endIndex; i--) {
				newStr.characters[counter] = characters[i];
				counter++;
			}
			newStr.characters[size_new] = NULLCHAR;
		}
		else if(startIndex<endIndex) {
			size_t size_new = endIndex - startIndex;
			Char* newStr_characters_new = (Char*)std::realloc(newStr.characters, (size_new+1)*sizeof(Char));
			if(newStr_characters_new == nullptr) {
				throw std::bad_alloc();
			}
			newStr.characters = newStr_characters_new;
			newStr.size = size_new;
			size_t counter = 0;
			for(size_t i=startIndex; i<endIndex; i++) {
				newStr.characters[counter] = characters[i];
				counter++;
			}
			newStr.characters[size_new] = NULLCHAR;
		}
		return newStr;
	}
	
	template<typename Char>
	BasicString<Char> BasicString<Char>::substring(size_t startIndex) const {
		return substring(startIndex, size);
	}
	
	template<typename Char>
	LinkedList<BasicString<Char>> BasicString<Char>::split(Char delim) const {
		LinkedList<BasicString<Char>> elements;
		size_t lastStart = 0;
		for(size_t i=0; i<size; i++) {
			if(characters[i] == delim) {
				elements.push_back(substring(lastStart, i));
				lastStart = i+1;
			}
		}
		elements.push_back(substring(lastStart, size));
		return elements;
	}
	
	template<typename Char>
	LinkedList<BasicString<Char>> BasicString<Char>::split(const Char* delim) const {
		size_t delim_size = BasicStringUtils::strlen<Char>(delim);
		if(delim_size == 0 || delim_size > size) {
			LinkedList<BasicString<Char>> elements;
			elements.push_back(*this);
			return elements;
		}
		LinkedList<BasicString<Char>> elements;
		size_t lastStart = 0;
		size_t finish = size - delim_size;
		for(size_t i=0; i<=finish; i++) {
			if(characters[i]==delim[0]) {
				bool match = true;
				for(size_t j=1; j<delim_size; j++) {
					if(characters[i+j] != delim[j]) {
						match = false;
						break;
					}
				}
				if(match) {
					elements.push_back(substring(lastStart, i));
					lastStart = i+delim_size;
					i = lastStart-1;
				}
			}
		}
		elements.push_back(substring(lastStart, size));
		return elements;
	}
	
	template<typename Char>
	LinkedList<BasicString<Char>> BasicString<Char>::split(const BasicString<Char>& delim) const {
		if(delim.size == 0 || delim.size > size) {
			LinkedList<BasicString<Char> > elements;
			elements.push_back(*this);
			return elements;
		}
		LinkedList<BasicString<Char> > elements;
		size_t lastStart = 0;
		size_t finish = size - delim.size;
		for(size_t i=0; i<=finish; i++) {
			if(characters[i] == delim.characters[0]) {
				bool match = true;
				for(size_t j=1; j<delim.size; j++) {
					if(characters[i+j] != delim.characters[j]) {
						match = false;
						break;
					}
				}
				if(match) {
					elements.push_back(substring(lastStart, i));
					lastStart = i+delim.size;
					i = lastStart-1;
				}
			}
		}
		elements.push_back(substring(lastStart, size));
		return elements;
	}
	
	template<typename Char>
	template<typename _Char,
	typename BasicStringUtils::is_same<_Char, Char>::null_type,
	typename BasicStringUtils::can_convert_string_type<_Char>::null_type>
	BasicString<Char> BasicString<Char>::trim(const std::locale& locale) const {
		typedef typename BasicStringUtils::utf_eqv<Char>::type utf_char;
		if(size == 0) {
			return *this;
		}
		bool hitLetter = false;
		size_t startIndex = 0;
		for(size_t i=0; i<size && !hitLetter; i++) {
			if(!std::isspace<utf_char>((utf_char)characters[i], locale)) {
				startIndex = i;
				hitLetter = true;
			}
		}
		if(!hitLetter) {
			return BasicString<Char>();
		}
		hitLetter = false;
		size_t endIndex = size;
		for(size_t i=(size-1); i>=startIndex && i!=(size_t)-1 && !hitLetter; i--) {
			if(!std::isspace<utf_char>((utf_char)characters[i], locale)) {
				endIndex = i+1;
				hitLetter = true;
			}
		}
		return substring(startIndex, endIndex);
	}
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char, Char>::null_type,
		typename BasicStringUtils::can_convert_string_type<_Char>::null_type>
	BasicString<Char> BasicString<Char>::toLowerCase(const std::locale& locale) const {
		BasicString<Char> newStr;
		Char* newStr_characters_new = (Char*)std::realloc(newStr.characters, (size+1)*sizeof(Char));
		if(newStr_characters_new == nullptr) {
			throw std::bad_alloc();
		}
		newStr.characters = newStr_characters_new;
		newStr.size = size;
		for(size_t i=0; i<size; i++) {
			typedef typename BasicStringUtils::utf_eqv<Char>::type UTFChar;
			newStr.characters[i] = (Char)std::tolower<UTFChar>((UTFChar)characters[i], locale);
		}
		newStr.characters[size] = NULLCHAR;
		return newStr;
	}
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char, Char>::null_type,
		typename BasicStringUtils::can_convert_string_type<_Char>::null_type>
	BasicString<Char> BasicString<Char>::toUpperCase(const std::locale& locale) const {
		BasicString<Char> newStr;
		Char* newStr_characters_new = (Char*)std::realloc(newStr.characters, (size+1)*sizeof(Char));
		if(newStr_characters_new == nullptr) {
			throw std::bad_alloc();
		}
		newStr.characters = newStr_characters_new;
		newStr.size = size;
		for(size_t i=0; i<size; i++) {
			typedef typename BasicStringUtils::utf_eqv<Char>::type UTFChar;
			newStr.characters[i] = (Char)std::toupper<UTFChar>((UTFChar)characters[i], locale);
		}
		newStr.characters[size] = NULLCHAR;
		return newStr;
	}
	
	template<typename Char>
	template<typename Num,
		typename BasicStringUtils::string_type_convertable_with_number<Char,Num>::null_type>
	Num BasicString<Char>::toArithmeticValue(const std::locale& locale) const {
		std::basic_istringstream<Char> convert(std::basic_string<Char>(characters, size));
		convert.imbue(locale);
		Num numVal = 0;
		if(!(convert >> numVal)) {
			throw std::logic_error("BasicString does not represent an arithmetic value");
		}
		return numVal;
	}
	
	
	
	
	template<typename Char>
	template<typename ListType,
		typename std::enable_if<std::is_same<BasicString<Char>,typename ListType::value_type>::value, std::nullptr_t>::type>
	BasicString<Char> BasicString<Char>::join(const ListType& list, const BasicString<Char>& separator) {
		if(list.size() == 0) {
			return BasicString<Char>();
		}
		size_t listSize = 0;
		size_t listIndex = 0;
		for(auto& str : list) {
			listSize = BasicStringUtils::get_safe_resize<Char>(listSize, str.length());
			if(listIndex != (list.size()-1)) {
				listSize = BasicStringUtils::get_safe_resize<Char>(listSize, separator.length());
			}
			listIndex++;
		}
		std::basic_string<Char> joined;
		joined.reserve(listSize);
		listIndex = 0;
		for(auto& str : list) {
			joined += str.template toStdString<Char>();
			if(listIndex != (list.size()-1)) {
				joined += separator.template toStdString<Char>();
			}
			listIndex++;
		}
		return joined;
	}
	
	
	
	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char, OtherChar>::null_type>
	BasicString<Char> operator+(const BasicString<Char>& left, const BasicString<OtherChar>& right) {
		return BasicStringUtils::concat(left, right);
	}
	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char, OtherChar>::null_type>
	BasicString<Char> operator+(const BasicString<Char>& left, const OtherChar* right) {
		return BasicStringUtils::concat(left, right);
	}
	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char, OtherChar>::null_type>
	BasicString<Char> operator+(const Char* left, const BasicString<OtherChar>& right) {
		return BasicStringUtils::concat(left, right);
	}
	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char, OtherChar>::null_type>
	BasicString<Char> operator+(const BasicString<Char>& left, const std::basic_string<OtherChar>& right) {
		return BasicStringUtils::concat(left, right);
	}
	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char, OtherChar>::null_type>
	BasicString<Char> operator+(const std::basic_string<Char>& left, const BasicString<OtherChar>& right) {
		return BasicStringUtils::concat(left, right);
	}
	
	#ifdef __OBJC__
	
	template<typename Char,
		typename BasicStringUtils::can_convert_string_types<Char, unichar>::null_type>
	BasicString<Char> operator+(const BasicString<Char>& left, NSString* right) {
		return BasicStringUtils::concat(left, right);
	}
	
	template<typename Char,
		typename BasicStringUtils::can_convert_string_types<Char, unichar>::null_type>
	BasicString<Char> operator+(NSString* left, const BasicString<Char>& right) {
		return BasicStringUtils::concat(left, right);
	}
	
	#endif
	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::string_type_convertable_with_char_type<Char, OtherChar>::null_type>
	BasicString<Char> operator+(const BasicString<Char>& left, const OtherChar& right) {
		return BasicStringUtils::concat(left, right);
	}
	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::string_type_convertable_with_char_type<OtherChar, Char>::null_type>
	BasicString<Char> operator+(const Char& left, const BasicString<OtherChar>& right) {
		return BasicStringUtils::concat(left, right);
	}
	
	template<typename Char, typename Bool,
		typename BasicStringUtils::string_type_convertable_with_bool<Char,Bool>::null_type>
	BasicString<Char> operator+(const BasicString<Char>& left, Bool right) {
		return BasicStringUtils::concat(left, right);
	}
	
	template<typename Char, typename Bool,
		typename BasicStringUtils::string_type_convertable_with_bool<Char,Bool>::null_type>
	BasicString<Char> operator+(Bool left, const BasicString<Char>& right) {
		return BasicStringUtils::concat(left, right);
	}
	
	template<typename Char, typename Num,
		typename BasicStringUtils::string_type_convertable_with_number_or_enum<Char,Num>::null_type>
	BasicString<Char> operator+(const BasicString<Char>& left, Num right) {
		return BasicStringUtils::concat(left, right);
	}
	
	template<typename Char, typename Num,
		typename BasicStringUtils::string_type_convertable_with_number_or_enum<Char,Num>::null_type>
	BasicString<Char> operator+(Num left, const BasicString<Char>& right) {
		return BasicStringUtils::concat(left, right);
	}
	
	
	
	
	template<typename Char>
	bool operator==(const BasicString<Char>& left, const BasicString<Char>& right) {
		return left.equals(right);
	}
	
	template<typename Char>
	bool operator==(const BasicString<Char>& left, const Char* right) {
		return left.equals(right);
	}
	
	template<typename Char>
	bool operator==(const Char* left, const BasicString<Char>& right) {
		return right.equals(left);
	}
	
	template<typename Char>
	bool operator==(const BasicString<Char>& left, Char right) {
		Char right_arr[] = {right, NULLCHAR};
		return left.equals(right_arr, 1);
	}
	
	template<typename Char>
	bool operator==(Char left, const BasicString<Char>& right) {
		Char left_arr[] = {left, NULLCHAR};
		return right.equals(left_arr, 1);
	}
	
	template<typename Char>
	bool operator!=(const BasicString<Char>& left, const BasicString<Char>& right) {
		return !left.equals(right);
	}
	
	template<typename Char>
	bool operator!=(const BasicString<Char>& left, const Char* right) {
		return !left.equals(right);
	}
	
	template<typename Char>
	bool operator!=(const Char* left, const BasicString<Char>& right) {
		return !right.equals(left);
	}
	
	template<typename Char>
	bool operator!=(const BasicString<Char>& left, const Char& right) {
		Char right_arr[] = {right, NULLCHAR};
		return !left.equals(right_arr, 1);
	}
	
	template<typename Char>
	bool operator!=(const Char& left, const BasicString<Char>& right) {
		Char left_arr[] = {left, NULLCHAR};
		return right.equals(left_arr, 1);
	}
	
	template<typename Char>
	bool operator<(const BasicString<Char>& left, const BasicString<Char>& right) {
		return (left.compare(right) < 0);
	}
	
	template<typename Char>
	bool operator<(const BasicString<Char>& left, const Char* right) {
		return (left.compare(right) < 0);
	}
	
	template<typename Char>
	bool operator<(const Char* left, const BasicString<Char>& right) {
		return (right.compare(left) > 0);
	}
	
	template<typename Char>
	bool operator<=(const BasicString<Char>& left, const BasicString<Char>& right) {
		return (left.compare(right) <= 0);
	}
	
	template<typename Char>
	bool operator<=(const BasicString<Char>& left, const Char* right) {
		return (left.compare(right) <= 0);
	}
	
	template<typename Char>
	bool operator<=(const Char* left, const BasicString<Char>& right) {
		return (right.compare(left) >= 0);
	}
	
	template<typename Char>
	bool operator>(const BasicString<Char>& left, const BasicString<Char>& right) {
		return (left.compare(right) > 0);
	}
	
	template<typename Char>
	bool operator>(const BasicString<Char>& left, const Char* right) {
		return (left.compare(right) > 0);
	}
	
	template<typename Char>
	bool operator>(const Char* left, const BasicString<Char>& right) {
		return (right.compare(left) < 0);
	}
	
	template<typename Char>
	bool operator>=(const BasicString<Char>& left, const BasicString<Char>& right) {
		return (left.compare(right) >= 0);
	}
	
	template<typename Char>
	bool operator>=(const BasicString<Char>& left, const Char* right) {
		return (left.compare(right) >= 0);
	}
	
	template<typename Char>
	bool operator>=(const Char* left, const BasicString<Char>& right) {
		return (right.compare(left) <= 0);
	}
}
