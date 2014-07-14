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
list(APPEND CMAKE_CXX_FLAGS "-std=c++11")
add_library(wap32 SHARED ${SOURCES})
set_target_properties(wap32 PROPERTIES PREFIX "lib")
find_package(ZLIB REQUIRED)
if(ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIRS})
    target_link_libraries(wap32 ${ZLIB_LIBRARIES})
endif(ZLIB_FOUND)