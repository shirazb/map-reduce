add_executable(print_version print_version.cpp)
target_compile_features(print_version PRIVATE cxx_std_17)
target_include_directories(print_version PUBLIC "${PROJECT_BINARY_DIR}/include")
