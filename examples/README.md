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

CMake examples are temporarily unavailable during the Diplomat migration. Use the Bazel targets above.

## Cargo (Rust)

- `cargo run --manifest-path examples/rust/Cargo.toml`

Notes

- The C and C++ examples now use Diplomat-generated bindings from `//c` and `//cpp`, backed by the Rust bridge in `core/ffi/diplomat`.
- The Rust example depends on the core crate directly via a path dependency and uses Bazel via `//core:dv_rs` when built with Bazel.
