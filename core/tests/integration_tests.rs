
use dv_rs::DimensionalVariable as dv;

const FAIL_MSG: &str = "Failed to create DimensionalVariable";

#[test]
fn nominal_value_and_unit() {
    let dv = dv::new(4.6483, "m").expect(FAIL_MSG);
    assert_eq!(dv.value(), 4.6483, "Expected value to be 4.6483");
}

#[test]
fn unit_str_to_unit_simple_simple() {
    let dv = dv::new(4.6483, "m/s").expect(FAIL_MSG);
    assert_eq!(dv.value(), 4.6483, "Expected value to be 4.6483");
    assert_eq!(dv.unit(), [1, 0, -1, 0, 0, 0, 0], "Expected unit to be [1, 0, -1, 0, 0, 0, 0]");

    let dv = dv::new(1.0, "mi/h").expect(FAIL_MSG);
    assert_eq!(dv.value(), 0.44704, "Incorrect conversion for mi/h");
    assert_eq!(dv.unit(), [1, 0, -1, 0, 0, 0, 0], "Incorrect unit for mi/h");
}

#[test]
fn unit_str_to_unit_simple_exponent() {
    let dv = dv::new(4.6483, "m/s^2").expect(FAIL_MSG);
    assert_eq!(dv.value(), 4.6483, "Expected value to be 4.6483");
    assert_eq!(dv.unit(), [1, 0, -2, 0, 0, 0, 0], "Expected unit to be [1, 0, -2, 0, 0, 0, 0]");

    let dv = dv::new(4.6483, "m2/s2").expect(FAIL_MSG);
    assert_eq!(dv.value(), 4.6483, "Expected value to be 4.6483");
    assert_eq!(dv.unit(), [2, 0, -2, 0, 0, 0, 0], "Expected unit to be [2, 0, -2, 0, 0, 0, 0]");

    let dv = dv::new(1.0, "m^2/s^-2").expect(FAIL_MSG);
    assert_eq!(dv.unit(), [2, 0, 2, 0, 0, 0, 0], "Incorrect unit for m^2/s^-2");
}

#[test]
fn unit_str_to_unit_complex() {
    let dv = dv::new(1.0, "BTU-in/h-ft^2-F").expect(FAIL_MSG);
    assert_eq!(dv.value(), 0.1442278885061242, "Incorrect conversion for BTU-in/h-ft^2-F");
    assert_eq!(dv.unit(), [1, 1, -3, -1, 0, 0, 0], "Incorrect unit for BTU-in/h-ft^2-F");
}

// TODO: Make this test work
// #[test]
// fn complex_arithmetic() {
//     // This test checks the arithmetic operations on DimensionalVariable using the conduction equation

//     let k = DimensionalVariable::new(4.0, "BTU-in/hr-ft^2-F".to_string()).expect(FAIL_MSG);
//     let L = DimensionalVariable::new(5.0, "mmm".to_string()).expect(FAIL_MSG);
//     let A = DimensionalVariable::new(10.0, "cm^2".to_string()).expect(FAIL_MSG);
//     let DT = DimensionalVariable::new(200.0, "K".to_string()).expect(FAIL_MSG);

//     let Q = k * A / L * DT;

//     assert_eq!(Q.value(), 4000.0, "Expected Q value to be 4000.0");
//     assert_eq!(Q.unit(), [1, 0, -1, 1, 0, 0, 0], "Expected unit to be Watts");
// }

#[test]
fn errors_on_unknown_unit() {
    let result = dv::new(1.0, "unknown_unit");
    assert!(result.is_err(), "Expected error on unknown unit");
}
