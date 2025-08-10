// --- dv core (Rust) ---

mod units;

pub fn version() -> &'static str {
    "0.1.0"
}

pub struct DimensionalVariable {
    pub value: f64,
    pub unit: [i32; 7]
}

impl DimensionalVariable {

    pub fn new(value: f64, unit_str: &str) -> Result<Self, String> {
        // Fetch the unit details from the unit map
        let unit_map = units::unit_map();
        let unit = unit_map.get(unit_str)
            .ok_or_else(|| format!("Unknown unit: {}", unit_str))?;

        // Create the DimensionalVariable with the converted value
        return Ok(DimensionalVariable{ value: value * unit.conversion_factor, unit: unit.base_unit });
    }


    pub fn value(&self) -> f64 {
        self.value
    }

    pub fn unit(&self) -> [i32; 7] {
        self.unit
    }
}