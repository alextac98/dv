cmake_minimum_required(VERSION 3.20)
project(dv_cpp_example CXX)

add_executable(dv_example_cpp main.cpp)
target_link_libraries(dv_example_cpp PRIVATE dv_cpp)
