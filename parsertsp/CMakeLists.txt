project(simpleRtsp)

include_directories("${CMAKE_SOURCE_DIR}/parsertsp")

aux_source_directory(. PARSERTSP_SRCS)

add_library(parsertsp ${PARSERTSP_SRCS})