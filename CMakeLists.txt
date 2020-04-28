cmake_minimum_required(VERSION 3.16...3.17)

project(SBMapReduce 
        VERSION 0.1
        DESCRIPTION "Implementation of MapReduce in C++"
        LANGUAGES CXX
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

configure_file(
        "${PROJECT_SOURCE_DIR}/include/sb-mapreduce/version.h.in"
        "${PROJECT_BINARY_DIR}/include/sb-mapreduce/version.h"
)

set(SBMapReduce_INCLUDE_DIR "${SBMapReduce_SOURCE_DIR}/include")
set(SBMapReduce_BINARY_INCLUDE_DIR "${PROJECT_BINARY_DIR}/include")

#add_subdirectory(src)
add_subdirectory(include)

add_subdirectory(exe)
