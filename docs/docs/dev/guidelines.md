---
sidebar_position: 1
---

# Contribution Guidelines
These guidelines describe how to contribute to, extend, and maintain the DV (Dimensional Variables) project. Like any good rule, developers shall follow these guidelines unless there are explicit reasons to deviate. This document is living, and will adapt and change with the project.

## Project Philosophy

In order to maintain a high standard, this project follows these standards, in order of importance:

1. Stability matters over all. Units shall never be wrong. Any mistakes shall be fixed immediately.
1. Units shall have 100% regression test coverage in every supported language.
1. Correctness over convenience - never silently drop/guess units.
1. The build system shall remain hermetic - all dependencies shall be managed and pulled in by Bazel.
1. Multi-language support is important. The Rust will remain the source of truth.
1. Additional unit and integration tests shall be added wherever makes sense.
1. Reviewers of pull requests shall be responsible to maintain the repo's level of rigor.

## Contributing to DV

The best way to contribute to the project is to follow a forking strategy - fork the repository, make your changes, and then open a pull request from your forked repository. Never commit artifacts, and rebase before merging to help keep linear history. Squashing can be used to combine noisy work-in-progress commits.

## Documentation & Docs Site
- Rust items: doc comments + examples (compile-tested when possible).
- Site (`docs/`): organize developer content under `docs/dev/*`; user tutorials under `tutorial-*`; avoid leaking WIP APIs.
- When adding a feature: include a “Why” paragraph (tradeoffs, rejected alternatives) if non-obvious.

Local docs generation (Rust):
```
bazel build //core:docs
python core/serve_docs.py  # lightweight server wrapper
```

## Versioning & Releases
- Semantic Versioning starting at `0.y.z` while API is unstable.
- Increment:
	- `y` for breaking changes
	- `z` for additive / bug fix
- Update `core/src/lib.rs:version()` and tag `vX.Y.Z` on `main`.
- Changelog (future): generate from Conventional Commits; for now summarize in release description.

