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

#ifdef __OBJC__
#import <Foundation/Foundation.h>
#endif

#ifndef OBJCPP_PTR
	#ifdef __OBJC__
		#define OBJCPP_PTR(type) type*
	#else
		#define OBJCPP_PTR(type) void*
	#endif
#endif

#endif
