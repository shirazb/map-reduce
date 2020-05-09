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

add_library(libsb-mapreduce ${SOURCES} ${HEADERS})

target_include_directories(libsb-mapreduce PUBLIC ${SBMapReduce_INCLUDE_DIR})

target_compile_features(libsb-mapreduce PRIVATE cxx_std_17)

target_link_libraries(libsb-mapreduce PRIVATE doctest)
