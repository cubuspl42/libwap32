cmake_minimum_required(VERSION 2.6)
project(libwap32)
add_subdirectory(tests)
include_directories("include")
file(GLOB SOURCES
    "include/wap32.h"
    "include/wap32/*.h"
    "src/*.h"
    "src/*.cpp"
)
add_definitions(-DWAP32_BUILD_SHARED)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
list(APPEND CMAKE_CXX_FLAGS "-std=c++11")
add_library(libwap32 SHARED ${SOURCES})