#![crate_name = "dv_rs"]
#![doc = include_str!("../DOCS.md")]


pub mod units;

pub fn version() -> &'static str {
    "0.1.0"
}

pub struct DimensionalVariable {
    pub value: f64,
    pub unit: [i32; units::BASE_UNITS_SIZE],
}

/// A struct representing a dimensional variable with a value and a unit.
impl DimensionalVariable {
    /// Creates a new DimensionalVariable with the given value and unit.
    ///
    /// Rules for writing the unit string:
    /// - Use a single `/` as a delimiter between the numerator and denominator.
    /// - Use `-` as a delimiter between individual units
    /// - Exponents can be represented either by using `^` to indicate exponent (ex. `m^2`) or without the delimiter (ex. `m2`)
    /// - Inverses can be represented either by negative exponents or in the denominator (ex. `m^-2` or `1/m^2`)
    /// 
    /// Returns an error if the unit string is invalid or contains unknown units.
    pub fn new(value: f64, unit_str: &str) -> Result<Self, String> {
        // Fetch the unit details from the unit map
        let (base_unit, conversion_factor) = unit_str_to_base_unit(unit_str)
            .map_err(|e| format!("Failed to parse unit '{}': {}", unit_str, e))?;

        // Create the DimensionalVariable with the converted value
        return Ok(DimensionalVariable {
            value: value * conversion_factor,
            unit: base_unit,
        });
    }

    /// Returns the value of this DimensionalVariable.
    pub fn value(&self) -> f64 {
        self.value
    }

    /// Converts the value of this DimensionalVariable to the specified unit.
    /// Returns an error if the unit string is invalid or incompatible.
    pub fn value_in(&self, unit_str: &str) -> Result<f64, String> {

        let (unit, conversion_factor) = unit_str_to_base_unit(unit_str)
            .map_err(|e| format!("Failed to parse unit '{}': {}", unit_str, e))?;

        // Check if the units are compatible
        if self.unit != unit {
            return Err(format!("Incompatible unit conversion to: {}", unit_str));
        }
        
        return Ok(self.value / conversion_factor);
    }

    /// Returns the base unit array of this DimensionalVariable.
    pub fn unit(&self) -> [i32; units::BASE_UNITS_SIZE] {
        self.unit
    }

    /// Returns whether the variable is unitless (all base exponents are 0).
    pub fn is_unitless(&self) -> bool {
        self.unit.iter().all(|&e| e == 0)
    }

    /// Fallible add with unit compatibility check.
    pub fn try_add(&self, other: &DimensionalVariable) -> Result<DimensionalVariable, String> {
        if self.unit != other.unit {
            return Err("Incompatible units for addition".to_string());
        }
        Ok(DimensionalVariable { value: self.value + other.value, unit: self.unit })
    }

    /// Fallible subtraction with unit compatibility check.
    pub fn try_sub(&self, other: &DimensionalVariable) -> Result<DimensionalVariable, String> {
        if self.unit != other.unit {
            return Err("Incompatible units for subtraction".to_string());
        }
        Ok(DimensionalVariable { value: self.value - other.value, unit: self.unit })
    }

    // ---- Math: powers and roots ----
    /// Raise to integer power. Units exponents are multiplied by exp.
    /// Returns a new DimensionalVariable.
    pub fn powi(&self, exp: i32) -> DimensionalVariable {
        let mut unit = self.unit;
        for i in 0..units::BASE_UNITS_SIZE { unit[i] *= exp; }
        DimensionalVariable { value: self.value.powi(exp), unit }
    }

    /// Raise to floating power. Requires unitless.
    /// Returns a new DimensionalVariable.
    pub fn powf(&self, exp: f64) -> Result<DimensionalVariable, String> {
        if !self.is_unitless() {
            return Err("powf requires a unitless quantity".to_string());
        }
        Ok(DimensionalVariable { value: self.value.powf(exp), unit: [0;units::BASE_UNITS_SIZE] })
    }

    /// Square root. Allowed only when all unit exponents are even and value >= 0.
    /// Returns a new DimensionalVariable.
    pub fn sqrt(&self) -> Result<DimensionalVariable, String> {
        if self.value < 0.0 {
            return Err("sqrt of negative value".to_string());
        }
        for &e in &self.unit {
            if e % 2 != 0 { return Err("sqrt requires all unit exponents to be even".to_string()); }
        }
        let mut unit = self.unit;
        for i in 0..units::BASE_UNITS_SIZE { unit[i] /= 2; }
        Ok(DimensionalVariable { value: self.value.sqrt(), unit })
    }

