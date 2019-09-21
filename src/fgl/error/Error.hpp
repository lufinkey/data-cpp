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
		String toString() const;
	};
}
