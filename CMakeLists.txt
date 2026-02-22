cmake_minimum_required(VERSION 3.20)
project(dv LANGUAGES C CXX)

message(FATAL_ERROR
    "CMake support is temporarily disabled during the Diplomat migration. "
    "Use Bazel targets (e.g. //examples/c:c_example and //examples/cpp:cpp_example)."
)
