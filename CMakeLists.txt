cmake_minimum_required(VERSION 3.24)
project(struct)

set(CMAKE_CXX_STANDARD 17)


add_executable(
        struct
        hash_table/hash_table.cpp
        hash_table/HashTable.h
)
