//
//  Error.hpp
//  DataCpp
//
//  Created by Luis Finke on 8/14/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#include <exception>
#include <fgl/data/Common.hpp>
#include <fgl/data/BasicString.hpp>

namespace fgl {
	/*! An error class to throw when a generic error arises */
	class Error: std::exception {
	public:
		/*! An explanation of the error*/
		String message;
		
		/*! default constructor*/
		Error();
		/*! Constructs an error with a message.
			\param message an explanation of the error */
		explicit Error(String message);
		/*! virtual destructor*/
		virtual ~Error();
		
		/*! Gets an explanation of the error.
		 \returns a const char pointer containing an error message*/
		virtual const char* what() const noexcept override;
		
		/*! Gets a String representation of the Error
		 \returns a String representation of the error */
		String toString() const;
		
		/*! Gets the name of the Error class. This function must be overridden in every subclass of Error.
		 \returns a String representing the class name of the error */
		virtual String getClassName() const;
	};
	
	#define ErrorClass(ClassName, BaseClass, ...) \
		class ClassName : BaseClass { \
			virtual String getClassName() const override { \
				return #ClassName; \
			} \
			__VA_ARGS__ \
		}
}
