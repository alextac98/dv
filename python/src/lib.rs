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
    inner: DimensionalVariable,
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
            Ok(inner) => Ok(PyDV { inner }),
            Err(e) => Err(DVError::new_err(e)),
        }
    }

    /// Get the raw value in SI base units.
    ///
    /// Returns:
    ///     float: The value in SI base units
    fn value(&self) -> f64 {
        self.inner.value()
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
        match self.inner.value_in(unit) {
            Ok(v) => Ok(v),
            Err(e) => Err(DVError::new_err(e)),
        }
    }

    /// Check if the variable is dimensionless (unitless).
    ///
    /// Returns:
    ///     bool: True if dimensionless, False otherwise
    fn is_unitless(&self) -> bool {
        self.inner.is_unitless()
    }

    /// Get the base unit exponents as a tuple.
    ///
    /// Returns:
    ///     tuple: (m, kg, s, K, A, mol, cd) exponents
    fn base_units(&self) -> (f64, f64, f64, f64, f64, f64, f64) {
        let units = self.inner.unit();
        (units[0], units[1], units[2], units[3], units[4], units[5], units[6])
    }

    /// Addition operator.
    fn __add__(&self, other: &PyDV) -> PyResult<PyDV> {
        match self.inner.try_add(&other.inner) {
            Ok(result) => Ok(PyDV { inner: result }),
            Err(e) => Err(DVError::new_err(e)),
        }
    }

    /// Subtraction operator.
    fn __sub__(&self, other: &PyDV) -> PyResult<PyDV> {
        match self.inner.try_sub(&other.inner) {
            Ok(result) => Ok(PyDV { inner: result }),
            Err(e) => Err(DVError::new_err(e)),
        }
    }

    /// Multiplication operator (DV * DV or DV * scalar).
    fn __mul__(&self, other: &Bound<'_, PyAny>) -> PyResult<PyDV> {
        // Try to extract as another PyDV
        if let Ok(other_dv) = other.downcast::<PyDV>() {
            let other_borrow = other_dv.borrow();
            Ok(PyDV { inner: &self.inner * &other_borrow.inner })
        } else if let Ok(scalar) = other.extract::<f64>() {
            Ok(PyDV { inner: &self.inner * scalar })
        } else {
            Err(PyTypeError::new_err("Cannot multiply DV with this type"))
        }
    }

    /// Right multiplication operator (scalar * DV).
    fn __rmul__(&self, other: &Bound<'_, PyAny>) -> PyResult<PyDV> {
        if let Ok(scalar) = other.extract::<f64>() {
            Ok(PyDV { inner: scalar * &self.inner })
        } else {
            Err(PyTypeError::new_err("Cannot multiply this type with DV"))
        }
    }

    /// Division operator (DV / DV or DV / scalar).
    fn __truediv__(&self, other: &Bound<'_, PyAny>) -> PyResult<PyDV> {
        // Try to extract as another PyDV
        if let Ok(other_dv) = other.downcast::<PyDV>() {
            let other_borrow = other_dv.borrow();
            Ok(PyDV { inner: &self.inner / &other_borrow.inner })
        } else if let Ok(scalar) = other.extract::<f64>() {
            Ok(PyDV { inner: &self.inner / scalar })
        } else {
            Err(PyTypeError::new_err("Cannot divide DV by this type"))
        }
    }

    /// Right division operator (scalar / DV).
    fn __rtruediv__(&self, other: &Bound<'_, PyAny>) -> PyResult<PyDV> {
        if let Ok(scalar) = other.extract::<f64>() {
            Ok(PyDV { inner: scalar / &self.inner })
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
            Ok(PyDV { inner: self.inner.powi(exp_int) })
        } else if let Ok(exp_float) = exponent.extract::<f64>() {
            match self.inner.powf(exp_float) {
                Ok(result) => Ok(PyDV { inner: result }),
                Err(e) => Err(DVError::new_err(e)),
            }
        } else {
            Err(PyTypeError::new_err("Exponent must be int or float"))
        }
    }

    /// Negation operator.
    fn __neg__(&self) -> PyDV {
        PyDV { inner: -&self.inner }
    }

    /// Absolute value.
    fn __abs__(&self) -> PyDV {
        PyDV { inner: self.inner.abs() }
    }

    /// Equality comparison.
    fn __eq__(&self, other: &PyDV) -> bool {
        self.inner == other.inner
    }

    /// Inequality comparison.
    fn __ne__(&self, other: &PyDV) -> bool {
        self.inner != other.inner
    }

    /// Less than comparison.
    fn __lt__(&self, other: &PyDV) -> PyResult<bool> {
        match self.inner.partial_cmp(&other.inner) {
            Some(std::cmp::Ordering::Less) => Ok(true),
            Some(_) => Ok(false),
            None => Err(DVError::new_err("Cannot compare values with incompatible units")),
        }
    }

    /// Less than or equal comparison.
    fn __le__(&self, other: &PyDV) -> PyResult<bool> {
        match self.inner.partial_cmp(&other.inner) {
            Some(std::cmp::Ordering::Less) | Some(std::cmp::Ordering::Equal) => Ok(true),
            Some(_) => Ok(false),
            None => Err(DVError::new_err("Cannot compare values with incompatible units")),
        }
    }

    /// Greater than comparison.
    fn __gt__(&self, other: &PyDV) -> PyResult<bool> {
        match self.inner.partial_cmp(&other.inner) {
            Some(std::cmp::Ordering::Greater) => Ok(true),
            Some(_) => Ok(false),
            None => Err(DVError::new_err("Cannot compare values with incompatible units")),
        }
    }

    /// Greater than or equal comparison.
    fn __ge__(&self, other: &PyDV) -> PyResult<bool> {
        match self.inner.partial_cmp(&other.inner) {
            Some(std::cmp::Ordering::Greater) | Some(std::cmp::Ordering::Equal) => Ok(true),
            Some(_) => Ok(false),
            None => Err(DVError::new_err("Cannot compare values with incompatible units")),
        }
    }

    /// String representation.
    fn __repr__(&self) -> String {
        format!("DV(value={}, base_units={:?})", self.inner.value(), self.inner.unit())
    }

    /// String conversion.
    fn __str__(&self) -> String {
        format!("{} (SI base units)", self.inner.value())
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
        match self.inner.sqrt() {
            Ok(result) => Ok(PyDV { inner: result }),
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
        match self.inner.ln() {
            Ok(result) => Ok(PyDV { inner: DimensionalVariable { value: result, unit: [0.0; 7] } }),
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
        match self.inner.log2() {
            Ok(result) => Ok(PyDV { inner: DimensionalVariable { value: result, unit: [0.0; 7] } }),
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
        match self.inner.log10() {
            Ok(result) => Ok(PyDV { inner: DimensionalVariable { value: result, unit: [0.0; 7] } }),
            Err(e) => Err(DVError::new_err(e)),
        }
    }

    /// Sine (requires unitless value).
    ///
    /// Returns:
    ///     DV: The sine (unitless)
    ///
    /// Raises:
    ///     DVError: If the value is not unitless
    fn sin(&self) -> PyResult<PyDV> {
        match self.inner.sin() {
            Ok(result) => Ok(PyDV { inner: DimensionalVariable { value: result, unit: [0.0; 7] } }),
            Err(e) => Err(DVError::new_err(e)),
        }
    }

    /// Cosine (requires unitless value).
    ///
    /// Returns:
    ///     DV: The cosine (unitless)
    ///
    /// Raises:
    ///     DVError: If the value is not unitless
    fn cos(&self) -> PyResult<PyDV> {
        match self.inner.cos() {
            Ok(result) => Ok(PyDV { inner: DimensionalVariable { value: result, unit: [0.0; 7] } }),
            Err(e) => Err(DVError::new_err(e)),
        }
    }

    /// Tangent (requires unitless value).
    ///
    /// Returns:
    ///     DV: The tangent (unitless)
    ///
    /// Raises:
    ///     DVError: If the value is not unitless
    fn tan(&self) -> PyResult<PyDV> {
        match self.inner.tan() {
            Ok(result) => Ok(PyDV { inner: DimensionalVariable { value: result, unit: [0.0; 7] } }),
            Err(e) => Err(DVError::new_err(e)),
        }
    }
}

/// Python module for dv (DimensionalVariable).
#[pymodule]
fn dv_pyo3(m: &Bound<'_, PyModule>) -> PyResult<()> {
    m.add_class::<PyDV>()?;
    m.add("DVError", m.py().get_type_bound::<DVError>())?;
    Ok(())
}
