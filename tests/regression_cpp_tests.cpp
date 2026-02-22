#include "dv.hpp"

#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

namespace fs = std::filesystem;
using json = nlohmann::json;

bool nearly_equal(double actual, double expected, double abs_tol, double rel_tol) {
    double diff = std::fabs(actual - expected);
    return diff <= std::max(abs_tol, rel_tol * std::fabs(expected));
}

fs::path resolve_vector_path(const std::string& file_name) {
    std::vector<fs::path> candidates;

    const char* test_srcdir = std::getenv("TEST_SRCDIR");
    const char* test_workspace = std::getenv("TEST_WORKSPACE");
    if (test_srcdir != nullptr) {
        std::string workspace = test_workspace != nullptr ? test_workspace : "dv";
        candidates.push_back(fs::path(test_srcdir) / workspace / "tests" / file_name);
    }

    candidates.push_back(fs::current_path() / "tests" / file_name);
    candidates.push_back(fs::current_path() / file_name);

    for (const auto& p : candidates) {
        if (fs::exists(p)) {
            return p;
        }
    }

    throw std::runtime_error("could not locate tests/" + file_name);
}

std::string read_file(const fs::path& path) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("failed to open " + path.string());
    return std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
}

std::string require_string(const json& obj, const char* key, const std::string& ctx) {
    if (!obj.contains(key) || !obj[key].is_string()) {
        throw std::runtime_error(ctx + ": missing or invalid string key " + key);
    }
    return obj[key].get<std::string>();
}

double require_number(const json& obj, const char* key, const std::string& ctx) {
    if (!obj.contains(key) || !obj[key].is_number()) {
        throw std::runtime_error(ctx + ": missing or invalid number key " + key);
    }
    return obj[key].get<double>();
}

bool require_bool(const json& obj, const char* key, const std::string& ctx) {
    if (!obj.contains(key) || !obj[key].is_boolean()) {
        throw std::runtime_error(ctx + ": missing or invalid bool key " + key);
    }
    return obj[key].get<bool>();
}

void assert_expected_dv(const std::string& case_id, const json& expect, const dv::DV& actual_dv, double abs_tol, double rel_tol) {
    const std::string exp_unit = require_string(expect, "unit", case_id + ".expect");
    const double exp_value = require_number(expect, "value", case_id + ".expect");
    const double actual = exp_unit.empty() ? actual_dv.value() : actual_dv.value_in(exp_unit.c_str());
    if (!nearly_equal(actual, exp_value, abs_tol, rel_tol)) {
        throw std::runtime_error(case_id + ": expected " + std::to_string(exp_value) + ", got " + std::to_string(actual));
    }
}

