cmake_minimum_required(VERSION 3.24)
project(struct)

set(CMAKE_CXX_STANDARD 17)


add_executable(
        hui
#        binary_tree/BST.cpp
#        binary_tree/BST.h
        stack/Stack.cpp
        stack/Stack.h
        stack/LStack.cpp
        stack/LStack.h
        stack/VStack.cpp
        stack/VStack.h
        stack/StackImplementation.h

)
