# dv C/C++ Bindings

This folder contains C and C++ headers wrapping the Rust core via a small C ABI shim.

See full docs under docs site pages “C” and “C++”.

Quick usage:
- Bazel: `bazel run //cpp:demo_c` and `bazel run //cpp:demo_cpp`
- CMake: `cmake -S . -B build && cmake --build build --target dv_c_demo dv_cpp_demo`

Headers in `cpp/include`:
- `dv_c.h`: C API
- `dv.hpp`: C++ RAII wrapper
