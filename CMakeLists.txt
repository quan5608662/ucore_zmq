cmake_minimum_required(VERSION 3.15)
project(ucore_zmq VERSION 1.0.0.0)

set(CMAKE_CXX_STANDARD 17)

include(cmake/build.cmake)
#include(cmake/cmrc.cmake)

add_subdirectory(src/app)

add_subdirectory(src/lib)

add_subdirectory(src/plugin)

add_subdirectory(src/protocol)

#add_subdirectory(setup)
