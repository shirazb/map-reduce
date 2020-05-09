include(FetchContent)
find_package(Git REQUIRED)

message(STATUS "Configuring doctest")

FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/onqtam/doctest
        GIT_TAG 2.3.7
)

FetchContent_MakeAvailable(doctest)

# Shouldn't need to do this: just link against doctest target and it will expose
# its own include dirs etc.
# set(DOCTEST_INCLUDE_DIR ${doctest_SOURCE_DIR}/doctest CACHE INTERNAL "Path to include directory for doctest")
