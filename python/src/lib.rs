use pyo3::prelude::*;
use pyo3::exceptions::PyTypeError;
use dv_rs::DimensionalVariable;

pyo3::create_exception!(dv_pyo3, DVError, pyo3::exceptions::PyException);

/// A dimensioned variable that combines a numerical value with physical units.
/// 
/// The DV class performs dimensional analysis to ensure physical correctness
/// in mathematical operations. Units are stored internally as SI base unit
/// exponents (m, kg, s, K, A, mol, cd).
///
/// Examples:
///     >>> from dv import DV
///     >>> velocity = DV(10.0, "m/s")
///     >>> time = DV(2.0, "s")
///     >>> distance = velocity * time
///     >>> distance.value_in("m")
///     20.0
///     >>> distance.value_in("km")
///     0.02
#[pyclass(name = "DimensionalVariable")]
struct PyDV {
    dv: DimensionalVariable,
}

#[pymethods]
impl PyDV {
    /// Create a new dimensioned variable.
    ///
    /// Args:
    ///     value (float): The numerical value
    ///     unit (str): The unit string (e.g., "m/s", "kg*m/s^2")
    ///
    /// Returns:
    ///     DV: A new dimensioned variable
    ///
    /// Raises:
    ///     DVError: If the unit string is invalid
    #[new]
    fn new(value: f64, unit: &str) -> PyResult<Self> {
        match DimensionalVariable::new(value, unit) {
            Ok(dv) => Ok(PyDV { dv }),
            Err(e) => Err(DVError::new_err(e)),
        }
    }

    /// Get the raw value in SI base units.
    ///
    /// Returns:
    ///     float: The value in SI base units
    fn value(&self) -> f64 {
        self.dv.value()
    }

    /// Convert the value to the specified unit.
    ///
    /// Args:
    ///     unit (str): The target unit string
    ///
    /// Returns:
    ///     float: The value in the target unit
    ///
    /// Raises:
    ///     DVError: If units are incompatible or unit string is invalid
    fn value_in(&self, unit: &str) -> PyResult<f64> {
        match self.dv.value_in(unit) {
            Ok(v) => Ok(v),
            Err(e) => Err(DVError::new_err(e)),
        }
    }

    /// Check if the variable is dimensionless (unitless).
    ///
    /// Returns:
    ///     bool: True if dimensionless, False otherwise
    fn is_unitless(&self) -> bool {
        self.dv.is_unitless()
    }

    /// Get the base unit exponents as a tuple.
    ///
    /// Returns:
    ///     tuple: (m, kg, s, K, A, mol, cd, rad) exponents
    fn base_units(&self) -> (f64, f64, f64, f64, f64, f64, f64, f64) {
        let units = self.dv.unit();
        (units[0], units[1], units[2], units[3], units[4], units[5], units[6], units[7])
    }

    /// Addition operator.
    fn __add__(&self, other: &PyDV) -> PyResult<PyDV> {
        match self.dv.try_add(&other.dv) {
            Ok(result) => Ok(PyDV { dv: result }),
            Err(e) => Err(DVError::new_err(e)),
        }
    }

    /// Subtraction operator.
    fn __sub__(&self, other: &PyDV) -> PyResult<PyDV> {
        match self.dv.try_sub(&other.dv) {
            Ok(result) => Ok(PyDV { dv: result }),
            Err(e) => Err(DVError::new_err(e)),
        }
    }

    /// Multiplication operator (DV * DV or DV * scalar).
    fn __mul__(&self, other: &Bound<'_, PyAny>) -> PyResult<PyDV> {
        // Try to extract as another PyDV
        if let Ok(other_dv) = other.downcast::<PyDV>() {
            let other_borrow = other_dv.borrow();
            Ok(PyDV { dv: &self.dv * &other_borrow.dv })
        } else if let Ok(scalar) = other.extract::<f64>() {
            Ok(PyDV { dv: &self.dv * scalar })
        } else {
            Err(PyTypeError::new_err("Cannot multiply DV with this type"))
        }
    }

    /// Right multiplication operator (scalar * DV).
    fn __rmul__(&self, other: &Bound<'_, PyAny>) -> PyResult<PyDV> {
        if let Ok(scalar) = other.extract::<f64>() {
            Ok(PyDV { dv: scalar * &self.dv })
        } else {
            Err(PyTypeError::new_err("Cannot multiply this type with DV"))
        }
    }

    /// Division operator (DV / DV or DV / scalar).
    fn __truediv__(&self, other: &Bound<'_, PyAny>) -> PyResult<PyDV> {
        // Try to extract as another PyDV
        if let Ok(other_dv) = other.downcast::<PyDV>() {
            let other_borrow = other_dv.borrow();
            Ok(PyDV { dv: &self.dv / &other_borrow.dv })
        } else if let Ok(scalar) = other.extract::<f64>() {
            Ok(PyDV { dv: &self.dv / scalar })
        } else {
            Err(PyTypeError::new_err("Cannot divide DV by this type"))
        }
    }

