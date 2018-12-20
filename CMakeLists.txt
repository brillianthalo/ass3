cmake_minimum_required(VERSION 3.10)
project(ass3 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g -fstack-protector-all")
add_executable(ass3 ass3.c)