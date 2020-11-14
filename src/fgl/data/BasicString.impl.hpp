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
#include <cstdlib>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>

#ifdef NAPI_MODULE
#include <fgl/util/NAPI.h>
#endif

namespace fgl {
	template<typename Char>
	BasicString<Char>::BasicString() noexcept {
		//
	}
	
	template<typename Char>
	BasicString<Char>::BasicString(size_type count, Char c)
	: storage(count, c) {
		//
	}
	
	template<typename Char>
	BasicString<Char>::BasicString(const Char* str, size_type length)
	: storage(str, length) {
		FGL_ASSERT(str != nullptr, "str cannot be null");
	}
	
	template<typename Char>
	BasicString<Char>::BasicString(const Char* str)
	: storage(str) {
		FGL_ASSERT(str != nullptr, "str cannot be null");
	}
	
	template<typename Char>
	BasicString<Char>::BasicString(const std::basic_string<Char>& str)
	: storage(str) {
		//
	}
	
	template<typename Char>
	BasicString<Char>::BasicString(std::basic_string<Char>&& str) noexcept
	: storage(str) {
		//
	}
	
	template<typename Char>
	BasicString<Char>::BasicString(std::initializer_list<Char> list)
	: storage(list) {
		//
	}
	
	template<typename Char>
	template<typename InputIt>
	BasicString<Char>::BasicString(InputIt first, InputIt last)
	: storage(first, last) {
		//
	}
	
