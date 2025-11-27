---
title: Python
sidebar_position: 3
---

# DV with Python

DV supports python via PyO3 bindings to the core library. Any Rust `Results<>` are converted to Python exceptions.

The project currently builds for these architectures:
* Linux
    * x86 64-bit
    * Arm 64-bit (aarch64-gnu)
    * Arm 32-bit (armv7-gnueabihf)
* MacOS
    * Arm 64-bit
    * x86 64-bit
* Windows
    * x86 64-bit

## Examples

```python title="python-example.py"
"""Example usage of the Python DV library.

This example demonstrates:
- Creating dimensioned variables
- Performing arithmetic operations with dimensional analysis
- Unit conversions
- Error handling for incompatible operations
"""

from dv import DV, DVError


def main():
    print("=== Python DV Example ===\n")

    # Example 1: Basic construction and value access
    print("1. Creating dimensioned variables:")
    velocity = DV(10.0, "m/s")
    print(f"   velocity = {velocity.value()} m/s (SI base units)")
    print(f"   velocity = {velocity.value_in('km/hr'):.2f} km/hr\n")

    # Example 2: Arithmetic operations
    print("2. Arithmetic with dimensional analysis:")
    time = DV(2.0, "s")
    distance = velocity * time
    print(f"   velocity = {velocity.value_in('m/s')} m/s")
    print(f"   time = {time.value_in('s')} s")
    print(f"   distance = velocity * time = {distance.value_in('m')} m\n")

    # Example 3: Unit conversions
    print("3. Unit conversions:")
    distance_km = distance.value_in("km")
    distance_ft = distance.value_in("ft")
    print(f"   distance = {distance.value_in('m')} m")
    print(f"   distance = {distance_km} km")
    print(f"   distance = {distance_ft:.2f} ft\n")

    # Example 4: Force calculation
    print("4. Calculating force (F = m * a):")
    mass = DV(10.0, "kg")
    accel = DV(9.81, "m/s^2")
    force = mass * accel
    print(f"   mass = {mass.value_in('kg')} kg")
    print(f"   acceleration = {accel.value_in('m/s^2')} m/s²")
    print(f"   force = {force.value_in('N'):.2f} N")
    print(f"   force = {force.value_in('lbf'):.2f} lbf\n")

    # Example 5: Energy calculation
    print("5. Calculating kinetic energy (KE = 0.5 * m * v²):")
    m = DV(2.0, "kg")
    v = DV(10.0, "m/s")
    ke = 0.5 * m * (v ** 2)
    print(f"   mass = {m.value_in('kg')} kg")
    print(f"   velocity = {v.value_in('m/s')} m/s")
    print(f"   kinetic energy = {ke.value_in('J'):.2f} J")
    print(f"   kinetic energy = {ke.value_in('kJ'):.4f} kJ\n")

    # Example 6: Power operations
    print("6. Power operations:")
    area = DV(4.0, "m^2")
    side = area.sqrt()
    print(f"   area = {area.value_in('m^2')} m²")
    print(f"   side = sqrt(area) = {side.value_in('m')} m")
    
    volume = side ** 3
    print(f"   volume = side³ = {volume.value_in('m^3')} m³\n")

    # Example 7: Comparison operations
    print("7. Comparing values:")
    a = DV(100.0, "m")
    b = DV(0.1, "km")
    print(f"   a = {a.value_in('m')} m")
    print(f"   b = {b.value_in('m')} m")
    print(f"   a == b: {a == b}")
    print(f"   a > b: {a > b}\n")

    # Example 8: Error handling
    print("8. Error handling - incompatible operations:")
    try:
        length = DV(10.0, "m")
        time_val = DV(5.0, "s")
        # This should fail - can't add length and time
        result = length + time_val
    except DVError as e:
        print(f"   Caught expected error: {e}\n")

    # Example 9: Math functions (requires unitless)
    print("9. Mathematical functions (unitless only):")
    ratio = DV(100.0, "m") / DV(10.0, "m")  # Results in unitless value
    print(f"   ratio = 100m / 10m = {ratio.value()}")
    print(f"   is_unitless: {ratio.is_unitless()}")
    log_val = ratio.log10()
    print(f"   log10(ratio) = {log_val.value()}\n")

    # Example 10: Base units inspection
    print("10. Inspecting base units:")
    accel = DV(9.81, "m/s^2")
    base = accel.base_units()
    print(f"   acceleration = {accel.value_in('m/s^2')} m/s²")
    print(f"   base units (m, kg, s, K, A, mol, cd): {base}")
    print(f"   interpretation: m^{base[0]} * s^{base[2]}\n")

    print("=== Example Complete ===")


if __name__ == "__main__":
    main()
```
