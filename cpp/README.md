# dv C/C++ Bindings

The Rust Diplomat bridge lives in `core/ffi/diplomat/`, while the Bazel-facing C/C++ binding targets live directly in the language directories.

- C target alias: `//cpp:dv_c`
- C++ target alias: `//cpp:dv_cpp`

See:
- `c/` for the generated C bindings and the `//c:dv_c` Bazel target
- `cpp/` for the C++ compatibility wrapper and the `//cpp:dv_cpp` Bazel target
- `docs/c.md` and `docs/cpp.md` for usage and build examples
