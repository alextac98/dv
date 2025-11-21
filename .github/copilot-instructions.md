# Project Overview
This project is a library in multiple languages for handling units and checking consistency in mathematical operations and conversions. The core functionality is implemented in Rust, with each language binding having their own folder. The entire project is managed by Bazel as a build system.

## Agent Instructions

Please use Bazel as the build system for all tasks related to building, testing, and managing dependencies. Ensure that any new code or changes adhere to the existing project structure and coding standards.

## Folder Structure
- `core`: Contains the core library implemented in Rust.
- `docs`: Documentation for the project for both users and developers.
- `python`: Python bindings for the core library.
- `cpp`: C++ bindings for the core library.

## Code Reviews

Code reviews should not include small nits like formatting or comments unless they are egregiously bad. Focus on the overall structure, design patterns, and correctness of the implementation. Ensure that the code adheres to the project's coding standards and best practices.