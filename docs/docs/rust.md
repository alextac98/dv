---
title: Rust
sidebar_position: 2
---
<div style={{display: 'flex', alignItems: 'center', justifyContent: 'space-between', marginBottom: '20px'}}>
  <h1 style={{margin: 0}}>DV with Rust</h1>
  <div style={{display: 'flex', gap: '8px', flexShrink: 0}}>
    <a href="https://crates.io/crates/dv" target="_blank" rel="noopener noreferrer">
      <img src="https://img.shields.io/crates/v/dv.svg" alt="crates.io" />
    </a>
    <a href="https://crates.io/crates/dv" target="_blank" rel="noopener noreferrer">
      <img src="https://img.shields.io/crates/d/dv.svg" alt="downloads" />
    </a>
  </div>
</div>
#

The core library is written in Rust, and can be installed via Crates.io or directly from GitHub.

## Quickstart

Add DV to your Rust project by adding it to your `Cargo.toml`:

```toml title="Cargo.toml"
[dependencies]
dv_rs = "*"  # Or specify a version like "0.4.0"
```

Or use cargo to add it:

```bash
cargo add dv_rs
```

Then import and use it in your code:

```rust
use dv_rs::DimensionalVariable as dv;
const FAIL_MSG: &str = "Failed!";

// Example
let len = dv::new(2.5, "m").unwrap();
let len_cm = len.value_in("cm").unwrap();
assert_eq!(len_cm, 250.0);

// Bad Example
let d = dv::new(1.0, "m").expect(FAIL_MSG);
let result = d.value_in("h");
assert!(result.is_err());
```

## Example

```rust
use dv_rs::DimensionalVariable as dv;
const FAIL_MSG: &str = "Failed!";

// Example
let v = dv::new(3.0, "m/s").expect(FAIL_MSG);
let t = dv::new(2.0, "s").expect(FAIL_MSG);
let d = &v * &t;
assert_eq!(d.value(), 6.0);
assert_eq!(d.unit(), [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);

// Bad Example
let m = dv::new(1.0, "m").expect(FAIL_MSG);
let s = dv::new(1.0, "s").expect(FAIL_MSG);
assert!(m.try_add(&s).is_err());
assert!(m.try_sub(&s).is_err());

// Bad Example, throws panic
let m = dv::new(1.0, "m").expect(FAIL_MSG);
let s = dv::new(1.0, "s").expect(FAIL_MSG);
let _ = m + s; // should panic due to incompatible units

// Fractional exponents via sqrt and powf
let a = dv::new(9.0, "m^2/s^2").unwrap();
let r = a.sqrt().unwrap();
assert_eq!(r.unit(), [1.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0]);

let b = dv::new(4.0, "m^3").unwrap();
let r2 = b.sqrt().unwrap();
assert_eq!(r2.unit(), [1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);

// Angles (radians and degrees)
let angle_rad = dv::new(std::f64::consts::PI, "rad").unwrap();
let angle_deg = dv::new(180.0, "deg").unwrap();
assert_eq!(angle_rad.value_in("rad").unwrap(), angle_deg.value_in("rad").unwrap());

// Trigonometric functions require radians
use std::f64::consts::PI;
let angle = dv::new(PI / 4.0, "rad").unwrap();
assert!((angle.sin().unwrap() - (PI / 4.0).sin()).abs() < 1e-12);
```

