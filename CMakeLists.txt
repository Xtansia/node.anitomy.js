cmake_minimum_required(VERSION 3.2)
project(anitomy_lib)

if(UNIX)
  if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -mmacosx-version-min=10.7")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++")
  endif(APPLE)

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++11 -Wall -Wextra -Wpedantic -Werror")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
else(UNIX)
  if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /MP")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /ZI")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
  endif(MSVC)
endif(UNIX)

if(NOT NODE_ANITOMY_USE_BOOST)
  try_compile(CAN_USE_CODECVT "${CMAKE_CURRENT_BINARY_DIR}/compiler_tests" "${CMAKE_CURRENT_SOURCE_DIR}/compiler_tests/codecvt_test.cpp")
  if(NOT CAN_USE_CODECVT)
    message(WARNING "<codecvt> not available, automatically enabling boost-lib")
    set(NODE_ANITOMY_USE_BOOST ON)
  endif()
endif()

if(NODE_ANITOMY_USE_BOOST)
  set(NODE_MODULES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/node_modules")
  
  if(NOT IS_DIRECTORY "${NODE_MODULES_DIR}")
    get_filename_component(PARENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" DIRECTORY)
    get_filename_component(PARENT_DIR_NAME "${PARENT_DIR}" NAME)
    if(PARENT_DIR_NAME STREQUAL "node_modules")
      set(NODE_MODULES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")
    endif()
  endif()
  
  if(NOT EXISTS "${NODE_MODULES_DIR}/boost-lib/cmake/BoostLib.cmake")
    message(FATAL_ERROR "Couldn't find boost-lib cmake file")
  endif()

  set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${NODE_MODULES_DIR}/boost-lib/cmake")
  
  include(BoostLib) 
  require_boost_libs(">= v1.58.0" locale) 
  
  add_definitions(-DNODE_ANITOMY_USE_BOOST)
endif()

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

# Add anitomy library target
#add_library(anitomy SHARED ${ANITOMY_SOURCE_FILES})

# Add the addon library target
add_library(${PROJECT_NAME} SHARED ${ADDON_SOURCE_FILES} ${ANITOMY_SOURCE_FILES})

# Set properties for NodeJS addon library file naming
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

# Include JS and Anitomy directories
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC})
target_include_directories(${PROJECT_NAME} PRIVATE lib/anitomy)

# Link against JS and Anitomy libraries
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_JS_LIB})
#target_link_libraries(${PROJECT_NAME} PRIVATE anitomy)

if(NODE_ANITOMY_USE_BOOST) 
  # Include and link against Boost
  target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})
endif()
