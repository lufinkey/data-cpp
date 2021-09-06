//
//  Data.mm
//  DataCpp
//
//  Created by Luis Finke on 9/5/21.
//  Copyright © 2021 Luis Finke. All rights reserved.
//

#import <fgl/data/Data.hpp>

#ifdef __OBJC__
namespace fgl {
	Data::Data(NSData* data): vector((uint8_t*)data.bytes, ((uint8_t*)data.bytes) + data.length) {
		//
	}
	
	NSData* Data::toNSData() const {
		return [NSData dataWithBytes:data() length:size()];
	}
}
#endif
