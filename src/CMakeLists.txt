project(simpleRtsp)

set(TARGET simpleRtsp)

include_directories("${CMAKE_SOURCE_DIR}/rtp")
include_directories("${CMAKE_SOURCE_DIR}/ntp")
include_directories("${CMAKE_SOURCE_DIR}/msocket")
include_directories("${CMAKE_SOURCE_DIR}/public")
include_directories("${CMAKE_SOURCE_DIR}/register")
include_directories("${CMAKE_SOURCE_DIR}/parsertsp")
link_directories("${CMAKE_SOURCE_DIR}/libs")

aux_source_directory(. M_SRCS)

add_executable(${TARGET} ${M_SRCS})

target_link_libraries(${TARGET} parsertsp register ntp rtp msocket public)
target_link_libraries(${TARGET} pthread)