add_library(dv_headers INTERFACE)
target_include_directories(dv_headers INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_executable(dv_cpp_demo examples/demo_cpp.cpp)
target_link_libraries(dv_cpp_demo PRIVATE dv_cpp)

add_executable(dv_c_demo examples/demo_c.c)
target_link_libraries(dv_c_demo PRIVATE dv_c)
