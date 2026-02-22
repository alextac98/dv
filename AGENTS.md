# AGENTS.md - Guidance for agentic coding tools working in this repo.

## Project Summary

This project is a library in multiple languages for handling units and checking consistency in mathematical operations and conversions. The core functionality is implemented in Rust, with each language binding having their own folder. The entire project is managed by Bazel as a build system.

### Common Targets (Bazel)

- Rust core library: `//core:dv_rs`
- Rust tests: `//core:unit_tests`, `//core:units_tests`, `//core:operator_tests`
- Python test: `//python:test_dv`
- C example: `//examples/c:c_example`
- C++ example: `//examples/cpp:cpp_example`

### Folder Structure

- `core`: Contains the core library implemented in Rust.
- `docs`: Documentation for the project for both users and developers.
- `python`: Python bindings for the core library.
- `cpp`: C++ bindings for the core library.

## Notes for Agents

- If you need CI parity, mirror workflow commands in `.github/workflows/*.yaml`.

### Code Review Guidelines

- Focus on overall structure, design patterns, and correctness.
- Avoid nitpicking formatting or comments unless they are egregiously bad.
- Ensure adherence to coding standards and best practices.

## Build, Test, and Lint Commands

### Bazel (preferred for all languages)

- Build everything: `bazel build //... --keep_going`
- Test everything: `bazel test //... --test_output=errors --keep_going`

### Rust (core)
- Build core: `bazel build //core:dv_rs`
- Run all core tests: `bazel test //core/... --test_output=errors --keep_going`
- Run a single Rust test target: `bazel test //core:units_tests`
- Run a single Rust test function:
  `bazel test //core:units_tests --test_arg=angle_conversions`
  (Use Rust test name; Bazel passes args to the test binary.)
- Generate docs: `bazel build //core:docs`
- Serve docs: `bazel run //core:serve_docs`

### Python (bindings)
- Run Python tests: `bazel test //python/... --test_output=errors --keep_going`
- Run the Python example: `bazel run //examples/python:python_example`
- Single Python test target: `bazel test //python:test_dv`
- Single Python test function (pytest -k):
  `bazel test //python:test_dv --test_arg=-k --test_arg=TestMathFunctions`

### C/C++ (bindings/examples)
- Run C example (Bazel): `bazel run //examples/c:c_example`
- Run C++ example (Bazel): `bazel run //examples/cpp:cpp_example`
- CMake demos (optional):
  `cmake -S . -B build && cmake --build build --target dv_example_c dv_example_cpp`

### Docs (Docusaurus)
- Docs live under `docs/` and are managed via Bazel + pnpm.
- Update pnpm lockfile (from `docs/`):
  `bazel run -- @pnpm --dir $PWD install --lockfile-only`

### Linting / Formatting
- No explicit lint or formatter config is enforced in this repo (yet)
- For Rust, follow `rustfmt`-style formatting if you need to reformat.
- For Python, follow PEP 8 style and keep imports sorted.

## Code Style Guidelines
### Cross-cutting
- Prefer Bazel targets over ad-hoc build scripts.
- Keep changes minimal; avoid introducing new tools unless required.
- Favor clear, explicit error messages that help users of the bindings.

### Rust (core)
- Edition: 2021 (see `core/BUILD.bazel`).
- Naming: `snake_case` for functions/modules, `CamelCase` for types.
- Error handling: return `Result<_, String>` with clear messages.
- Use `expect` with a shared `FAIL_MSG` in tests (see `core/tests`).
- Prefer explicit conversions and unit compatibility checks.
- Keep unit parsing rules documented in doc comments.
- Use `&DimensionalVariable` for operator implementations to avoid copies.
- Avoid unnecessary `return` but keep consistent with existing style.

### Python (bindings/tests)
- Testing framework: `pytest`.
- Test classes named `TestSomething`, methods named `test_*`.
- Keep docstrings on test classes/methods for clarity.
- Import order: stdlib, third-party, local (`dv_py`), then specific symbols.
- Raise and assert `DVError` for invalid operations.

### C/C++ (bindings)
- Bazel-facing bindings are generated under `generated/diplomat/{c,cpp}`.
- `cpp/include/dv.hpp` is a thin compatibility wrapper over the generated Diplomat C++ bindings.
- CMake support is minimal; prefer Bazel for consistency.

### Docs (Docusaurus)
- Source in `docs/src` and `docs/docs`.
- Follow existing structure and avoid large refactors for small changes.
