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

#ifdef __OBJC__
#import <Foundation/Foundation.h>
#endif

namespace fgl {
	class Data: public std::vector<uint8_t> {
	public:
		using vector::vector;
		using vector::begin;
		using vector::end;
		
		explicit Data(const String&);
		#ifdef __OBJC__
		Data(NSData* data);
		#endif
		
		String toString() const;
		#ifdef __OBJC__
		NSData* toNSData() const;
		#endif
	};
}
