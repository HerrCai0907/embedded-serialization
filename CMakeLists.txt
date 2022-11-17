cmake_minimum_required(VERSION 3.13.4)
project(embedded-serialization)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)
list(APPEND CMAKE_MODULE_PATH
  ${PROJECT_SOURCE_DIR}/cmake-modules)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

if(NOT DEFINED EMBEDDED_SERIALIZATION_INTEGRATION)
  if(NOT DISABLE_TEST)
    enable_testing()
    add_subdirectory(tests)
  endif()
else()
  message(STATUS "EMBEDDED_SERIALIZATION_INTEGRATION mode")
endif()
