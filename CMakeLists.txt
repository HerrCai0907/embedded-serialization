cmake_minimum_required(VERSION 3.13.4)
project(embedded-serialization)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

if (NOT DISABLE_TEST)
  enable_testing()
  add_subdirectory(tests)
endif()