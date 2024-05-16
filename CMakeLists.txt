cmake_minimum_required(VERSION 3.28.1)

project(task LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP true)

FetchContent_MakeAvailable(googletest)

add_executable(${PROJECT_NAME} main.cpp)

add_executable(tests tests.cpp)

add_test(NAME my_test COMMAND tests)

target_link_libraries(tests 
  gtest 
  gtest_main)