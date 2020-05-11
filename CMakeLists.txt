cmake_minimum_required(VERSION 3.16...3.17)

project(SBMapReduce 
        VERSION 0.1
        DESCRIPTION "Implementation of MapReduce in C++"
        LANGUAGES CXX
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

# Configure version.h and move into build dir.
configure_file(
        "${PROJECT_SOURCE_DIR}/include/sb-mapreduce/version.h.in"
        "${PROJECT_BINARY_DIR}/include/sb-mapreduce/version.h"
)
message(STATUS "Configured version.h.in -> version.h in build location")

# Move data to build dir.
file(COPY data DESTINATION ${PROJECT_BINARY_DIR})
message(STATUS "Copied data directory to build location.")

set(SBMapReduce_INCLUDE_DIR "${SBMapReduce_SOURCE_DIR}/include")
set(SBMapReduce_BINARY_INCLUDE_DIR "${PROJECT_BINARY_DIR}/include")

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    option(SB_MAPREDUCE_IS_ROOT_PROJECT
            "Internal global for querying if we are the root CMake project"
            ON
    )
elseif()
    option(SB_MAPREDUCE_IS_ROOT_PROJECT
            "Internal global for querying if we are the root CMake project"
            ON
    )
endif()
# Hide from user-settable variables, only internal.
mark_as_advanced(SB_MAPREDUCE_IS_ROOT_PROJECT)

# Testing only available if this is the main app.
# Override SB_MAPREDUCE_BUILD_TESTING provided as well.
if(SB_MAPREDUCE_IS_ROOT_PROJECT OR SB_MAPREDUCE_BUILD_TESTING)
    include(CTest)     # Calls enable_testing() which sets BUILD_TESTING
    set(SB_MAPREDUCE_SHOULD_TEST ON)
else()
    set(SB_MAPREDUCE_SHOULD_TEST OFF)
endif()

# Must always build doctest - tests are in the production code!
message(STATUS "Configuring doctest...")
add_subdirectory(extern/doctest)
message(STATUS "...done doctest")

message(STATUS "Configuring libsb-mapreduce")
add_subdirectory(src)
message(STATUS "...done libsb-mapreduce")

# Demo only available if this is the main app.
# Override SB_MAPREDUCE_BUILD_DEMO provided as well.
if(SB_MAPREDUCE_IS_ROOT_PROJECT OR SB_MAPREDUCE_BUILD_DEMO)
    message(STATUS "Configuring demo")
    add_subdirectory(exe)
    message(STATUS "...done demo")
endif()

if(SB_MAPREDUCE_SHOULD_TEST) # why AND BUILD_TESTING
    message(STATUS "Configuring test...")
    add_subdirectory(test)
    message(STATUS "...done test")
endif()

message("")
