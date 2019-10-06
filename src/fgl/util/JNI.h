//
// Created by Luis Finke on 2019-10-06.
//

#pragma once

#ifndef _DATACPP_JNI_H
#define _DATACPP_JNI_H

#ifndef JNI_PTR
	#ifdef JNIEXPORT
		#define JNI_PTR(type) type;
	#else
		#define JNI_PTR(type) void*;
	#endif
#endif

#endif
