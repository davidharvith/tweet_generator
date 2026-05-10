cmake_minimum_required(VERSION 3.26)
project(ex3 C)

set(CMAKE_C_STANDARD 99)

include_directories(include)

add_executable(ex3
        src/linked_list.c
        src/markov_chain.c
        src/tweets_generator.c
)
