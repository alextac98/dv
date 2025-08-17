// --- dv core (Rust) ---

mod units;

pub fn version() -> &'static str {
    "0.1.0"
}

pub struct DimensionalVariable {
    pub value: f64,
    pub unit: [i32; 7],
}
/// A struct representing a dimensional variable with a value and a unit.
///
impl DimensionalVariable {
    /// Creates a new DimensionalVariable with the given value and unit.
    ///
    /// Rules for writing the unit string:
    /// - Use a single `/` as a delimeter between the numerator and denominator.
    /// - Use `-` as a delimeter between individual units
    /// - Exponents can be represented either by using `^` to indicate exponent (ex. `m^2`) or without the delimeter (ex. `m2`)
    /// - Inverses can be represented either by negative exponents or in the denominator (ex. `m^-2` or `1/m^2`)
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

    pub fn value(&self) -> f64 {
        self.value
    }

    pub fn unit(&self) -> [i32; 7] {
        self.unit
    }
}

fn unit_str_to_base_unit(units_str: &str) -> Result<([i32; 7], f64), String> {

    // Start by removing any parentheses or brackets
    let cleaned_units_str = units_str.replace(['(', ')', '[', ']'], "");

    // Split the cleaned string by '/' to separate numerator and denominator
    let parts: Vec<&str> = cleaned_units_str.split('/').collect();
    if parts.len() > 2 {
        return Err("Unit string can only have one '/'".to_string());
    }

    let mut base_unit =  [0; 7];
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

            // TODO: Make this more efficient by using a mathematical operation instead of a loop
            for j in 0..7 {
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
    let base = &u[..base_end].trim();
    if base.is_empty() {
        return Err(format!("Missing unit symbol before exponent in \"{}\"", u));
    }

    Ok((base, exp))
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
