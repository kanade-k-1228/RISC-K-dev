cmake_minimum_required(VERSION 3.13)

# コンパイラの設定
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# プロジェクトの設定
project(rkasm CXX)
add_executable(rkasm 
    main.cpp
    utils.cpp
    utils/args.cpp
    utils/error.cpp
    class/line.cpp
    class/label.cpp
    class/operation.cpp
    class/imm.cpp
    ../rkarch/arch.cpp
)

# Boost ライブラリのリンク
find_package(Boost 1.74 REQUIRED COMPONENTS 
    program_options
)
if(Boost_FOUND)
    message("# Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message("# Boost_LIBRARIES   : ${Boost_LIBRARIES}")
    message("# Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(rkasm ${Boost_LIBRARIES})
else()
    message(FATAL_ERROR "Boost not found. Please install Boost or specify Boost path.")
endif()
