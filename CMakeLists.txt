cmake_minimum_required(VERSION 3.6)
project(algs)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-g -Wall")

include_directories(AFTER ${CMAKE_SOURCE_DIR}/algstl)
include_directories(AFTER ${CMAKE_SOURCE_DIR}/lib)
include_directories(AFTER ${CMAKE_SOURCE_DIR}/algs)

add_subdirectory(lib)
add_subdirectory(test)
add_subdirectory(practice)
add_subdirectory(algs)
add_subdirectory(util)
