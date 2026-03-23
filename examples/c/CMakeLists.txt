cmake_minimum_required(VERSION 3.20)
project(dv_c_example C)

add_executable(dv_example_c main.c)
target_link_libraries(dv_example_c PRIVATE dv_c)
