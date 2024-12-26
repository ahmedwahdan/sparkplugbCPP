cmake_minimum_required(VERSION 3.10)
project(SparkplugV3Cpp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_TESTS "Build test cases" OFF)
option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_DOCS "Build documentation" OFF)

find_package(Protobuf REQUIRED)
find_package(nlohmann_json REQUIRED)

set(PROTO_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/proto")
set(PROTO_GEN_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
file(MAKE_DIRECTORY ${PROTO_GEN_DIR})

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS 
   ${PROTO_SRC_DIR}/sparkplug_b.proto
)

# Generate protobuf files
add_custom_command(
    OUTPUT 
        ${PROTO_GEN_DIR}/sparkplug_b.pb.cc
        ${PROTO_GEN_DIR}/sparkplug_b.pb.h
    COMMAND ${Protobuf_PROTOC_EXECUTABLE}
        --cpp_out=${PROTO_GEN_DIR}
        -I${PROTO_SRC_DIR}
        ${PROTO_SRC_DIR}/sparkplug_b.proto
    DEPENDS ${PROTO_SRC_DIR}/sparkplug_b.proto
)

add_custom_target(proto_gen DEPENDS ${PROTO_GEN_DIR}/sparkplug_b.pb.cc ${PROTO_GEN_DIR}/sparkplug_b.pb.h)


add_library(sparkplugv3 STATIC
   src/HostApp.cpp
   ${PROTO_SRCS}
   ${PROTO_HDRS}
)

target_include_directories(sparkplugv3
   PUBLIC
        ${PROTO_GEN_DIR}
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
       
        $<INSTALL_INTERFACE:include>
   PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(sparkplugv3
   PUBLIC
       protobuf::libprotobuf
       nlohmann_json::nlohmann_json
)

add_dependencies(sparkplugv3 proto_gen) 

install(TARGETS sparkplugv3
   EXPORT sparkplugv3Targets
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
   RUNTIME DESTINATION bin
   INCLUDES DESTINATION include
)

if(BUILD_EXAMPLES)
   add_subdirectory(examples)
endif()