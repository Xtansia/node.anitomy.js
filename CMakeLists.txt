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
set(ADDON_SOURCE_FILES
    src/anitomy_async.cpp
    src/anitomy_async.h
    src/anitomy_element_categories.h
    src/anitomy_elements.cpp
    src/anitomy_elements.h
    src/anitomy_lib.cpp
    src/anitomy_sync.cpp
    src/anitomy_sync.h
    src/utils.cpp
    src/utils.h
    )

# Anitomy source files
set(ANITOMY_SOURCE_FILES
    lib/anitomy/anitomy/anitomy.cpp
    lib/anitomy/anitomy/anitomy.h
    lib/anitomy/anitomy/element.cpp
    lib/anitomy/anitomy/element.h
    lib/anitomy/anitomy/keyword.cpp
    lib/anitomy/anitomy/keyword.h
    lib/anitomy/anitomy/options.h
    lib/anitomy/anitomy/parser.cpp
    lib/anitomy/anitomy/parser.h
    lib/anitomy/anitomy/parser_helper.cpp
    lib/anitomy/anitomy/parser_number.cpp
    lib/anitomy/anitomy/string.cpp
    lib/anitomy/anitomy/string.h
    lib/anitomy/anitomy/token.cpp
    lib/anitomy/anitomy/token.h
    lib/anitomy/anitomy/tokenizer.cpp
    lib/anitomy/anitomy/tokenizer.h
    )

# Source groups for VS
source_group("addon" FILES ${ADDON_SOURCE_FILES})
source_group("anitomy" FILES ${ANITOMY_SOURCE_FILES})

# Add the addon library target
add_library(${PROJECT_NAME} SHARED ${ADDON_SOURCE_FILES} ${ANITOMY_SOURCE_FILES})

# Set properties for NodeJS addon library file naming
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

# Link against JS and Boost libraries
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} ${Boost_LIBRARIES})
