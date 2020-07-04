//
//  Stringify.cpp
//  DataCpp
//
//  Created by Luis Finke on 2/26/20.
//  Copyright © 2020 Luis Finke. All rights reserved.
//

#include <fgl/data/Stringify.hpp>
#include <fgl/data/String.hpp>
#include <cxxabi.h>

namespace fgl {
	std::string demangled_type(std::string const& sym) {
		std::unique_ptr<char, void(*)(void*)>
		name{abi::__cxa_demangle(sym.c_str(), nullptr, nullptr, nullptr), std::free};
		return {name.get()};
	}
}
