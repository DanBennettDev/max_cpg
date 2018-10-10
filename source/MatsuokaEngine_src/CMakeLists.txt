cmake_minimum_required(VERSION 3.0)

if (POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif ()

project(MatsuokaEngine)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} MAT_SOURCES)

#file(GLOB MAT_SOURCES "*.cpp *.c")

add_library(MatsuokaEngine STATIC ${MAT_SOURCES})



if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_definitions(
		-DDEBUG
	)
	set(
		CMAKE_LIBRARY_OUTPUT_DIRECTORY
		../../build/x64/Debug
	)
else ()
	set(
		CMAKE_LIBRARY_OUTPUT_DIRECTORY
		../../build/x64/Release
	)
endif ()
