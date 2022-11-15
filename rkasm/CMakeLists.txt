cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_FLAGS "-std=c++2a -Wall -Wextra -g -static")

project(rkasm CXX)

add_executable(rkasm main.cpp code.cpp utils.cpp)
