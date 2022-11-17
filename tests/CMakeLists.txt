set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "-Wall")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/15460959cbbfa20e66ef0b5ab497367e47fc0a04.zip
)
set(gtest_force_shared_crt
  ON
  CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

set(TESTCASES)

function(TESTCASE test_name)
  message(STATUS "test case ${test_name}")
  add_executable(${test_name} ${test_name}.cpp)
  target_link_libraries(${test_name} GTest::gtest_main)
  gtest_discover_tests(${test_name})
  set(TESTCASES ${TESTCASES} ${test_name} PARENT_SCOPE)
endfunction()

TESTCASE(deserialized_type)
TESTCASE(serialize_integer)
TESTCASE(serialize_dyn_array)
TESTCASE(serialize_static_array)
TESTCASE(serialize_tuple)
TESTCASE(size_traits)
TESTCASE(span)

if(ENABLE_COV)
  # Coverage setup
  set(CMAKE_BUILD_TYPE "Debug")
  include(CodeCoverage)
  APPEND_COVERAGE_COMPILER_FLAGS()

  set(COVERAGE_LCOV_EXCLUDES
    "/usr/include/*"
    "/**/*/usr/include/*"
    "${gtest_SOURCE_DIR}/*"
    "${PROJECT_SOURCE_DIR}/tests/*"
  )

  SETUP_TARGET_FOR_COVERAGE_LCOV(NAME coverage
    EXECUTABLE ctest
    DEPENDENCIES ${TESTCASES}
    LCOV_ARGS --rc lcov_branch_coverage=1
    GENHTML_ARGS --rc lcov_branch_coverage=1
  )
endif()