    // ---- Math: logarithms (unitless only) ----
    /// Natural logarithm. Requires unitless and value > 0.
    pub fn ln(&self) -> Result<f64, String> {
        if !self.is_unitless() { return Err("ln requires a unitless quantity".to_string()); }
        if self.value <= 0.0 { return Err("ln domain error (value <= 0)".to_string()); }
        Ok(self.value.ln())
    }

    /// Base-2 logarithm. Requires unitless and value > 0.
    pub fn log2(&self) -> Result<f64, String> {
        if !self.is_unitless() { return Err("log2 requires a unitless quantity".to_string()); }
        if self.value <= 0.0 { return Err("log2 domain error (value <= 0)".to_string()); }
        Ok(self.value.log2())
    }

    /// Base-10 logarithm. Requires unitless and value > 0.
    pub fn log10(&self) -> Result<f64, String> {
        if !self.is_unitless() { return Err("log10 requires a unitless quantity".to_string()); }
        if self.value <= 0.0 { return Err("log10 domain error (value <= 0)".to_string()); }
        Ok(self.value.log10())
    }

    // ---- Math: trigonometry (unitless only, radians recommended) ----
    /// Sine function. Requires unitless (radians).
    pub fn sin(&self) -> Result<f64, String> {
        if !self.is_unitless() { return Err("sin requires a unitless quantity (radians)".to_string()); }
        Ok(self.value.sin())
    }

    /// Cosine function. Requires unitless (radians).
    pub fn cos(&self) -> Result<f64, String> {
        if !self.is_unitless() { return Err("cos requires a unitless quantity (radians)".to_string()); }
        Ok(self.value.cos())
    }

    /// Tangent function. Requires unitless (radians).
    pub fn tan(&self) -> Result<f64, String> {
        if !self.is_unitless() { return Err("tan requires a unitless quantity (radians)".to_string()); }
        Ok(self.value.tan())
    }

    // ---- Scalar helpers on single values ----
    /// Negate the value, keeping the same unit.
    pub fn neg(&self) -> DimensionalVariable {
        DimensionalVariable { value: -self.value, unit: self.unit }
    }

    /// Returns the absolute value, keeping the same unit.
    pub fn abs(&self) -> DimensionalVariable {
        DimensionalVariable { value: self.value.abs(), unit: self.unit }
    }
 
}

/// Convert a unit string like "m/s^2" or "kg-m/s^2" into base unit exponents and a conversion factor.
/// Returns an error if the unit string is invalid or contains unknown units.
fn unit_str_to_base_unit(units_str: &str) -> Result<([i32; units::BASE_UNITS_SIZE], f64), String> {

    // Start by removing any parentheses or brackets
    let cleaned_units_str = units_str.replace(['(', ')', '[', ']'], "");

    // Split the cleaned string by '/' to separate numerator and denominator
    let parts: Vec<&str> = cleaned_units_str.split('/').collect();
    if parts.len() > 2 {
        return Err("Unit string can only have one '/'".to_string());
    }

    let mut base_unit =  [0; units::BASE_UNITS_SIZE];
    let mut conversion_factor: f64 = 1.0;

    for i in 0..parts.len() {
        // Detect whether it's the numerator or denominator
        let denominator_multiplier = if i == 1 { -1 } else { 1 };

        // Split by '-' to handle individual units, but keep '-' that is an exponent sign (after '^')
        let units: Vec<&str> = {
            let s = parts[i];
            let mut out = Vec::new();
            let mut start = 0usize;
            let mut prev: Option<char> = None;
            for (idx, ch) in s.char_indices() {
                if ch == '-' && prev != Some('^') {
                    if idx > start {
                        out.push(&s[start..idx]);
                    }
                    start = idx + ch.len_utf8();
                }
                prev = Some(ch);
            }
            if start < s.len() {
                out.push(&s[start..]);
            }
            out
        };
        for unit_str in units {

            let (base, power) = read_unit_power(unit_str)?;

            let unit_map = units::unit_map();
            let unit = unit_map.get(base)
                .ok_or_else(|| format!("Unknown unit: {}", base))?; 

            for j in 0..units::BASE_UNITS_SIZE {
                base_unit[j] += unit.base_unit[j] * power * denominator_multiplier;
            }

            // Apply the conversion factor
            conversion_factor *= unit.conversion_factor.powi(power * denominator_multiplier);
        }
    }

    return Ok((base_unit, conversion_factor));
}

