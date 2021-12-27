cmake_minimum_required(VERSION 3.21)
project(fast_interaction_nets C)

set(CMAKE_C_STANDARD 99)

include_directories(include)

add_executable(fast_interaction_nets src/main/c/main.c src/main/c/allocator.c)
