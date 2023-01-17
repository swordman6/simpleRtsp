project(simpleRtsp)

include_directories("${CMAKE_SOURCE_DIR}/ntp")

aux_source_directory(. NTP_SRCS)

add_library(ntp ${NTP_SRCS})