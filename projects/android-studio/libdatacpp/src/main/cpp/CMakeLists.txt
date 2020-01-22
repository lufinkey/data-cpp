# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.4.1)
set( CMAKE_CXX_STANDARD 17 )

set( DATACPP_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../.." )

include_directories(
		"${CMAKE_CURRENT_SOURCE_DIR}"
		"${DATACPP_ROOT}/src")

file(GLOB_RECURSE DATACPP_SOURCES ../../../../../../src/fgl/*.cpp)

add_library(DataCpp SHARED
		DataCpp_android.cpp
		${DATACPP_SOURCES})

find_library(
		log-lib
		log)

target_link_libraries(
		DataCpp

		${log-lib})
