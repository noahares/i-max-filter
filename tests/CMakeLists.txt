if (CMAKE_CXX_COMPILER_ID MATCHES GNU|Clang)
    message("Using GCC/Clang library binary file.")
    set(path_to_binary_lib "${CMAKE_SOURCE_DIR}/binaries/libbinaries.a")
elseif(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
    if (CMAKE_BUILD_TYPE MATCHES Debug)
    message("Using MSVC library binary file. (binaries-deb.lib)")
        set(path_to_binary_lib "${CMAKE_SOURCE_DIR}/binaries/binaries-deb.lib")
    else()
        message("Using MSVC library binary file. (binaries-rel.lib)")
        set(path_to_binary_lib "${CMAKE_SOURCE_DIR}/binaries/binaries-rel.lib")
    endif()
else()
    message(ERROR "No library binary file exists for '${CMAKE_CXX_COMPILER_ID}'. If you absolutely need to use this compiler, please contact the Algorithm Engineering lecture team.")
endif()

add_library(imported_binaries_for_tests STATIC IMPORTED)
set_target_properties(imported_binaries_for_tests PROPERTIES IMPORTED_LOCATION "${path_to_binary_lib}")

add_executable(test_edge_classification test_edge_classification.cpp)
target_link_libraries(test_edge_classification PRIVATE base_target  imported_binaries_for_tests)

enable_testing()

add_executable(
  jarnik_prim_test
  jarnik_prim_test.cc
)
target_link_libraries(
  jarnik_prim_test
  implementation
  imported_binaries_for_tests
  GTest::gtest_main
)

add_executable(
  range_maximum_query_test
  range_maximum_query_test.cc
)
target_link_libraries(
  range_maximum_query_test
  implementation
  GTest::gtest_main
)

add_executable(
  i_max_filter_test
  i_max_filter_test.cc
)
target_link_libraries(
  i_max_filter_test
  implementation
  imported_binaries_for_tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(jarnik_prim_test)
gtest_discover_tests(range_maximum_query_test)
gtest_discover_tests(i_max_filter_test)
