add_executable(dev-demo dev-demo.cpp ${SBMapReduce_BINARY_INCLUDE_DIR}/sb-mapreduce/version.h)

target_compile_features(dev-demo PRIVATE cxx_std_17)

# For version.h
target_include_directories(dev-demo PRIVATE ${SBMapReduce_BINARY_INCLUDE_DIR})

target_link_libraries(dev-demo PRIVATE libsb-mapreduce)
