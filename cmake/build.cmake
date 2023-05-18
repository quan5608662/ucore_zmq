include_guard(GLOBAL)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/target/bin)
set(LIBRARY_OUTPUT_PATH    ${PROJECT_BINARY_DIR}/target/lib)
set(PLUGIN_OUT_PATH        ${PROJECT_BINARY_DIR}/target/plugin)
set(PROTOCOL_OUT_PATH      ${PROJECT_BINARY_DIR}/target/protocol)
set(SETUP_OUTPUT_PATH      ${PROJECT_BINARY_DIR}/setup)
set(SETUP_RESOURCE_PATH    ${PROJECT_SOURCE_DIR}/setup/resource)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/staticlib)

link_directories(${LIBRARY_OUTPUT_PATH})
link_directories(${PROTOCOL_OUT_PATH})
link_directories(${PROJECT_BINARY_DIR})
link_directories(${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

#include_directories(${PROJECT_SOURCE_DIR}/src/lib/ucore/include)
include_directories(${PROJECT_SOURCE_DIR}/src/lib/uzmq/include)
include_directories(${PROJECT_SOURCE_DIR}/src/protocol)


#动态生成头文件包含目录
include_directories(${SETUP_OUTPUT_PATH})


find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


add_compile_options(-Wno-psabi -g)
