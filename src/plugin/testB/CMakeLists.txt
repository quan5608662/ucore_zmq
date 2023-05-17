cmake_minimum_required(VERSION 3.15)
project(testB)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)


add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

target_link_libraries(${PROJECT_NAME}  px_zmq)

set_target_properties(${PROJECT_NAME} PROPERTIES
    PREFIX ""
    SUFFIX .px
    LIBRARY_OUTPUT_DIRECTORY ${PLUGIN_OUT_PATH}
)
