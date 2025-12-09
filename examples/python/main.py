"""Example usage of the Python DV library.

This example demonstrates:
- Creating dimensioned variables
- Performing arithmetic operations with dimensional analysis
- Unit conversions
- Error handling for incompatible operations
- Working with angles (radians and degrees)
"""

from dv_py import DimensionalVariable, DVError, asin, acos, atan


def main():
    print("=== Python DV Example ===\n")

    # Example 1: Basic construction and value access
    print("1. Creating dimensioned variables:")
    velocity = DimensionalVariable(10.0, "m/s")
    print(f"   velocity = {velocity.value()} m/s (SI base units)")
    print(f"   velocity = {velocity.value_in('km/hr'):.2f} km/hr\n")

    # Example 2: Arithmetic operations
    print("2. Arithmetic with dimensional analysis:")
    time = DimensionalVariable(2.0, "s")
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
    mass = DimensionalVariable(10.0, "kg")
    accel = DimensionalVariable(9.81, "m/s^2")
    force = mass * accel
    print(f"   mass = {mass.value_in('kg')} kg")
    print(f"   acceleration = {accel.value_in('m/s^2')} m/s²")
    print(f"   force = {force.value_in('N'):.2f} N")
    print(f"   force = {force.value_in('lbf'):.2f} lbf\n")

    # Example 5: Energy calculation
    print("5. Calculating kinetic energy (KE = 0.5 * m * v²):")
    m = DimensionalVariable(2.0, "kg")
    v = DimensionalVariable(10.0, "m/s")
    ke = 0.5 * m * (v ** 2)
    print(f"   mass = {m.value_in('kg')} kg")
    print(f"   velocity = {v.value_in('m/s')} m/s")
    print(f"   kinetic energy = {ke.value_in('J'):.2f} J")
    print(f"   kinetic energy = {ke.value_in('kJ'):.4f} kJ\n")

    # Example 6: Power operations
    print("6. Power operations:")
    area = DimensionalVariable(4.0, "m^2")
    side = area.sqrt()
    print(f"   area = {area.value_in('m^2')} m²")
    print(f"   side = sqrt(area) = {side.value_in('m')} m")
    
    volume = side ** 3
    print(f"   volume = side³ = {volume.value_in('m^3')} m³\n")

    # Example 7: Comparison operations
    print("7. Comparing values:")
    a = DimensionalVariable(100.0, "m")
    b = DimensionalVariable(0.1, "km")
    print(f"   a = {a.value_in('m')} m")
    print(f"   b = {b.value_in('m')} m")
    print(f"   a == b: {a == b}")
    print(f"   a > b: {a > b}\n")

    # Example 8: Error handling
    print("8. Error handling - incompatible operations:")
    try:
        length = DimensionalVariable(10.0, "m")
        time_val = DimensionalVariable(5.0, "s")
        # This should fail - can't add length and time
        result = length + time_val
    except DVError as e:
        print(f"   Caught expected error: {e}\n")

    # Example 9: Math functions (requires unitless)
    print("9. Mathematical functions (unitless only):")
    ratio = DimensionalVariable(100.0, "m") / DimensionalVariable(10.0, "m")  # Results in unitless value
    print(f"   ratio = 100m / 10m = {ratio.value()}")
    print(f"   is_unitless: {ratio.is_unitless()}")
    log_val = ratio.log10()
    print(f"   log10(ratio) = {log_val.value()}\n")

    # Example 10: Base units inspection
    print("10. Inspecting base units:")
    accel = DimensionalVariable(9.81, "m/s^2")
    base = accel.base_units()
    print(f"   acceleration = {accel.value_in('m/s^2')} m/s²")
    print(f"   base units (m, kg, s, K, A, mol, cd, rad): {base}")
    print(f"   interpretation: m^{base[0]} * s^{base[2]}\n")

    # Example 11: Working with angles
    print("11. Working with angles:")
    import math
    angle_rad = DimensionalVariable(math.pi, "rad")
    angle_deg = DimensionalVariable(180.0, "deg")
    print(f"   π radians = {angle_rad.value_in('rad'):.4f} rad")
    print(f"   180 degrees = {angle_deg.value_in('deg')} deg")
    print(f"   180 degrees = {angle_deg.value_in('rad'):.4f} rad")
    print(f"   π radians = {angle_rad.value_in('deg'):.2f} deg")
    
    # Trigonometric functions with radians
    angle = DimensionalVariable(math.pi / 4, "rad")
    print(f"   sin(π/4) = {angle.sin().value():.4f}")
    print(f"   cos(π/4) = {angle.cos().value():.4f}")
    
    # Free-standing inverse trig functions
    print(f"   asin(0.5) = {asin(0.5).value_in('rad'):.4f} rad")
    print(f"   acos(0.5) = {acos(0.5).value_in('deg'):.2f} deg")
    print(f"   atan(1.0) = {atan(1.0).value_in('deg'):.2f} deg (should be 45)\n")

    print("=== Example Complete ===")


if __name__ == "__main__":
    main()
