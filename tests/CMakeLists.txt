cmake_minimum_required(VERSION 2.6)

include_directories(../include)

add_executable(demo demo.cpp)
target_link_libraries(demo wap32)

add_executable(test_wwd wwd.cpp)
target_link_libraries(test_wwd wap32)
