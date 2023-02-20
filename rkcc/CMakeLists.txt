cmake_minimum_required(VERSION 3.13)
set(C_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -g -static")
project(rkcc CXX)
add_executable(rkcc 
    main.cpp 
    token.cpp 
    ast.cpp 
    dfs.cpp
    ast_print.cpp
    symbol.cpp
    symbols.cpp
    type.cpp
    code.cpp
    gen.cpp
    ../utils/utils.cpp
)
