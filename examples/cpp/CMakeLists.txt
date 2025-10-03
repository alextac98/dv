cmake_minimum_required(VERSION 3.20)
project(dv_cpp_example CXX)

add_executable(dv_example_cpp main.cpp)
target_include_directories(dv_example_cpp PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../cpp/include)
target_link_libraries(dv_example_cpp PRIVATE dv_cpp)

if(APPLE)
	set_target_properties(dv_example_cpp PROPERTIES
		BUILD_RPATH ${CMAKE_CURRENT_LIST_DIR}/../../cpp/capi/target/release
	)
endif()
