"""Shared regression test runner for Python bindings."""

from __future__ import annotations

import json
import math
import os
from pathlib import Path

import pytest

from dv_py import DVError, DimensionalVariable


DEFAULT_ABS_TOL = 1e-10
DEFAULT_REL_TOL = 1e-10


def _candidate_paths(file_name: str) -> list[Path]:
    workspace = os.environ.get("TEST_WORKSPACE", "dv")
    test_srcdir = os.environ.get("TEST_SRCDIR")
    here = Path(__file__).resolve()
    candidates = [
        here.parent / file_name,
        Path.cwd() / "tests" / file_name,
        Path.cwd() / file_name,
    ]
    if test_srcdir:
        candidates.append(Path(test_srcdir) / workspace / "tests" / file_name)
    return candidates


def _load_vectors(file_name: str) -> dict:
    for path in _candidate_paths(file_name):
        if path.exists():
            with path.open("r", encoding="utf-8") as f:
                return json.load(f)
    attempted = "\n".join(str(p) for p in _candidate_paths(file_name))
    raise RuntimeError(f"Could not locate {file_name}. Attempted:\n{attempted}")


def _assert_close(actual: float, expected: float, abs_tol: float, rel_tol: float, case_id: str) -> None:
    if not math.isclose(actual, expected, abs_tol=abs_tol, rel_tol=rel_tol):
        raise AssertionError(f"{case_id}: expected {expected}, got {actual}")


def _extract_dv_value(v: DimensionalVariable, unit: str) -> float:
    if unit == "":
        return v.value()
    return v.value_in(unit)


def _assert_expected_dv(case_id: str, expect: dict, dv: DimensionalVariable, abs_tol: float, rel_tol: float) -> None:
    actual = _extract_dv_value(dv, expect["unit"])
    _assert_close(actual, expect["value"], abs_tol, rel_tol, case_id)


def _build_input(case: dict) -> DimensionalVariable:
    return DimensionalVariable(case["input"]["value"], case["input"]["unit"])


def _run_case(case: dict, abs_tol: float, rel_tol: float) -> None:
    case_id = case["id"]
    op = case["op"]
    expect = case["expect"]
    ok = expect["ok"]

    if op == "construct":
        value = case["input"]["value"]
        unit = case["input"]["unit"]
        if ok:
            dv = DimensionalVariable(value, unit)
            _assert_expected_dv(case_id, expect, dv, abs_tol, rel_tol)
        else:
            with pytest.raises(DVError):
                DimensionalVariable(value, unit)
        return

    if op == "convert":
        value = case["input"]["value"]
        from_unit = case["input"]["from_unit"]
        to_unit = case["input"]["to_unit"]
        dv = DimensionalVariable(value, from_unit)
        if ok:
            actual = dv.value_in(to_unit)
            _assert_close(actual, expect["value"], abs_tol, rel_tol, case_id)
        else:
            with pytest.raises(DVError):
                dv.value_in(to_unit)
        return

    if op in {"add", "sub", "mul", "div"}:
        left = DimensionalVariable(case["left"]["value"], case["left"]["unit"])
        right = DimensionalVariable(case["right"]["value"], case["right"]["unit"])

        if op == "add":
            if ok:
                _assert_expected_dv(case_id, expect, left + right, abs_tol, rel_tol)
            else:
                with pytest.raises(DVError):
                    _ = left + right
            return

        if op == "sub":
            if ok:
                _assert_expected_dv(case_id, expect, left - right, abs_tol, rel_tol)
            else:
                with pytest.raises(DVError):
                    _ = left - right
            return

        if op == "mul":
            _assert_expected_dv(case_id, expect, left * right, abs_tol, rel_tol)
            return

        if op == "div":
            _assert_expected_dv(case_id, expect, left / right, abs_tol, rel_tol)
            return

    input_dv = _build_input(case)

    if op == "powi":
        result = input_dv ** int(case["exponent"])
        _assert_expected_dv(case_id, expect, result, abs_tol, rel_tol)
        return

    if op == "powf":
        exponent = float(case["exponent"])
        if ok:
            result = input_dv ** exponent
            _assert_expected_dv(case_id, expect, result, abs_tol, rel_tol)
        else:
            with pytest.raises(DVError):
                _ = input_dv ** exponent
        return

    if op == "sqrt":
        if ok:
            _assert_expected_dv(case_id, expect, input_dv.sqrt(), abs_tol, rel_tol)
        else:
            with pytest.raises(DVError):
                input_dv.sqrt()
        return

    if op == "ln":
        if ok:
            _assert_expected_dv(case_id, expect, input_dv.ln(), abs_tol, rel_tol)
        else:
            with pytest.raises(DVError):
                input_dv.ln()
        return

    if op == "log2":
        if ok:
            _assert_expected_dv(case_id, expect, input_dv.log2(), abs_tol, rel_tol)
        else:
            with pytest.raises(DVError):
                input_dv.log2()
        return

    if op == "log10":
        if ok:
            _assert_expected_dv(case_id, expect, input_dv.log10(), abs_tol, rel_tol)
        else:
            with pytest.raises(DVError):
                input_dv.log10()
        return

    if op == "sin":
        if ok:
            _assert_expected_dv(case_id, expect, input_dv.sin(), abs_tol, rel_tol)
        else:
            with pytest.raises(DVError):
                input_dv.sin()
        return

    if op == "cos":
        if ok:
            _assert_expected_dv(case_id, expect, input_dv.cos(), abs_tol, rel_tol)
        else:
            with pytest.raises(DVError):
                input_dv.cos()
        return

    if op == "tan":
        if ok:
            _assert_expected_dv(case_id, expect, input_dv.tan(), abs_tol, rel_tol)
        else:
            with pytest.raises(DVError):
                input_dv.tan()
        return

    if op == "asin":
        if ok:
            _assert_expected_dv(case_id, expect, input_dv.asin(), abs_tol, rel_tol)
        else:
            with pytest.raises(DVError):
                input_dv.asin()
        return

    if op == "acos":
        if ok:
            _assert_expected_dv(case_id, expect, input_dv.acos(), abs_tol, rel_tol)
        else:
            with pytest.raises(DVError):
                input_dv.acos()
        return

    if op == "atan":
        if ok:
            _assert_expected_dv(case_id, expect, input_dv.atan(), abs_tol, rel_tol)
        else:
            with pytest.raises(DVError):
                input_dv.atan()
        return

    if op == "neg":
        _assert_expected_dv(case_id, expect, -input_dv, abs_tol, rel_tol)
        return

    if op == "abs":
        _assert_expected_dv(case_id, expect, abs(input_dv), abs_tol, rel_tol)
        return

    raise AssertionError(f"{case_id}: unsupported op {op}")


def _run_file(file_name: str) -> None:
    vectors = _load_vectors(file_name)
    abs_tol = vectors.get("defaults", {}).get("abs_tol", DEFAULT_ABS_TOL)
    rel_tol = vectors.get("defaults", {}).get("rel_tol", DEFAULT_REL_TOL)
    for case in vectors["cases"]:
        _run_case(case, abs_tol, rel_tol)


def test_units_regression_vectors() -> None:
    _run_file("units_tests.json")


def test_math_regression_vectors() -> None:
    _run_file("math_tests.json")