/// Parse a token like "m3", "m-2", or "m^3"/"m^-2" into (base, power).
/// If no trailing exponent is found, defaults to power = 1.
/// Returns an error if a trailing '^' has no number.
fn read_unit_power(unit: &str) -> Result<(&str, i32), String> {
    let u = unit.trim();
    if u.is_empty() {
        return Err("Empty unit token".to_string());
    }

    let bytes = u.as_bytes();

    // Find the trailing digits
    let mut end = u.len();
    while end > 0 && bytes[end - 1].is_ascii_digit() {
        end -= 1;
    }

    if end == u.len() {
        // No trailing digits. If it ends with '^', that's an error; otherwise power = 1.
        if end > 0 && bytes[end - 1] == b'^' {
            return Err(format!("Missing exponent after '^' in \"{}\"", u));
        }
        return Ok((u, 1));
    }

    let mut start = end;
    if start > 0 && (bytes[start - 1] == b'-') {
        start -= 1;
    }

    let exp_str = &u[start..];
    let exp: i32 = exp_str
        .parse()
        .map_err(|_| format!("Unable to read numeric power from \"{}\"", u))?;

    // Base is everything before the exponent; strip a trailing '^' if present.
    let mut base_end = start;
    if base_end > 0 && bytes[base_end - 1] == b'^' {
        base_end -= 1;
    }
    let base = u[..base_end].trim();
    if base.is_empty() {
        return Err(format!("Missing unit symbol before exponent in \"{}\"", u));
    }

    Ok((base, exp))
}

// ---- Helpers for unit arithmetic ----
/// Add two unit exponent arrays element-wise.
fn add_unit_exponents(a: [i32; units::BASE_UNITS_SIZE], b: [i32; units::BASE_UNITS_SIZE]) -> [i32; units::BASE_UNITS_SIZE] {
    let mut out = a;
    for i in 0..units::BASE_UNITS_SIZE { out[i] += b[i]; }
    out
}

/// Subtract two unit exponent arrays element-wise.
fn sub_unit_exponents(a: [i32; units::BASE_UNITS_SIZE], b: [i32; units::BASE_UNITS_SIZE]) -> [i32; units::BASE_UNITS_SIZE] {
    let mut out = a;
    for i in 0..units::BASE_UNITS_SIZE { out[i] -= b[i]; }
    out
}

// ---- Operator trait impls ----
use std::ops::{Add, Sub, Mul, Div, Neg, AddAssign, SubAssign, MulAssign, DivAssign};
use std::cmp::Ordering;

// Keep only reference-based binary ops to avoid duplication. Autoref handles owned values.
impl<'a, 'b> Add<&'b DimensionalVariable> for &'a DimensionalVariable {
    type Output = DimensionalVariable;
    fn add(self, rhs: &'b DimensionalVariable) -> Self::Output {
        assert!(self.unit == rhs.unit, "Incompatible units for addition: {:?} vs {:?}", self.unit, rhs.unit);
        DimensionalVariable { value: self.value + rhs.value, unit: self.unit }
    }
}

// Delegating wrappers for owned LHS/RHS
impl Add<DimensionalVariable> for DimensionalVariable {
    type Output = DimensionalVariable;
    fn add(self, rhs: DimensionalVariable) -> Self::Output {
        <&DimensionalVariable as Add<&DimensionalVariable>>::add(&self, &rhs)
    }
}

impl<'b> Add<&'b DimensionalVariable> for DimensionalVariable {
    type Output = DimensionalVariable;
    fn add(self, rhs: &'b DimensionalVariable) -> Self::Output {
        <&DimensionalVariable as Add<&DimensionalVariable>>::add(&self, rhs)
    }
}

impl<'a> Add<DimensionalVariable> for &'a DimensionalVariable {
    type Output = DimensionalVariable;
    fn add(self, rhs: DimensionalVariable) -> Self::Output {
        <&DimensionalVariable as Add<&DimensionalVariable>>::add(self, &rhs)
    }
}

impl<'a, 'b> Sub<&'b DimensionalVariable> for &'a DimensionalVariable {
    type Output = DimensionalVariable;
    fn sub(self, rhs: &'b DimensionalVariable) -> Self::Output {
        assert!(self.unit == rhs.unit, "Incompatible units for subtraction: {:?} vs {:?}", self.unit, rhs.unit);
        DimensionalVariable { value: self.value - rhs.value, unit: self.unit }
    }
}

