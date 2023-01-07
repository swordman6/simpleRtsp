project(simpleRtsp)

include_directories("${CMAKE_SOURCE_DIR}/public")

aux_source_directory(. PUBLIC_SRCS)

add_library(public ${PUBLIC_SRCS})