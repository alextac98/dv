use std::collections::HashMap;

/// Base SI units: meter (m), kilogram (kg), second (s), kelvin (K), ampere (A), mole (mol), candela (cd)
pub static BASE_UNITS: [&str; 7] = ["m", "kg", "s", "K", "A", "mol", "cd"];

/// Size of the base units array
pub static BASE_UNITS_SIZE: usize = BASE_UNITS.len();

/// A physical unit, defined by its name, conversion factor to SI base units, and its representation in base units.
pub struct Unit {
    #[allow(dead_code)]
    /// The human-readable name of the unit (e.g., "meter").
    pub name: &'static str,                 // Unused, kept for documentation and future extensibility
    /// The conversion factor to the SI base unit.
    pub conversion_factor: f64,
    /// The representation of the unit in terms of base units.
    pub base_unit: [f64; BASE_UNITS_SIZE], 
}

/// Unit map from unit strings to their definitions
pub fn unit_map() -> HashMap<&'static str, Unit> {
    HashMap::from([
        // ===== Length Units =====
        ("m", Unit { name: "meter", conversion_factor: 1.0, base_unit: [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0] }),
        ("cm", Unit { name: "centimeter", conversion_factor: 1e-2, base_unit: [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0] }),
        ("mm", Unit { name: "millimeter", conversion_factor: 1e-3, base_unit: [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0] }),
        ("um", Unit { name: "micrometer", conversion_factor: 1e-6, base_unit: [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0] }),
        ("nm", Unit { name: "nanometer", conversion_factor: 1e-9, base_unit: [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0] }),
        ("km", Unit { name: "kilometer", conversion_factor: 1e3, base_unit: [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0] }),

        ("mi", Unit { name: "mile", conversion_factor: 1609.344, base_unit: [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0] }),
        ("yd", Unit { name: "yard", conversion_factor: 0.9144, base_unit: [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0] }),
        ("ft", Unit { name: "foot", conversion_factor: 0.3048, base_unit: [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0] }),
        ("in", Unit { name: "inch", conversion_factor: 2.54e-2, base_unit: [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0] }),

        ("ly", Unit { name: "light year", conversion_factor: 9.4607e15, base_unit: [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0] }),
        ("pc", Unit { name: "parsec", conversion_factor: 3.0857e16, base_unit: [1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0] }),

        // ===== Area Units =====
        ("ha", Unit { name: "hectare", conversion_factor: 1e4, base_unit: [2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0] }),
        ("ac", Unit { name: "acre", conversion_factor: 4046.8564224, base_unit: [2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0] }),

        // ===== Volume Units =====
        ("l", Unit { name: "liter", conversion_factor: 1e-3, base_unit: [3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0] }),
        ("ml", Unit { name: "milliliter", conversion_factor: 1e-6, base_unit: [3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0] }),
        ("gal", Unit { name: "gallon", conversion_factor: 3.785411784, base_unit: [3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0] }),
        ("qt", Unit { name: "quart", conversion_factor: 9.4635284e-1, base_unit: [3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0] }),
        ("pt", Unit { name: "pint", conversion_factor: 4.7317642e-1, base_unit: [3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0] }),
        ("cup", Unit { name: "cup", conversion_factor: 2.3658821e-1, base_unit: [3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0] }),

        // ===== Velocity Units =====
        ("kn", Unit { name: "knot", conversion_factor: 1852.0 / 3600.0, base_unit: [1.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0] }),

        // ===== Force Units =====
        ("N", Unit { name: "newton", conversion_factor: 1.0, base_unit: [1.0, 1.0, -2.0, 0.0, 0.0, 0.0, 0.0] }),
        ("kN", Unit { name: "kilonewton", conversion_factor: 1e3, base_unit: [1.0, 1.0, -2.0, 0.0, 0.0, 0.0, 0.0] }),
        ("lbf", Unit { name: "pound-force", conversion_factor: 4.4482216152605, base_unit: [1.0, 1.0, -2.0, 0.0, 0.0, 0.0, 0.0] }),

        // ===== Mass Units =====
        ("kg", Unit { name: "kilogram", conversion_factor: 1.0, base_unit: [0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0] }),
        ("g", Unit { name: "gram", conversion_factor: 1e-3, base_unit: [0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0] }),
        ("mg", Unit { name: "milligram", conversion_factor: 1e-6, base_unit: [0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0] }),
        ("t", Unit { name: "metric ton", conversion_factor: 1e3, base_unit: [0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0] }),
        ("lb", Unit { name: "pound", conversion_factor: 0.45359237, base_unit: [0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0] }),
        ("oz", Unit { name: "ounce", conversion_factor: 0.028349523125, base_unit: [0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0] }),

        // ===== Time Units =====
        ("ns", Unit { name: "nanosecond", conversion_factor: 1e-9, base_unit: [0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0] }),
        ("us", Unit { name: "microsecond", conversion_factor: 1e-6, base_unit: [0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0] }),
        ("ms", Unit { name: "millisecond", conversion_factor: 1e-3, base_unit: [0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0] }),
        ("s", Unit { name: "second", conversion_factor: 1.0, base_unit: [0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0] }),
        ("min", Unit { name: "minute", conversion_factor: 60.0, base_unit: [0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0] }),
        ("hr", Unit { name: "hour", conversion_factor: 3600.0, base_unit: [0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0] }),
        ("d", Unit { name: "day", conversion_factor: 86400.0, base_unit: [0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0] }),
        ("wk", Unit { name: "week", conversion_factor: 604800.0, base_unit: [0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0] }),
        ("mo", Unit { name: "month", conversion_factor: 2629800.0, base_unit: [0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0] }),
        ("yr", Unit { name: "year", conversion_factor: 31536000.0, base_unit: [0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0] }),

        // ===== Temperature Units =====
        ("C", Unit { name: "celsius", conversion_factor: 1.0, base_unit: [0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0] }),
        ("F", Unit { name: "fahrenheit", conversion_factor: 5.0 / 9.0, base_unit: [0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0] }),
        ("K", Unit { name: "kelvin", conversion_factor: 1.0, base_unit: [0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0] }),

        // ===== Electric Current Units =====
        ("A", Unit {name: "ampere", conversion_factor: 1.0, base_unit: [0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0] }),
        ("mA", Unit { name: "milliampere", conversion_factor: 1e-3, base_unit: [0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0] }),
        ("kA", Unit { name: "kiloampere", conversion_factor: 1e3, base_unit: [0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0] }),
        ("MA", Unit { name: "megaampere", conversion_factor: 1e6, base_unit: [0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0] }),

        // ===== Energy Units =====
        ("ev", Unit { name: "electronvolt", conversion_factor: 1.602176634e-19, base_unit: [2.0, 1.0, -2.0, 0.0, 0.0, 0.0, 0.0] }),
        ("mJ", Unit { name: "millijoule", conversion_factor: 1e-3, base_unit: [2.0, 1.0, -2.0, 0.0, 0.0, 0.0, 0.0] }),
        ("J", Unit { name: "joule", conversion_factor: 1.0, base_unit: [2.0, 1.0, -2.0, 0.0, 0.0, 0.0, 0.0] }),
        ("kJ", Unit { name: "kilojoule", conversion_factor: 1e3, base_unit: [2.0, 1.0, -2.0, 0.0, 0.0, 0.0, 0.0] }),
        ("MJ", Unit { name: "megajoule", conversion_factor: 1e6, base_unit: [2.0, 1.0, -2.0, 0.0, 0.0, 0.0, 0.0] }),
        ("GJ", Unit { name: "gigajoule", conversion_factor: 1e9, base_unit: [2.0, 1.0, -2.0, 0.0, 0.0, 0.0, 0.0] }),
        ("Tj", Unit { name: "terajoule", conversion_factor: 1e12, base_unit: [2.0, 1.0, -2.0, 0.0, 0.0, 0.0, 0.0] }),
        ("cal", Unit { name: "calorie", conversion_factor: 4.184, base_unit: [2.0, 1.0, -2.0, 0.0, 0.0, 0.0, 0.0] }),
        ("kcal", Unit { name: "kilocalorie", conversion_factor: 4.184e3, base_unit: [2.0, 1.0, -2.0, 0.0, 0.0, 0.0, 0.0] }),
        ("Wh", Unit { name: "watt-hour", conversion_factor: 3600.0, base_unit: [2.0, 1.0, -2.0, 0.0, 0.0, 0.0, 0.0] }),
        ("kWh", Unit { name: "kilowatt-hour", conversion_factor: 3.6e6, base_unit: [2.0, 1.0, -2.0, 0.0, 0.0, 0.0, 0.0] }),
        ("BTU", Unit { name: "British thermal unit", conversion_factor: 1055.05585, base_unit: [2.0, 1.0, -2.0, 0.0, 0.0, 0.0, 0.0] }),
        ("erg", Unit { name: "erg", conversion_factor: 1e-7, base_unit: [2.0, 1.0, -2.0, 0.0, 0.0, 0.0, 0.0] }),

        // ===== Power Units =====
        ("W", Unit { name: "watt", conversion_factor: 1.0, base_unit: [2.0, 1.0, -3.0, 0.0, 0.0, 0.0, 0.0] }),
        ("kW", Unit { name: "kilowatt", conversion_factor: 1e3, base_unit: [2.0, 1.0, -3.0, 0.0, 0.0, 0.0, 0.0] }),
        ("MW", Unit { name: "megawatt", conversion_factor: 1e6, base_unit: [2.0, 1.0, -3.0, 0.0, 0.0, 0.0, 0.0] }),
        ("GW", Unit { name: "gigawatt", conversion_factor: 1e9, base_unit: [2.0, 1.0, -3.0, 0.0, 0.0, 0.0, 0.0] }),
        ("TW", Unit { name: "terawatt", conversion_factor: 1e12, base_unit: [2.0, 1.0, -3.0, 0.0, 0.0, 0.0, 0.0] }),
        ("hp", Unit { name: "horsepower", conversion_factor: 745.6998715822702, base_unit: [2.0, 1.0, -3.0, 0.0, 0.0, 0.0, 0.0] }),

        // ===== Amount of Substance Units =====
        ("mol", Unit { name: "mole", conversion_factor: 1.0, base_unit: [0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0] }),
        ("kmol", Unit { name: "kilomole", conversion_factor: 1e3, base_unit: [0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0] }),
        ("mmol", Unit { name: "millimole", conversion_factor: 1e-3, base_unit: [0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0] }),
        ("umol", Unit { name: "micromole", conversion_factor: 1e-6, base_unit: [0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0] }),
        ("nmol", Unit { name: "nanomole", conversion_factor: 1e-9, base_unit: [0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0] }),
        ("pmol", Unit { name: "picomole", conversion_factor: 1e-12, base_unit: [0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0] }),
        ("fmol", Unit { name: "femtomole", conversion_factor: 1e-15, base_unit: [0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0] }),

        // ===== Luminous Intensity Units =====
        ("cd", Unit { name: "candela", conversion_factor: 1.0, base_unit: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0] }),

        ("lux", Unit { name: "lux", conversion_factor: 1.0, base_unit: [0.0, 0.0, -2.0, 0.0, 0.0, 0.0, 1.0] }),

        // ===== Angle Units =====
        ("rad", Unit { name: "radian", conversion_factor: 1.0, base_unit: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0] }),
        ("deg", Unit { name: "degree", conversion_factor: std::f64::consts::PI / 180.0, base_unit: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0] }),
    ])
}
