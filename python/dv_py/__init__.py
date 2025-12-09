"""Python bindings for dv (DimensionalVariable).

This module provides dimensional analysis for physical quantities,
ensuring unit correctness in mathematical operations.

Example:
    >>> from dv import DV
    >>> velocity = DV(10.0, "m/s")
    >>> time = DV(2.0, "s")
    >>> distance = velocity * time
    >>> print(distance.value_in("m"))
    20.0
"""

# When used as a package, use relative import
# The .so file is in the same directory as this __init__.py
try:
    # Try relative import first (when used as package 'dv')
    from . import dv_pyo3 as dv
except ImportError:
    # Fall back to direct import (when __init__.py is run directly)
    import dv_pyo3 as dv

DimensionalVariable = dv.DimensionalVariable
DVError = dv.DVError
asin = dv.asin
acos = dv.acos
atan = dv.atan

__all__ = ["DimensionalVariable", "DVError", "asin", "acos", "atan"]