//
//  Common.hpp
//  DataCpp
//
//  Created by Luis Finke on 8/14/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#include <exception>
#include <functional>
#include <iostream>
#include <list>
#include <tuple>
#include <vector>
#ifdef __OBJC__
#import <Foundation/Foundation.h>
#endif

namespace fgl {
	template<typename T>
	using Function = std::function<T>;

	struct empty{};

	using std::make_pair;
	using std::make_tuple;

	template<typename ...T>
	using Tuple = std::tuple<T...>;
	
	#ifndef FGL_ASSERT
		#ifdef NDEBUG
			#define FGL_ASSERT(condition, message) assert(condition)
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

	#ifndef FGL_FINALLY_DEF
		#define FGL_FINALLY_DEF
		template<typename Callable>
		class Finally {
		public:
			Finally(Callable c): callable(c), enabled(true) {}
			Finally(Finally&& f): callable(f.callable), enabled(f.enabled) {
				f.disable();
			}
			~Finally() {
				if(enabled) {
					callable();
				}
			}
			void disable() {
				enabled = false;
			}
		private:
			Callable callable;
			bool enabled;
		};
		template<typename Callable>
		Finally<Callable> make_finally(Callable c) {
			return Finally<Callable>(c);
		}
	#endif
}
