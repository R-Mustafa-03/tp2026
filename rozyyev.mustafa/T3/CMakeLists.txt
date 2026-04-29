cmake_minimum_required(VERSION 3.10)
project(T3)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(T3 main.cpp
        Polygon.cpp
        Polygon.h
        Commands.cpp
        Commands.h
)

target_compile_options(T3 PRIVATE -Wall -Wextra -Werror)
