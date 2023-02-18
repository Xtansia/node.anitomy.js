cmake_minimum_required(VERSION 3.2)
project(anitomy_lib)

if(UNIX)
  if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -mmacosx-version-min=10.7")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++")
  endif(APPLE)

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall -Wextra -Wpedantic -Werror")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
else(UNIX)
  if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX /MP")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /ZI")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
  endif(MSVC)
endif(UNIX)

# Addon source files
set(ADDON_SOURCE_FILES
    src/anitomy_lib.cpp
    src/element_categories.h
    src/elements_object.cpp
    src/elements_object.h
    src/parse_worker.cpp
    src/parse_worker.h
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
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

# Include JS and Anitomy directories
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${CMAKE_JS_INC})
target_include_directories(${PROJECT_NAME} PRIVATE lib/anitomy)

# Link against JS libraries
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_JS_LIB})

if(MSVC)
  target_compile_definitions(${PROJECT_NAME} PRIVATE _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING)
endif(MSVC)
