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

# Expose as object library. From these objects, we can make both the regular
# library, and the test executables, without compiling the sources from scratch
# each time.

add_library(libsb-mapreduce-objects OBJECT ${SOURCES} ${HEADERS})
target_include_directories(libsb-mapreduce-objects PUBLIC ${SBMapReduce_INCLUDE_DIR})
target_link_libraries(libsb-mapreduce-objects PUBLIC doctest)
target_compile_features(libsb-mapreduce-objects PUBLIC cxx_std_17)

# The actual libsb-mapreduce library.

add_library(libsb-mapreduce $<TARGET_OBJECTS:libsb-mapreduce-objects>)

# Needs doctest to compile itself as contains tests in production code,
# but if an external project links against our lib they don't need to use doctest,
# or if they do, will link against it themselves.
target_link_libraries(libsb-mapreduce PRIVATE doctest)
target_compile_features(libsb-mapreduce PRIVATE cxx_std_17)
# Yes we already get the above two commands transitively from the object
# library, as it used PUBLIC, but the sb-mapreduce library only needs them
# privately, so restate the commands here as PRIVATE.
