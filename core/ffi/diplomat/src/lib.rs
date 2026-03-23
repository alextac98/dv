#![allow(clippy::needless_return)]

#[diplomat::bridge]
pub mod ffi {
    use core::fmt::Write as _;
    use diplomat_runtime::DiplomatWrite;
    use dv_rs::DimensionalVariable as CoreDV;
    use dv_rs::units;

    fn into_boxed_dv(dv: CoreDV) -> Box<DimensionalVariable> {
        Box::new(DimensionalVariable(dv))
    }

    fn clone_core(dv: &CoreDV) -> CoreDV {
        CoreDV {
            value: dv.value,
            unit: dv.unit,
        }
    }

    #[diplomat::rust_link(dv_rs::units::BASE_UNITS_SIZE, Mod)]
    pub struct BaseUnits {
        pub m: f64,
        pub kg: f64,
        pub s: f64,
        pub k: f64,
        pub a: f64,
        pub mol: f64,
        pub cd: f64,
        pub rad: f64,
    }

    #[diplomat::opaque]
    pub struct DimensionalVariable(CoreDV);

    impl DimensionalVariable {
        pub fn new(value: f64, unit: &str) -> Result<Box<DimensionalVariable>, ()> {
            match CoreDV::new(value, unit) {
                Ok(dv) => Ok(Box::new(DimensionalVariable(dv))),
                Err(_) => Err(()),
            }
        }

        pub fn asin_scalar(x: f64) -> Result<Box<DimensionalVariable>, ()> {
            dv_rs::asin(x).map(into_boxed_dv).map_err(|_| ())
        }

        pub fn acos_scalar(x: f64) -> Result<Box<DimensionalVariable>, ()> {
            dv_rs::acos(x).map(into_boxed_dv).map_err(|_| ())
        }

        pub fn atan_scalar(x: f64) -> Result<Box<DimensionalVariable>, ()> {
            dv_rs::atan(x).map(into_boxed_dv).map_err(|_| ())
        }

        pub fn base_units_size() -> usize {
            units::BASE_UNITS_SIZE
        }

        pub fn value(&self) -> f64 {
            self.0.value()
        }

        pub fn value_in(&self, unit: &str) -> Result<f64, ()> {
            self.0.value_in(unit).map_err(|_| ())
        }

        pub fn is_unitless(&self) -> bool {
            self.0.is_unitless()
        }

        pub fn base_units(&self) -> BaseUnits {
            let u = self.0.unit();
            BaseUnits {
                m: u[0],
                kg: u[1],
                s: u[2],
                k: u[3],
                a: u[4],
                mol: u[5],
                cd: u[6],
                rad: u[7],
            }
        }

        pub fn add(&self, other: &DimensionalVariable) -> Result<Box<DimensionalVariable>, ()> {
            self.0.try_add(&other.0).map(into_boxed_dv).map_err(|_| ())
        }

        pub fn sub(&self, other: &DimensionalVariable) -> Result<Box<DimensionalVariable>, ()> {
            self.0.try_sub(&other.0).map(into_boxed_dv).map_err(|_| ())
        }

        pub fn mul(&self, other: &DimensionalVariable) -> Box<DimensionalVariable> {
            into_boxed_dv(&self.0 * &other.0)
        }

        pub fn div(&self, other: &DimensionalVariable) -> Box<DimensionalVariable> {
            into_boxed_dv(&self.0 / &other.0)
        }

        pub fn mul_scalar(&self, scalar: f64) -> Box<DimensionalVariable> {
            into_boxed_dv(&self.0 * scalar)
        }

        pub fn div_scalar(&self, scalar: f64) -> Box<DimensionalVariable> {
            into_boxed_dv(&self.0 / scalar)
        }

        pub fn rdiv_scalar(&self, scalar: f64) -> Box<DimensionalVariable> {
            into_boxed_dv(scalar / &self.0)
        }

        pub fn powi(&self, exp: i32) -> Box<DimensionalVariable> {
            into_boxed_dv(self.0.powi(exp))
        }

        pub fn powf(&self, exp: f64) -> Result<Box<DimensionalVariable>, ()> {
            self.0.powf(exp).map(into_boxed_dv).map_err(|_| ())
        }

