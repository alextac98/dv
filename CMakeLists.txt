cmake_minimum_required(VERSION 3.20)
project(dv LANGUAGES C CXX)

# Options
option(DV_BUILD_SHARED "Build shared library" ON)

# Build Rust C-ABI via Cargo. We use ExternalProject for portability.
include(ExternalProject)
set(DV_RUST_DIR ${CMAKE_CURRENT_LIST_DIR}/cpp/capi)
set(DV_BUILD_CMD cargo build --manifest-path ${DV_RUST_DIR}/Cargo.toml --release)

ExternalProject_Add(dv_capi_ext
    SOURCE_DIR ${DV_RUST_DIR}
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ${DV_BUILD_CMD}
    INSTALL_COMMAND ""
    BUILD_BYPRODUCTS 
        ${DV_RUST_DIR}/target/release/libdv_capi.a
        ${DV_RUST_DIR}/target/release/libdv_capi.dylib
)

# Library artifact path
if(APPLE)
    set(DV_CAPI_SHARED ${DV_RUST_DIR}/target/release/libdv_capi.dylib)
elseif(UNIX)
    set(DV_CAPI_SHARED ${DV_RUST_DIR}/target/release/libdv_capi.so)
elseif(WIN32)
    set(DV_CAPI_SHARED ${DV_RUST_DIR}/target/release/dv_capi.dll)
endif()
set(DV_CAPI_STATIC ${DV_RUST_DIR}/target/release/libdv_capi.a)

add_library(dv_c STATIC IMPORTED GLOBAL)
set_target_properties(dv_c PROPERTIES
    IMPORTED_LOCATION ${DV_CAPI_STATIC}
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_LIST_DIR}/cpp/include
)
add_dependencies(dv_c dv_capi_ext)

# Help executables find the dylib at runtime on macOS when running from build tree
if(APPLE)
    set_property(TARGET dv_c PROPERTY INTERFACE_LINK_DIRECTORIES ${DV_RUST_DIR}/target/release)
endif()

add_library(dv_cpp INTERFACE)
target_include_directories(dv_cpp INTERFACE ${CMAKE_CURRENT_LIST_DIR}/cpp/include)
target_link_libraries(dv_cpp INTERFACE dv_c)

add_subdirectory(cpp)

add_subdirectory(examples/c)
add_subdirectory(examples/cpp)
