---
title: C/C++
sidebar_position: 3
---

# DV with C/C++

DV has been designed to work in either C or C++, with either Bazel or CMake as a build system.

## A note about build systems

We currently do not publish C/C++ libraries, so you will need to build the library from scratch every time (see [GH-32](https://github.com/alextac98/dv/issues/32)).

For CMake, you'll need to make sure that Cargo is installed on the system you intend to build. Follow instructions here: https://rust-lang.org/tools/install

For Bazel, make sure you properly have both Rust and C/C++ toolchains (ideally hermetic) set up and running. We recommend [llvm-toolchains](https://github.com/bazel-contrib/toolchains_llvm) for C/C++ hermeticity.

## C++

DV exposes a C API and a thin C++ wrapper (`dv.hpp`). You can use it from Bazel or CMake.

```cpp 
#include "dv.hpp"
#include <iostream>

int main() {
    dv::DV mass(10.0, "kg");
    dv::DV accel(9.81, "m/s^2");
    dv::DV force = mass * accel; // N
    std::cout << "force = " << force.value_in("lbf") << " lbf\n";
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
    deps = ["@dv//cpp:dv_cpp"],  # C++ wrapper
)
```

### CMake with C++

```cmake title="CMakeLists.txt"
include(FetchContent)
FetchContent_Declare(dv GIT_REPOSITORY https://github.com/alextac98/dv.git GIT_TAG main)
FetchContent_MakeAvailable(dv)

add_executable(example main.cpp)
target_link_libraries(example PRIVATE dv_cpp)
```

### Static vs dynamic linking

- Default in this repo uses static linking to avoid runtime `rpath` issues.
- If you choose dynamic linking on macOS, set `install_name` to `@rpath/libdv_capi.dylib` and add an rpath (e.g., `@loader_path`).

## C

Use the C header `dv_c.h` for a simple opaque handle API.

```c
#include "dv_c.h"
#include <stdio.h>
int main(){
  dv_var* d = dv_var_new(10.0, "m");
  dv_var* t = dv_var_new(2.0, "s");
  dv_var* v = dv_var_div(d, t);
  double mph = 0.0; dv_var_value_in(v, "mi/hr", &mph);
  printf("%f\n", mph);
  dv_var_free(v); dv_var_free(d); dv_var_free(t);
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
  deps = ["@dv//cpp:dv_c"],
)
```

### CMake with C

```cmake title="CMakeLists.txt"
include(FetchContent)
FetchContent_Declare(dv GIT_REPOSITORY https://github.com/alextac98/dv.git GIT_TAG main)
FetchContent_MakeAvailable(dv)

add_executable(example_c main.c)
target_link_libraries(example_c PRIVATE dv_c)
```
