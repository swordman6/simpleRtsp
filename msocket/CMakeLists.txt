project(simpleRtsp)

include_directories("${CMAKE_SOURCE_DIR}/msocket")

aux_source_directory(. MSOCKET_SRCS)

add_library(msocket ${MSOCKET_SRCS})