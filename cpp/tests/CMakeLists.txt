include(CTest)

# target_link_libraries links the executable to the library.
# Since the library has public include directories we will use those link directories when building

# Build a list of test binaries
add_custom_target(all_test_binaries COMMENT "all unit tests.")

#add_executable(test_name test_name.cpp)
#target_link_libraries(class_unit_test LINK_PUBLIC MyLib)
#add_dependencies(all_test_binaries class_unit_test)

# Can be enabled if necessary
#if (CXX_CLANG_TIDY)
#    foreach (bin ${all_test_binaries})
#        set_target_properties(${bin}
#                PROPERTIES
#                CXX_CLANG_TIDY ${CXX_CLANG_TIDY})
#    endforeach ()
#endif ()

## Manual test
#add_test(NAME test_name COMMAND test_name)

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
