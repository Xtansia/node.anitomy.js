cmake_minimum_required(VERSION 3.2)
project(anitomy_lib)

# Boost lib dependency
set(NODE_MODULES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/node_modules")
if(NOT EXISTS "${NODE_MODULES_DIR}" OR NOT IS_DIRECTORY "${NODE_MODULES_DIR}")
  set(NODE_MODULES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../node_modules")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${NODE_MODULES_DIR}/boost-lib/cmake")
include(BoostLib)
require_boost_libs(">= v1.58.0" locale)

# Include JS, Boost and Anitomy directories
include_directories(${CMAKE_JS_INC} ${Boost_INCLUDE_DIRS} lib/anitomy)

# Addon source files
file(GLOB ADDON_SOURCE_FILES "src/*.cpp" "src/*.h")

# Anitomy source files
file(GLOB ANITOMY_SOURCE_FILES "lib/anitomy/anitomy/*.cpp" "lib/anitomy/anitomy/*.h")

# Source groups for VS
source_group("addon" FILES ${ADDON_SOURCE_FILES})
source_group("anitomy" FILES ${ANITOMY_SOURCE_FILES})

# Add the addon library target
add_library(${PROJECT_NAME} SHARED ${ADDON_SOURCE_FILES} ${ANITOMY_SOURCE_FILES})

# Set properties for NodeJS addon library file naming
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

# Link against JS and Boost libraries
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} ${Boost_LIBRARIES})
