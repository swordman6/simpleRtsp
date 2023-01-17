cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER /opt/hisi-linux/x86-arm/arm-himix100-linux/bin/arm-himix100-linux-gcc)
set(CMAKE_C_FLAGS "-g ${CMAKE_C_FLAGS}")
add_compile_options(-Wall)

project(simpleRtsp)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/libs")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

add_subdirectory(msocket)
add_subdirectory(register)
add_subdirectory(public)
add_subdirectory(parsertsp)
add_subdirectory(rtp)
add_subdirectory(ntp)
add_subdirectory(src)
