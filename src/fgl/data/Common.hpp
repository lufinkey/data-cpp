//
//  Common.hpp
//  DataCpp
//
//  Created by Luis Finke on 8/14/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#include <any>
#include <exception>
#include <functional>
#include <iostream>
#include <list>
#include <vector>
#ifdef __OBJC__
#import <Foundation/Foundation.h>
#endif

namespace fgl {
	template<typename T>
	using Function = std::function<T>;
	using Any = std::any;

	template<typename T>
	using $ = std::shared_ptr<T>;
	template<typename T>
	using w$ = std::weak_ptr<T>;
	template<typename T, typename... Args>
	$<T> new$(Args&&... args) {
		return std::make_shared<T>(args...);
	}
	
	#ifndef FGL_ASSERT
		#ifdef NDEBUG
			#define FGL_ASSERT(condition, message)
		#else
			#define FGL_ASSERT(condition, message) { \
				if(!(condition)) { \
					std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
						<< " line " << __LINE__ << ": " << (message) << std::endl; \
					assert(false); \
				} \
			}
		#endif
	#endif
	
	#ifndef FGL_WARN
		#ifdef NDEBUG
			#define FGL_WARN(message)
		#else
			#define FGL_WARN(message) { \
				std::cerr << "Warning: " << (message) << std::endl; \
			}
		#endif
	#endif
}
