project(anitomy_lib)

file(GLOB ANITOMY_SOURCE_FILES "deps/anitomy/anitomy/*.cpp" "deps/anitomy/anitomy/*.h")
add_library(anitomy STATIC ${ANITOMY_SOURCE_FILES})

include_directories(${CMAKE_JS_INC} deps/anitomy)
file(GLOB SOURCE_FILES "src/*.cpp" "src/*.h")
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB} anitomy)