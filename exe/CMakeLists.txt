add_executable(print_version print_version.cpp)

target_compile_features(print_version PRIVATE cxx_std_17)

target_link_libraries(print_version PRIVATE libsb-mapreduce)
