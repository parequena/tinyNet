cmake_minimum_required(VERSION 3.10)
project(tinyNet)

set(PROJECT_WARNINGS "${SANITIZER_FLAGS};${GENERAL_COMPILER_WARNINGS};${SPECIFIC_COMPILER_WARNINGS}")

add_library(tinyNet)
target_compile_options(tinyNet PRIVATE ${PROJECT_WARNINGS})
target_compile_features(tinyNet PRIVATE cxx_std_23)
target_include_directories(tinyNet PUBLIC include)
target_link_libraries(tinyNet PRIVATE ${LINKER_FLAGS} ${SANITIZER_FLAGS})
target_sources(tinyNet
PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}/include/Socket.hpp
${CMAKE_CURRENT_SOURCE_DIR}/include/Server.hpp
${CMAKE_CURRENT_SOURCE_DIR}/include/Client.hpp
PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/src/Socket.cpp
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