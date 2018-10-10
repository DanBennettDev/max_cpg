cmake_minimum_required(VERSION 3.0)

set(C74_MIN_API_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../min-api)
include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)

if (POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif ()

set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

#############################################################
# MAX EXTERNAL
#############################################################


include_directories( 
    "${C74_INCLUDES}"
    # project specific
    "../../MatsuokaEngine_src"
)


set( SOURCE_FILES
    ${PROJECT_NAME}.cpp
)


add_library( 
    ${PROJECT_NAME} 
    MODULE
    ${SOURCE_FILES}
)


target_link_libraries(${PROJECT_NAME} MatsuokaEngine)


include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)


#############################################################
# UNIT TEST
#############################################################

include(${C74_MIN_API_DIR}/test/min-object-unittest.cmake)

target_link_libraries(${PROJECT_NAME} MatsuokaEngine)