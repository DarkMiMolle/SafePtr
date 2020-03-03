cmake_minimum_required(VERSION 3.8)

project(SafePtr)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SafePtr main.cpp Ptr.hpp)