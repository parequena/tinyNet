cmake_minimum_required(VERSION 3.10)
project(tinyNet)

set(TINYNET_PROTOS
"${CMAKE_CURRENT_SOURCE_DIR}/proto/Message.proto"
)

# Add protobuf as depency
find_package(Protobuf CONFIG REQUIRED)
protobuf_generate_cpp(TNT_PROTO_SRCS TNT_PROTO_HDRS ${TINYNET_PROTOS})

set(PROJECT_WARNINGS "${SANITIZER_FLAGS};${GENERAL_COMPILER_WARNINGS};${SPECIFIC_COMPILER_WARNINGS}")

add_library(tinyNet)
target_compile_options(tinyNet PRIVATE ${PROJECT_WARNINGS})
target_compile_features(tinyNet PRIVATE cxx_std_23)
target_include_directories(tinyNet PUBLIC include  ${Protobuf_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(tinyNet PRIVATE ${LINKER_FLAGS} ${SANITIZER_FLAGS} protobuf::libprotoc protobuf::libprotobuf-lite)
target_sources(tinyNet
PUBLIC
${TNT_PROTO_HDRS}
${CMAKE_CURRENT_SOURCE_DIR}/include/Socket.hpp
${CMAKE_CURRENT_SOURCE_DIR}/include/ServerHandler.hpp
${CMAKE_CURRENT_SOURCE_DIR}/include/Server.hpp
${CMAKE_CURRENT_SOURCE_DIR}/include/Client.hpp
PRIVATE
${TNT_PROTO_SRCS}
${CMAKE_CURRENT_SOURCE_DIR}/src/Socket.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/ServerHandler.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/Server.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/Client.cpp
)

#####################################################
# server
add_executable(server_main server_main.cpp)
target_compile_options(server_main PRIVATE ${PROJECT_WARNINGS})
set_property(TARGET server_main PROPERTY COMPILE_WARNING_AS_ERROR ON)
target_compile_features(server_main PRIVATE cxx_std_23)
target_include_directories(server_main PUBLIC include)
target_link_libraries(server_main PRIVATE ${LINKER_FLAGS} ${SANITIZER_FLAGS} tinyNet)

#####################################################
# client
add_executable(client_main client_main.cpp)
target_compile_options(client_main PRIVATE ${PROJECT_WARNINGS})
set_property(TARGET client_main PROPERTY COMPILE_WARNING_AS_ERROR ON)
target_compile_features(client_main PRIVATE cxx_std_23)
target_include_directories(client_main PUBLIC include)
target_link_libraries(client_main PRIVATE ${LINKER_FLAGS} ${SANITIZER_FLAGS} tinyNet)