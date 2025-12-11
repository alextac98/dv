use dv_rs::DimensionalVariable as dv;

const FAIL_MSG: &str = "Failed to create DimensionalVariable";

#[test]
fn nominal_value_and_unit() {
    let dv = dv::new(4.6483, "m").expect(FAIL_MSG);
    assert_eq!(dv.value(), 4.6483, "Expected value to be 4.6483");
}

#[test]
fn angle_conversions() {
    use std::f64::consts::PI;
    
    // Test radian to degree conversion
    let angle_rad = dv::new(PI, "rad").expect(FAIL_MSG);
    assert!((angle_rad.value_in("deg").expect(FAIL_MSG) - 180.0).abs() < 1e-10);
    
    // Test degree to radian conversion
    let angle_deg = dv::new(180.0, "deg").expect(FAIL_MSG);
    assert!((angle_deg.value_in("rad").expect(FAIL_MSG) - PI).abs() < 1e-10);
    
    // Test that radians have proper base unit
    assert_eq!(angle_rad.unit(), [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0]);
    
    // Test milliradian
    let mrad = dv::new(1000.0, "mrad").expect(FAIL_MSG);
    assert!((mrad.value_in("rad").expect(FAIL_MSG) - 1.0).abs() < 1e-10);
    
    // Test arcminute and arcsecond
    let arcmin = dv::new(60.0, "arcmin").expect(FAIL_MSG);
    assert!((arcmin.value_in("deg").expect(FAIL_MSG) - 1.0).abs() < 1e-10);
    
    let arcsec = dv::new(3600.0, "arcsec").expect(FAIL_MSG);
    assert!((arcsec.value_in("deg").expect(FAIL_MSG) - 1.0).abs() < 1e-10);
}

#[test]
fn angle_to_linear_conversions() {
    let torque = dv::new(10.0, "N-m").expect(FAIL_MSG);
    let angular_speed = dv::new(2.0, "rad/s").expect(FAIL_MSG);

    let power = torque * angular_speed;

    // Power = Torque * Angular Speed
    assert_eq!(power.unit(), [2.0, 1.0, -3.0, 0.0, 0.0, 0.0, 0.0, 1.0], "Incorrect unit for angular power.");
    assert!((power.value_in("W").expect(FAIL_MSG) - 20.0).abs() < 1e-10);
}

#[test]
fn unit_str_to_unit_simple_simple() {
    let dv = dv::new(4.6483, "m/s").expect(FAIL_MSG);
    assert_eq!(dv.value(), 4.6483, "Expected value to be 4.6483");
    assert_eq!(dv.unit(), [1.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0], "Expected unit to be [1, 0, -1, 0, 0, 0, 0]");

    let dv = dv::new(1.0, "mi/hr").expect(FAIL_MSG);
    assert_eq!(dv.value(), 0.44704, "Incorrect conversion for mi/hr");
    assert_eq!(dv.unit(), [1.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0], "Incorrect unit for mi/hr");
}

#[test]
fn unit_str_to_unit_simple_exponent() {
    let dv = dv::new(4.6483, "m/s^2").expect(FAIL_MSG);
    assert_eq!(dv.value(), 4.6483, "Expected value to be 4.6483");
    assert_eq!(dv.unit(), [1.0, 0.0, -2.0, 0.0, 0.0, 0.0, 0.0, 0.0], "Incorrect unit for m/s^2");

    let dv = dv::new(4.6483, "mi/hr^2").expect(FAIL_MSG);
    assert_eq!(dv.value(), 0.0005772155644444445, "Incorrect conversion for mi/hr^2");
    assert_eq!(dv.unit(), [1.0, 0.0, -2.0, 0.0, 0.0, 0.0, 0.0, 0.0], "Incorrect unit for mi/hr^2");

    let dv = dv::new(1.0, "m^2/s^-2").expect(FAIL_MSG);
    assert_eq!(dv.unit(), [2.0, 0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0], "Incorrect unit for m^2/s^-2");
}

#[test]
fn unit_str_to_unit_complex() {
    let dv = dv::new(1.0, "BTU-in/hr-ft^2-F").expect(FAIL_MSG);
    assert_eq!(dv.value(), 0.1442278885061242, "Incorrect conversion for BTU-in/hr-ft^2-F");
    assert_eq!(dv.unit(), [1.0, 1.0, -3.0, -1.0, 0.0, 0.0, 0.0, 0.0], "Incorrect unit for BTU-in/hr-ft^2-F");
}

