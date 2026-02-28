cmake_minimum_required(VERSION 3.20)
project(dv LANGUAGES C CXX)

include(ExternalProject)

find_program(BAZEL_EXECUTABLE bazel REQUIRED)
find_program(CARGO_EXECUTABLE cargo REQUIRED)

set(DV_BRIDGE_DIR ${CMAKE_CURRENT_LIST_DIR}/ffi/diplomat)
set(DV_BRIDGE_ENTRY ${DV_BRIDGE_DIR}/src/lib.rs)
set(DV_CODEGEN_ROOT ${CMAKE_BINARY_DIR}/generated/diplomat)
set(DV_CODEGEN_C_DIR ${DV_CODEGEN_ROOT}/c)
set(DV_CODEGEN_CPP_DIR ${DV_CODEGEN_ROOT}/cpp)

set(DV_GEN_C_HEADERS
    ${DV_CODEGEN_C_DIR}/BaseUnits.d.h
    ${DV_CODEGEN_C_DIR}/BaseUnits.h
    ${DV_CODEGEN_C_DIR}/DimensionalVariable.d.h
    ${DV_CODEGEN_C_DIR}/DimensionalVariable.h
    ${DV_CODEGEN_C_DIR}/diplomat_runtime.h
)
set(DV_GEN_CPP_HEADERS
    ${DV_CODEGEN_CPP_DIR}/BaseUnits.d.hpp
    ${DV_CODEGEN_CPP_DIR}/BaseUnits.hpp
    ${DV_CODEGEN_CPP_DIR}/DimensionalVariable.d.hpp
    ${DV_CODEGEN_CPP_DIR}/DimensionalVariable.hpp
    ${DV_CODEGEN_CPP_DIR}/diplomat_runtime.hpp
)

add_custom_command(
    OUTPUT ${DV_GEN_C_HEADERS}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${DV_CODEGEN_C_DIR}
    COMMAND ${BAZEL_EXECUTABLE} run //tools/diplomat_codegen:diplomat_codegen -- c ${DV_CODEGEN_C_DIR} --entry ${DV_BRIDGE_ENTRY}
    DEPENDS ${DV_BRIDGE_ENTRY}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    VERBATIM
)

add_custom_command(
    OUTPUT ${DV_GEN_CPP_HEADERS}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${DV_CODEGEN_CPP_DIR}
    COMMAND ${BAZEL_EXECUTABLE} run //tools/diplomat_codegen:diplomat_codegen -- cpp ${DV_CODEGEN_CPP_DIR} --entry ${DV_BRIDGE_ENTRY}
    DEPENDS ${DV_BRIDGE_ENTRY}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    VERBATIM
)

add_custom_target(dv_diplomat_codegen DEPENDS ${DV_GEN_C_HEADERS} ${DV_GEN_CPP_HEADERS})

set(DV_CARGO_TARGET_DIR ${DV_BRIDGE_DIR}/target)
set(DV_CARGO_BUILD_CMD
    ${CARGO_EXECUTABLE} build
    --manifest-path ${DV_BRIDGE_DIR}/Cargo.toml
    --release
)

ExternalProject_Add(dv_diplomat_bridge_ext
    SOURCE_DIR ${DV_BRIDGE_DIR}
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ${DV_CARGO_BUILD_CMD}
    INSTALL_COMMAND ""
    BUILD_BYPRODUCTS
        ${DV_BRIDGE_DIR}/target/release/libdv_diplomat_bridge.a
)

set(DV_DIPLOMAT_STATIC ${DV_BRIDGE_DIR}/target/release/libdv_diplomat_bridge.a)

add_library(dv_c STATIC IMPORTED GLOBAL)
set_target_properties(dv_c PROPERTIES
    IMPORTED_LOCATION ${DV_DIPLOMAT_STATIC}
    INTERFACE_INCLUDE_DIRECTORIES ${DV_CODEGEN_C_DIR}
)
add_dependencies(dv_c dv_diplomat_bridge_ext dv_diplomat_codegen)

add_library(dv_cpp INTERFACE)
target_include_directories(dv_cpp INTERFACE ${DV_CODEGEN_CPP_DIR})
target_link_libraries(dv_cpp INTERFACE dv_c)
add_dependencies(dv_cpp dv_diplomat_codegen)

add_subdirectory(examples/c)
add_subdirectory(examples/cpp)
