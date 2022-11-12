cmake_minimum_required(VERSION 3.13.4)
project(embedded-serialization)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)
SET(CMAKE_CXX_FLAGS "-Wall")
list(APPEND CMAKE_MODULE_PATH
  ${PROJECT_SOURCE_DIR}/cmake-modules)

if(NOT DISABLE_TEST)
  enable_testing()
  add_subdirectory(tests)
endif()
