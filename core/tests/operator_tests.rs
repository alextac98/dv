use dv_rs::DimensionalVariable as dv;

const FAIL_MSG: &str = "Failed to create DimensionalVariable";

#[test]
fn add_sub_with_same_units() {
    let a = dv::new(3.5, "m").expect(FAIL_MSG);
    let b = dv::new(1.5, "m").expect(FAIL_MSG);

    let c = &a + &b;
    assert_eq!(c.value(), 5.0);
    assert_eq!(c.unit(), [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);

    let d = &a - &b;
    assert_eq!(d.value(), 2.0);
    assert_eq!(d.unit(), [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);

    // Owned variants delegate to reference implementations
    let e = a + b;
    assert_eq!(e.value(), 5.0);
    assert_eq!(e.unit(), [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);
}

#[test]
fn try_add_sub_incompatible_units() {
    let m = dv::new(1.0, "m").expect(FAIL_MSG);
    let s = dv::new(1.0, "s").expect(FAIL_MSG);

    assert!(m.try_add(&s).is_err());
    assert!(m.try_sub(&s).is_err());
}

#[test]
#[should_panic]
fn add_incompatible_units_panics() {
    let m = dv::new(1.0, "m").expect(FAIL_MSG);
    let s = dv::new(1.0, "s").expect(FAIL_MSG);
    let _ = m + s; // should panic due to incompatible units
}

#[test]
fn mul_div_unit_exponents() {
    // (m/s) * s = m
    let v = dv::new(3.0, "m/s").expect(FAIL_MSG);
    let t = dv::new(2.0, "s").expect(FAIL_MSG);
    let d = &v * &t;
    assert_eq!(d.value(), 6.0);
    assert_eq!(d.unit(), [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);

    // N * m = J (exponents [2, 1, -2])
    let f = dv::new(5.0, "N").expect(FAIL_MSG);
    let x = dv::new(2.0, "m").expect(FAIL_MSG);
    let w = &f * &x;
    assert_eq!(w.value(), 10.0);
    assert_eq!(w.unit(), [2.0, 1.0, -2.0, 0.0, 0.0, 0.0, 0.0]);

    // (m^2/s) / m = m/s
    let a = dv::new(8.0, "m^2/s").expect(FAIL_MSG);
    let m1 = dv::new(2.0, "m").expect(FAIL_MSG);
    let r = &a / &m1;
    assert_eq!(r.value(), 4.0);
    assert_eq!(r.unit(), [1.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0]);
}

#[test]
fn scalar_ops() {
    let m = dv::new(5.0, "m").expect(FAIL_MSG);

    let a = &m * 2.0;
    assert_eq!(a.value(), 10.0);
    assert_eq!(a.unit(), [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);

    let b = 2.0 * &m;
    assert_eq!(b.value(), 10.0);
    assert_eq!(b.unit(), [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);

    let c = &m / 2.0;
    assert_eq!(c.value(), 2.5);
    assert_eq!(c.unit(), [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);

    // 2 / (2 1/s) = 1 s
    let inv = dv::new(2.0, "s^-1").expect(FAIL_MSG);
    let d = 2.0 / &inv;
    assert_eq!(d.value(), 1.0);
    assert_eq!(d.unit(), [0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0]);
}

#[test]
fn assignment_ops() {
    let mut a = dv::new(3.0, "m").expect(FAIL_MSG);
    let b = dv::new(2.0, "m").expect(FAIL_MSG);

    a += &b; // 5 m
    assert_eq!(a.value(), 5.0);
    assert_eq!(a.unit(), [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);

    a -= &b; // 3 m
    assert_eq!(a.value(), 3.0);
    assert_eq!(a.unit(), [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);

    let s_inv = dv::new(2.0, "s^-1").expect(FAIL_MSG);
    a *= &s_inv; // 6 m/s
    assert_eq!(a.value(), 6.0);
    assert_eq!(a.unit(), [1.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0]);

    let s_inv2 = dv::new(3.0, "s^-1").expect(FAIL_MSG);
    a /= &s_inv2; // 2 m
    assert_eq!(a.value(), 2.0);
    assert_eq!(a.unit(), [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);
}

#[test]
fn negation_and_unitless() {
    let m = dv::new(5.0, "m").expect(FAIL_MSG);
    let n = -&m;
    assert_eq!(n.value(), -5.0);
    assert_eq!(n.unit(), [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);

    // Ratio of same units is unitless
    let a = dv::new(10.0, "m").expect(FAIL_MSG);
    let b = dv::new(2.0, "m").expect(FAIL_MSG);
    let r = &a / &b;
    assert!(r.is_unitless());
    assert_eq!(r.value(), 5.0);
}

#[test]
fn mixed_owned_and_borrowed_forms() {
    let a = dv::new(2.0, "m").expect(FAIL_MSG);
    let b = dv::new(3.0, "m").expect(FAIL_MSG);

    // & + &
    let r1 = &a + &b;
    // owned + &
    let r2 = a + &b;
    // & + owned
    let a2 = dv::new(2.0, "m").expect(FAIL_MSG);
    let b2 = dv::new(3.0, "m").expect(FAIL_MSG);
    let r3 = &a2 + b2;

    assert_eq!(r1.value(), 5.0);
    assert_eq!(r2.value(), 5.0);
    assert_eq!(r3.value(), 5.0);
    assert_eq!(r1.unit(), [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);
}

#[test]
fn complex_arithmetic() {
    // This test checks the arithmetic operations on DimensionalVariable using the conduction equation

    let k = dv::new(4.0, "BTU-in/hr-ft^2-F").expect(FAIL_MSG);
    let l = dv::new(5.0, "mm").expect(FAIL_MSG);
    let a = dv::new(10.0, "cm^2").expect(FAIL_MSG);
    let dt = dv::new(200.0, "K").expect(FAIL_MSG);

    let q = k * a * dt/ l;

    assert_eq!(q.value(), 23.076462160979872, "Incorrect value for q!");
    assert_eq!(q.unit(), [2.0, 1.0, -3.0, 0.0, 0.0, 0.0, 0.0], "Expected unit to be Watts!");
}

#[test]
fn mixed_owned_and_borrowed_forms_sub_mul_div() {
    // Subtraction
    let a = dv::new(5.0, "m").expect(FAIL_MSG);
    let b = dv::new(3.0, "m").expect(FAIL_MSG);
    let s1 = &a - &b; // & - &
    assert_eq!(s1.value(), 2.0);
    assert_eq!(s1.unit(), [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);

    let s2 = a - &b; // owned - &
    assert_eq!(s2.value(), 2.0);
    assert_eq!(s2.unit(), [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);

    let a2 = dv::new(5.0, "m").expect(FAIL_MSG);
    let b2 = dv::new(3.0, "m").expect(FAIL_MSG);
    let s3 = &a2 - b2; // & - owned
    assert_eq!(s3.value(), 2.0);
    assert_eq!(s3.unit(), [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);

    // Multiplication
    let m1 = dv::new(2.0, "m").expect(FAIL_MSG);
    let s = dv::new(4.0, "s").expect(FAIL_MSG);
    let p1 = &m1 * &s; // & * &
    assert_eq!(p1.value(), 8.0);
    assert_eq!(p1.unit(), [1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0]);

    let m2 = dv::new(2.0, "m").expect(FAIL_MSG);
    let p2 = m2 * &dv::new(4.0, "s").expect(FAIL_MSG); // owned * &
    assert_eq!(p2.value(), 8.0);
    assert_eq!(p2.unit(), [1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0]);

    let m3 = dv::new(2.0, "m").expect(FAIL_MSG);
    let s2 = dv::new(4.0, "s").expect(FAIL_MSG);
    let p3 = &m3 * s2; // & * owned
    assert_eq!(p3.value(), 8.0);
    assert_eq!(p3.unit(), [1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0]);

    // Division
    let num1 = dv::new(8.0, "m^2-s").expect(FAIL_MSG);
    let den1 = dv::new(2.0, "s").expect(FAIL_MSG);
    let d1 = &num1 / &den1; // & / &
    assert_eq!(d1.value(), 4.0);
    assert_eq!(d1.unit(), [2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);

    let num2 = dv::new(8.0, "m^2-s").expect(FAIL_MSG);
    let den2 = dv::new(2.0, "s").expect(FAIL_MSG);
    let d2 = num2 / &den2; // owned / &
    assert_eq!(d2.value(), 4.0);
    assert_eq!(d2.unit(), [2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);

    let num3 = dv::new(8.0, "m^2-s").expect(FAIL_MSG);
    let den3 = dv::new(2.0, "s").expect(FAIL_MSG);
    let d3 = &num3 / den3; // & / owned
    assert_eq!(d3.value(), 4.0);
    assert_eq!(d3.unit(), [2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);
}

#[test]
fn scalar_owned_variants_and_assign() {
    // Owned * f64
    let m = dv::new(3.0, "m").expect(FAIL_MSG);
    let r1 = m * 2.0;
    assert_eq!(r1.value(), 6.0);
    assert_eq!(r1.unit(), [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);

    // Owned / f64
    let m2 = dv::new(3.0, "m").expect(FAIL_MSG);
    let r2 = m2 / 2.0;
    assert_eq!(r2.value(), 1.5);
    assert_eq!(r2.unit(), [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);

    // f64 * owned
    let m3 = dv::new(3.0, "m").expect(FAIL_MSG);
    let r3 = 2.0 * m3;
    assert_eq!(r3.value(), 6.0);
    assert_eq!(r3.unit(), [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);

    // f64 / owned (inverse units)
    let inv = dv::new(2.0, "s^-1").expect(FAIL_MSG);
    let r4 = 2.0 / inv;
    assert_eq!(r4.value(), 1.0);
    assert_eq!(r4.unit(), [0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0]);

    // Compound assignments with f64
    let mut kg = dv::new(5.0, "kg").expect(FAIL_MSG);
    kg *= 2.0;
    assert_eq!(kg.value(), 10.0);
    assert_eq!(kg.unit(), [0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0]);

    kg /= 4.0;
    assert_eq!(kg.value(), 2.5);
    assert_eq!(kg.unit(), [0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0]);
}

#[test]
fn assignment_ops_owned_rhs() {
    let mut a = dv::new(3.0, "m").expect(FAIL_MSG);
    let b = dv::new(2.0, "m").expect(FAIL_MSG);

    a += &b; // RHS by reference
    assert_eq!(a.value(), 5.0);
    assert_eq!(a.unit(), [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);

    let s_inv = dv::new(2.0, "s^-1").expect(FAIL_MSG);
    a *= &s_inv; // 10 m/s
    assert_eq!(a.value(), 10.0);
    assert_eq!(a.unit(), [1.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0]);

    let s_inv2 = dv::new(2.0, "s^-1").expect(FAIL_MSG);
    a /= &s_inv2; // back to 5 m
    assert_eq!(a.value(), 5.0);
    assert_eq!(a.unit(), [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);

    let one_m = dv::new(1.0, "m").expect(FAIL_MSG);
    a -= &one_m; // 4 m
    assert_eq!(a.value(), 4.0);
    assert_eq!(a.unit(), [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);
}

#[test]
fn negation_owned() {
    let m = dv::new(5.0, "m").expect(FAIL_MSG);
    let n = -m;
    assert_eq!(n.value(), -5.0);
    assert_eq!(n.unit(), [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);
}

#[test]
fn comparisons_equality_and_ordering() {
    // Equality with same units
    let a1 = dv::new(2.0, "m").expect(FAIL_MSG);
    let a2 = dv::new(2.0, "m").expect(FAIL_MSG);
    assert!(a1 == a2);

    // Ordering with same units
    let b1 = dv::new(2.0, "m").expect(FAIL_MSG);
    let b2 = dv::new(3.0, "m").expect(FAIL_MSG);
    assert!(b1 < b2);
    assert!(b2 > b1);
    assert!(b2 >= b1);
    assert!(b1 <= b2);

    // Mismatched units: not equal and no ordering
    let c1 = dv::new(1.0, "m").expect(FAIL_MSG);
    let c2 = dv::new(1.0, "s").expect(FAIL_MSG);
    assert!(c1 != c2);
    assert!(c1.partial_cmp(&c2).is_none());
    assert!(!(c1 < c2));
    assert!(!(c1 > c2));
    assert!(!(c1 <= c2));
    assert!(!(c1 >= c2));
}

#[test]
fn powi_and_sqrt() {
    // powi on dimensional values multiplies exponents
    let a = dv::new(2.0, "m/s^2").expect(FAIL_MSG);
    let p = a.powi(3);
    assert!((p.value() - 8.0).abs() < 1e-12);
    assert_eq!(p.unit(), [3.0, 0.0, -6.0, 0.0, 0.0, 0.0, 0.0]);

    // sqrt of even exponents works
    let b = dv::new(9.0, "m^2/s^2").expect(FAIL_MSG);
    let r = b.sqrt().expect(FAIL_MSG);
    assert!((r.value() - 3.0).abs() < 1e-12);
    assert_eq!(r.unit(), [1.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0]);

    // sqrt now allows fractional exponents
    let c = dv::new(4.0, "m^3").expect(FAIL_MSG);
    let sr = c.sqrt().expect(FAIL_MSG);
    assert!((sr.value() - 2.0).abs() < 1e-12);
    assert_eq!(sr.unit(), [1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);

    // sqrt error: negative value
    let d = dv::new(-1.0, "m^2").expect(FAIL_MSG);
    assert!(d.sqrt().is_err());
}

#[test]
fn powf_logs_trig_require_unitless() {
    use std::f64::consts::PI;

    // Construct a unitless value via ratio
    let x = dv::new(10.0, "m").expect(FAIL_MSG);
    let y = dv::new(2.0, "m").expect(FAIL_MSG);
    let u = &x / &y; // value = 5, unitless
    assert!(u.is_unitless());

    // powf works for dimensional too (exponents become fractional)
    let p = u.powf(2.5).expect(FAIL_MSG);
    assert!((p.value() - 5f64.powf(2.5)).abs() < 1e-12);
    assert!(p.is_unitless());

    // logs on unitless positive
    assert!((u.ln().unwrap() - 5f64.ln()).abs() < 1e-12);
    assert!((u.log2().unwrap() - 5f64.log2()).abs() < 1e-12);
    assert!((u.log10().unwrap() - 5f64.log10()).abs() < 1e-12);

    // trig on unitless
    let angle = dv::new(PI / 4.0, "rad").expect(FAIL_MSG); // rad is dimensionless in our base
    // Ensure rad is treated as unitless in base representation
    assert!(angle.is_unitless());
    assert!((angle.sin().unwrap() - (PI / 4.0).sin()).abs() < 1e-12);
    assert!((angle.cos().unwrap() - (PI / 4.0).cos()).abs() < 1e-12);
    assert!((angle.tan().unwrap() - (PI / 4.0).tan()).abs() < 1e-12);

    // Errors for dimensional logs/trig
    let m = dv::new(2.0, "m").expect(FAIL_MSG);
    assert!(m.ln().is_err());
    assert!(m.log2().is_err());
    assert!(m.log10().is_err());
    assert!(m.sin().is_err());
    assert!(m.cos().is_err());
    assert!(m.tan().is_err());

    // Log domain errors
    let zero = dv::new(0.0, "").expect(FAIL_MSG); // unitless zero
    assert!(zero.ln().is_err());
    assert!(zero.log2().is_err());
    assert!(zero.log10().is_err());
}

#[test]
fn comparisons() {
    // Same physical dimension but provided in different units are equal after conversion
    let a = dv::new(1.0, "m").expect(FAIL_MSG);
    let b = dv::new(100.0, "cm").expect(FAIL_MSG);
    assert!(a == b);
    assert!(!(a != b));
    assert!(!(a < b));
    assert!(a <= b);
    assert!(!(a > b));
    assert!(a >= b);

    // Different units (incompatible dimensions) -> comparisons are None/false
    let m = dv::new(1.0, "m").expect(FAIL_MSG);
    let s = dv::new(1.0, "s").expect(FAIL_MSG);
    assert!(!(m < s));
    assert!(!(m <= s));
    assert!(!(m > s));
    assert!(!(m >= s));
    assert!(!(m == s));
    assert!(m != s);
    assert!(PartialOrd::partial_cmp(&m, &s).is_none());

    // Ordering within same units
    let c = dv::new(2.0, "m").expect(FAIL_MSG);
    let d = dv::new(3.0, "m").expect(FAIL_MSG);
    assert!(c < d);
    assert!(d > c);
}

#[test]
fn absolute_value() {
    let m = dv::new(-5.0, "m").expect(FAIL_MSG);
    let a = m.abs();
    assert_eq!(a.value(), 5.0);
    assert_eq!(a.unit(), [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]);
}