impl Sub<DimensionalVariable> for DimensionalVariable {
    type Output = DimensionalVariable;
    fn sub(self, rhs: DimensionalVariable) -> Self::Output {
        <&DimensionalVariable as Sub<&DimensionalVariable>>::sub(&self, &rhs)
    }
}

impl<'b> Sub<&'b DimensionalVariable> for DimensionalVariable {
    type Output = DimensionalVariable;
    fn sub(self, rhs: &'b DimensionalVariable) -> Self::Output {
        <&DimensionalVariable as Sub<&DimensionalVariable>>::sub(&self, rhs)
    }
}

impl<'a> Sub<DimensionalVariable> for &'a DimensionalVariable {
    type Output = DimensionalVariable;
    fn sub(self, rhs: DimensionalVariable) -> Self::Output {
        <&DimensionalVariable as Sub<&DimensionalVariable>>::sub(self, &rhs)
    }
}

impl<'a, 'b> Mul<&'b DimensionalVariable> for &'a DimensionalVariable {
    type Output = DimensionalVariable;
    fn mul(self, rhs: &'b DimensionalVariable) -> Self::Output {
        DimensionalVariable { value: self.value * rhs.value, unit: add_unit_exponents(self.unit, rhs.unit) }
    }
}

impl Mul<DimensionalVariable> for DimensionalVariable {
    type Output = DimensionalVariable;
    fn mul(self, rhs: DimensionalVariable) -> Self::Output {
        <&DimensionalVariable as Mul<&DimensionalVariable>>::mul(&self, &rhs)
    }
}

impl<'b> Mul<&'b DimensionalVariable> for DimensionalVariable {
    type Output = DimensionalVariable;
    fn mul(self, rhs: &'b DimensionalVariable) -> Self::Output {
        <&DimensionalVariable as Mul<&DimensionalVariable>>::mul(&self, rhs)
    }
}

impl<'a> Mul<DimensionalVariable> for &'a DimensionalVariable {
    type Output = DimensionalVariable;
    fn mul(self, rhs: DimensionalVariable) -> Self::Output {
        <&DimensionalVariable as Mul<&DimensionalVariable>>::mul(self, &rhs)
    }
}

impl<'a, 'b> Div<&'b DimensionalVariable> for &'a DimensionalVariable {
    type Output = DimensionalVariable;
    fn div(self, rhs: &'b DimensionalVariable) -> Self::Output {
        DimensionalVariable { value: self.value / rhs.value, unit: sub_unit_exponents(self.unit, rhs.unit) }
    }
}

impl Div<DimensionalVariable> for DimensionalVariable {
    type Output = DimensionalVariable;
    fn div(self, rhs: DimensionalVariable) -> Self::Output {
        <&DimensionalVariable as Div<&DimensionalVariable>>::div(&self, &rhs)
    }
}

impl<'b> Div<&'b DimensionalVariable> for DimensionalVariable {
    type Output = DimensionalVariable;
    fn div(self, rhs: &'b DimensionalVariable) -> Self::Output {
        <&DimensionalVariable as Div<&DimensionalVariable>>::div(&self, rhs)
    }
}

impl<'a> Div<DimensionalVariable> for &'a DimensionalVariable {
    type Output = DimensionalVariable;
    fn div(self, rhs: DimensionalVariable) -> Self::Output {
        <&DimensionalVariable as Div<&DimensionalVariable>>::div(self, &rhs)
    }
}

// Assignment ops: implement only for &DimensionalVariable RHS. Owned RHS will autoref.
impl AddAssign<&DimensionalVariable> for DimensionalVariable {
    fn add_assign(&mut self, rhs: &DimensionalVariable) {
        assert!(self.unit == rhs.unit, "Incompatible units for addition assignment: {:?} vs {:?}", self.unit, rhs.unit);
        self.value += rhs.value;
    }
}

impl SubAssign<&DimensionalVariable> for DimensionalVariable {
    fn sub_assign(&mut self, rhs: &DimensionalVariable) {
        assert!(self.unit == rhs.unit, "Incompatible units for subtraction assignment: {:?} vs {:?}", self.unit, rhs.unit);
        self.value -= rhs.value;
    }
}

impl MulAssign<&DimensionalVariable> for DimensionalVariable {
    fn mul_assign(&mut self, rhs: &DimensionalVariable) {
        self.value *= rhs.value;
        self.unit = add_unit_exponents(self.unit, rhs.unit);
    }
}

impl DivAssign<&DimensionalVariable> for DimensionalVariable {
    fn div_assign(&mut self, rhs: &DimensionalVariable) {
        self.value /= rhs.value;
        self.unit = sub_unit_exponents(self.unit, rhs.unit);
    }
}

