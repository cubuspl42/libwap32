cmake_minimum_required(VERSION 3.0)
project(libwap32)

include_directories("include")
file(GLOB SOURCES
    "include/wap32.h"
    "include/wap32/*.h"
    "src/*.h"
    "src/*.cpp"
)

add_definitions(-DWAP_BUILD_SHARED)
list(APPEND CMAKE_CXX_FLAGS "-std=c++11")

add_library(wap32 SHARED ${SOURCES})
set_target_properties(wap32 PROPERTIES PREFIX "lib")

find_package(ZLIB REQUIRED)
if(ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIRS})
    target_link_libraries(wap32 ${ZLIB_LIBRARIES})
endif(ZLIB_FOUND)

add_subdirectory(tests)
enable_testing()
add_test(
    NAME wwd
    COMMAND test_wwd RETAIL01_nocompress.WWD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests/wwd_files
)

set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include
                          ${ZLIB_INCLUDE_DIRS}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
 