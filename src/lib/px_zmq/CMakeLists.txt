cmake_minimum_required(VERSION 3.5)
project(px_zmq LANGUAGES CXX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB LIB_SRC
    ${PROJECT_SOURCE_DIR}/include/*.h
    ${PROJECT_SOURCE_DIR}/src/*.h
    ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_library(px_zmq SHARED ${LIB_SRC})

find_package(Boost 1.78 REQUIRED COMPONENTS filesystem log log_setup json program_options)
find_package(cppzmq REQUIRED)
target_link_libraries(px_zmq ${Boost_LIBRARIES} cppzmq dl)
set_target_properties(px_zmq PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})






