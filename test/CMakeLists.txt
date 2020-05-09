
# Compile using the (shared) objects of the OBJECT library we created in src/,
add_executable(test-runner test-runner.cpp $<TARGET_OBJECTS:libsb-mapreduce-objects>)
add_test(NAME test-runner COMMAND test-runner)

target_link_libraries(test-runner PRIVATE doctest)
target_compile_features(test-runner PRIVATE cxx_std_17)
