"""Python bindings for dv (DimensionalVariable) via Diplomat + nanobind."""

from __future__ import annotations

from typing import Any

try:
    from . import dv_pyo3 as _nb
except ImportError:
    import dv_pyo3 as _nb


class DVError(Exception):
    """Library error for invalid dimensional operations."""


def _map_exc(exc: Exception) -> DVError:
    msg = str(exc) or "dv operation failed"
    return DVError(msg)


def _as_inner(value: Any):
    if isinstance(value, DimensionalVariable):
        return value._inner
    return value


class DimensionalVariable:
    """Python compatibility wrapper around the generated nanobind object."""

    __slots__ = ("_inner",)

    def __init__(self, value: float, unit: str):
        try:
            self._inner = _nb.DimensionalVariable.new(float(value), unit)
        except Exception as exc:  # nanobind generated exceptions are generic today
            raise _map_exc(exc) from None

    @classmethod
    def _from_inner(cls, inner):
        obj = cls.__new__(cls)
        obj._inner = inner
        return obj

    def value(self) -> float:
        return self._inner.value()

    def value_in(self, unit: str) -> float:
        try:
            return self._inner.value_in(unit)
        except Exception as exc:
            raise _map_exc(exc) from None

    def is_unitless(self) -> bool:
        return self._inner.is_unitless()

    def base_units(self):
        u = self._inner.base_units()
        return (u.m, u.kg, u.s, u.k, u.a, u.mol, u.cd, u.rad)

    def clone(self):
        return self._from_inner(self._inner.clone_var())

    def add(self, other: "DimensionalVariable"):
        try:
            return self._from_inner(self._inner.add(other._inner))
        except Exception as exc:
            raise _map_exc(exc) from None

    def sub(self, other: "DimensionalVariable"):
        try:
            return self._from_inner(self._inner.sub(other._inner))
        except Exception as exc:
            raise _map_exc(exc) from None

    def mul(self, other: "DimensionalVariable"):
        return self._from_inner(self._inner.mul(other._inner))

    def div(self, other: "DimensionalVariable"):
        return self._from_inner(self._inner.div(other._inner))

    def mul_scalar(self, scalar: float):
        return self._from_inner(self._inner.mul_scalar(float(scalar)))

    def div_scalar(self, scalar: float):
        return self._from_inner(self._inner.div_scalar(float(scalar)))

    def rdiv_scalar(self, scalar: float):
        return self._from_inner(self._inner.rdiv_scalar(float(scalar)))

    def powi(self, exp: int):
        return self._from_inner(self._inner.powi(int(exp)))

    def powf(self, exp: float):
        try:
            return self._from_inner(self._inner.powf(float(exp)))
        except Exception as exc:
            raise _map_exc(exc) from None

    def sqrt(self):
        try:
            return self._from_inner(self._inner.sqrt())
        except Exception as exc:
            raise _map_exc(exc) from None

    def ln(self):
        try:
            return self._from_inner(self._inner.ln())
        except Exception as exc:
            raise _map_exc(exc) from None

    def log2(self):
        try:
            return self._from_inner(self._inner.log2())
        except Exception as exc:
            raise _map_exc(exc) from None

    def log10(self):
        try:
            return self._from_inner(self._inner.log10())
        except Exception as exc:
            raise _map_exc(exc) from None

    def sin(self):
        try:
            return self._from_inner(self._inner.sin())
        except Exception as exc:
            raise _map_exc(exc) from None

    def cos(self):
        try:
            return self._from_inner(self._inner.cos())
        except Exception as exc:
            raise _map_exc(exc) from None

    def tan(self):
        try:
            return self._from_inner(self._inner.tan())
        except Exception as exc:
            raise _map_exc(exc) from None

    def asin(self):
        try:
            return self._from_inner(self._inner.asin())
        except Exception as exc:
            raise _map_exc(exc) from None

    def acos(self):
        try:
            return self._from_inner(self._inner.acos())
        except Exception as exc:
            raise _map_exc(exc) from None

    def atan(self):
        try:
            return self._from_inner(self._inner.atan())
        except Exception as exc:
            raise _map_exc(exc) from None

    def __add__(self, other: "DimensionalVariable"):
        return self.add(other)

    def __sub__(self, other: "DimensionalVariable"):
        return self.sub(other)

    def __mul__(self, other):
        if isinstance(other, DimensionalVariable):
            return self.mul(other)
        if isinstance(other, (int, float)):
            return self.mul_scalar(float(other))
        raise TypeError("Cannot multiply DV with this type")

    def __rmul__(self, other):
        if isinstance(other, (int, float)):
            return self.mul_scalar(float(other))
        raise TypeError("Cannot multiply this type with DV")

    def __truediv__(self, other):
        if isinstance(other, DimensionalVariable):
            return self.div(other)
        if isinstance(other, (int, float)):
            return self.div_scalar(float(other))
        raise TypeError("Cannot divide DV by this type")

    def __rtruediv__(self, other):
        if isinstance(other, (int, float)):
            return self.rdiv_scalar(float(other))
        raise TypeError("Cannot divide this type by DV")

    def __pow__(self, exponent, _modulo=None):
        if isinstance(exponent, int):
            return self.powi(exponent)
        if isinstance(exponent, float):
            return self.powf(exponent)
        raise TypeError("Exponent must be int or float")

    def __neg__(self):
        return self._from_inner(self._inner.neg())

    def __abs__(self):
        return self._from_inner(self._inner.abs())

    def __eq__(self, other):
        if not isinstance(other, DimensionalVariable):
            return False
        return self._inner.equals(other._inner)

    def __ne__(self, other):
        if not isinstance(other, DimensionalVariable):
            return True
        return self._inner.not_equals(other._inner)

    def __lt__(self, other):
        try:
            return self._inner.less_than(other._inner)
        except Exception as exc:
            raise _map_exc(exc) from None

    def __le__(self, other):
        try:
            return self._inner.less_equal(other._inner)
        except Exception as exc:
            raise _map_exc(exc) from None

    def __gt__(self, other):
        try:
            return self._inner.greater_than(other._inner)
        except Exception as exc:
            raise _map_exc(exc) from None

    def __ge__(self, other):
        try:
            return self._inner.greater_equal(other._inner)
        except Exception as exc:
            raise _map_exc(exc) from None

    def __repr__(self):
        try:
            return f"DimensionalVariable({self.value()}, '{str(self).split(' ', 1)[1]}')"
        except Exception:
            return "DimensionalVariable(<invalid>)"

    def __str__(self):
        try:
            return self._inner.to_string()
        except Exception as exc:
            raise _map_exc(exc) from None


def asin(x: float) -> DimensionalVariable:
    try:
        return DimensionalVariable._from_inner(_nb.DimensionalVariable.asin_scalar(float(x)))
    except Exception as exc:
        raise _map_exc(exc) from None


def acos(x: float) -> DimensionalVariable:
    try:
        return DimensionalVariable._from_inner(_nb.DimensionalVariable.acos_scalar(float(x)))
    except Exception as exc:
        raise _map_exc(exc) from None


def atan(x: float) -> DimensionalVariable:
    try:
        return DimensionalVariable._from_inner(_nb.DimensionalVariable.atan_scalar(float(x)))
    except Exception as exc:
        raise _map_exc(exc) from None


__all__ = ["DimensionalVariable", "DVError", "asin", "acos", "atan"]
