//
//  Error.hpp
//  DataCpp
//
//  Created by Luis Finke on 8/14/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#include <fgl/data/Common.hpp>
#include <fgl/data/BasicString.hpp>
#include <exception>

namespace fgl {
	class Any;

	/*! An error interface to conform to for errors */
	class Error {
	public:
		/*! virtual destructor*/
		virtual ~Error();
		
		/*! Gets an explanation of the error.
		 \returns a String containing an error message*/
		virtual String getMessage() const = 0;
		
		/*! Gets a String representation of the Error
		 \returns a String representation of the error */
		virtual String toString() const;
		
		/*! Gets a detail of the Error
		 \returns an object for a particular error detail, or an empty Any, if no matching detail exists */
		virtual Any getDetail(const String& key) const;
	};

	template<typename Exception = std::exception>
	class BasicError: public Exception, public Error {
		//
	};
}
