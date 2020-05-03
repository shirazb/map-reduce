add_executable(dev_demo dev_demo.cpp ${SBMapReduce_BINARY_INCLUDE_DIR}/sb-mapreduce/version.h)

target_compile_features(dev_demo PRIVATE cxx_std_17)

# For version.h
target_include_directories(dev_demo PRIVATE ${SBMapReduce_BINARY_INCLUDE_DIR})

target_link_libraries(dev_demo PRIVATE libsb-mapreduce)
