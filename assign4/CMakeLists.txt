cmake_minimum_required(VERSION 3.9)
project(assign4 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra -Werror")

add_executable(assign4 prob_1.c)

