cmake_minimum_required(VERSION 2.8)
project(anitomy_lib)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/node_modules/boost-lib/cmake")
include(BoostLib)

require_boost_libs(">= v1.61.0" locale)

include_directories(${CMAKE_JS_INC} ${Boost_INCLUDE_DIRS} deps/anitomy)
file(GLOB SOURCE_FILES "src/*.cpp" "src/*.h" "deps/anitomy/anitomy/*.cpp" "deps/anitomy/anitomy/*.h")
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} ${Boost_LIBRARIES})