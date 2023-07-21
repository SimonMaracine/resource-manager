cmake_minimum_required(VERSION 3.20)

add_executable(example "main.cpp")

target_link_libraries(example PRIVATE resmanager)

if(UNIX)
    target_compile_options(example PRIVATE "-Wall" "-Wextra" "-Wpedantic")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(example PRIVATE "/W3")
else()
    message(WARNING "Compiler is not MSVC or platform is not supported; warnings were not enabled")
endif()

target_compile_features(example PRIVATE cxx_std_17)
set_target_properties(example PROPERTIES CXX_EXTENSIONS OFF)
