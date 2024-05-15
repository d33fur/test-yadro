cmake_minimum_required(VERSION 3.28.1)

project(task LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE)

FetchContent_MakeAvailable(googletest)

ADD_EXECUTABLE(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} 
  gtest 
  gtest_main)