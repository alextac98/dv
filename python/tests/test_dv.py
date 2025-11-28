"""Unit tests for the Python DimensionalVariable bindings."""

import pytest
from dv_py import DimensionalVariable, DVError


class TestConstruction:
    """Tests for DimensionalVariable construction."""

    def test_basic_construction(self):
        """Test creating a DimensionalVariable with a simple unit."""
        v = DimensionalVariable(10.0, "m/s")
        assert v.value() > 0

    def test_unitless_construction(self):
        """Test creating a unitless DimensionalVariable."""
        v = DimensionalVariable(5.0, "")
        assert v.is_unitless()

    def test_invalid_unit_string(self):
        """Test that invalid unit strings raise DVError."""
        with pytest.raises(DVError):
            DimensionalVariable(10.0, "invalid_unit")


class TestValueAccess:
    """Tests for accessing values."""

    def test_value(self):
        """Test getting the raw SI value."""
        v = DimensionalVariable(10.0, "m")
        assert v.value() == 10.0

    def test_value_in_same_unit(self):
        """Test converting to the same unit."""
        v = DimensionalVariable(10.0, "m")
        assert v.value_in("m") == 10.0

    def test_value_in_different_unit(self):
        """Test converting to a different compatible unit."""
        v = DimensionalVariable(1000.0, "m")
        assert v.value_in("km") == pytest.approx(1.0)

    def test_value_in_incompatible_unit(self):
        """Test that incompatible unit conversion raises DVError."""
        v = DimensionalVariable(10.0, "m")
        with pytest.raises(DVError):
            v.value_in("s")

    def test_is_unitless(self):
        """Test checking if a value is unitless."""
        unitless = DimensionalVariable(5.0, "")
        unit_value = DimensionalVariable(5.0, "m")
        assert unitless.is_unitless()
        assert not unit_value.is_unitless()

    def test_base_units(self):
        """Test getting base unit exponents."""
        v = DimensionalVariable(10.0, "m")
        base = v.base_units()
        assert base == (1, 0, 0, 0, 0, 0, 0, 0)  # [m, kg, s, K, A, mol, cd, rad]


class TestArithmetic:
    """Tests for arithmetic operations."""

    def test_addition(self):
        """Test adding two DVs with compatible units."""
        a = DimensionalVariable(10.0, "m")
        b = DimensionalVariable(5.0, "m")
        c = a + b
        assert c.value_in("m") == pytest.approx(15.0)

    def test_addition_incompatible(self):
        """Test that adding incompatible units raises DVError."""
        a = DimensionalVariable(10.0, "m")
        b = DimensionalVariable(5.0, "s")
        with pytest.raises(DVError):
            a + b

    def test_subtraction(self):
        """Test subtracting two DVs with compatible units."""
        a = DimensionalVariable(10.0, "m")
        b = DimensionalVariable(3.0, "m")
        c = a - b
        assert c.value_in("m") == pytest.approx(7.0)

    def test_subtraction_incompatible(self):
        """Test that subtracting incompatible units raises DVError."""
        a = DimensionalVariable(10.0, "m")
        b = DimensionalVariable(5.0, "s")
        with pytest.raises(DVError):
            a - b

    def test_multiplication(self):
        """Test multiplying two DVs."""
        v = DimensionalVariable(10.0, "m/s")
        t = DimensionalVariable(2.0, "s")
        d = v * t
        assert d.value_in("m") == pytest.approx(20.0)

    def test_division(self):
        """Test dividing two DVs."""
        d = DimensionalVariable(100.0, "m")
        t = DimensionalVariable(5.0, "s")
        v = d / t
        assert v.value_in("m/s") == pytest.approx(20.0)


class TestScalarOperations:
    """Tests for scalar operations."""

    def test_multiply_by_scalar(self):
        """Test multiplying a DimensionalVariable by a scalar."""
        v = DimensionalVariable(10.0, "m")
        result = v * 2.0
        assert result.value_in("m") == pytest.approx(20.0)

    def test_scalar_multiply(self):
        """Test multiplying a scalar by a DimensionalVariable."""
        v = DimensionalVariable(10.0, "m")
        result = 2.0 * v
        assert result.value_in("m") == pytest.approx(20.0)

    def test_divide_by_scalar(self):
        """Test dividing a DimensionalVariable by a scalar."""
        v = DimensionalVariable(10.0, "m")
        result = v / 2.0
        assert result.value_in("m") == pytest.approx(5.0)

    def test_scalar_divide(self):
        """Test dividing a scalar by a DimensionalVariable."""
        v = DimensionalVariable(2.0, "s")
        result = 10.0 / v
        # This should give 5.0 s^-1
        assert result.value() == pytest.approx(5.0)


class TestPowerOperations:
    """Tests for power operations."""

    def test_integer_power(self):
        """Test raising a DimensionalVariable to an integer power."""
        v = DimensionalVariable(3.0, "m")
        result = v ** 2
        assert result.value_in("m^2") == pytest.approx(9.0)

    def test_float_power(self):
        """Test raising a DimensionalVariable to a float power."""
        v = DimensionalVariable(4.0, "m^2")
        result = v ** 0.5
        assert result.value_in("m") == pytest.approx(2.0)

    def test_sqrt(self):
        """Test square root."""
        v = DimensionalVariable(16.0, "m^2")
        result = v.sqrt()
        assert result.value_in("m") == pytest.approx(4.0)


