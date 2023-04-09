cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_FLAGS "-std=c++2a -Wall -Wextra -g -static")

project(rkasm CXX)

add_executable(rkasm 
    imm.cpp
    label_def.cpp
    label_table.cpp
    line.cpp
    main.cpp
    operation.cpp
    utils.cpp
)
