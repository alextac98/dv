cmake_minimum_required(VERSION 3.20)
project(dv_c_example C)

# Expect to be added from repo root as part of the overall build. Use include and link from cpp/ setup.
add_executable(dv_example_c main.c)
target_include_directories(dv_example_c PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../cpp/include)
target_link_libraries(dv_example_c PRIVATE dv_c)

if(APPLE)
	# Allow running example if dv_capi.dylib is used at runtime
	set_target_properties(dv_example_c PROPERTIES
		BUILD_RPATH ${CMAKE_CURRENT_LIST_DIR}/../../cpp/capi/target/release
	)
endif()
