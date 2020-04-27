set(HEADERS
        ${SBMapReduce_INCLUDE_DIR}/sb-mapreduce/version.h.in
        ${SBMapReduce_INCLUDE_DIR}/sb-mapreduce/map-reduce.h
)

add_executable(print_version print_version.cpp ${HEADERS})

target_compile_features(print_version PRIVATE cxx_std_17)

# for version header
target_include_directories(print_version PUBLIC "${SBMapReduce_BINARY_INCLUDE_DIR}")

target_link_libraries(print_version PRIVATE libsb-mapreduce)