// Scalar ops
impl<'a> Mul<f64> for &'a DimensionalVariable {
    type Output = DimensionalVariable;
    fn mul(self, rhs: f64) -> Self::Output {
        DimensionalVariable { value: self.value * rhs, unit: self.unit }
    }
}

impl Mul<f64> for DimensionalVariable {
    type Output = DimensionalVariable;
    fn mul(self, rhs: f64) -> Self::Output {
        <&DimensionalVariable as Mul<f64>>::mul(&self, rhs)
    }
}

impl MulAssign<f64> for DimensionalVariable {
    fn mul_assign(&mut self, rhs: f64) {
        self.value *= rhs;
    }
}

impl<'a> Div<f64> for &'a DimensionalVariable {
    type Output = DimensionalVariable;
    fn div(self, rhs: f64) -> Self::Output {
        DimensionalVariable { value: self.value / rhs, unit: self.unit }
    }
}

impl Div<f64> for DimensionalVariable {
    type Output = DimensionalVariable;
    fn div(self, rhs: f64) -> Self::Output {
        <&DimensionalVariable as Div<f64>>::div(&self, rhs)
    }
}

impl DivAssign<f64> for DimensionalVariable {
    fn div_assign(&mut self, rhs: f64) {
        self.value /= rhs;
    }
}

// Symmetric scalar ops
impl<'a> Mul<&'a DimensionalVariable> for f64 {
    type Output = DimensionalVariable;
    fn mul(self, rhs: &'a DimensionalVariable) -> Self::Output {
        DimensionalVariable { value: self * rhs.value, unit: rhs.unit }
    }
}

impl Mul<DimensionalVariable> for f64 {
    type Output = DimensionalVariable;
    fn mul(self, rhs: DimensionalVariable) -> Self::Output {
        <f64 as Mul<&DimensionalVariable>>::mul(self, &rhs)
    }
}

impl<'a> Div<&'a DimensionalVariable> for f64 {
    type Output = DimensionalVariable;
    fn div(self, rhs: &'a DimensionalVariable) -> Self::Output {
        DimensionalVariable { value: self / rhs.value, unit: sub_unit_exponents([0; units::BASE_UNITS_SIZE], rhs.unit) }
    }
}

impl Div<DimensionalVariable> for f64 {
    type Output = DimensionalVariable;
    fn div(self, rhs: DimensionalVariable) -> Self::Output {
        <f64 as Div<&DimensionalVariable>>::div(self, &rhs)
    }
}

// Unary negation on references and delegating owned variant
impl<'a> Neg for &'a DimensionalVariable {
    type Output = DimensionalVariable;
    fn neg(self) -> Self::Output {
        DimensionalVariable { value: -self.value, unit: self.unit }
    }
}

impl Neg for DimensionalVariable {
    type Output = DimensionalVariable;
    fn neg(self) -> Self::Output {
        <&DimensionalVariable as Neg>::neg(&self)
    }
}

// ---- Comparisons: equalities and ordering ----
impl PartialEq for DimensionalVariable {
    fn eq(&self, other: &Self) -> bool {
        if self.unit != other.unit { return false; }
        self.value == other.value
    }
}

impl PartialOrd for DimensionalVariable {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        if self.unit != other.unit { return None; }
        self.value.partial_cmp(&other.value)
    }
}

#[cfg(test)]
mod tests {
    use super::read_unit_power;

    #[test]
    fn read_unit_power_basic_cases() {
        assert_eq!(read_unit_power("m").unwrap(), ("m", 1));
        assert_eq!(read_unit_power("m3").unwrap(), ("m", 3));
        assert_eq!(read_unit_power("m^3").unwrap(), ("m", 3));
        assert_eq!(read_unit_power("m^-2").unwrap(), ("m", -2));
        assert_eq!(read_unit_power("m-2").unwrap(), ("m", -2));
        assert_eq!(read_unit_power("  kg^2 ").unwrap(), ("kg", 2));
        assert_eq!(read_unit_power("undef").unwrap(), ("undef", 1));    // We don't check for known units here
    }

    #[test]
    fn read_unit_power_errors() {
        assert!(read_unit_power("").is_err());
        let err = read_unit_power("m^").unwrap_err();
        assert!(err.contains("Missing exponent"));
        let err = read_unit_power("^2").unwrap_err();
        assert!(err.contains("Missing unit symbol"));
    }
}
