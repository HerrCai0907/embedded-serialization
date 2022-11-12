set(CMAKE_CXX_STANDARD 14)

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
  target_link_libraries (${test_name} GTest::gtest_main)
  gtest_discover_tests(${test_name}) 
endfunction()



