//
//  Data.cpp
//  DataCpp
//
//  Created by Luis Finke on 9/5/21.
//  Copyright © 2021 Luis Finke. All rights reserved.
//

#include <fgl/data/Data.hpp>
#include <fgl/data/String.hpp>

namespace fgl {
	Data Data::fromString(const String& str) {
		return Data(str.begin(), str.end());
	}

	String Data::toString() const {
		return String(begin(), end());
	}
}
