---
title: Intro to DV
sidebar_position: 1
---
# What is DV (DimensionalVariable)?

DV is a multi‑language library for numeric values with physical units. It parses human‑friendly unit strings (e.g. `"m/s^2"`, `"kWh"`, `"1/ft^2"`), standardizes them to a fixed SI base vector, and enforces dimensional correctness during math operations and conversion. The goal is to make working with units in different languages safe and as close to computationally cheap as working with raw numbers.

## Core Functionality

DV as a standard is simply a struct with two major parts:
* A 64-bit float representing the value expressed in canonical SI base units
* A vector of 8 64-bit floats representing the exponents for the base SI units plus angle (allows fractional exponents)

The process from import to storage is as follows:

1. Tokenize unit string (supports `/` for numerator/denominator, `-` separator, exponents with `^` or suffix digits, negative powers).
1. Map each symbol to its base unit exponent vector + conversion factor.
1. Accumulate exponents (denominator subtracts) into a 7‑element array.
1. Multiply the numeric value by each unit’s conversion factor to yield SI value.
1. Store normalized pair; subsequent arithmetic never needs the original string.

The vector follows this standard: `[m, kg, s, K, A, mol, cd, rad]`, where each unit represents the base SI units:

| Unit | Name | Representation |
| --- | --- | --- |
| `m` | meters | distance |
| `kg` | kilograms | mass |
| `s` | seconds | time |
| `K` | kelvin | temperature |
| `A` | amps | electrical current |
| `mol` | mol | quantity |
| `cd` | candela | light intensity |
| `rad` | radians | angle |

Example: `9.81 km/s^2` stores as: `{9810.0, [1.0, 0.0, -2.0, 0.0, 0.0, 0.0, 0.0, 0.0]}`.

The full list of available core units can be found under `core/src/units.rs`. There will be a future feature for users to be able to add their own additional units without needing to re-compile or rebuild the library.

<!-- TODO: we should really auto-render all the supported core units here. -->

### Angles as units?!?!

You may ask - aren't angles unitless? You'd be technically correct, but we chose to do something a bit special to add some useful functionality.

Angles are often used in engineering, and keeping track of angles separately can be very useful! When you get into more complex math, especially with angle math operators like `sin/cos/tan` and their inverses `arcsin/arccos/arctan`, relying on having the proper units becomes very necessary.

Therefore, this library treats angle units in a slightly special way. As expected trigonometry functions (`sin/cos/tan`) requre a dimension that is only an angle, and return a unitless dimension. The inverse trigonometyr functions (`asin/acos/atan`) do the exact opposite - they require a unitless dimension and return an angle dimension.

Additionally, the DimensionalVariable system allow for conversion from angular dimensions (angle exponent == 1) to also be equivalent to non-angular dimenions (angle exponent != 1), but not the other way around. This allows for equations (torque * angular speed = power), but protects against accidental math (frequency should not equal angular speed).

If you come up with an edge case that breaks this logic, please open a GitHub issue to start the discussion, or propose a new fix!

### Parsing Unit Strings

DV intelligently can determine what units to parse down to based on some simple string parsing that happens at the creation of the object and when data is extracted out (importantly not during math operations). To effectively parse the unit strings, users must follow these rules:

* Use a single `/` as a delimiter between the numerator and denominator.
* Use a `-` as a delimiter between individual units.
* Exponents can be represented either by using a `^` to indicate an exponent (ex. `m^2`) or without the delimiter (ex. `m2`).
* Inverses can be either represented by negative exponents (ex. `m^-2`) or in the denominator (ex. 1/m^2).

For those who care about performance, the string lookup table is stored as a hash map, giving us an O(1) complexity.

### Conversion Operations

Unit conversions are built in to the architecture - getting the value out of a DimensionalVariable will automatically convert to the requested unit. See respective language documentation for examples!

### Math Operations

The DV library overrides common math operators to add in additional checks and fail on incorrect/bad math operations that don't have compatible units. Each language has specifics on how this overriding works, so users should consult relevant pages. Supported math operations include:

| Operation | Checks |
| --- | --- |
| addition / subtraction | unit exponent vectors must match |
| multiplication / division | no checks, unit exponent vectors are added |
| power / sqrt | powi multiplies exponents by integer power; powf and sqrt supported with fractional exponents; value must be valid for sqrt; logs/trig remain unitless-only |
| sin / cos / tan | DV must be angle (radians), returns a unitless value |
| asin / acos / atan | DV must be unitless, returns an angled value |
| neg / abs | no checks |

## Why make another unit management system?

Most languages do have their own way of managing units. However, they fall short on a couple of fronts:

* Cross-language support 
* No support for safe math / equations
* Possible wire format for messaging

DV fixes all of these issues

* Deterministic canonical form: any compatible spelling (e.g. `cm`, `m`) collapses to identical exponent vector.
* Constant‑time arithmetic: ops are just value math + 7‑slot integer add/sub.
* Explicit safety: incompatible dimensions either return `Err` (in `try_*`) or panic in operator overloads (programmer error surfaced immediately).
* Stable wire format: serialize once, trust everywhere.
* Easy to implement bindings—logic is small and testable.

## Supported & Planned Languages
Status | Language | Notes
-------|----------|------
✅ Core | Rust | Reference implementation
✅ C | Diplomat-generated C bindings
✅ C++ | Diplomat-generated C++ bindings
✅ Python | Diplomat-generated nanobind bindings (+ Python compatibility wrapper)
🛠 Planned | JavaScript / TypeScript | WASM or N-API layer
🛠 Planned | Java | JNI via C ABI (or pure port)
🛠 Planned | Matlab | MEX wrapper

Cross‑language parity tests will ensure identical exponent math and error strings (where sensible).
