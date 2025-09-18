# DimensionalVariable (DV) Rust Library

> **Info**: This crate is the multi‑language core. For the exhaustive unit catalog and extended guides see [dv.alextac.com](https://dv.alextac.com).

DimensionalVariable (DV) povides parsing of unit strings (e.g. `"m/s^2"`, `"kWh"`, `"1/ft^2"`), normalization to base SI dimensions, arithmetic with unit checking, and value conversion. Features include:

- Parse compound units with `/`, `-`, exponents (`^` or suffixed digits), negatives.
- Convert between compatible units (`m` ↔ `cm`, `kWh` ↔ `J`, etc.).
- Dimensionally aware math (add/sub match units, pow/sqrt/log validation).

# Examples

The library will use the units to convert to a standardized unit (base SI units). Therefore, you can switch between units simply by using the `.value_in` function. These get checked so you are unable to convert between 2 incompatible units.

```rust
use dv_rs::DimensionalVariable as dv;

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

// Example
let v = dv::new(3.0, "m/s").expect(FAIL_MSG);
let t = dv::new(2.0, "s").expect(FAIL_MSG);
let d = &v * &t;
assert_eq!(d.value(), 6.0);
assert_eq!(d.unit(), [1, 0, 0, 0, 0, 0, 0]);

// Bad Example
let m = dv::new(1.0, "m").expect(FAIL_MSG);
let s = dv::new(1.0, "s").expect(FAIL_MSG);
assert!(m.try_add(&s).is_err());
assert!(m.try_sub(&s).is_err());

// Bad Example, throws panic
let m = dv::new(1.0, "m").expect(FAIL_MSG);
let s = dv::new(1.0, "s").expect(FAIL_MSG);
let _ = m + s; // should panic due to incompatible units
```

Check out the the [docs for more](https://dv.alextac.com)!
