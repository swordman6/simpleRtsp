cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_C_FLAGS "-g ${CMAKE_C_FLAGS}")
add_compile_options(-Wall)

project(simpleRtsp)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/libs")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

add_subdirectory(mtcp)
add_subdirectory(src)
