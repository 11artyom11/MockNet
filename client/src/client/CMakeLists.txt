cmake_minimum_required(VERSION 2.8.9)
project (client)

add_subdirectory(src/client_src)
add_subdirectory(src/client_interface)

include_directories(include/
                    include/client_interface/)

set (CMAKE_BINARY_DIR "./../../bin/client/")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_CXX_FLAGS "-g -w -O2 -pthread")

add_executable(client_exe src/main.cpp)

target_link_libraries(client_exe PUBLIC client_lib)
target_link_libraries(client_exe PUBLIC client_interface)