cmake_minimum_required(VERSION 3.19)
project(OSLab3)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
add_executable(OSLab3 main.cpp)