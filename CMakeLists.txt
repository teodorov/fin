cmake_minimum_required(VERSION 3.21)
project(fast_interaction_nets C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

include_directories(include)

add_executable(fast_interaction_nets src/main/c/main.c src/main/c/allocator.c src/main/c/bitset.c include/bitset.h src/main/c/fin_core.c include/fin_core.h src/main/c/peano.c)
