
#ifdef __ANDROID__
#include <fgl/data/JNIDataCpp.hpp>
#include <android/log.h>

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
	__android_log_print(ANDROID_LOG_DEBUG, "DataCpp", "JNI module loaded");
	return JNI_VERSION_1_6;
}

#endif
