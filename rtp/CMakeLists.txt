project(simpleRtsp)

include_directories("${CMAKE_SOURCE_DIR}/msocket")
include_directories("${CMAKE_SOURCE_DIR}/register")
include_directories("${CMAKE_SOURCE_DIR}/public")
include_directories("${CMAKE_SOURCE_DIR}/rtp")


aux_source_directory(. RTP_SRCS)

add_library(rtp ${RTP_SRCS})