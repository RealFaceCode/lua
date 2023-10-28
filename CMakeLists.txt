cmake_minimum_required(VERSION 3.19)
project(App VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES 0)
set(CMAKE_C_USE_RESPONSE_FILE_FOR_INCLUDES 0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/arch)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE SOURCE_FILES "project/*.cpp" "project/*.hpp" "project/*.c" "project/*.h")
file(GLOB_RECURSE LUA_SOURCE "dependencies/lua/src/*.c" "dependencies/lua/src/*.cpp")

add_library(lua ${LUA_SOURCE})

include_directories("dependencies/lua/src")

add_library(App ${SOURCE_FILES})
target_link_libraries(App PRIVATE lua)
target_link_directories(App PUBLIC "dependencies/lua/src")