#[test]
fn unit_conversion() {
    let d = dv::new(1.0, "m").expect(FAIL_MSG);
    assert_eq!(d.value_in("cm").expect(FAIL_MSG), 100.0);

    let d = dv::new(1.0, "m/s").expect(FAIL_MSG);
    assert_eq!(d.value_in("mi/hr").expect(FAIL_MSG), 2.2369362920544025);
}

#[test]
fn bad_unit_conversion() {
    let d = dv::new(1.0, "m").expect(FAIL_MSG);
    let result = d.value_in("hr");
    assert!(result.is_err(), "Expected error on unknown unit conversion");

    let d = dv::new(1.0, "m/s").expect(FAIL_MSG);
    let result = d.value_in("btu");
    assert!(result.is_err(), "Expected error on unknown unit conversion");
}

#[test]
fn errors_on_unknown_unit() {
    let result = dv::new(1.0, "unknown_unit");
    assert!(result.is_err(), "Expected error on unknown unit");
}

#[test]
fn electrical_units() {
    let voltage = dv::new(10.0, "V").expect(FAIL_MSG);
    let current = dv::new(2.0, "A").expect(FAIL_MSG);
    let resistance = dv::new(5.0, "Ohm").expect(FAIL_MSG);

    assert!(voltage == &current * &resistance, "Ohm's law failed");

    let power = dv::new(20.0, "W").expect(FAIL_MSG);
    assert!(power == &voltage * &current, "Power calculation failed");
}

// ===== Display / to_string() tests =====

#[test]
fn to_string_simple_unit() {
    let length = dv::new(5.0, "m").expect(FAIL_MSG);
    assert_eq!(length.to_string(), "5 m");

    let mass = dv::new(10.0, "kg").expect(FAIL_MSG);
    assert_eq!(mass.to_string(), "10 kg");

    let time = dv::new(3.5, "s").expect(FAIL_MSG);
    assert_eq!(time.to_string(), "3.5 s");
}

#[test]
fn to_string_unitless() {
    let unitless = dv::new(42.0, "").expect(FAIL_MSG);
    assert_eq!(unitless.to_string(), "42 (unitless)");
}

#[test]
fn to_string_with_exponents() {
    let area = dv::new(100.0, "m^2").expect(FAIL_MSG);
    assert_eq!(area.to_string(), "100 m^2");

    let volume = dv::new(8.0, "m^3").expect(FAIL_MSG);
    assert_eq!(volume.to_string(), "8 m^3");
}

#[test]
fn to_string_with_denominator() {
    let velocity = dv::new(10.0, "m/s").expect(FAIL_MSG);
    assert_eq!(velocity.to_string(), "10 m/s");

    let acceleration = dv::new(9.81, "m/s^2").expect(FAIL_MSG);
    assert_eq!(acceleration.to_string(), "9.81 m/s^2");
}

#[test]
fn to_string_complex_units() {
    // Force: kg*m/s^2
    let force = dv::new(1.0, "N").expect(FAIL_MSG);
    assert_eq!(force.to_string(), "1 m*kg/s^2");

    // Energy: kg*m^2/s^2
    let energy = dv::new(100.0, "J").expect(FAIL_MSG);
    assert_eq!(energy.to_string(), "100 m^2*kg/s^2");

    // Power: kg*m^2/s^3
    let power = dv::new(50.0, "W").expect(FAIL_MSG);
    assert_eq!(power.to_string(), "50 m^2*kg/s^3");
}

#[test]
fn to_string_only_denominator() {
    // Create a frequency (1/s) by dividing unitless by time
    let one = dv::new(60.0, "").expect(FAIL_MSG);
    let time = dv::new(1.0, "s").expect(FAIL_MSG);
    let frequency = &one / &time;
    assert_eq!(frequency.to_string(), "60 1/s");

    // Also test via negative exponent
    let freq2 = dv::new(100.0, "s^-1").expect(FAIL_MSG);
    assert_eq!(freq2.to_string(), "100 1/s");
}

#[test]
fn to_string_angle() {
    use std::f64::consts::PI;
    let angle = dv::new(PI, "rad").expect(FAIL_MSG);
    assert!(angle.to_string().starts_with("3.14159"));
    assert!(angle.to_string().ends_with(" rad"));
}

#[test]
fn to_string_format_macro() {
    let dv_val = dv::new(2.5, "m/s").expect(FAIL_MSG);
    let formatted = format!("The velocity is {}", dv_val);
    assert_eq!(formatted, "The velocity is 2.5 m/s");
}
