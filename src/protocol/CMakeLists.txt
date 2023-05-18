find_package(Protobuf 3.16 REQUIRED)
if(Protobuf_FOUND)
    message("Protobuf package found ->${Protobuf_VERSION}:${Protobuf_LIBRARIES},${Protobuf_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "Protobuf package not found -> specify search path via PROTOBUF_ROOT variable")
endif()


function(proto_def _name _ver)
    file(GLOB _proto ${CMAKE_CURRENT_SOURCE_DIR}/proto/*.proto)
    message(${_proto})

    protobuf_generate_cpp(_proto_h _proto_src ${_proto})


    file(GLOB _src
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

add_library(${_name} SHARED ${_src} ${_proto_h} ${_proto_src} ${_proto})

target_link_libraries(${_name} PUBLIC ${Protobuf_LIBRARIES})
target_include_directories(${_name} PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${Protobuf_INCLUDE_DIRS})

set_target_properties(${_name} PROPERTIES
    VERSION ${_ver}
    LIBRARY_OUTPUT_DIRECTORY ${PROTOCOL_OUT_PATH})
endfunction()

add_subdirectory(proto_uzmq)
