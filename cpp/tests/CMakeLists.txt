include(CTest)

find_package(GTest REQUIRED)
include(GoogleTest)

# Build a list of test binaries
add_custom_target(all_test_binaries COMMENT "all unit tests.")

add_executable(first_test first_test.cpp)
add_dependencies(all_test_binaries first_test)
target_link_libraries(first_test PRIVATE GTest::gtest GTest::gtest_main)
target_include_directories(first_test PRIVATE toto ${GTest_INCLUDE_DIRS})
gtest_discover_tests(first_test) # helps to split one ctest executable into multiple google tests
#add_test(NAME first_test COMMAND first_test)
#set_property(TEST first_test PROPERTY LABELS "GTest")

# Can be enabled if necessary
#if (CXX_CLANG_TIDY)
#    foreach (bin ${all_test_binaries})
#        set_target_properties(${bin}
#                PROPERTIES
#                CXX_CLANG_TIDY ${CXX_CLANG_TIDY})
#    endforeach ()
#endif ()

#set(PARSE_CATCH_TESTS_VERBOSE ON)

## Test using catch2 (parse catch tests)
#ParseAndAddCatchTests(catch2_unit_test)
#ParseAndAddCatchTests(regression_unit_test)
#
#set(AdditionalCatchParameters --benchmark-samples 10)
#ParseAndAddCatchTests(Test)
#unset(AdditionalCatchParameters)

# example of callgrind command line
# valgrind --tool=callgrind --dump-instr=yes --trace-jump=yes -v ./tests/Test "logLikelihood benchmark" --benchmark-samples 10
# catch2 commande line options: https://github.com/catchorg/Catch2/blob/master/docs/command-line.md#specifying-which-tests-to-run

# list of all tests
# get_directory_property(all_tests TESTS)
