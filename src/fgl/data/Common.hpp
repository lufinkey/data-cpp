//
//  Common.hpp
//  DataCpp
//
//  Created by Luis Finke on 8/14/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#include <exception>
#include <iostream>

namespace fgl {
	template<typename T>
	using Function = std::function<T>;
	
	#ifndef FGL_ASSERT
		#ifdef NDEBUG
			#define FGL_ASSERT(condition, message)
		#else
			#define FGL_ASSERT(condition, message) { \
				if(!(condition)) { \
					std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
						<< " line " << __LINE__ << ": " << (message) << std::endl; \
					std::terminate(); \
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
