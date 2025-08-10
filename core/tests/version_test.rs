// Integration tests for dv_core
// These tests exercise the public API exposed by the dv_core library.

use dv_rs::version;

#[test]
fn version_returns_semver_like_string() {
    let v = version();
    // Basic checks: not empty and contains at least one dot
    assert!(!v.trim().is_empty(), "version should not be empty");
    assert!(v.contains('.'), "version should contain a dot, got: {}", v);
}