	#ifdef __OBJC__
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char,Char>::null_type,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
			&& sizeof(unichar)==sizeof(_Char) && sizeof(_Char)!=sizeof(char)), std::nullptr_t>::type>
	BasicString<Char>::BasicString(NSString* nsString) {
		if(nsString == nil) {
			return;
		}
		NSUInteger nsString_length = nsString.length;
		size_t size_new = BasicStringUtils::get_safe_resize<Char>((size_t)nsString_length, 0);
		NSRange range = NSMakeRange(0, nsString_length);
		storage.resize(size_new);
		[nsString getCharacters:(unichar*)((Char*)storage.data()) range:range];
	}
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char,Char>::null_type,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
			&& sizeof(_Char)==sizeof(char)), std::nullptr_t>::type>
	BasicString<Char>::BasicString(NSString* nsString)
	: storage((nsString != nil) ? (const Char*)[nsString UTF8String] : "") {
		//
	}
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char,Char>::null_type,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
			&& sizeof(unichar)!=sizeof(_Char) && sizeof(_Char)!=sizeof(char)), std::nullptr_t>::type>
	BasicString<Char>::BasicString(NSString* nsString) {
		if(nsString == nil) {
			return;
		}
		NSUInteger nsString_length = nsString.length;
		auto buffer = std::make_unique<unichar[]>((size_t)nsString_length);
		NSRange range = NSMakeRange(0, nsString_length);
		[nsString getCharacters:buffer.get() range:range];
		storage = BasicStringUtils::convert<Char,unichar>(buffer.get(), (size_t)nsString_length);
	}
	
	#endif

	#ifdef JNIEXPORT

	template<typename Char>
	BasicString<Char>::BasicString(JNIEnv* env, jstring javaString) {
		const char* javaStringChars = env->GetStringUTFChars(javaString, nullptr);
		operator=(javaStringChars);
		env->ReleaseStringUTFChars(javaString, javaStringChars);
	}

	#endif

	#ifdef NODE_API_MODULE
	
	template<typename Char>
	BasicString<Char>::BasicString(Napi::String napiString) {
		if(napiString.IsEmpty() || napiString.IsNull() || napiString.IsUndefined()) {
			return;
		}
		operator=(napiString.Utf8Value());
	}
	
	#endif

	#ifdef NAPI_MODULE

	template<typename Char>
	BasicString<Char>::BasicString(napi_env env, napi_value value) {
		if(value == nullptr) {
			return;
		}
		DATACPP_NAPI_ASSERT_TYPE(env, value, napi_string);
		size_t stringLength = 0;
		DATACPP_NAPI_CALL(env, "failed to get js string length", napi_get_value_string_utf8((env), value, nullptr, 0, &stringLength));
		storage.resize(stringLength);
		size_t stringLengthCopied = 0;
		char* stringData = (char*)storage.data();
		DATACPP_NAPI_CALL(env, "failed to copy string", napi_get_value_string_utf8((env), value, stringData, stringLength + 1, &stringLengthCopied));
		DATACPP_NAPI_ASSERT(env, stringLength == stringLengthCopied, "Couldn't fully copy data to string");
	}

	#endif


	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>::BasicString(const OtherChar* str, size_type length)
	: storage((const Char*)str, length) {
		FGL_ASSERT(str != nullptr, "str cannot be null");
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>::BasicString(const OtherChar* str, size_type length)
	: storage(BasicStringUtils::convert<Char,OtherChar>(str, length)) {
		//
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>::BasicString(const OtherChar* str)
	: storage((const Char*)str) {
		FGL_ASSERT(str != nullptr, "str cannot be null");
		// same char size
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>::BasicString(const OtherChar* str)
	: storage(BasicStringUtils::convert<Char,OtherChar>(str, BasicStringUtils::strlen<OtherChar>(str))) {
		// different char size
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>::BasicString(const BasicString<OtherChar>& str)
	: storage(str.storage) {
		// same char size
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>::BasicString(BasicString<OtherChar>&& str)
	: storage(str.storage) {
		// same char size
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>::BasicString(const BasicString<OtherChar>& str)
	: storage(BasicStringUtils::convert<Char,OtherChar>(str.data(), str.length())) {
		// different char size
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>::BasicString(const std::basic_string<OtherChar>& str)
	: storage(str.storage) {
		// same char size
	}
	
	template<typename Char>
	template<typename OtherChar,
	typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>::BasicString(std::basic_string<OtherChar>&& str)
	: storage(str.storage) {
		// same char size
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>::BasicString(const std::basic_string<OtherChar>& str)
	: storage(BasicStringUtils::convert<Char,OtherChar>(str.data(), str.length())) {
		// different char size
	}
	
	
	
	template<typename Char>
	BasicString<Char>::operator const Char*() const noexcept {
		return storage.c_str();
	}
	
	template<typename Char>
	BasicString<Char>::operator std::basic_string<Char>&() & noexcept {
		return storage;
	}

	template<typename Char>
	BasicString<Char>::operator std::basic_string<Char>&&() && noexcept {
		return storage;
	}
	
	template<typename Char>
	BasicString<Char>::operator const std::basic_string<Char>&() const& noexcept {
		return storage;
	}
	
	template<typename Char>
	BasicString<Char>::operator const std::basic_string_view<Char>() const noexcept {
		return storage;
	}
	
	
	
	#ifdef __OBJC__
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char,Char>::null_type,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
			&& sizeof(unichar)==sizeof(_Char) && sizeof(_Char)!=sizeof(char)), std::nullptr_t>::type>
	NSString* BasicString<Char>::toNSString() const {
		return [NSString stringWithCharacters:(const unichar*)storage.data() length:(NSUInteger)storage.length()];
	}
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char,Char>::null_type,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
			&& sizeof(_Char)==sizeof(char)), std::nullptr_t>::type>
	NSString* BasicString<Char>::toNSString() const {
		return [NSString stringWithUTF8String:(const char*)storage.data()];
	}
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char, Char>::null_type,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
			&& sizeof(unichar)!=sizeof(_Char) && sizeof(_Char)!=sizeof(char)), std::nullptr_t>::type>
	NSString* BasicString<Char>::toNSString() const {
		auto output = BasicStringUtils::convert<unichar,Char>(storage.data(), storage.length());
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

	#ifdef JNIEXPORT

	template<typename Char>
	jstring BasicString<Char>::toJavaString(JNIEnv* env) const {
		if constexpr(std::is_same<Char,char>::value) {
			return env->NewStringUTF(storage.c_str());
		} else {
			return env->NewStringUTF(BasicStringUtils::convert<char,Char>(storage.data(), storage.length()).c_str());
		}
	}

	#endif

	#ifdef NODE_API_MODULE

	template<typename Char>
	Napi::String BasicString<Char>::toNapiString(napi_env env) const {
		return Napi::String::New(env, storage);
	}

	#endif

	#ifdef NAPI_MODULE

	template<typename Char>
	napi_value BasicString<Char>::toNodeJSValue(napi_env env) const {
		if constexpr(sizeof(Char) == 1 && std::is_integral<Char>::value) {
			napi_value value = nullptr;
			DATACPP_NAPI_CALL(env, "failed to create napi_value", napi_create_string_utf8(env, storage.data(), storage.length(), &value));
			return value;
		} else {
			auto str = toStdString<char>();
			napi_value value = nullptr;
			DATACPP_NAPI_CALL(env, "failed to create napi_value", napi_create_string_utf8(env, str.data(), str.length(), &value));
			return value;
		}
	}

	#endif
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type>
	std::basic_string<OtherChar> BasicString<Char>::toStdString() const {
		// same char size
		return std::basic_string<OtherChar>((const OtherChar*)storage.data(), storage.length());
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type>
	std::basic_string<OtherChar> BasicString<Char>::toStdString() const {
		// different char size
		return BasicStringUtils::convert<OtherChar,Char>(storage.data(), storage.length());
	}
	
	template<typename Char>
	template<typename SameChar,
		typename BasicStringUtils::is_same<Char,SameChar>::null_type>
	std::basic_string<SameChar> BasicString<Char>::toStdString() const {
		// same char
		return std::basic_string<SameChar>(storage.data(), storage.length());
	}
	
	template<typename Char>
	template<typename SomeChar,
		typename BasicStringUtils::can_convert_string_types<Char,SomeChar>::null_type>
	BasicString<SomeChar> BasicString<Char>::toBasicString() const {
		return BasicString<SomeChar>(storage.data(), storage.length());
	}
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::can_convert_string_types<Char,char>::null_type>
	BasicString<char> BasicString<Char>::toString() const {
		if constexpr(std::is_same<Char,char>::value) {
			return *this;
		}
		return BasicStringUtils::convert<char,Char>(storage.data(),storage.length());
	}
	
	
	
	template<typename Char>
	BasicString<Char>& BasicString<Char>::assign(const BasicString<Char>& str) {
		storage.assign(str.storage);
		return *this;
	}
	
	template<typename Char>
	BasicString<Char>& BasicString<Char>::assign(BasicString<Char>&& str) {
		storage.assign(str.storage);
		return *this;
	}
	
	template<typename Char>
	BasicString<Char>& BasicString<Char>::assign(const std::basic_string<Char>& str) {
		storage.assign(str);
		return *this;
	}
	
	template<typename Char>
	BasicString<Char>& BasicString<Char>::assign(std::basic_string<Char>&& str) noexcept {
		storage.assign(str);
		return *this;
	}
	
	template<typename Char>
	BasicString<Char>& BasicString<Char>::assign(const Char* str, size_type length) {
		FGL_ASSERT(str != nullptr, "str cannot be null");
		storage.assign(str, length);
		return *this;
	}
	
	template<typename Char>
	BasicString<Char>& BasicString<Char>::assign(const Char* str) {
		FGL_ASSERT(str != nullptr, "str cannot be null");
		storage.assign(str);
		return *this;
	}
	
	template<typename Char>
	BasicString<Char>& BasicString<Char>::assign(size_type count, Char c) {
		storage.assign(count, c);
		return *this;
	}
	
	template<typename Char>
	template<typename InputIt>
	BasicString<Char>& BasicString<Char>::assign(InputIt first, InputIt last) {
		storage.assign(first, last);
		return *this;
	}
	
	template<typename Char>
	BasicString<Char>& BasicString<Char>::assign(std::initializer_list<Char> list) {
		storage.assign(list);
		return *this;
	}
	
	template<typename Char>
	BasicString<Char>& BasicString<Char>::operator=(const std::basic_string<Char>& str) {
		storage.operator=(str);
		return *this;
	}
	
	template<typename Char>
	BasicString<Char>& BasicString<Char>::operator=(std::basic_string<Char>&& str) noexcept {
		storage.operator=(str);
		return *this;
	}
	
	template<typename Char>
	BasicString<Char>& BasicString<Char>::operator=(const Char* str) {
		FGL_ASSERT(str != nullptr, "str cannot be null");
		storage.operator=(str);
		return *this;
	}
	
	template<typename Char>
	BasicString<Char>& BasicString<Char>::operator=(std::initializer_list<Char> list) {
		storage.operator=(list);
		return *this;
	}
	
	#ifdef __OBJC__
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char,Char>::null_type,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
			&& sizeof(unichar)==sizeof(_Char) && sizeof(_Char)!=sizeof(char)), std::nullptr_t>::type>
	BasicString<Char>& BasicString<Char>::operator=(NSString* nsString) {
		if(nsString == nil) {
			clear();
			return *this;
		}
		NSUInteger nsString_length = nsString.length;
		size_t size_new = BasicStringUtils::get_safe_resize<Char>((size_t)nsString_length, 0);
		NSRange range = NSMakeRange(0, nsString_length);
		storage.resize(size_new);
		[nsString getCharacters:(unichar*)((Char*)storage.data()) range:range];
		return *this;
	}
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char, Char>::null_type,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
			&& sizeof(_Char)==sizeof(char)), std::nullptr_t>::type>
	BasicString<Char>& BasicString<Char>::operator=(NSString* nsString) {
		if(nsString == nil) {
			clear();
			return *this;
		}
		storage.assign((const Char*)[nsString UTF8String]);
		return *this;
	}
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char, Char>::null_type,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
			&& sizeof(unichar)!=sizeof(_Char) && sizeof(_Char)!=sizeof(char)), std::nullptr_t>::type>
	BasicString<Char>& BasicString<Char>::operator=(NSString* nsString) {
		if(nsString == nil) {
			clear();
			return *this;
		}
		NSUInteger nsString_length = nsString.length;
		size_t length = BasicStringUtils::get_safe_resize<Char>((size_t)nsString_length, 0);
		auto buffer = std::make_unique<unichar[]>(length);
		auto buffer_data = buffer.get();
		NSRange range = NSMakeRange(0, nsString_length);
		[nsString getCharacters:buffer_data range:range];
		buffer_data[length] = NULLCHAR;
		storage = BasicStringUtils::convert<Char,unichar>(buffer_data, length);
		return *this;
	}
	
	#endif
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::same_size_convertable_strings<Char, OtherChar>::null_type>
	BasicString<Char>& BasicString<Char>::operator=(const OtherChar* str) {
		// same char size
		assign((const Char*)str);
		return *this;
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::diff_size_convertable_strings<Char, OtherChar>::null_type>
	BasicString<Char>& BasicString<Char>::operator=(const OtherChar* str) {
		// different char size
		storage = BasicStringUtils::convert<Char,OtherChar>(str, BasicStringUtils::strlen<OtherChar>(str));
		return *this;
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::same_size_convertable_strings<Char, OtherChar>::null_type>
	BasicString<Char>& BasicString<Char>::operator=(const BasicString<OtherChar>& str) {
		// same char size
		assign((const Char*)str.data(), str.length());
		return *this;
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::diff_size_convertable_strings<Char, OtherChar>::null_type>
	BasicString<Char>& BasicString<Char>::operator=(const BasicString<OtherChar>& str) {
		// different char size
		storage = BasicStringUtils::convert<Char,OtherChar>(str.data(), str.length());
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
		storage = BasicStringUtils::convert<Char,OtherChar>(str.data(), str.length());
		return *this;
	}
	
	
	
	template<typename Char>
	BasicString<Char>& BasicString<Char>::append(const BasicString<Char>& str) {
		storage.append(str.storage);
		return *this;
	}
	
	template<typename Char>
	BasicString<Char>& BasicString<Char>::append(const std::basic_string<Char>& str) {
		storage.append(str);
		return *this;
	}
	
	template<typename Char>
	BasicString<Char>& BasicString<Char>::append(const Char* str, size_type length) {
		FGL_ASSERT(str != nullptr, "str cannot be null");
		storage.append(str,length);
		return *this;
	}
	
	template<typename Char>
	BasicString<Char>& BasicString<Char>::append(const Char* str) {
		FGL_ASSERT(str != nullptr, "str cannot be null");
		storage.append(str);
		return *this;
	}
	
	template<typename Char>
	BasicString<Char>& BasicString<Char>::append(size_type count, Char c) {
		storage.append(count, c);
		return *this;
	}
	
	template<typename Char>
	template<typename InputIt>
	BasicString<Char>& BasicString<Char>::append(InputIt first, InputIt last) {
		storage.append(first, last);
	}
	
	template<typename Char>
	BasicString<Char>& BasicString<Char>::append(std::initializer_list<Char> list) {
		storage.append(list);
	}
	
	
	
	template<typename Char>
	BasicString<Char>& BasicString<Char>::operator+=(const BasicString<Char>& str) {
		storage.operator+=(str.storage);
		return *this;
	}
	
	template<typename Char>
	BasicString<Char>& BasicString<Char>::operator+=(const std::basic_string<Char>& str) {
		storage.operator+=(str);
		return *this;
	}
	
	template<typename Char>
	BasicString<Char>& BasicString<Char>::operator+=(const Char* str) {
		FGL_ASSERT(str != nullptr, "str cannot be null");
		storage.operator+=(str);
		return *this;
	}
	
	template<typename Char>
	BasicString<Char>& BasicString<Char>::operator+=(Char c) {
		storage.operator+=(c);
		return *this;
	}
	
	#ifdef __OBJC__
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char,Char>::null_type,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
			&& sizeof(unichar)==sizeof(_Char) && sizeof(_Char)!=sizeof(char)), std::nullptr_t>::type>
	BasicString<Char>& BasicString<Char>::operator+=(NSString* nsString) {
		if(nsString == nil) {
			return *this;
		}
		NSUInteger nsString_length = nsString.length;
		size_t size_new = BasicStringUtils::get_safe_resize<Char>(storage.size(), (size_t)nsString_length);
		size_t size_old = storage.size();
		NSRange range = NSMakeRange(0, nsString_length);
		storage.resize(size_new);
		[nsString getCharacters:(unichar*)((Char*)(storage.data()+size_old)) range:range];
		return *this;
	}
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char,Char>::null_type,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
			&& sizeof(_Char)==sizeof(char)), std::nullptr_t>::type>
	BasicString<Char>& BasicString<Char>::operator+=(NSString* nsString) {
		if(nsString == nil) {
			return *this;
		}
		append((const Char*)[nsString UTF8String]);
		return *this;
	}
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char,Char>::null_type,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
			&& sizeof(unichar)!=sizeof(_Char) && sizeof(_Char)!=sizeof(char)), std::nullptr_t>::type>
	BasicString<Char>& BasicString<Char>::operator+=(NSString* nsString) {
		if(nsString == nil) {
			return *this;
		}
		NSUInteger nsString_length = nsString.length;
		auto buffer = std::make_unique<unichar[]>((size_t)nsString_length);
		auto buffer_data = buffer.get();
		NSRange range = NSMakeRange(0, nsString_length);
		[nsString getCharacters:buffer_data range:range];
		buffer_data[nsString_length] = NULLCHAR;
		auto output = BasicStringUtils::convert<Char,unichar>(buffer_data, (size_t)nsString_length);
		buffer = nullptr;
		append(output);
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
		append(BasicStringUtils::convert<Char,OtherChar>(std::basic_string<OtherChar>(c).data(), 1));
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
		append(BasicStringUtils::convert<Char,OtherChar>(str, BasicStringUtils::strlen<Char>(str)));
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
		append(BasicStringUtils::convert<Char,OtherChar>(str.characters, str.size));
		return *this;
	}
	
	template<typename Char>
	template<typename OtherChar, typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>& BasicString<Char>::operator+=(const std::basic_string<OtherChar>& str) {
		// same char size
		append((const Char*)str.data(), str.length());
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
	int BasicString<Char>::compare(const Char* cmp, size_type length) const noexcept {
		FGL_ASSERT(cmp != nullptr, "cmp cannot be null");
		return storage.compare(0, length, cmp);
	}
	
	template<typename Char>
	int BasicString<Char>::compare(const Char* cmp) const noexcept {
		FGL_ASSERT(cmp != nullptr, "cmp cannot be null");
		return storage.compare(cmp);
	}
	
	template<typename Char>
	int BasicString<Char>::compare(const BasicString<Char>& cmp) const noexcept {
		return storage.compare(cmp.storage);
	}
	
	template<typename Char>
	int BasicString<Char>::compare(const std::basic_string<Char>& cmp) const noexcept {
		return storage.compare(cmp);
	}
	
	template<typename Char>
	int BasicString<Char>::compare(const Char* cmp, size_type length, const std::locale& locale) const {
		FGL_ASSERT(cmp != nullptr, "cmp cannot be null");
		return std::use_facet<std::collate<Char>>(locale).compare(storage.data(), storage.data()+storage.length(), cmp, cmp+length);
	}
	
	template<typename Char>
	int BasicString<Char>::compare(const Char* cmp, const std::locale& locale) const {
		FGL_ASSERT(cmp != nullptr, "cmp cannot be null");
		return compare(cmp, BasicStringUtils::strlen<Char>(cmp), locale);
	}
	
	template<typename Char>
	int BasicString<Char>::compare(const BasicString<Char>& cmp, const std::locale& locale) const {
		return compare(cmp.storage.data(), cmp.storage.length(), locale);
	}
	
	template<typename Char>
	int BasicString<Char>::compare(const std::basic_string<Char>& cmp, const std::locale& locale) const {
		return compare(cmp.data(), cmp.length(), locale);
	}
	
	template<typename Char>
	constexpr bool BasicString<Char>::equals(const Char* str, size_type length) const {
		FGL_ASSERT(str != nullptr, "str cannot be null");
		if(storage.length() == length) {
			for(size_t i=0; i<length; i++) {
				if(storage[i] != str[i]) {
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
		return equals(str.storage.data(), str.storage.length());
	}
	
	
	
	
	template<typename Char>
	Char* BasicString<Char>::data() noexcept {
		return (Char*)storage.data();
	}
	
	template<typename Char>
	const Char* BasicString<Char>::data() const noexcept {
		return storage.data();
	}
	
	template<typename Char>
	const Char* BasicString<Char>::c_str() const noexcept {
		return storage.c_str();
	}
	
	template<typename Char>
	typename BasicString<Char>::size_type BasicString<Char>::size() const noexcept {
		return storage.size();
	}
	
	template<typename Char>
	typename BasicString<Char>::size_type BasicString<Char>::length() const noexcept {
		return storage.length();
	}
	
	template<typename Char>
	typename BasicString<Char>::size_type BasicString<Char>::maxSize() const noexcept {
		return storage.max_size();
	}
	
	template<typename Char>
	bool BasicString<Char>::empty() const noexcept {
		return storage.empty();
	}
	
	template<typename Char>
	void BasicString<Char>::clear() {
		storage.clear();
	}
	
	template<typename Char>
	void BasicString<Char>::resize(size_type size_new) {
		storage.resize(size_new);
	}
	
	template<typename Char>
	void BasicString<Char>::resize(size_type size_new, Char c) {
		storage.resize(size_new, c);
	}
	
	template<typename Char>
	void BasicString<Char>::reserve(size_type capacity) {
		storage.reserve(capacity);
	}
	
	template<typename Char>
	Char& BasicString<Char>::charAt(size_type index) {
		return storage.at(index);
	}
	
	template<typename Char>
	Char& BasicString<Char>::operator[](size_type index) {
		return storage[index];
	}
	
	template<typename Char>
	const Char& BasicString<Char>::charAt(size_type index) const {
		return storage.at(index);
	}
	
	template<typename Char>
	const Char& BasicString<Char>::operator[](size_type index) const {
		return storage[index];
	}
	
	template<typename Char>
	typename BasicString<Char>::size_type BasicString<Char>::indexOf(Char find, size_type startIndex) const noexcept {
		return storage.find(find, startIndex);
	}
	
	template<typename Char>
	typename BasicString<Char>::size_type BasicString<Char>::indexOf(const std::basic_string<Char>& find, size_type startIndex) const noexcept {
		return storage.find(find, startIndex);
	}
	
	template<typename Char>
	typename BasicString<Char>::size_type BasicString<Char>::indexOf(const BasicString<Char>& find, size_type startIndex) const noexcept {
		return storage.find(find.storage, startIndex);
	}
	
	template<typename Char>
	typename BasicString<Char>::size_type BasicString<Char>::indexOf(const Char* find, size_type startIndex) const {
		FGL_ASSERT(find != nullptr, "find cannot be null");
		return storage.find(find, startIndex);
	}
	
	template<typename Char>
	typename BasicString<Char>::size_type BasicString<Char>::lastIndexOf(Char find, size_type startIndex) const noexcept {
		return storage.rfind(find, startIndex);
	}
	
	template<typename Char>
	typename BasicString<Char>::size_type BasicString<Char>::lastIndexOf(const std::basic_string<Char>& find, size_type startIndex) const noexcept {
		return storage.rfind(find, startIndex);
	}
	
	template<typename Char>
	typename BasicString<Char>::size_type BasicString<Char>::lastIndexOf(const BasicString<Char>& find, size_type startIndex) const noexcept {
		return storage.rfind(find.storage, startIndex);
	}
	
	template<typename Char>
	typename BasicString<Char>::size_type BasicString<Char>::lastIndexOf(const Char* find, size_type startIndex) const {
		FGL_ASSERT(find != nullptr, "find cannot be null");
		return storage.rfind(find, startIndex);
	}
	
	template<typename Char>
	bool BasicString<Char>::startsWith(const Char* str, size_type length) const {
		FGL_ASSERT(str != nullptr, "str cannot be null");
		if(length > storage.length()) {
			return false;
		}
		for(size_type i=0; i<length; i++) {
			if(storage[i] != str[i]) {
				return false;
			}
		}
		return true;
	}
	
	template<typename Char>
	bool BasicString<Char>::startsWith(const Char* str) const {
		FGL_ASSERT(str != nullptr, "str cannot be null");
		return startsWith(str, BasicStringUtils::strlen<Char>(str));
	}
	
	template<typename Char>
	bool BasicString<Char>::startsWith(const std::basic_string<Char>& str) const {
		return startsWith(str.data(), str.length());
	}
	
	template<typename Char>
	bool BasicString<Char>::startsWith(const BasicString<Char>& str) const {
		return startsWith(str.storage.data(), str.storage.length());
	}
	
	template<typename Char>
	bool BasicString<Char>::endsWith(const Char* str, size_type length) const {
		FGL_ASSERT(str != nullptr, "str cannot be null");
		if(length > storage.length()) {
			return false;
		}
		size_type str_index = length-1;
		size_type str_end = storage.length()-length-1;
		for(size_type i=(storage.length()-1); i!=str_end; i--) {
			if(storage[i] != str[str_index]) {
				return false;
			}
			str_index--;
		}
		return true;
	}

	template<typename Char>
	bool BasicString<Char>::endsWith(const Char* str) const {
		FGL_ASSERT(str != nullptr, "str cannot be null");
		return endsWith(str, BasicStringUtils::strlen<Char>(str));
	}
	
	template<typename Char>
	bool BasicString<Char>::endsWith(const std::basic_string<Char>& str) const {
		return endsWith(str.data(), str.length());
	}
	
	template<typename Char>
	bool BasicString<Char>::endsWith(const BasicString<Char>& str) const {
		return endsWith(str.storage.data(), str.storage.length());
	}
	
	
	
	template<typename Char>
	BasicString<Char> BasicString<Char>::replacing(Char find, Char replace) const {
		BasicString<Char> newStr;
		newStr.reserve(storage.length());
		for(size_type i=0; i<storage.length(); i++) {
			Char c = storage[i];
			if(c == find) {
				newStr.append(1, replace);
			}
			else {
				newStr.append(1, c);
			}
		}
		return newStr;
	}
	
	template<typename Char>
	BasicString<Char> BasicString<Char>::replacing(const std::basic_string<Char>& find, const std::basic_string<Char>& replace) const {
		if(find.length() == 0) {
			return *this;
		}
		else if(find.length() > storage.length()) {
			return *this;
		}
		LinkedList<size_type> indexes;
		size_type finish = storage.length() - find.size;
		for(size_type i=0; i<=finish; i++) {
			if(storage[i] == find[0]) {
				bool match = true;
				for(size_t j=1; j<find.size; j++) {
					if(storage[i+j] != find[j]) {
						match = false;
						break;
					}
				}
				if(match) {
					indexes.pushBack(i);
					i += (find.size-1);
				}
			}
		}
		size_type indexes_size = indexes.size();
		if(indexes_size == 0) {
			return *this;
		}
		BasicString<Char> newStr;
		size_type size_new = storage.length() + (replace.size*indexes_size) - (find.size*indexes_size);
		newStr.reserve(size_new);
		size_t oldStr_counter = 0;
		auto indexes_it = indexes.begin();
		for(size_t i=0; i<size_new; i++) {
			if(oldStr_counter == *indexes_it) {
				for(size_t j=0; j<replace.length(); j++) {
					newStr.append(1,replace.storage[j]);
					i++;
				}
				i--;
				oldStr_counter += find.length();
				indexes_it++;
			}
			else {
				newStr.append(1,storage[oldStr_counter]);
				oldStr_counter++;
			}
		}
		return newStr;
	}
	
	template<typename Char>
	BasicString<Char> BasicString<Char>::replacing(const std::basic_regex<Char>& find, const std::basic_string<Char>& replace, std::regex_constants::match_flag_type flags) const {
		return std::regex_replace(storage, find, replace, flags);
	}
	
	template<typename Char>
	template<typename InputIt>
	BasicString<Char> BasicString<Char>::replacing(size_type startIndex, size_type count, InputIt first, InputIt last) const {
		size_type endIndex = startIndex + count;
		if(startIndex >= storage.length()) {
			throw std::out_of_range("index " + std::to_string(startIndex) + " is out of bounds in BasicString<" + typeid(Char).name() + "> with a length of " + std::to_string(storage.length()));
		}
		else if(endIndex < startIndex || endIndex > storage.length()) {
			throw std::out_of_range("index " + std::to_string(startIndex) + " plus count " + std::to_string(count) + " is out of bounds in BasicString<" + typeid(Char).name() + "> with a length of " + std::to_string(storage.length()));
		}
		size_type replaceLength = std::distance(first, last);
		BasicString<Char> newStr;
		size_type size_new = BasicStringUtils::get_safe_resize<Char>(storage.length() - count, replaceLength);
		newStr.reserve(size_new);
		newStr.append(storage.data(), startIndex);
		newStr.append(first, last);
		newStr.append(storage.data()+endIndex, storage.length()-endIndex);
		return newStr;
	}
	
	template<typename Char>
	BasicString<Char> BasicString<Char>::replacing(size_type startIndex, size_type count, const std::basic_string<Char>& replace) const {
		return replacing(startIndex, count, replace.begin(), replace.end());
	}
	
	template<typename Char>
	BasicString<Char> BasicString<Char>::substring(size_type startIndex, size_type count) const {
		return storage.substr(startIndex, count);
	}
	
	template<typename Char>
	LinkedList<BasicString<Char>> BasicString<Char>::split(Char delim) const {
		LinkedList<BasicString<Char>> items;
		size_t lastStart = 0;
		for(size_t i=0; i<storage.length(); i++) {
			if(storage[i] == delim) {
				items.pushBack(substring(lastStart, i-lastStart));
				lastStart = i+1;
			}
		}
		items.pushBack(substring(lastStart, length()-lastStart));
		return items;
	}
	
	template<typename Char>
	LinkedList<BasicString<Char>> BasicString<Char>::split(const Char* delim) const {
		FGL_ASSERT(delim != nullptr, "delim cannot be null");
		size_t delim_size = BasicStringUtils::strlen<Char>(delim);
		if(delim_size == 0 || delim_size > storage.length()) {
			LinkedList<BasicString<Char>> elements;
			elements.pushBack(*this);
			return elements;
		}
		LinkedList<BasicString<Char>> items;
		size_t lastStart = 0;
		size_t finish = storage.length() - delim_size;
		for(size_t i=0; i<=finish; i++) {
			if(storage[i] == delim[0]) {
				bool match = true;
				for(size_t j=1; j<delim_size; j++) {
					if(storage[i+j] != delim[j]) {
						match = false;
						break;
					}
				}
				if(match) {
					items.pushBack(substring(lastStart, i-lastStart));
					lastStart = i+delim_size;
					i = lastStart-1;
				}
			}
		}
		items.pushBack(substring(lastStart, storage.length()-lastStart));
		return items;
	}
	
	template<typename Char>
	LinkedList<BasicString<Char>> BasicString<Char>::split(const std::basic_string<Char>& delim) const {
		if(delim.length() == 0 || delim.length() > storage.length()) {
			LinkedList<BasicString<Char>> items;
			items.pushBack(*this);
			return items;
		}
		LinkedList<BasicString<Char>> items;
		size_t lastStart = 0;
		size_t finish = storage.length() - delim.length();
		for(size_t i=0; i<=finish; i++) {
			if(storage[i] == delim[0]) {
				bool match = true;
				for(size_t j=1; j<delim.length(); j++) {
					if(storage[i+j] != delim[j]) {
						match = false;
						break;
					}
				}
				if(match) {
					items.pushBack(substring(lastStart, i-lastStart));
					lastStart = i+delim.size;
					i = lastStart-1;
				}
			}
		}
		items.pushBack(substring(lastStart, storage.length()-lastStart));
		return items;
	}
	
	template<typename Char>
	LinkedList<BasicString<Char>> BasicString<Char>::split(const BasicString<Char>& delim) const {
		return split(delim.storage);
	}
	
	template<typename Char>
	template<typename _Char,
	typename BasicStringUtils::is_same<_Char, Char>::null_type,
	typename BasicStringUtils::can_convert_string_type<_Char>::null_type>
	BasicString<Char> BasicString<Char>::trim(const std::locale& locale) const {
		typedef typename BasicStringUtils::utf_eqv<Char>::type utf_char;
		if(storage.length() == 0) {
			return *this;
		}
		bool hitLetter = false;
		size_type startIndex = 0;
		for(size_type i=0; i<storage.length() && !hitLetter; i++) {
			if(!std::isspace<utf_char>((utf_char)storage[i], locale)) {
				startIndex = i;
				hitLetter = true;
			}
		}
		if(!hitLetter) {
			return BasicString<Char>();
		}
		hitLetter = false;
		size_type endIndex = storage.length();
		for(size_type i=(storage.length()-1); i>=startIndex && i!=(size_type)-1 && !hitLetter; i--) {
			if(!std::isspace<utf_char>((utf_char)storage[i], locale)) {
				endIndex = i+1;
				hitLetter = true;
			}
		}
		return substring(startIndex, endIndex-startIndex);
	}
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char, Char>::null_type,
		typename BasicStringUtils::can_convert_string_type<_Char>::null_type>
	BasicString<Char> BasicString<Char>::toLowerCase(const std::locale& locale) const {
		BasicString<Char> newStr;
		newStr.reserve(storage.length());
		for(size_type i=0; i<storage.length(); i++) {
			typedef typename BasicStringUtils::utf_eqv<Char>::type UTFChar;
			newStr.append(1,(Char)std::tolower<UTFChar>((UTFChar)storage[i], locale));
		}
		return newStr;
	}
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char, Char>::null_type,
		typename BasicStringUtils::can_convert_string_type<_Char>::null_type>
	BasicString<Char> BasicString<Char>::toUpperCase(const std::locale& locale) const {
		BasicString<Char> newStr;
		newStr.reserve(storage.length());
		for(size_type i=0; i<storage.length(); i++) {
			typedef typename BasicStringUtils::utf_eqv<Char>::type UTFChar;
			newStr.append(1,(Char)std::toupper<UTFChar>((UTFChar)storage[i], locale));
		}
		return newStr;
	}
	
	template<typename Char>
	template<typename Num,
		typename BasicStringUtils::string_type_convertable_with_number<Char,Num>::null_type>
	Num BasicString<Char>::toArithmeticValue(const std::locale& locale) const {
		std::basic_istringstream<Char> convert(storage);
		convert.imbue(locale);
		Num numVal = 0;
		convert >> numVal;
		if(convert.fail()) {
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
		size_type listSize = 0;
		size_type i = 0;
		size_type lastListIndex = list.size()-1;
		for(auto& str : list) {
			listSize = BasicStringUtils::get_safe_resize<Char>(listSize, str.length());
			if(i != lastListIndex) {
				listSize = BasicStringUtils::get_safe_resize<Char>(listSize, separator.length());
			}
			i++;
		}
		BasicString<Char> joined;
		joined.reserve(listSize);
		i = 0;
		for(auto& str : list) {
			joined.append(str);
			if(i != lastListIndex) {
				joined.append(separator);
			}
			i++;
		}
		return joined;
	}

	template<typename Char>
	BasicString<Char> BasicString<Char>::join(std::initializer_list<BasicString<Char>> list, const BasicString<Char>& separator) {
		return join<std::initializer_list<BasicString<Char>>>(list, separator);
	}


	template<typename Char>
	BasicString<Char> random(size_t length, const std::vector<Char>& charSet) {
		BasicString<Char> str;
		str.reserve(length);
		for(size_t i=0; i<length; i++) {
			size_t randIndex = (size_t)(((double)rand() / (double)RAND_MAX) * charSet.size());
			str.append(1, charSet[randIndex]);
		}
		return str;
	}

	template<typename Char>
	BasicString<Char> random(size_t length, const std::string& charSet) {
		BasicString<Char> str;
		str.reserve(length);
		for(size_t i=0; i<length; i++) {
			size_t randIndex = (size_t)(((double)rand() / (double)RAND_MAX) * charSet.size());
			str.append(1, charSet[randIndex]);
		}
		return str;
	}
	
	

	template<typename Char>
	std::ostream& operator<<(std::ostream& os, const BasicString<Char>& str) {
		os << str.storage;
		return os;
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
	bool operator==(const BasicString<Char>& left, const std::basic_string<Char>& right) {
		return left.storage == right;
	}

	template<typename Char>
	bool operator==(const std::basic_string<Char>& left, const BasicString<Char>& right) {
		return left == right.storage;
	}
	
	template<typename Char>
	bool operator==(const BasicString<Char>& left, const Char* right) {
		FGL_ASSERT(right != nullptr, "right cannot be null");
		return left.equals(right);
	}
	
	template<typename Char>
	bool operator==(const Char* left, const BasicString<Char>& right) {
		FGL_ASSERT(left != nullptr, "left cannot be null");
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
		FGL_ASSERT(right != nullptr, "right cannot be null");
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
		FGL_ASSERT(right != nullptr, "right cannot be null");
		return (left.compare(right) < 0);
	}
	
	template<typename Char>
	bool operator<(const Char* left, const BasicString<Char>& right) {
		FGL_ASSERT(left != nullptr, "left cannot be null");
		return (right.compare(left) > 0);
	}
	
	template<typename Char>
	bool operator<=(const BasicString<Char>& left, const BasicString<Char>& right) {
		return (left.compare(right) <= 0);
	}
	
	template<typename Char>
	bool operator<=(const BasicString<Char>& left, const Char* right) {
		FGL_ASSERT(right != nullptr, "right cannot be null");
		return (left.compare(right) <= 0);
	}
	
	template<typename Char>
	bool operator<=(const Char* left, const BasicString<Char>& right) {
		FGL_ASSERT(left != nullptr, "left cannot be null");
		return (right.compare(left) >= 0);
	}
	
	template<typename Char>
	bool operator>(const BasicString<Char>& left, const BasicString<Char>& right) {
		return (left.compare(right) > 0);
	}
	
	template<typename Char>
	bool operator>(const BasicString<Char>& left, const Char* right) {
		FGL_ASSERT(right != nullptr, "right cannot be null");
		return (left.compare(right) > 0);
	}
	
	template<typename Char>
	bool operator>(const Char* left, const BasicString<Char>& right) {
		FGL_ASSERT(left != nullptr, "left cannot be null");
		return (right.compare(left) < 0);
	}
	
	template<typename Char>
	bool operator>=(const BasicString<Char>& left, const BasicString<Char>& right) {
		return (left.compare(right) >= 0);
	}
	
	template<typename Char>
	bool operator>=(const BasicString<Char>& left, const Char* right) {
		FGL_ASSERT(right != nullptr, "right cannot be null");
		return (left.compare(right) >= 0);
	}
	
	template<typename Char>
	bool operator>=(const Char* left, const BasicString<Char>& right) {
		FGL_ASSERT(left != nullptr, "left cannot be null");
		return (right.compare(left) <= 0);
	}
}

#ifdef NAPI_MODULE
#include <fgl/util/NAPI_undef.h>
#endif
