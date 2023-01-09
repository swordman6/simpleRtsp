project(simpleRtsp)

include_directories("${CMAKE_SOURCE_DIR}/public")
include_directories("${CMAKE_SOURCE_DIR}/register")

aux_source_directory(. REGISTER_SRCS)

add_library(register ${REGISTER_SRCS})