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
	BasicString<Char>::BasicString(const BaseType& str): BaseType(str) {
		//
	}

	template<typename Char>
	BasicString<Char>::BasicString(BaseType&& str): BaseType(str) {
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
		resize(size_new);
		[nsString getCharacters:(unichar*)((Char*)data()) range:range];
	}
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char,Char>::null_type,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
			&& sizeof(_Char)==sizeof(char)), std::nullptr_t>::type>
	BasicString<Char>::BasicString(NSString* nsString)
	: BaseType((nsString != nil) ? (const Char*)[nsString UTF8String] : "") {
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
		assign(std::move(BasicStringUtils::convert<Char,unichar>(buffer.get(), (size_t)nsString_length)));
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
		resize(stringLength);
		size_t stringLengthCopied = 0;
		char* stringData = (char*)data();
		DATACPP_NAPI_CALL(env, "failed to copy string", napi_get_value_string_utf8((env), value, stringData, stringLength + 1, &stringLengthCopied));
		DATACPP_NAPI_ASSERT(env, stringLength == stringLengthCopied, "Couldn't fully copy data to string");
	}

	#endif


	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>::BasicString(const OtherChar* str, size_type length)
	: BaseType((const Char*)str, length) {
		FGL_ASSERT(str != nullptr, "str cannot be null");
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>::BasicString(const OtherChar* str, size_type length)
	: BaseType(BasicStringUtils::convert<Char,OtherChar>(str, length)) {
		//
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>::BasicString(const OtherChar* str)
	: BaseType((const Char*)str) {
		FGL_ASSERT(str != nullptr, "str cannot be null");
		// same char size
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>::BasicString(const OtherChar* str)
	: BaseType(BasicStringUtils::convert<Char,OtherChar>(str, BasicStringUtils::strlen<OtherChar>(str))) {
		// different char size
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>::BasicString(const BasicString<OtherChar>& str)
	: BaseType(str) {
		// same char size
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>::BasicString(BasicString<OtherChar>&& str)
	: BaseType(str) {
		// same char size
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>::BasicString(const BasicString<OtherChar>& str)
	: BaseType(BasicStringUtils::convert<Char,OtherChar>(str.data(), str.length())) {
		// different char size
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>::BasicString(const std::basic_string<OtherChar>& str)
	: BaseType(str) {
		// same char size
	}
	
	template<typename Char>
	template<typename OtherChar,
	typename BasicStringUtils::same_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>::BasicString(std::basic_string<OtherChar>&& str)
	: BaseType(str) {
		// same char size
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type>
	BasicString<Char>::BasicString(const std::basic_string<OtherChar>& str)
	: BaseType(BasicStringUtils::convert<Char,OtherChar>(str.data(), str.length())) {
		// different char size
	}
	
	
	
	template<typename Char>
	BasicString<Char>::operator const Char*() const noexcept {
		return c_str();
	}
	
	template<typename Char>
	BasicString<Char>::operator std::basic_string<Char>&() & noexcept {
		return (std::basic_string<Char>&)*this;
	}

	template<typename Char>
	BasicString<Char>::operator std::basic_string<Char>&&() && noexcept {
		return (std::basic_string<Char>&&)*this;
	}
	
	template<typename Char>
	BasicString<Char>::operator const std::basic_string<Char>&() const& noexcept {
		return (const std::basic_string<Char>&)*this;
	}
	
	
	
	#ifdef __OBJC__
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char,Char>::null_type,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
			&& sizeof(unichar)==sizeof(_Char) && sizeof(_Char)!=sizeof(char)), std::nullptr_t>::type>
	NSString* BasicString<Char>::toNSString() const {
		return [NSString stringWithCharacters:(const unichar*)data() length:(NSUInteger)length()];
	}
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char,Char>::null_type,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
			&& sizeof(_Char)==sizeof(char)), std::nullptr_t>::type>
	NSString* BasicString<Char>::toNSString() const {
		return [NSString stringWithUTF8String:(const char*)data()];
	}
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char, Char>::null_type,
		typename std::enable_if<(BasicStringUtils::can_convert_string_type<_Char>::value
			&& sizeof(unichar)!=sizeof(_Char) && sizeof(_Char)!=sizeof(char)), std::nullptr_t>::type>
	NSString* BasicString<Char>::toNSString() const {
		auto output = BasicStringUtils::convert<unichar,Char>(data(), length());
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
			return env->NewStringUTF(c_str());
		} else {
			return env->NewStringUTF(BasicStringUtils::convert<char,Char>(data(), length()).c_str());
		}
	}

	#endif

	#ifdef NODE_API_MODULE

	template<typename Char>
	Napi::String BasicString<Char>::toNapiString(napi_env env) const {
		return Napi::String::New(env, *this);
	}

	#endif

	#ifdef NAPI_MODULE

	template<typename Char>
	napi_value BasicString<Char>::toNodeJSValue(napi_env env) const {
		if constexpr(sizeof(Char) == 1 && std::is_integral<Char>::value) {
			napi_value value = nullptr;
			DATACPP_NAPI_CALL(env, "failed to create napi_value", napi_create_string_utf8(env, data(), length(), &value));
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
		return std::basic_string<OtherChar>((const OtherChar*)data(), length());
	}
	
	template<typename Char>
	template<typename OtherChar,
		typename BasicStringUtils::diff_size_convertable_strings<Char,OtherChar>::null_type>
	std::basic_string<OtherChar> BasicString<Char>::toStdString() const {
		// different char size
		return BasicStringUtils::convert<OtherChar,Char>(data(), length());
	}
	
	template<typename Char>
	template<typename SameChar,
		typename BasicStringUtils::is_same<Char,SameChar>::null_type>
	std::basic_string<SameChar> BasicString<Char>::toStdString() const {
		// same char
		return std::basic_string<SameChar>(data(), length());
	}
	
	template<typename Char>
	template<typename SomeChar,
		typename BasicStringUtils::can_convert_string_types<Char,SomeChar>::null_type>
	BasicString<SomeChar> BasicString<Char>::toBasicString() const {
		return BasicString<SomeChar>(data(), length());
	}
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::can_convert_string_types<Char,char>::null_type>
	BasicString<char> BasicString<Char>::toString() const {
		if constexpr(std::is_same<Char,char>::value) {
			return *this;
		}
		return BasicStringUtils::convert<char,Char>(data(), length());
	}
	
	

	template<typename Char>
	BasicString<Char>& BasicString<Char>::operator=(const BaseType& str) {
		BaseType::operator=(str);
		return *this;
	}

	template<typename Char>
	BasicString<Char>& BasicString<Char>::operator=(BaseType&& str) {
		BaseType::operator=(str);
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
		resize(size_new);
		[nsString getCharacters:(unichar*)((Char*)data()) range:range];
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
		assign((const Char*)[nsString UTF8String]);
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
		assign(std::move(BasicStringUtils::convert<Char,unichar>(buffer_data, length)));
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
		assign(std::move(BasicStringUtils::convert<Char,OtherChar>(str, BasicStringUtils::strlen<OtherChar>(str))));
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
		assign(std::move(BasicStringUtils::convert<Char,OtherChar>(str.data(), str.length())));
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
		assign(std::move(BasicStringUtils::convert<Char,OtherChar>(str.data(), str.length())));
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
		size_t size_new = BasicStringUtils::get_safe_resize<Char>(size(), (size_t)nsString_length);
		size_t size_old = size();
		NSRange range = NSMakeRange(0, nsString_length);
		resize(size_new);
		[nsString getCharacters:(unichar*)((Char*)(data()+size_old)) range:range];
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
	int BasicString<Char>::compare(const Char* cmp, size_type cmp_length, const std::locale& locale) const {
		FGL_ASSERT(cmp != nullptr, "cmp cannot be null");
		return std::use_facet<std::collate<Char>>(locale).compare(data(), data()+length(), cmp, cmp+cmp_length);
	}
	
	template<typename Char>
	int BasicString<Char>::compare(const Char* cmp, const std::locale& locale) const {
		FGL_ASSERT(cmp != nullptr, "cmp cannot be null");
		return compare(cmp, BasicStringUtils::strlen<Char>(cmp), locale);
	}
	
	template<typename Char>
	int BasicString<Char>::compare(const std::basic_string<Char>& cmp, const std::locale& locale) const {
		return compare(cmp.data(), cmp.length(), locale);
	}
	
	template<typename Char>
	constexpr bool BasicString<Char>::equals(const Char* str, size_type str_length) const {
		FGL_ASSERT(str != nullptr, "str cannot be null");
		if(length() != str_length) {
			return false;
		}
		for(size_t i=0; i<length; i++) {
			if(BaseType::operator[](i) != str[i]) {
				return false;
			}
		}
		return true;
	}
	
	template<typename Char>
	bool BasicString<Char>::equals(const Char* str) const {
		return equals(str, BasicStringUtils::strlen<Char>(str));
	}
	
	template<typename Char>
	bool BasicString<Char>::equals(const std::basic_string<Char>& str) const {
		return equals(str.data(), str.length());
	}



	template<typename Char>
	typename BasicString<Char>::size_type BasicString<Char>::maxSize() const noexcept {
		return max_size();
	}
	
	template<typename Char>
	typename BasicString<Char>::reference BasicString<Char>::operator[](size_type index) {
		FGL_ASSERT(index >= 0 && index < (length() + 1), "index out of bounds");
		return BaseType::operator[](index);
	}

	template<typename Char>
	typename BasicString<Char>::const_reference BasicString<Char>::operator[](size_type index) const {
		FGL_ASSERT(index >= 0 && index < (length() + 1), "index out of bounds");
		return BaseType::operator[](index);
	}

	template<typename Char>
	typename BasicString<Char>::reference BasicString<Char>::charAt(size_type index) {
		return at(index);
	}
	
	template<typename Char>
	typename BasicString<Char>::const_reference BasicString<Char>::charAt(size_type index) const {
		return at(index);
	}
	
	template<typename Char>
	typename BasicString<Char>::size_type BasicString<Char>::indexOf(Char c, size_type startIndex) const noexcept {
		return find(c, startIndex);
	}
	
	template<typename Char>
	typename BasicString<Char>::size_type BasicString<Char>::indexOf(const std::basic_string<Char>& str, size_type startIndex) const noexcept {
		return find(str, startIndex);
	}
	
	template<typename Char>
	typename BasicString<Char>::size_type BasicString<Char>::indexOf(const Char* str, size_type startIndex) const {
		FGL_ASSERT(str != nullptr, "find cannot be null");
		return find(str, startIndex);
	}
	
	template<typename Char>
	typename BasicString<Char>::size_type BasicString<Char>::lastIndexOf(Char c, size_type startIndex) const noexcept {
		return rfind(c, startIndex);
	}
	
	template<typename Char>
	typename BasicString<Char>::size_type BasicString<Char>::lastIndexOf(const std::basic_string<Char>& str, size_type startIndex) const noexcept {
		return rfind(str, startIndex);
	}
	
	template<typename Char>
	typename BasicString<Char>::size_type BasicString<Char>::lastIndexOf(const Char* str, size_type startIndex) const {
		FGL_ASSERT(str != nullptr, "find cannot be null");
		return rfind(str, startIndex);
	}
	
	template<typename Char>
	bool BasicString<Char>::startsWith(const Char* str, size_type str_length) const {
		FGL_ASSERT(str != nullptr, "str cannot be null");
		if(str_length > length()) {
			return false;
		}
		for(size_type i=0; i<str_length; i++) {
			if(BaseType::operator[](i) != str[i]) {
				return false;
			}
		}
		return true;
	}
	
	template<typename Char>
	bool BasicString<Char>::startsWith(const Char* str) const {
		FGL_ASSERT(str != nullptr, "str cannot be null");
		size_t i = 0;
		Char c = str[i];
		while(c != '\0') {
			if(i >= length()) {
				return false;
			}
			if(BaseType::operator[](i) != c) {
				return false;
			}
			i++;
			c = str[i];
		}
		return true;
	}
	
	template<typename Char>
	bool BasicString<Char>::startsWith(const std::basic_string<Char>& str) const {
		return startsWith(str.data(), str.length());
	}
	
	template<typename Char>
	bool BasicString<Char>::endsWith(const Char* str, size_type str_length) const {
		FGL_ASSERT(str != nullptr, "str cannot be null");
		if(str_length > length()) {
			return false;
		}
		size_type str_index = str_length - 1;
		size_type str_end = length() - str_length - 1;
		for(size_type i=(length()-1); i!=str_end; i--) {
			if(BaseType::operator[](i) != str[str_index]) {
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
	BasicString<Char> BasicString<Char>::replacing(Char find, Char replace) const {
		BasicString<Char> newStr;
		newStr.reserve(length());
		for(size_type i=0; i<length(); i++) {
			Char c = BaseType::operator[](i);
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
		else if(find.length() > length()) {
			return *this;
		}
		LinkedList<size_type> indexes;
		size_type finish = length() - find.size;
		for(size_type i=0; i<=finish; i++) {
			if(BaseType::operator[](i) == find[0]) {
				bool match = true;
				for(size_t j=1; j<find.size; j++) {
					if(BaseType::operator[](i+j) != find[j]) {
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
		size_type size_new = length() + (replace.size*indexes_size) - (find.size*indexes_size);
		newStr.reserve(size_new);
		size_t oldStr_counter = 0;
		auto indexes_it = indexes.begin();
		for(size_t i=0; i<size_new; i++) {
			if(oldStr_counter == *indexes_it) {
				for(size_t j=0; j<replace.length(); j++) {
					newStr.append(1,replace[j]);
					i++;
				}
				i--;
				oldStr_counter += find.length();
				indexes_it++;
			}
			else {
				newStr.append(1, BaseType::operator[](oldStr_counter));
				oldStr_counter++;
			}
		}
		return newStr;
	}
	
	template<typename Char>
	BasicString<Char> BasicString<Char>::replacing(const std::basic_regex<Char>& find, const std::basic_string<Char>& replace, std::regex_constants::match_flag_type flags) const {
		return std::regex_replace(*this, find, replace, flags);
	}
	
	template<typename Char>
	template<typename InputIt>
	BasicString<Char> BasicString<Char>::replacing(size_type startIndex, size_type count, InputIt first, InputIt last) const {
		size_type endIndex = startIndex + count;
		if(startIndex >= length()) {
			throw std::out_of_range("index " + std::to_string(startIndex) + " is out of bounds in BasicString<" + typeid(Char).name() + "> with a length of " + std::to_string(length()));
		}
		else if(endIndex < startIndex || endIndex > length()) {
			throw std::out_of_range("index " + std::to_string(startIndex) + " plus count " + std::to_string(count) + " is out of bounds in BasicString<" + typeid(Char).name() + "> with a length of " + std::to_string(length()));
		}
		size_type replaceLength = std::distance(first, last);
		BasicString<Char> newStr;
		size_type size_new = BasicStringUtils::get_safe_resize<Char>(length() - count, replaceLength);
		newStr.reserve(size_new);
		newStr.append(data(), startIndex);
		newStr.append(first, last);
		newStr.append(data()+endIndex, length()-endIndex);
		return newStr;
	}
	
	template<typename Char>
	BasicString<Char> BasicString<Char>::replacing(size_type startIndex, size_type count, const std::basic_string<Char>& replace) const {
		return replacing(startIndex, count, replace.begin(), replace.end());
	}
	
	template<typename Char>
	BasicString<Char> BasicString<Char>::substring(size_type startIndex, size_type count) const {
		return substr(startIndex, count);
	}

	template<typename Char>
	BasicStringView<Char> BasicString<Char>::viewSubstring(size_type startIndex, size_type count) const {
		FGL_ASSERT(startIndex <= size(), "startIndex must be within the range of the string");
		auto start = std::next(this->begin(), startIndex);
		auto end = ((startIndex + count) >= size()) ? this->end() : std::next(start, count);
		return BasicStringView<Char>(start, end);
	}
	
	template<typename Char>
	LinkedList<BasicString<Char>> BasicString<Char>::split(Char delim) const {
		LinkedList<BasicString<Char>> items;
		size_t lastStart = 0;
		for(size_t i=0; i<length(); i++) {
			if(BaseType::operator[](i) == delim) {
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
		if(delim_size == 0 || delim_size > length()) {
			LinkedList<BasicString<Char>> elements;
			elements.pushBack(*this);
			return elements;
		}
		LinkedList<BasicString<Char>> items;
		size_t lastStart = 0;
		size_t finish = length() - delim_size;
		for(size_t i=0; i<=finish; i++) {
			if(BaseType::operator[](i) == delim[0]) {
				bool match = true;
				for(size_t j=1; j<delim_size; j++) {
					if(BaseType::operator[](i+j) != delim[j]) {
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
		items.pushBack(substring(lastStart, length()-lastStart));
		return items;
	}
	
	template<typename Char>
	LinkedList<BasicString<Char>> BasicString<Char>::split(const std::basic_string<Char>& delim) const {
		if(delim.length() == 0 || delim.length() > length()) {
			LinkedList<BasicString<Char>> items;
			items.pushBack(*this);
			return items;
		}
		LinkedList<BasicString<Char>> items;
		size_t lastStart = 0;
		size_t finish = length() - delim.length();
		for(size_t i=0; i<=finish; i++) {
			if(BaseType::operator[](i) == delim[0]) {
				bool match = true;
				for(size_t j=1; j<delim.length(); j++) {
					if(BaseType::operator[](i+j) != delim[j]) {
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
		items.pushBack(substring(lastStart, length()-lastStart));
		return items;
	}
	
	template<typename Char>
	template<typename _Char,
	typename BasicStringUtils::is_same<_Char, Char>::null_type,
	typename BasicStringUtils::can_convert_string_type<_Char>::null_type>
	BasicString<Char> BasicString<Char>::trim(const std::locale& locale) const {
		typedef typename BasicStringUtils::utf_eqv<Char>::type utf_char;
		if(length() == 0) {
			return *this;
		}
		bool hitLetter = false;
		size_type startIndex = 0;
		for(size_type i=0; i<length() && !hitLetter; i++) {
			if(!std::isspace<utf_char>((utf_char)BaseType::operator[](i), locale)) {
				startIndex = i;
				hitLetter = true;
			}
		}
		if(!hitLetter) {
			return BasicString<Char>();
		}
		hitLetter = false;
		size_type endIndex = length();
		for(size_type i=(length()-1); i>=startIndex && i!=(size_type)-1 && !hitLetter; i--) {
			if(!std::isspace<utf_char>((utf_char)BaseType::operator[](i), locale)) {
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
		newStr.reserve(length());
		for(size_type i=0; i<length(); i++) {
			typedef typename BasicStringUtils::utf_eqv<Char>::type UTFChar;
			newStr.append(1,(Char)std::tolower<UTFChar>((UTFChar)BaseType::operator[](i), locale));
		}
		return newStr;
	}
	
	template<typename Char>
	template<typename _Char,
		typename BasicStringUtils::is_same<_Char, Char>::null_type,
		typename BasicStringUtils::can_convert_string_type<_Char>::null_type>
	BasicString<Char> BasicString<Char>::toUpperCase(const std::locale& locale) const {
		BasicString<Char> newStr;
		newStr.reserve(length());
		for(size_type i=0; i<length(); i++) {
			typedef typename BasicStringUtils::utf_eqv<Char>::type UTFChar;
			newStr.append(1,(Char)std::toupper<UTFChar>((UTFChar)BaseType::operator[](i), locale));
		}
		return newStr;
	}

	template<typename Char>
	bool BasicString<Char>::isDigits(std::locale locale) const {
		size_t len = length();
		for(size_t i=0; i<len; i++) {
			Char c = (*this)[i];
			if(!std::isdigit<Char>(c, locale)) {
				return false;
			}
		}
		return false;
	}
	
	template<typename Char>
	template<typename Num,
		typename BasicStringUtils::string_type_convertable_with_number<Char,Num>::null_type>
	Num BasicString<Char>::toArithmeticValue(const std::locale& locale) const {
		std::basic_istringstream<Char> convert(*this);
		convert.imbue(locale);
		Num numVal = 0;
		convert >> numVal;
		if(convert.fail()) {
			throw std::logic_error("BasicString does not represent an arithmetic value");
		}
		return numVal;
	}
	
	
	
	
	template<typename Char>
	template<typename Collection, typename _>
	BasicString<Char> BasicString<Char>::join(Collection&& collection, const BasicString<Char>& separator) {
		if(collection.size() == 0) {
			return BasicString<Char>();
		}
		size_type listSize = 0;
		size_type i = 0;
		size_type lastListIndex = collection.size() - 1;
		for(auto& str : collection) {
			listSize = BasicStringUtils::get_safe_resize<Char>(listSize, str.length());
			if(i != lastListIndex) {
				listSize = BasicStringUtils::get_safe_resize<Char>(listSize, separator.length());
			}
			i++;
		}
		BasicString<Char> joined;
		joined.reserve(listSize);
		i = 0;
		for(auto& str : collection) {
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
		if(list.size() == 0) {
			return BasicString<Char>();
		}
		size_type listSize = 0;
		size_type i = 0;
		size_type lastListIndex = list.size() - 1;
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
	BasicString<Char> BasicString<Char>::random(size_t length, const std::vector<Char>& charSet) {
		BasicString<Char> str;
		str.reserve(length);
		for(size_t i=0; i<length; i++) {
			size_t randIndex = (size_t)(((double)rand() / (double)RAND_MAX) * charSet.size());
			str.append(1, charSet[randIndex]);
		}
		return str;
	}

	template<typename Char>
	BasicString<Char> BasicString<Char>::random(size_t length, const std::string& charSet) {
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
		os << (const std::basic_string<Char>&)str;
		return os;
	}


	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type>
	BasicString<Char> operator+(const BasicString<Char>& left, const BasicString<OtherChar>& right) {
		if constexpr(std::is_same_v<Char,OtherChar>) {
			return operator+((const std::basic_string<Char>&)left, (const std::basic_string<Char>&)right);
		} else {
			return BasicStringUtils::concat(left, right);
		}
	}

	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type>
	BasicString<Char> operator+(BasicString<Char>&& left, const BasicString<OtherChar>& right) {
		if constexpr(std::is_same_v<Char,OtherChar>) {
			return operator+((std::basic_string<Char>&&)left, (const std::basic_string<Char>&)right);
		} else {
			return BasicStringUtils::concat(left, right);
		}
	}

	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type>
	BasicString<Char> operator+(const BasicString<Char>& left, BasicString<OtherChar>&& right) {
		if constexpr(std::is_same_v<Char,OtherChar>) {
			return operator+((const std::basic_string<Char>&)left, (std::basic_string<Char>&&)right);
		} else {
			return BasicStringUtils::concat(left, right);
		}
	}

	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type>
	BasicString<Char> operator+(BasicString<Char>&& left, BasicString<OtherChar>&& right) {
		if constexpr(std::is_same_v<Char,OtherChar>) {
			return operator+((std::basic_string<Char>&&)left, (std::basic_string<Char>&&)right);
		} else {
			return BasicStringUtils::concat(left, right);
		}
	}


	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type>
	BasicString<Char> operator+(const BasicString<Char>& left, const OtherChar* right) {
		return BasicStringUtils::concat(left, right);
	}
	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type>
	BasicString<Char> operator+(const Char* left, const BasicString<OtherChar>& right) {
		return BasicStringUtils::concat(left, right);
	}


	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type>
	BasicString<Char> operator+(const BasicString<Char>& left, const std::basic_string<OtherChar>& right) {
		if constexpr(std::is_same_v<Char,OtherChar>) {
			return operator+((const std::basic_string<Char>&)left, right);
		} else {
			return BasicStringUtils::concat(left, right);
		}
	}

	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type>
	BasicString<Char> operator+(BasicString<Char>&& left, const std::basic_string<OtherChar>& right) {
		if constexpr(std::is_same_v<Char,OtherChar>) {
			return operator+((std::basic_string<Char>&&)left, right);
		} else {
			return BasicStringUtils::concat(left, right);
		}
	}

	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type>
	BasicString<Char> operator+(const BasicString<Char>& left, std::basic_string<OtherChar>&& right) {
		if constexpr(std::is_same_v<Char,OtherChar>) {
			return operator+((const std::basic_string<Char>&)left, right);
		} else {
			return BasicStringUtils::concat(left, right);
		}
	}

	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type>
	BasicString<Char> operator+(BasicString<Char>&& left, std::basic_string<OtherChar>&& right) {
		if constexpr(std::is_same_v<Char,OtherChar>) {
			return operator+((std::basic_string<Char>&&)left, right);
		} else {
			return BasicStringUtils::concat(left, right);
		}
	}


	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type>
	BasicString<Char> operator+(const std::basic_string<Char>& left, const BasicString<OtherChar>& right) {
		if constexpr(std::is_same_v<Char,OtherChar>) {
			return operator+(left, (const std::basic_string<Char>&)right);
		} else {
			return BasicStringUtils::concat(left, right);
		}
	}

	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type>
	BasicString<Char> operator+(std::basic_string<Char>&& left, const BasicString<OtherChar>& right) {
		if constexpr(std::is_same_v<Char,OtherChar>) {
			return operator+(left, (const std::basic_string<Char>&)right);
		} else {
			return BasicStringUtils::concat(left, right);
		}
	}

	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type>
	BasicString<Char> operator+(const std::basic_string<Char>& left, BasicString<OtherChar>&& right) {
		if constexpr(std::is_same_v<Char,OtherChar>) {
			return operator+(left, (std::basic_string<Char>&&)right);
		} else {
			return BasicStringUtils::concat(left, right);
		}
	}

	template<typename Char, typename OtherChar,
		typename BasicStringUtils::can_convert_string_types<Char,OtherChar>::null_type>
	BasicString<Char> operator+(std::basic_string<Char>&& left, BasicString<OtherChar>&& right) {
		if constexpr(std::is_same_v<Char,OtherChar>) {
			return operator+(left, (std::basic_string<Char>&&)right);
		} else {
			return BasicStringUtils::concat(left, right);
		}
	}


	
	#ifdef __OBJC__
	
	template<typename Char,
		typename BasicStringUtils::can_convert_string_types<Char,unichar>::null_type>
	BasicString<Char> operator+(const BasicString<Char>& left, NSString* right) {
		return BasicStringUtils::concat(left, right);
	}
	
	template<typename Char,
		typename BasicStringUtils::can_convert_string_types<Char,unichar>::null_type>
	BasicString<Char> operator+(NSString* left, const BasicString<Char>& right) {
		return BasicStringUtils::concat(left, right);
	}
	
	#endif
	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::string_type_convertable_with_char_type<Char,OtherChar>::null_type>
	BasicString<Char> operator+(const BasicString<Char>& left, OtherChar right) {
		return BasicStringUtils::concat(left, right);
	}
	
	template<typename Char, typename OtherChar,
		typename BasicStringUtils::string_type_convertable_with_char_type<Char,OtherChar>::null_type>
	BasicString<Char> operator+(Char left, const BasicString<OtherChar>& right) {
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
	auto operator<=>(const BasicString<Char>& left, const BasicString<Char>& right) {
		return operator<=>((const std::basic_string<Char>&)left, (const std::basic_string<Char>&)right);
	}
	
	
	
	template<typename Char>
	bool operator==(const BasicString<Char>& left, const BasicString<Char>& right) {
		return operator==((const std::basic_string<Char>&)left, (const std::basic_string<Char>&)right);
	}

	template<typename Char>
	bool operator==(const BasicString<Char>& left, const std::basic_string<Char>& right) {
		return operator==((const std::basic_string<Char>&)left, right);
	}

	template<typename Char>
	bool operator==(const std::basic_string<Char>& left, const BasicString<Char>& right) {
		return operator==(left, (const std::basic_string<Char>&)right);
	}
	
	template<typename Char>
	bool operator==(const BasicString<Char>& left, const Char* right) {
		return operator==((const std::basic_string<Char>&)left, right);
	}
	
	template<typename Char>
	bool operator==(const Char* left, const BasicString<Char>& right) {
		return operator==(left, (const std::basic_string<Char>&)right);
	}
	
	template<typename Char>
	bool operator==(const BasicString<Char>& left, Char right) {
		Char right_arr[] = {right, NULLCHAR};
		return operator==((const std::basic_string<Char>&)left, right_arr);
	}
	
	template<typename Char>
	bool operator==(Char left, const BasicString<Char>& right) {
		Char left_arr[] = {left, NULLCHAR};
		return operator==(left_arr, (const std::basic_string<Char>&)right);
	}
	
	template<typename Char>
	bool operator!=(const BasicString<Char>& left, const BasicString<Char>& right) {
		return operator!=((const std::basic_string<Char>&)left, (const std::basic_string<Char>&)right);
	}
	
	template<typename Char>
	bool operator!=(const BasicString<Char>& left, const Char* right) {
		return operator!=((const std::basic_string<Char>&)left, right);
	}
	
	template<typename Char>
	bool operator!=(const Char* left, const BasicString<Char>& right) {
		return operator!=(left, (const std::basic_string<Char>&)right);
	}
	
	template<typename Char>
	bool operator!=(const BasicString<Char>& left, const Char& right) {
		Char right_arr[] = {right, NULLCHAR};
		return operator!=((const std::basic_string<Char>&)left, right_arr);
	}
	
	template<typename Char>
	bool operator!=(const Char& left, const BasicString<Char>& right) {
		Char left_arr[] = {left, NULLCHAR};
		return operator!=(left_arr, (const std::basic_string<Char>&)right);
	}
	
	template<typename Char>
	bool operator<(const BasicString<Char>& left, const BasicString<Char>& right) {
		return operator<((const std::basic_string<Char>&)left, (const std::basic_string<Char>&)right);
	}
	
	template<typename Char>
	bool operator<(const BasicString<Char>& left, const Char* right) {
		return operator<((const std::basic_string<Char>&)left, right);
	}
	
	template<typename Char>
	bool operator<(const Char* left, const BasicString<Char>& right) {
		return operator<(left, (const std::basic_string<Char>&)right);
	}
	
	template<typename Char>
	bool operator<=(const BasicString<Char>& left, const BasicString<Char>& right) {
		return operator<=((const std::basic_string<Char>&)left, (const std::basic_string<Char>&)right);
	}
	
	template<typename Char>
	bool operator<=(const BasicString<Char>& left, const Char* right) {
		return operator<=((const std::basic_string<Char>&)left, right);
	}
	
	template<typename Char>
	bool operator<=(const Char* left, const BasicString<Char>& right) {
		return operator<=(left, (const std::basic_string<Char>&)right);
	}
	
	template<typename Char>
	bool operator>(const BasicString<Char>& left, const BasicString<Char>& right) {
		return operator>((const std::basic_string<Char>&)left, (const std::basic_string<Char>&)right);
	}
	
	template<typename Char>
	bool operator>(const BasicString<Char>& left, const Char* right) {
		return operator>((const std::basic_string<Char>&)left, right);
	}
	
	template<typename Char>
	bool operator>(const Char* left, const BasicString<Char>& right) {
		return operator>(left, (const std::basic_string<Char>&)right);
	}
	
	template<typename Char>
	bool operator>=(const BasicString<Char>& left, const BasicString<Char>& right) {
		return operator>=((const std::basic_string<Char>&)left, (const std::basic_string<Char>&)right);
	}
	
	template<typename Char>
	bool operator>=(const BasicString<Char>& left, const Char* right) {
		return operator>=((const std::basic_string<Char>&)left, right);
	}
	
	template<typename Char>
	bool operator>=(const Char* left, const BasicString<Char>& right) {
		return operator>=(left, (const std::basic_string<Char>&)right);
	}
}

#ifdef NAPI_MODULE
#include <fgl/util/NAPI_undef.h>
#endif
