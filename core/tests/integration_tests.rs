
use dv_rs::DimensionalVariable as dv;

#[test]
fn nominal_value_and_unit() {
    let dv = dv::new(4.6483, "m").expect("Failed to create DimensionalVariable");
    assert_eq!(dv.value(), 4.6483, "Expected value to be 4.6483");
}

// TODO: Make this test work
// #[test]
// fn complex_arithmetic() {
//     // This test checks the arithmetic operations on DimensionalVariable using the conduction equation

//     let fail_msg = "Failed to create DimensionalVariable";

//     let k = DimensionalVariable::new(4.0, "BTU-in/hr-ft^2-F".to_string()).expect(fail_msg);
//     let L = DimensionalVariable::new(5.0, "mmm".to_string()).expect(fail_msg);
//     let A = DimensionalVariable::new(10.0, "cm^2".to_string()).expect(fail_msg);
//     let DT = DimensionalVariable::new(200.0, "K".to_string()).expect(fail_msg);

//     let Q = k * A / L * DT;

//     assert_eq!(Q.value(), 4000.0, "Expected Q value to be 4000.0");
//     assert_eq!(Q.unit(), [1, 0, -1, 1, 0, 0, 0], "Expected unit to be Watts");
// }

#[test]
fn errors_on_unknown_unit() {
    let result = dv::new(1.0, "unknown_unit");
    assert!(result.is_err(), "Expected error on unknown unit");
}
