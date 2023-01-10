project(simpleRtsp)

include_directories("${CMAKE_SOURCE_DIR}/msocket")
include_directories("${CMAKE_SOURCE_DIR}/public")
include_directories("${CMAKE_SOURCE_DIR}/register")
include_directories("${CMAKE_SOURCE_DIR}/parsertsp")

aux_source_directory(. PARSERTSP_SRCS)

add_library(parsertsp ${PARSERTSP_SRCS})