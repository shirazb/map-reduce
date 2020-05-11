####################### HELPERS ################################################

# Initialise list that sb_add_test() will populate. Then we will use it in
# sb_build_all_tests_runner().
set(SB_ALL_TEST_SOURCES "")

function(sb_test_common_configure_steps TARGET DOCTEST_CONFIG)
    # We have built from individual objects, not linking against a library, so
    # we do not get its interface properties transitively.
    target_include_directories(${TARGET} PRIVATE ${SBMapReduce_INCLUDE_DIR})
    target_link_libraries(${TARGET} PRIVATE doctest)
    target_compile_features(${TARGET} PRIVATE cxx_std_17)

    target_compile_definitions(${TARGET} PRIVATE ${DOCTEST_CONFIG})
endfunction()

function(sb_build_all_tests_runner)
    message(STATUS "BUILD ALL: ${SB_ALL_TEST_SOURCES}")
    add_executable(all-tests-runner all-tests-runner.cpp ${SB_ALL_TEST_SOURCES}
            $<TARGET_OBJECTS:libsb-mapreduce-objects>
    )

    sb_test_common_configure_steps(all-tests-runner
            DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
    )
endfunction()

# Parameters:
# TO_TEST: string
# SOURCE: string -> If TO_TEST, defaults to "test-<TEST>.cpp". Is source file
#                   for add_executable().
# TARGET: string -> If TO_TEST, defaults to "test-<TEST>". Is target name
#                   created by add_exectuable().
# TEST_NAME: string    -> If TO_TEST, defaults to "test-<TEST>". Is NAME for
#                         add_test().
# TEST_COMMAND: string -> If TO_TEST, defaults to "test-<TEST>". Is COMMAND for
#                         add_test().
# OTHER_SOURCE_FILES: [string], optional -> other source files to build test
#                                           executable with.
# IMPLEMENT_WITHOUT_MAIN: bool, optional -> Defines DOCTEST_IMPLEMENT instead
#                                           of DOCTEST_IMPLEMENT_WITH_MAIN.
#                                           Defaults to false.
# Builds exectuable target then adds it as a test. If TO_TEST given, the other
# parameters (except OTHER_SOURCE_FILES) are defaulted; or if TO_TEST not given,
# ALL other parameters must be supplied.
function(sb_add_test)
    ###### ARGS ######

    set(options IMPLEMENT_WITHOUT_MAIN)
    set(one_value_args TO_TEST SOURCE TARGET TEST_NAME TEST_COMMAND)
    set(multi_value_args OTHER_SOURCE_FILES)
    cmake_parse_arguments(SB
            "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN}
    )

    # Default args
    if(SB_TO_TEST)
        string(CONCAT SB_SOURCE "test-" ${SB_TO_TEST} ".cpp")
        string(CONCAT SB_TARGET "test-" ${SB_TO_TEST})
        string(CONCAT SB_TEST_NAME ${SB_TARGET})
        string(CONCAT SB_TEST_COMMAND ${SB_TARGET})
    endif()
    # Else, variables are assumed given.

    if(SB_IMPLEMENT_WITHOUT_MAIN)
        set(SB_DOCTEST_CONFIG DOCTEST_CONFIG_IMPLEMENT)
    else()
        set(SB_DOCTEST_CONFIG DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN)
    endif()

    ###### BEGIN ######

    add_executable(${SB_TARGET} ${SB_SOURCE} ${SB_OTHER_SOURCE_FILES}
            $<TARGET_OBJECTS:libsb-mapreduce-objects>
    )
    add_test(NAME ${SB_TEST_NAME} COMMAND ${SB_TEST_COMMAND})

    sb_test_common_configure_steps(${SB_TARGET} ${SB_DOCTEST_CONFIG})

    # Add to be built later
    # TODO: Support adding SB_OTHER_TEST_FILES list
    list(APPEND SB_ALL_TEST_SOURCES ${SB_SOURCE})

    # why cmake pls :'(
    set(SB_ALL_TEST_SOURCES ${SB_ALL_TEST_SOURCES} PARENT_SCOPE)
endfunction()

####################### BUILD TESTS ############################################

# FIXME: What if individual test was built with DOCTEST_CONFIG_IMPLEMENT
#        (without main)? Object libraries only compiled once, so does the first
#        test's DOCTEST_CONFIG set it for all future tests?

set(TESTS
        emit-intermediate-stream
        common
)

foreach(TEST ${TESTS})
    sb_add_test(TO_TEST ${TEST})
endforeach()

# FIXME: Broken; linker error duplicate symbols with individual files
# Maybe include all these files from all-tests-runner, rather than building them
# as separate sources (that then possibly all get their own main).
# sb_build_all_tests_runner()
