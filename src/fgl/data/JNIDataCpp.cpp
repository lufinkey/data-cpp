//
//  JNIDataCpp.cpp
//  DataCpp
//
//  Created by Luis Finke on 9/14/20.
//  Copyright © 2020 Luis Finke. All rights reserved.
//

#ifdef __ANDROID__
#include <fgl/data/JNIDataCpp.hpp>
#include <android/log.h>

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
	__android_log_print(ANDROID_LOG_DEBUG, "DataCpp", "JNI module initialized");
	return JNI_VERSION_1_6;
}

#endif
