# dv examples

This directory contains minimal examples for each supported language.

- C: `examples/c`
- C++: `examples/cpp`
- Rust: `examples/rust`

## Bazel

- C example: `bazel run //examples/c:c_example`
- C++ example: `bazel run //examples/cpp:cpp_example`
- Rust example: `bazel run //examples/rust:rust_example`

## CMake (C/C++)

From the repository root:

1. Configure
   - `cmake -S . -B build`
2. Build targets
   - `cmake --build build --target dv_example_c`
   - `cmake --build build --target dv_example_cpp`

## Cargo (Rust)

- `cargo run --manifest-path examples/rust/Cargo.toml`

Notes

- The C and C++ examples link against the Rust C-ABI produced by the `cpp/capi` crate via the imported `dv_c` and `dv_cpp` targets.
- The Rust example depends on the core crate directly via a path dependency and uses Bazel via `//core:dv_rs` when built with Bazel.