    /// Right division operator (scalar / DV).
    fn __rtruediv__(&self, other: &Bound<'_, PyAny>) -> PyResult<PyDV> {
        if let Ok(scalar) = other.extract::<f64>() {
            Ok(PyDV { dv: scalar / &self.dv })
        } else {
            Err(PyTypeError::new_err("Cannot divide this type by DV"))
        }
    }

    /// Power operator.
    ///
    /// Args:
    ///     exponent (int or float): The exponent
    ///
    /// Returns:
    ///     DV: The result of raising to the power
    fn __pow__(&self, exponent: &Bound<'_, PyAny>, _modulo: Option<&Bound<'_, PyAny>>) -> PyResult<PyDV> {
        if let Ok(exp_int) = exponent.extract::<i32>() {
            Ok(PyDV { dv: self.dv.powi(exp_int) })
        } else if let Ok(exp_float) = exponent.extract::<f64>() {
            match self.dv.powf(exp_float) {
                Ok(result) => Ok(PyDV { dv: result }),
                Err(e) => Err(DVError::new_err(e)),
            }
        } else {
            Err(PyTypeError::new_err("Exponent must be int or float"))
        }
    }

    /// Negation operator.
    fn __neg__(&self) -> PyDV {
        PyDV { dv: -&self.dv }
    }

    /// Absolute value.
    fn __abs__(&self) -> PyDV {
        PyDV { dv: self.dv.abs() }
    }

    /// Equality comparison.
    fn __eq__(&self, other: &PyDV) -> bool {
        self.dv == other.dv
    }

    /// Inequality comparison.
    fn __ne__(&self, other: &PyDV) -> bool {
        self.dv != other.dv
    }

    /// Less than comparison.
    fn __lt__(&self, other: &PyDV) -> PyResult<bool> {
        match self.dv.partial_cmp(&other.dv) {
            Some(std::cmp::Ordering::Less) => Ok(true),
            Some(_) => Ok(false),
            None => Err(DVError::new_err("Cannot compare values with incompatible units")),
        }
    }

    /// Less than or equal comparison.
    fn __le__(&self, other: &PyDV) -> PyResult<bool> {
        match self.dv.partial_cmp(&other.dv) {
            Some(std::cmp::Ordering::Less) | Some(std::cmp::Ordering::Equal) => Ok(true),
            Some(_) => Ok(false),
            None => Err(DVError::new_err("Cannot compare values with incompatible units")),
        }
    }

    /// Greater than comparison.
    fn __gt__(&self, other: &PyDV) -> PyResult<bool> {
        match self.dv.partial_cmp(&other.dv) {
            Some(std::cmp::Ordering::Greater) => Ok(true),
            Some(_) => Ok(false),
            None => Err(DVError::new_err("Cannot compare values with incompatible units")),
        }
    }

    /// Greater than or equal comparison.
    fn __ge__(&self, other: &PyDV) -> PyResult<bool> {
        match self.dv.partial_cmp(&other.dv) {
            Some(std::cmp::Ordering::Greater) | Some(std::cmp::Ordering::Equal) => Ok(true),
            Some(_) => Ok(false),
            None => Err(DVError::new_err("Cannot compare values with incompatible units")),
        }
    }

    /// String representation (for developers/debugging).
    fn __repr__(&self) -> String {
        format!("DimensionalVariable({}, '{}')", self.dv.value(), self.dv.to_string().split_whitespace().skip(1).collect::<Vec<_>>().join(""))
    }

    /// String conversion (human-readable).
    fn __str__(&self) -> String {
        self.dv.to_string()
    }

    // Mathematical functions

    /// Square root.
    ///
    /// Returns:
    ///     DV: The square root
    ///
    /// Raises:
    ///     DVError: If the operation fails
    fn sqrt(&self) -> PyResult<PyDV> {
        match self.dv.sqrt() {
            Ok(result) => Ok(PyDV { dv: result }),
            Err(e) => Err(DVError::new_err(e)),
        }
    }

    /// Natural logarithm (requires unitless value).
    ///
    /// Returns:
    ///     DV: The natural logarithm (unitless)
    ///
    /// Raises:
    ///     DVError: If the value is not unitless
    fn ln(&self) -> PyResult<PyDV> {
        match self.dv.ln() {
            Ok(result) => Ok(PyDV { dv: DimensionalVariable { value: result, unit: [0.0; 8] } }),
            Err(e) => Err(DVError::new_err(e)),
        }
    }

    /// Base-2 logarithm (requires unitless value).
    ///
    /// Returns:
    ///     DV: The base-2 logarithm (unitless)
    ///
    /// Raises:
    ///     DVError: If the value is not unitless
    fn log2(&self) -> PyResult<PyDV> {
        match self.dv.log2() {
            Ok(result) => Ok(PyDV { dv: DimensionalVariable { value: result, unit: [0.0; 8] } }),
            Err(e) => Err(DVError::new_err(e)),
        }
    }

