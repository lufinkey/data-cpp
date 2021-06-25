#include <jni.h>
#include <string>
#include <fgl/data.hpp>
#include <test/DataCppTests.hpp>

extern "C" JNIEXPORT jstring JNICALL
Java_com_lufinkey_datacpp_MainActivity_stringFromJNI(
		JNIEnv *env,
		jobject /* this */) {
	auto stringTest = fgl::String("Hello from C++");
	fgl_data_cpp_tests::runTests();
	return env->NewStringUTF(stringTest.c_str());
}
