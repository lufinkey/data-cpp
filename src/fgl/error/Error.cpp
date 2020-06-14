//
//  Error.cpp
//  DataCpp
//
//  Created by Luis Finke on 8/18/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#include "Error.hpp"
#include <fgl/data/String.hpp>
#include <fgl/data/Any.hpp>

namespace fgl {
	Error::~Error() {
		//
	}
	
	String Error::toString() const {
		return this->getMessage();
	}

	Any Error::getDetail(String key) const {
		return Any();
	}
}
