cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(csvreader)
set(CMAKE_BUILD_TYPE Debug)
add_executable(csvreader main.cpp inout.cpp)
