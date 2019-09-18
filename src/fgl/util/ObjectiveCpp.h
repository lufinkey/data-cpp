//
//  ObjectiveCpp.h
//  DataCpp
//
//  Created by Luis Finke on 9/18/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#ifndef _DATACPP_OBJECTIVECPP_H
#define _DATACPP_OBJECTIVECPP_H

#ifndef OBJCPP_PTR
	#ifdef __OBJC__
		#define OBJCPP_PTR(type) type*
	#else
		#define OBJCPP_PTR(type) void*
	#endif
#endif

#endif
