---
title: C/C++
sidebar_position: 3
---

# DV with C/C++

DV has been designed to work in either C or C++, and the primary supported build path is Bazel.

## A note about build systems

We currently do not publish C/C++ libraries, so you will need to build the library from scratch every time (see [GH-32](https://github.com/alextac98/dv/issues/32)).

For Bazel, make sure you properly have both Rust and C/C++ toolchains (ideally hermetic) set up and running. We recommend [llvm-toolchains](https://github.com/bazel-contrib/toolchains_llvm) for C/C++ hermeticity.

## C++

DV exposes generated C and C++ bindings via Diplomat. The Rust bridge crate lives in `core/ffi/diplomat`, while the Bazel targets for each language live in `c/` and `cpp/`.

```cpp 
#include "DimensionalVariable.hpp"

int main() {
    // See examples/cpp/main.cpp for the generated Diplomat C++ API with result handling.
    return 0;
}

```

### Bazel with C++

```bzl title="MODULE.bazel"
bazel_dep(name = "dv", version = "0.0.0")
git_override(
    module_name = "dv",
    remote = "https://github.com/alextac98/dv.git",
    commit = "main",
)`
```

```bzl title="BUILD.bazel"
load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "example",
    srcs = ["main.cpp"],
    deps = ["@dv//cpp:dv_cpp"],  # compatibility wrapper target
)
```

If you want the raw generated C++ API directly, depend on `@dv//cpp:dv_generated`.

### Static vs dynamic linking

- Default in this repo uses static linking to avoid runtime `rpath` issues.
- If you choose dynamic linking on macOS, ensure your produced shared library install name/rpath are configured appropriately for your build system.

## C

Use the generated C header `DimensionalVariable.h` for the opaque handle API.

```c
#include "DimensionalVariable.h"
int main(void){
  // See examples/c/main.c for Diplomat-generated result structs and string views.
  // The generated API names are prefixed with `DimensionalVariable_*`.
  return 0;
}
```

### Bazel with C

```bzl title="MODULE.bazel"
bazel_dep(name = "dv", version = "0.0.0")
git_override(
    module_name = "dv",
    remote = "https://github.com/alextac98/dv.git",
    commit = "main",
)`
```

```bzl title="BUILD.bazel"
load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
  name = "example_c",
  srcs = ["main.c"],
  deps = ["@dv//c:dv_c"],
)
```

### CMake

CMake support shells out to the Rust Diplomat bridge in `core/ffi/diplomat` to regenerate C and C++ headers in the build tree. Bazel remains the primary development path.
