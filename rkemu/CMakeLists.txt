cmake_minimum_required(VERSION 3.13)

project(rkemu CXX)

set(CMAKE_CXX_FLAGS "-std=c++2a -Wall -Wextra -g")

add_executable(rkemu 
    main.cpp 
    debug.cpp 
    model.cpp 
    utils.cpp
    ../rkisa/isa.cpp
)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

target_link_libraries(rkemu ${CURSES_LIBRARIES})
