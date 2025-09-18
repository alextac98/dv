---
title: Rust
sidebar_position: 2
---

# DV + Rust Examples

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

Most math is supported, including `+`, `-`, `*`, `/`, `powi`, `powf`, `sqrt`, `abs`. Units are checked to make sure math operations are compatible.

```rust
use dv_rs::DimensionalVariable as dv;
const FAIL_MSG: &str = "Failed!";

// Example
let v = dv::new(3.0, "m/s").expect(FAIL_MSG);
let t = dv::new(2.0, "s").expect(FAIL_MSG);
let d = &v * &t;
assert_eq!(d.value(), 6.0);
assert_eq!(d.unit(), [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);

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
assert_eq!(r.unit(), [1.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0]);

let b = dv::new(4.0, "m^3").unwrap();
let r2 = b.sqrt().unwrap();
assert_eq!(r2.unit(), [1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);
```

