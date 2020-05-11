set(SOURCES
        master.cpp
        worker.cpp
        common.cpp
        emit-streams.cpp
)

set(HEADERS
        ${SBMapReduce_INCLUDE_DIR}/sb-mapreduce/master.h
        ${SBMapReduce_INCLUDE_DIR}/sb-mapreduce/worker.h
        ${SBMapReduce_INCLUDE_DIR}/sb-mapreduce/common.h
        ${SBMapReduce_INCLUDE_DIR}/sb-mapreduce/emit-streams.h
)

# Expose as object library and static library. We build the static library once
# for actual use, and the object library once to be shared by each test exe.
# We do not have tests in the production code as an executable that links against
# it with DOCTEST_CONFIG_DISABLE will get linker errors

#################### OBJ LIB FOR TESTS #########################################

if(SB_MAPREDUCE_SHOULD_TEST)
    add_library(libsb-mapreduce-objects OBJECT ${SOURCES} ${HEADERS})
    target_include_directories(libsb-mapreduce-objects PUBLIC ${SBMapReduce_INCLUDE_DIR})
    target_link_libraries(libsb-mapreduce-objects PUBLIC doctest)
    target_compile_features(libsb-mapreduce-objects PUBLIC cxx_std_17)
endif()

#################### ACTUAL STATIC LIB #########################################

add_library(libsb-mapreduce ${SOURCES} ${HEADERS})

# Must be public because tests exist in headers.
target_link_libraries(libsb-mapreduce PUBLIC doctest)

target_include_directories(libsb-mapreduce PUBLIC ${SBMapReduce_INCLUDE_DIR})

target_compile_features(libsb-mapreduce PRIVATE cxx_std_17)

# No need to define DOCTEST_CONFIG_DISABLE, as doctest doesn't exist in the
# production code.
