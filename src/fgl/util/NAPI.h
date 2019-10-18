//
//  NAPI.h
//  DataCpp
//
//  Created by Luis Finke on 10/16/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#define DATACPP_NAPI_THROW_LAST_ERROR(env, fail_action) { \
	const napi_extended_error_info* error_info; \
	napi_get_last_error_info((env), &error_info); \
	bool is_pending; \
	napi_is_exception_pending((env), &is_pending); \
	/* If an exception is already pending, don't rethrow it */ \
	if (!is_pending) { \
		const char* error_message = (error_info->error_message != nullptr) ? \
			(error_info->error_message) \
			: "empty error message"; \
		napi_throw_error((env), NULL, error_message); \
		printf("Error in file %s at line %i: %s\n", __FILE__, __LINE__, error_message); \
		fail_action; \
	} \
}

#define DATACPP_NAPI_ASSERT_BASE(env, assertion, message, fail_action) { \
	if (!(assertion)) { \
		napi_throw_error((env), "ASSERTION_FAILED", "assertion (" #assertion ") failed: " message); \
		fail_action; \
	} \
}

// Returns NULL on failed assertion.
// This is meant to be used inside napi_callback methods.
#define DATACPP_NAPI_ASSERT(env, assertion, message) \
	DATACPP_NAPI_ASSERT_BASE(env, assertion, message, throw std::runtime_error(message))

#define DATACPP_NAPI_CALL_BASE(env, the_call, fail_action, default_fail_action) { \
	if ((the_call) != napi_ok) { \
		DATACPP_NAPI_THROW_LAST_ERROR(env, fail_action); \
		default_fail_action; \
	} \
}

// Returns nullptr if the_call doesn't return napi_ok.
#define DATACPP_NAPI_CALL(env, error, the_call) \
	DATACPP_NAPI_CALL_BASE(env, the_call, throw std::runtime_error(error_message), throw std::runtime_error(error))

// Ensures an napi_value is a certain type
#define DATACPP_NAPI_ASSERT_TYPE(env, value, expectedType) { \
	napi_valuetype valueType; \
	DATACPP_NAPI_CALL(env, "Failed to determine type of value", napi_typeof(env, value, &valueType)); \
	DATACPP_NAPI_ASSERT(env, valueType == expectedType, "Expected a " #expectedType " for value " #value); \
}

#define DATACPP_NAPI_METHOD_DESCRIPTOR(name, func) \
	napi_property_descriptor{ name, 0, func, 0, 0, 0, napi_default, 0 }
