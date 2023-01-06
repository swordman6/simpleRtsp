project(simpleRtsp)

set(TARGET simpleRtsp)

include_directories("${CMAKE_SOURCE_DIR}/mtcp")
link_directories("${CMAKE_SOURCE_DIR}/libs")

aux_source_directory(. M_SRCS)

add_executable(${TARGET} ${M_SRCS})

target_link_libraries(${TARGET} mtcp)