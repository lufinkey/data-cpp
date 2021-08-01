//
//  Variant.hpp
//  DataCpp
//
//  Created by Luis Finke on 8/1/21.
//  Copyright © 2021 Luis Finke. All rights reserved.
//

#pragma once

#include <fgl/data/Common.hpp>
#include <variant>

namespace fgl {
	template<typename... Ts> struct make_overload: Ts... { using Ts::operator()...; };
	template<typename... Ts> make_overload(Ts...) -> make_overload<Ts...>;

	template<typename Variant, typename... Alternatives>
	decltype(auto) mapVariant(Variant&& variant, Alternatives&&... alternatives) {
		return std::visit(
			make_overload{std::forward<Alternatives>(alternatives)...},
			std::forward<Variant>(variant)
		);
	}
}
