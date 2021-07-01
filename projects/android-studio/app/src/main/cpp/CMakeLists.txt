# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

project(TestApp)
cmake_minimum_required(VERSION 3.4.1)
set( CMAKE_CXX_STANDARD 17 )

set( DATACPP_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../.." )

add_subdirectory("${DATACPP_ROOT}/projects/android-studio/libdatacpp/src/main/cpp" DataCpp_build)

add_library( TestApp SHARED native-lib.cpp "${DATACPP_ROOT}/src/test/DataCppTests.cpp" )

target_include_directories(
		TestApp
		PRIVATE
		"${DATACPP_ROOT}/src")

find_library(
		log-lib
		log)

target_link_libraries( # Specifies the target library.
		TestApp
		DataCpp
		${log-lib})