void run_case(const json& test_case, double abs_tol, double rel_tol) {
    if (!test_case.is_object()) throw std::runtime_error("case must be object");

    const std::string case_id = require_string(test_case, "id", "case");
    const std::string op = require_string(test_case, "op", case_id);

    if (!test_case.contains("expect") || !test_case["expect"].is_object()) {
        throw std::runtime_error(case_id + ": missing expect object");
    }
    const json& expect = test_case["expect"];
    const bool ok = require_bool(expect, "ok", case_id + ".expect");

    if (op == "construct") {
        const json& input = test_case.at("input");
        const double value = require_number(input, "value", case_id + ".input");
        const std::string unit = require_string(input, "unit", case_id + ".input");

        if (ok) {
            dv::DV v(value, unit.c_str());
            assert_expected_dv(case_id, expect, v, abs_tol, rel_tol);
        } else {
            bool threw = false;
            try {
                dv::DV bad(value, unit.c_str());
            } catch (const std::exception&) {
                threw = true;
            }
            if (!threw) throw std::runtime_error(case_id + ": expected construct failure");
        }
        return;
    }

    if (op == "convert") {
        const json& input = test_case.at("input");
        const double value = require_number(input, "value", case_id + ".input");
        const std::string from_unit = require_string(input, "from_unit", case_id + ".input");
        const std::string to_unit = require_string(input, "to_unit", case_id + ".input");

        dv::DV v(value, from_unit.c_str());
        if (ok) {
            const double exp_value = require_number(expect, "value", case_id + ".expect");
            const double actual = v.value_in(to_unit.c_str());
            if (!nearly_equal(actual, exp_value, abs_tol, rel_tol)) {
                throw std::runtime_error(case_id + ": expected " + std::to_string(exp_value) + ", got " + std::to_string(actual));
            }
        } else {
            bool threw = false;
            try {
                (void)v.value_in(to_unit.c_str());
            } catch (const std::exception&) {
                threw = true;
            }
            if (!threw) throw std::runtime_error(case_id + ": expected conversion failure");
        }
        return;
    }

    auto build_input = [&]() -> dv::DV {
        const json& input = test_case.at("input");
        const double value = require_number(input, "value", case_id + ".input");
        const std::string unit = require_string(input, "unit", case_id + ".input");
        return dv::DV(value, unit.c_str());
    };

    if (op == "add" || op == "sub" || op == "mul" || op == "div") {
        const json& left_obj = test_case.at("left");
        const json& right_obj = test_case.at("right");

        dv::DV left(
            require_number(left_obj, "value", case_id + ".left"),
            require_string(left_obj, "unit", case_id + ".left").c_str()
        );
        dv::DV right(
            require_number(right_obj, "value", case_id + ".right"),
            require_string(right_obj, "unit", case_id + ".right").c_str()
        );

        if ((op == "add" || op == "sub") && !ok) {
            bool threw = false;
            try {
                if (op == "add") {
                    (void)(left + right);
                } else {
                    (void)(left - right);
                }
            } catch (const std::exception&) {
                threw = true;
            }
            if (!threw) throw std::runtime_error(case_id + ": expected arithmetic failure");
            return;
        }

        dv::DV result;
        if (op == "add") {
            result = left + right;
        } else if (op == "sub") {
            result = left - right;
        } else if (op == "mul") {
            result = left * right;
        } else {
            result = left / right;
        }
        assert_expected_dv(case_id, expect, result, abs_tol, rel_tol);
        return;
    }

    dv::DV input = build_input();

    if (op == "powi") {
        int exponent = static_cast<int>(require_number(test_case, "exponent", case_id));
        dv::DV result = input.powi(exponent);
        assert_expected_dv(case_id, expect, result, abs_tol, rel_tol);
        return;
    }

    if (op == "powf") {
        double exponent = require_number(test_case, "exponent", case_id);
        if (ok) {
            dv::DV result = input.powf(exponent);
            assert_expected_dv(case_id, expect, result, abs_tol, rel_tol);
        } else {
            bool threw = false;
            try {
                (void)input.powf(exponent);
            } catch (const std::exception&) {
                threw = true;
            }
            if (!threw) throw std::runtime_error(case_id + ": expected powf failure");
        }
        return;
    }

    auto run_unary = [&](auto fn, const std::string& fail_label) {
        if (ok) {
            dv::DV result = fn();
            assert_expected_dv(case_id, expect, result, abs_tol, rel_tol);
        } else {
            bool threw = false;
            try {
                (void)fn();
            } catch (const std::exception&) {
                threw = true;
            }
            if (!threw) throw std::runtime_error(case_id + ": expected " + fail_label + " failure");
        }
    };

    if (op == "sqrt") {
        run_unary([&]() { return input.sqrt(); }, "sqrt");
        return;
    }
    if (op == "ln") {
        run_unary([&]() { return input.ln(); }, "ln");
        return;
    }
    if (op == "log2") {
        run_unary([&]() { return input.log2(); }, "log2");
        return;
    }
    if (op == "log10") {
        run_unary([&]() { return input.log10(); }, "log10");
        return;
    }
    if (op == "sin") {
        run_unary([&]() { return input.sin(); }, "sin");
        return;
    }
    if (op == "cos") {
        run_unary([&]() { return input.cos(); }, "cos");
        return;
    }
    if (op == "tan") {
        run_unary([&]() { return input.tan(); }, "tan");
        return;
    }
    if (op == "asin") {
        run_unary([&]() { return input.asin(); }, "asin");
        return;
    }
    if (op == "acos") {
        run_unary([&]() { return input.acos(); }, "acos");
        return;
    }
    if (op == "atan") {
        run_unary([&]() { return input.atan(); }, "atan");
        return;
    }
    if (op == "neg") {
        dv::DV result = -input;
        assert_expected_dv(case_id, expect, result, abs_tol, rel_tol);
        return;
    }
    if (op == "abs") {
        dv::DV result = input.abs();
        assert_expected_dv(case_id, expect, result, abs_tol, rel_tol);
        return;
    }

    throw std::runtime_error(case_id + ": unsupported op " + op);
}

void run_vector_file(const std::string& file_name) {
    const fs::path path = resolve_vector_path(file_name);
    const std::string content = read_file(path);
    const json root = json::parse(content);

    if (!root.is_object()) {
        throw std::runtime_error("root JSON must be an object");
    }

    double abs_tol = 1e-10;
    double rel_tol = 1e-10;
    if (root.contains("defaults") && root["defaults"].is_object()) {
        const json& defaults = root["defaults"];
        abs_tol = require_number(defaults, "abs_tol", "defaults");
        rel_tol = require_number(defaults, "rel_tol", "defaults");
    }

    if (!root.contains("cases") || !root["cases"].is_array()) {
        throw std::runtime_error("root.cases must be an array");
    }

    for (const auto& c : root["cases"]) {
        run_case(c, abs_tol, rel_tol);
    }
}

int main() {
    try {
        run_vector_file("units_tests.json");
        run_vector_file("math_tests.json");
        std::cout << "Regression vectors passed for C++\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Regression test failure: " << e.what() << "\n";
        return 1;
    }
}
