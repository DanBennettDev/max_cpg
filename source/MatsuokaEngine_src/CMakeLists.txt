cmake_minimum_required(VERSION 3.0)

project(MatsuokaEngine)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} MAT_SOURCES)

#file(GLOB MAT_SOURCES "*.cpp *.c")

add_library(MatsuokaEngine STATIC ${MAT_SOURCES})

set(
    CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ../../build/x64/Release
)