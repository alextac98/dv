---
sidebar_position: 1
---

# Developing for DV

DV (DimensionalVariable) is designed to be easily contributed to! Whether you're updating documentation, adding more units, or contributing to code, we welcome and greatly appreciate your contributions and support.

## Quick References

For those who just need to get started, or reminded what command to use for specific functions, here is a quick reference for the biggest commands needed:

#### General
```shell
# Build everything in the repository
bazel build //...
# Run all tests
bazel test //...
```

#### Rust

```shell
# Build Rust library
bazel build //core:dv_rs
# Run tests
bazel test //core/...
# Run Rust docs dev server
bazel run //core:serve_docs
```

#### Python

```shell
# Build the extension module (.so)
bazel build //generated/diplomat/nanobind:dv_pyo3.so

# Run tests
bazel test //python:test_dv

# Run example
bazel run //examples/python:python_example
```

#### Documentation Site

```shell
# Build and run dev server
bazel run //docs:dev
# Build for production
bazel run //docs:prod
```

## Core Concepts

DV is built to have multi-language support, therefore it uses Bazel as a fully integrated and centralized build system. It also has the added benefit of being fully hermetic - developers only need to ever install Bazel to contribute to the project.

The core of the library is written in rust, and lives in the `core` directory. Language specific implementations and wrappers exist in the relevant top level directory.

### Repo Layout

| Path | Description |
|---|---|
| `core/` | Rust crate + core functionality |
| `docs/` | Docs site for users and developers |
| `cpp/` | C/C++ bindings |
| `python/` | Python bindings |
| `MODULE.bazel` | Bazel module dependencies |

### Contributing Units

If you are looking to add or modify a unit, take a look at `src/units.rs`. Adding/modifying should be fairly self-explanatory, and any changes should also include changes to the regression tests to help with protecting from regressions. Read more about how the units system works on the [intro page](../intro.md).

Only contribute unit names with individual names, such as `watts` or `fahrenheit`. Units such as `rpm` should be written out, such as `rev/min`.

## Developer Setup

This project is tested to work on both Linux and MacOS for development. It may "just work" on Windows directly, but it is not a supported build environment, and may break at any time. WSL is recommended if developing on a Windows device.

The only local dependency needed for the project is Bazel/Bazelisk - all other dependencies are pulled in by Bazel at build-time. We recommend installing Bazelisk to automatically manage the correct version of Bazel for the given repo. You can install Bazelisk by following the instructions here: https://github.com/bazelbuild/bazelisk.

:::info

Do not be alarmed if your code changes compile quickly! Bazel is designed to be extremely efficient and stable at caching. You should never need to clear the cache to see your changes. If you do, then that is a serious bug and should either be fixed immediately or reported.

:::

If you're new to Bazel, learn more about it here: https://bazel.build/about/intro. However, unless you're making major contributions, the build system is set up to be fairly easy to use and understand. Take a look at the relevant sections for the part of the code you're developing.

## Documentation

The documentation system is centralized around this website, which lives in the `docs/` directory in the repository, and uses Docusaurus as a document engine. Each language may additionally have their own documentation pages in accordance to best practices for each of the languages. For example, Rust cargo crates require that they are published with documentation. In all cases, the main documentation site shall contain all the information needed, and the language-specific documentation sites should point back to the main site.

## Testing Framework

The testing framework is split up into several key tests:

* Unit tests: tests individual and small pieces of code critical to operations.
* Integration tests: verifies all functionality is working as expected.
* Regression tests: checks that no features have regressed, including units values.

Units and their scalars are included in the regression test suite. 100% test coverage on all units is the goal, but currently not enforced. There are plans to create a regression test suite that will cover all supported languages, but that isn't implemented yet (see https://github.com/alextac98/dv/issues/15).

## Updating Tools Requirements.txt

If you're interfacing with python and need to add a new dependency, here's how to do it:

1. Add your new requirement to `requirements.in`

2. Using `uv` or a similar tool, run the following command to generate the requirements.txt:

```bash
uv pip compile tools/requirements.in -o tools/requirements.txt
``
