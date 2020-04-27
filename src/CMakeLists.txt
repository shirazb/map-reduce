set(SOURCES
        map-reduce.cpp
)

set(HEADERS
        ${SBMapReduce_INCLUDE_DIR}/sb-mapreduce/map-reduce.h
)

add_library(libsb-mapreduce ${SOURCES} ${HEADERS})
target_compile_features(libsb-mapreduce PRIVATE cxx_std_17)

target_include_directories(libsb-mapreduce PUBLIC "${SBMapReduce_INCLUDE_DIR}")
