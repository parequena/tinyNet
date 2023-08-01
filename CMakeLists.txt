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
add_executable(main_server main_server.cpp)
target_compile_options(main_server PRIVATE ${PROJECT_WARNINGS})
set_property(TARGET main_server PROPERTY COMPILE_WARNING_AS_ERROR ON)
target_compile_features(main_server PRIVATE cxx_std_23)
target_include_directories(main_server PUBLIC include)
target_link_libraries(main_server PRIVATE ${LINKER_FLAGS} ${SANITIZER_FLAGS} tinyNet)

#####################################################
# client
add_executable(main_client main_client.cpp)
target_compile_options(main_client PRIVATE ${PROJECT_WARNINGS})
set_property(TARGET main_client PROPERTY COMPILE_WARNING_AS_ERROR ON)
target_compile_features(main_client PRIVATE cxx_std_23)
target_include_directories(main_client PUBLIC include)
target_link_libraries(main_client PRIVATE ${LINKER_FLAGS} ${SANITIZER_FLAGS} tinyNet)