        pub fn sqrt(&self) -> Result<Box<DimensionalVariable>, ()> {
            self.0.sqrt().map(into_boxed_dv).map_err(|_| ())
        }

        pub fn ln(&self) -> Result<Box<DimensionalVariable>, ()> {
            self.0
                .ln()
                .map(|v| CoreDV { value: v, unit: [0.0; units::BASE_UNITS_SIZE] })
                .map(into_boxed_dv)
                .map_err(|_| ())
        }

        pub fn log2(&self) -> Result<Box<DimensionalVariable>, ()> {
            self.0
                .log2()
                .map(|v| CoreDV { value: v, unit: [0.0; units::BASE_UNITS_SIZE] })
                .map(into_boxed_dv)
                .map_err(|_| ())
        }

        pub fn log10(&self) -> Result<Box<DimensionalVariable>, ()> {
            self.0
                .log10()
                .map(|v| CoreDV { value: v, unit: [0.0; units::BASE_UNITS_SIZE] })
                .map(into_boxed_dv)
                .map_err(|_| ())
        }

        pub fn sin(&self) -> Result<Box<DimensionalVariable>, ()> {
            self.0
                .sin()
                .map(|v| CoreDV { value: v, unit: [0.0; units::BASE_UNITS_SIZE] })
                .map(into_boxed_dv)
                .map_err(|_| ())
        }

        pub fn cos(&self) -> Result<Box<DimensionalVariable>, ()> {
            self.0
                .cos()
                .map(|v| CoreDV { value: v, unit: [0.0; units::BASE_UNITS_SIZE] })
                .map(into_boxed_dv)
                .map_err(|_| ())
        }

        pub fn tan(&self) -> Result<Box<DimensionalVariable>, ()> {
            self.0
                .tan()
                .map(|v| CoreDV { value: v, unit: [0.0; units::BASE_UNITS_SIZE] })
                .map(into_boxed_dv)
                .map_err(|_| ())
        }

        pub fn asin(&self) -> Result<Box<DimensionalVariable>, ()> {
            self.0.asin().map(into_boxed_dv).map_err(|_| ())
        }

        pub fn acos(&self) -> Result<Box<DimensionalVariable>, ()> {
            self.0.acos().map(into_boxed_dv).map_err(|_| ())
        }

        pub fn atan(&self) -> Result<Box<DimensionalVariable>, ()> {
            self.0.atan().map(into_boxed_dv).map_err(|_| ())
        }

        pub fn neg(&self) -> Box<DimensionalVariable> {
            into_boxed_dv(self.0.neg())
        }

        pub fn abs(&self) -> Box<DimensionalVariable> {
            into_boxed_dv(self.0.abs())
        }

        pub fn equals(&self, other: &DimensionalVariable) -> bool {
            self.0 == other.0
        }

        pub fn not_equals(&self, other: &DimensionalVariable) -> bool {
            self.0 != other.0
        }

        pub fn less_than(&self, other: &DimensionalVariable) -> Result<bool, ()> {
            self.0.partial_cmp(&other.0).map(|o| o.is_lt()).ok_or(())
        }

        pub fn less_equal(&self, other: &DimensionalVariable) -> Result<bool, ()> {
            self.0.partial_cmp(&other.0).map(|o| o.is_lt() || o.is_eq()).ok_or(())
        }

        pub fn greater_than(&self, other: &DimensionalVariable) -> Result<bool, ()> {
            self.0.partial_cmp(&other.0).map(|o| o.is_gt()).ok_or(())
        }

        pub fn greater_equal(&self, other: &DimensionalVariable) -> Result<bool, ()> {
            self.0.partial_cmp(&other.0).map(|o| o.is_gt() || o.is_eq()).ok_or(())
        }

        pub fn clone_var(&self) -> Box<DimensionalVariable> {
            into_boxed_dv(clone_core(&self.0))
        }

        pub fn to_string(&self, to: &mut DiplomatWrite) -> Result<(), ()> {
            write!(to, "{}", self.0).map_err(|_| ())
        }
    }
}
