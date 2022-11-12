set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/15460959cbbfa20e66ef0b5ab497367e47fc0a04.zip
)
set(gtest_force_shared_crt
  ON
  CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(${CMAKE_SOURCE_DIR}/src)
include(GoogleTest)

function(TESTCASE test_name)
  message(STATUS "test case ${test_name}")
  add_executable(${test_name} ${test_name}.cpp)
  target_link_libraries(${test_name} GTest::gtest_main)
  gtest_discover_tests(${test_name})
endfunction()

TESTCASE(serialize_integer)
TESTCASE(serialize_dyn_array)

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
    DEPENDENCIES serialize_integer
    LCOV_ARGS --rc lcov_branch_coverage=1
    GENHTML_ARGS --rc lcov_branch_coverage=1
  )
endif()