    /// Base-10 logarithm (requires unitless value).
    ///
    /// Returns:
    ///     DV: The base-10 logarithm (unitless)
    ///
    /// Raises:
    ///     DVError: If the value is not unitless
    fn log10(&self) -> PyResult<PyDV> {
        match self.dv.log10() {
            Ok(result) => Ok(PyDV { dv: DimensionalVariable { value: result, unit: [0.0; 8] } }),
            Err(e) => Err(DVError::new_err(e)),
        }
    }

    /// Sine (requires angle).
    ///
    /// Returns:
    ///     DV: The sine (unitless)
    ///
    /// Raises:
    ///     DVError: If the value is not an angle
    fn sin(&self) -> PyResult<PyDV> {
        match self.dv.sin() {
            Ok(result) => Ok(PyDV { dv: DimensionalVariable { value: result, unit: [0.0; 8] } }),
            Err(e) => Err(DVError::new_err(e)),
        }
    }

    /// Cosine (requires angle).
    ///
    /// Returns:
    ///     DV: The cosine (unitless)
    ///
    /// Raises:
    ///     DVError: If the value is not an angle
    fn cos(&self) -> PyResult<PyDV> {
        match self.dv.cos() {
            Ok(result) => Ok(PyDV { dv: DimensionalVariable { value: result, unit: [0.0; 8] } }),
            Err(e) => Err(DVError::new_err(e)),
        }
    }

    /// Tangent (requires angle).
    ///
    /// Returns:
    ///     DV: The tangent (unitless)
    ///
    /// Raises:
    ///     DVError: If the value is not an angle
    fn tan(&self) -> PyResult<PyDV> {
        match self.dv.tan() {
            Ok(result) => Ok(PyDV { dv: DimensionalVariable { value: result, unit: [0.0; 8] } }),
            Err(e) => Err(DVError::new_err(e)),
        }
    }

    /// Arcsine (requires unitless value in [-1, 1]).
    ///
    /// Returns:
    ///     DV: The arcsine as an angle in radians
    ///
    /// Raises:
    ///     DVError: If the value is not unitless or outside [-1, 1]
    fn asin(&self) -> PyResult<PyDV> {
        match self.dv.asin() {
            Ok(result) => Ok(PyDV { dv: result }),
            Err(e) => Err(DVError::new_err(e)),
        }
    }

    /// Arccosine (requires unitless value in [-1, 1]).
    ///
    /// Returns:
    ///     DV: The arccosine as an angle in radians
    ///
    /// Raises:
    ///     DVError: If the value is not unitless or outside [-1, 1]
    fn acos(&self) -> PyResult<PyDV> {
        match self.dv.acos() {
            Ok(result) => Ok(PyDV { dv: result }),
            Err(e) => Err(DVError::new_err(e)),
        }
    }

    /// Arctangent (requires unitless value).
    ///
    /// Returns:
    ///     DV: The arctangent as an angle in radians
    ///
    /// Raises:
    ///     DVError: If the value is not unitless
    fn atan(&self) -> PyResult<PyDV> {
        match self.dv.atan() {
            Ok(result) => Ok(PyDV { dv: result }),
            Err(e) => Err(DVError::new_err(e)),
        }
    }
}

/// Arcsine function for f64 input, returns DimensionalVariable in radians.
#[pyfunction]
fn asin(x: f64) -> PyResult<PyDV> {
    match dv_rs::asin(x) {
        Ok(result) => Ok(PyDV { dv: result }),
        Err(e) => Err(DVError::new_err(e)),
    }
}

/// Arccosine function for f64 input, returns DimensionalVariable in radians.
#[pyfunction]
fn acos(x: f64) -> PyResult<PyDV> {
    match dv_rs::acos(x) {
        Ok(result) => Ok(PyDV { dv: result }),
        Err(e) => Err(DVError::new_err(e)),
    }
}

/// Arctangent function for f64 input, returns DimensionalVariable in radians.
#[pyfunction]
fn atan(x: f64) -> PyResult<PyDV> {
    match dv_rs::atan(x) {
        Ok(result) => Ok(PyDV { dv: result }),
        Err(e) => Err(DVError::new_err(e)),
    }
}

/// Python module for dv (DimensionalVariable).
#[pymodule]
fn dv_pyo3(m: &Bound<'_, PyModule>) -> PyResult<()> {
    m.add_class::<PyDV>()?;
    m.add("DVError", m.py().get_type::<DVError>())?;
    m.add_function(wrap_pyfunction!(asin, m)?)?;
    m.add_function(wrap_pyfunction!(acos, m)?)?;
    m.add_function(wrap_pyfunction!(atan, m)?)?;
    Ok(())
}
