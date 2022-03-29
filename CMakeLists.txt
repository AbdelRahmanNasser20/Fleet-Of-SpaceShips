cmake_minimum_required(VERSION 3.21)
project(Proj2Fleet)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Proj2Fleet
        fleet.cpp
        fleet.h
        mytest.cpp)
