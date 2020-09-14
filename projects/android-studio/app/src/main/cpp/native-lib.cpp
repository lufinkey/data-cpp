#include <jni.h>
#include <string>
#include <fgl/data.hpp>

using namespace fgl;

extern "C" JNIEXPORT jstring JNICALL
Java_com_lufinkey_datacpp_MainActivity_stringFromJNI(
		JNIEnv *env,
		jobject /* this */) {
	auto any = Any(String("hello world"));
	return env->NewStringUTF(any.as<String>().c_str());
}
