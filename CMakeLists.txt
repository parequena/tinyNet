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
${CMAKE_CURRENT_SOURCE_DIR}/include/Server.hpp

PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/src/Server.cpp
)

#####################################################
add_executable(main main.cpp)
target_compile_options(main PRIVATE ${PROJECT_WARNINGS})
set_property(TARGET main PROPERTY COMPILE_WARNING_AS_ERROR ON)
target_compile_features(main PRIVATE cxx_std_23)
target_include_directories(main PUBLIC include)
target_link_libraries(main PRIVATE ${LINKER_FLAGS} ${SANITIZER_FLAGS} tinyNet)