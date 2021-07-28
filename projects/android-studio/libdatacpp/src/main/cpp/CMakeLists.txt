
project(DataCpp)
cmake_minimum_required(VERSION 3.4.1)
set( CMAKE_CXX_STANDARD 20 )

set( DATACPP_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../../../.." )

file(GLOB_RECURSE DATACPP_SOURCES "${DATACPP_ROOT}/src/fgl/*.cpp")

add_library(DataCpp SHARED
		_unused.cpp
		${DATACPP_SOURCES})

target_include_directories(
		DataCpp
		PUBLIC
		"${DATACPP_ROOT}/src")

find_library(log-lib log)

target_link_libraries(
		DataCpp
		${log-lib})
