project(simpleRtsp)

include_directories("${CMAKE_SOURCE_DIR}/rtp")

aux_source_directory(. REGISTER_SRCS)

add_library(register ${REGISTER_SRCS})