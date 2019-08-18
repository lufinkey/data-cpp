//
//  Error.cpp
//  DataCpp
//
//  Created by Luis Finke on 8/18/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#include "Error.hpp"
#include <fgl/data/String.hpp>

namespace fgl {
	Error::Error() {
		//
	}
	
	Error::Error(String msg)
#ifdef _MSC_VER
	: std::exception(msg),
	message(msg)
#else
	: message(msg)
#endif
	{
		//
	}
	
	Error::~Error() {
		//
	}
	
	const char* Error::what() const noexcept {
		return message;
	}
	
	String Error::toString() const {
		return what();
	}
	
	String Error::getClassName() const {
		return "Error";
	}
}
