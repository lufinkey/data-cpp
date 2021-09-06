//
//  Data.hpp
//  DataCpp
//
//  Created by Luis Finke on 9/5/21.
//  Copyright © 2021 Luis Finke. All rights reserved.
//

#pragma once

#include <fgl/data/Common.hpp>
#include <fgl/data/BasicString.hpp>
#include <vector>

namespace fgl {
	class Data: public std::vector<uint8_t> {
	public:
		using vector::vector;
		using vector::begin;
		using vector::end;
		
		Data fromString(const String&);
		String toString() const;
	};
}
