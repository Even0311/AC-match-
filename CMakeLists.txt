cmake_minimum_required(VERSION 3.5)
project(a3search)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(a3search ${SOURCE_FILES})