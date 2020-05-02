set(SOURCES
        master.cpp
)

set(HEADERS
        ${SBMapReduce_INCLUDE_DIR}/sb-mapreduce/master.h
)

add_library(libsb-mapreduce ${SOURCES} ${HEADERS})

target_include_directories(libsb-mapreduce PUBLIC ${SBMapReduce_INCLUDE_DIR})

target_compile_features(libsb-mapreduce PRIVATE cxx_std_17)
