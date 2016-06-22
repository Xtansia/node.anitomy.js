cmake_minimum_required(VERSION 3.2)
project(anitomy_lib)

# Include JS and Anitomy directories
include_directories(${CMAKE_JS_INC} lib/anitomy)

# Addon source files
file(GLOB ADDON_SOURCE_FILES "src/*.cpp" "src/*.h")

# Anitomy source files
file(GLOB ANITOMY_SOURCE_FILES "lib/anitomy/anitomy/*.cpp" "lib/anitomy/anitomy/*.h")

# Add the addon library target
add_library(${PROJECT_NAME} SHARED ${ADDON_SOURCE_FILES} ${ANITOMY_SOURCE_FILES})

# Set properties for NodeJS addon library file naming
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

# Link against JS libraries
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})