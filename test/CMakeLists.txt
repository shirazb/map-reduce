
# Compile using the (shared) objects of the OBJECT library we created in src/,
add_executable(test-runner test-runner.cpp $<TARGET_OBJECTS:libsb-mapreduce-objects>)
add_test(NAME test-runner COMMAND test-runner)

target_link_libraries(test-runner PRIVATE doctest)
target_compile_features(test-runner PRIVATE cxx_std_17)

# Have doctest disable itself if running in release mode; or have it generate
# the default main test runner instead.
if(SB_MAPREDUCE_SHOULD_TEST)
    target_compile_definitions(test-runner PUBLIC
            $<IF:$<AND:$<CONFIG:Release>,$<BOOL:${SB_MAPREDUCE_SHOULD_TEST}>>, DOCTEST_CONFIG_DISABLE, DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN>
            )   #     if        config is release  and  not SB_MAPREDUCE_SHOULD_TEST     then  ^                else  ^
endif()
