//
//  Variant.hpp
//  DataCpp
//
//  Created by Luis Finke on 9/1/21.
//  Copyright © 2021 Luis Finke. All rights reserved.
//

#pragma once

#include <fgl/data/Common.hpp>
#include <variant>

namespace fgl {
	template<typename... Types>
	using Variant = std::variant<Types...>;

	template<typename T>
	struct is_variant: std::false_type {};
	template<typename... T>
	struct is_variant<std::variant<T...>>: std::true_type {};
}
