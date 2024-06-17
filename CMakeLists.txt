cmake_minimum_required(VERSION 3.20)

project("resmanager" LANGUAGES CXX)

option(RESMANAGER_BUILD_EXAMPLE "Enable building example or not" OFF)

add_library(resmanager INTERFACE
    "src/resmanager/detail/cache.hpp"
    "src/resmanager/detail/hashing.hpp"
    "src/resmanager/detail/loader.hpp"
    "src/resmanager/detail/version.hpp"
    "src/resmanager/resmanager.hpp"
)

target_include_directories(resmanager INTERFACE "src")

if(RESMANAGER_BUILD_EXAMPLE)
    add_subdirectory(example)
endif()
