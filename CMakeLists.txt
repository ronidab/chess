cmake_minimum_required(VERSION 3.17)
project(chess)

set(CMAKE_CXX_STANDARD 98)

add_executable(chess main.cpp Checker.cpp Checker.h GameBoard.cpp GameBoard.h Player.cpp Player.h)