class TestUnaryOperations:
    """Tests for unary operations."""

    def test_negation(self):
        """Test negating a DimensionalVariable."""
        v = DimensionalVariable(10.0, "m")
        result = -v
        assert result.value_in("m") == pytest.approx(-10.0)

    def test_abs_positive(self):
        """Test absolute value of a positive DimensionalVariable."""
        v = DimensionalVariable(10.0, "m")
        result = abs(v)
        assert result.value_in("m") == pytest.approx(10.0)

    def test_abs_negative(self):
        """Test absolute value of a negative DimensionalVariable."""
        v = DimensionalVariable(-10.0, "m")
        result = abs(v)
        assert result.value_in("m") == pytest.approx(10.0)


class TestComparison:
    """Tests for comparison operations."""

    def test_equality(self):
        """Test equality comparison."""
        a = DimensionalVariable(10.0, "m")
        b = DimensionalVariable(10.0, "m")
        assert a == b

    def test_inequality(self):
        """Test inequality comparison."""
        a = DimensionalVariable(10.0, "m")
        b = DimensionalVariable(5.0, "m")
        assert a != b

    def test_less_than(self):
        """Test less than comparison."""
        a = DimensionalVariable(5.0, "m")
        b = DimensionalVariable(10.0, "m")
        assert a < b
        assert not b < a

    def test_less_equal(self):
        """Test less than or equal comparison."""
        a = DimensionalVariable(5.0, "m")
        b = DimensionalVariable(10.0, "m")
        c = DimensionalVariable(5.0, "m")
        assert a <= b
        assert a <= c

    def test_greater_than(self):
        """Test greater than comparison."""
        a = DimensionalVariable(10.0, "m")
        b = DimensionalVariable(5.0, "m")
        assert a > b
        assert not b > a

    def test_greater_equal(self):
        """Test greater than or equal comparison."""
        a = DimensionalVariable(10.0, "m")
        b = DimensionalVariable(5.0, "m")
        c = DimensionalVariable(10.0, "m")
        assert a >= b
        assert a >= c

    def test_comparison_incompatible_units(self):
        """Test that comparing incompatible units raises DVError."""
        a = DimensionalVariable(10.0, "m")
        b = DimensionalVariable(5.0, "s")
        with pytest.raises(DVError):
            a < b


class TestMathFunctions:
    """Tests for mathematical functions."""

    def test_ln_unitless(self):
        """Test natural logarithm of a unitless value."""
        import math
        v = DimensionalVariable(math.e, "")
        result = v.ln()
        assert result.value() == pytest.approx(1.0)

    def test_ln_with_units_fails(self):
        """Test that ln of a value with units raises DVError."""
        v = DimensionalVariable(10.0, "m")
        with pytest.raises(DVError):
            v.ln()

    def test_log2_unitless(self):
        """Test base-2 logarithm of a unitless value."""
        v = DimensionalVariable(8.0, "")
        result = v.log2()
        assert result.value() == pytest.approx(3.0)

    def test_log10_unitless(self):
        """Test base-10 logarithm of a unitless value."""
        v = DimensionalVariable(100.0, "")
        result = v.log10()
        assert result.value() == pytest.approx(2.0)

    def test_sin_unitless(self):
        """Test sine of a unitless value."""
        import math
        v = DimensionalVariable(math.pi / 2, "")
        result = v.sin()
        assert result.value() == pytest.approx(1.0)

    def test_cos_unitless(self):
        """Test cosine of a unitless value."""
        import math
        v = DimensionalVariable(0.0, "")
        result = v.cos()
        assert result.value() == pytest.approx(1.0)

    def test_tan_unitless(self):
        """Test tangent of a unitless value."""
        import math
        v = DimensionalVariable(math.pi / 4, "")
        result = v.tan()
        assert result.value() == pytest.approx(1.0)


class TestComplexExamples:
    """Tests for complex real-world examples."""

    def test_force_calculation(self):
        """Test calculating force from mass and acceleration."""
        mass = DimensionalVariable(10.0, "kg")
        accel = DimensionalVariable(9.81, "m/s^2")
        force = mass * accel
        # Force should be in Newtons (kg*m/s^2)
        assert force.value_in("N") == pytest.approx(98.1, rel=1e-3)

    def test_energy_calculation(self):
        """Test calculating kinetic energy."""
        mass = DimensionalVariable(2.0, "kg")
        velocity = DimensionalVariable(10.0, "m/s")
        # KE = 0.5 * m * v^2
        ke = 0.5 * mass * (velocity ** 2)
        # Energy should be in Joules (kg*m^2/s^2)
        assert ke.value_in("J") == pytest.approx(100.0)

    def test_unit_conversion_chain(self):
        """Test a chain of unit conversions."""
        distance = DimensionalVariable(5.0, "km")
        time = DimensionalVariable(0.5, "hr")
        velocity = distance / time
        # Should be 10 km/hr
        assert velocity.value_in("km/hr") == pytest.approx(10.0)
        # Convert to m/s
        assert velocity.value_in("m/s") == pytest.approx(2.777778, rel=1e-3)

    def test_angle_conversion(self):
        """Test angle conversions between radians and degrees."""
        import math
        angle_rad = DimensionalVariable(math.pi, "rad")
        angle_deg = DimensionalVariable(180.0, "deg")
        
        # π radians should equal 180 degrees
        assert angle_rad.value_in("rad") == pytest.approx(angle_deg.value_in("rad"))
        assert angle_rad.value_in("deg") == pytest.approx(180.0)
        assert angle_deg.value_in("rad") == pytest.approx(math.pi)
    
    def test_angle_trigonometry(self):
        """Test trigonometric functions with angles."""
        import math
        angle = DimensionalVariable(math.pi / 4, "rad")
        
        # sin(π/4) ≈ 0.707
        assert angle.sin().value() == pytest.approx(math.sin(math.pi / 4))
        # cos(π/4) ≈ 0.707
        assert angle.cos().value() == pytest.approx(math.cos(math.pi / 4))
        # tan(π/4) = 1
        assert angle.tan().value() == pytest.approx(1.0)
