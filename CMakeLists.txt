cmake_minimum_required(VERSION 3.17)
project(shell)

set(CMAKE_CXX_STANDARD 20)

add_executable(shell main.cpp src/shell.cpp src/shell.h src/input.cpp src/input.h)