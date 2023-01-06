project(simpleRtsp)

include_directories("${CMAKE_SOURCE_DIR}/mtcp")

aux_source_directory(. MTCP_SRCS)

add_library(mtcp ${MTCP_SRCS})