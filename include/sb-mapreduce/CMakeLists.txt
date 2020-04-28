set(SOURCES
        map-reduce.hpp
        map-reduce.tpp
        version.h.in
)

add_library(libsb-mapreduce INTERFACE)
target_sources(libsb-mapreduce INTERFACE ${SOURCES})

target_include_directories(libsb-mapreduce INTERFACE
        ${SBMapReduce_INCLUDE_DIR}
        ${SBMapReduce_BINARY_INCLUDE_DIR} # for version header
)
