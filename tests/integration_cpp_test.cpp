#include "dv.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

namespace fs = std::filesystem;

struct Json;
using JsonObject = std::map<std::string, Json>;
using JsonArray = std::vector<Json>;

struct Json {
    std::variant<std::nullptr_t, bool, double, std::string, JsonArray, JsonObject> value;
};

class JsonParser {
public:
    explicit JsonParser(const std::string& text) : s_(text), i_(0) {}

    Json parse() {
        Json v = parse_value();
        skip_ws();
        if (i_ != s_.size()) {
            throw std::runtime_error("unexpected trailing JSON content");
        }
        return v;
    }

private:
    Json parse_value() {
        skip_ws();
        char c = peek();
        if (c == '{') return parse_object();
        if (c == '[') return parse_array();
        if (c == '"') return Json{parse_string()};
        if (c == 't' || c == 'f') return Json{parse_bool()};
        if (c == 'n') {
            expect_token("null");
            return Json{nullptr};
        }
        if (c == '-' || std::isdigit(static_cast<unsigned char>(c))) return Json{parse_number()};
        throw std::runtime_error("invalid JSON value");
    }

    Json parse_object() {
        expect('{');
        skip_ws();
        JsonObject obj;
        if (peek() == '}') {
            ++i_;
            return Json{obj};
        }

        while (true) {
            skip_ws();
            std::string key = parse_string();
            skip_ws();
            expect(':');
            obj.emplace(key, parse_value());
            skip_ws();
            if (peek() == ',') {
                ++i_;
                continue;
            }
            if (peek() == '}') {
                ++i_;
                break;
            }
            throw std::runtime_error("expected ',' or '}' in object");
        }
        return Json{obj};
    }

    Json parse_array() {
        expect('[');
        skip_ws();
        JsonArray arr;
        if (peek() == ']') {
            ++i_;
            return Json{arr};
        }

        while (true) {
            arr.push_back(parse_value());
            skip_ws();
            if (peek() == ',') {
                ++i_;
                continue;
            }
            if (peek() == ']') {
                ++i_;
                break;
            }
            throw std::runtime_error("expected ',' or ']' in array");
        }
        return Json{arr};
    }

    std::string parse_string() {
        expect('"');
        std::string out;
        while (i_ < s_.size()) {
            char c = s_[i_++];
            if (c == '"') return out;
            if (c == '\\') {
                if (i_ >= s_.size()) throw std::runtime_error("unterminated escape sequence");
                char esc = s_[i_++];
                switch (esc) {
                    case '"': out.push_back('"'); break;
                    case '\\': out.push_back('\\'); break;
                    case '/': out.push_back('/'); break;
                    case 'b': out.push_back('\b'); break;
                    case 'f': out.push_back('\f'); break;
                    case 'n': out.push_back('\n'); break;
                    case 'r': out.push_back('\r'); break;
                    case 't': out.push_back('\t'); break;
                    default: throw std::runtime_error("unsupported JSON escape");
                }
                continue;
            }
            out.push_back(c);
        }
        throw std::runtime_error("unterminated string literal");
    }

    bool parse_bool() {
        if (starts_with("true")) {
            i_ += 4;
            return true;
        }
        if (starts_with("false")) {
            i_ += 5;
            return false;
        }
        throw std::runtime_error("invalid boolean");
    }

    double parse_number() {
        const std::size_t start = i_;
        if (peek() == '-') ++i_;
        consume_digits();
        if (peek() == '.') {
            ++i_;
            consume_digits();
        }
        if (peek() == 'e' || peek() == 'E') {
            ++i_;
            if (peek() == '+' || peek() == '-') ++i_;
            consume_digits();
        }
        return std::stod(s_.substr(start, i_ - start));
    }

    void consume_digits() {
        while (i_ < s_.size() && std::isdigit(static_cast<unsigned char>(s_[i_]))) {
            ++i_;
        }
    }

    void skip_ws() {
        while (i_ < s_.size()) {
            char c = s_[i_];
            if (c == ' ' || c == '\n' || c == '\r' || c == '\t') {
                ++i_;
                continue;
            }
            break;
        }
    }

    void expect(char c) {
        if (peek() != c) throw std::runtime_error(std::string("expected '") + c + "'");
        ++i_;
    }

    void expect_token(const std::string& token) {
        if (!starts_with(token)) throw std::runtime_error("unexpected token");
        i_ += token.size();
    }

    bool starts_with(const std::string& token) const {
        return s_.compare(i_, token.size(), token) == 0;
    }

    char peek() const {
        if (i_ >= s_.size()) throw std::runtime_error("unexpected end of input");
        return s_[i_];
    }

    std::string s_;
    std::size_t i_;
};

const JsonObject& as_object(const Json& v, const std::string& ctx) {
    auto p = std::get_if<JsonObject>(&v.value);
    if (!p) throw std::runtime_error(ctx + ": expected object");
    return *p;
}

const JsonArray& as_array(const Json& v, const std::string& ctx) {
    auto p = std::get_if<JsonArray>(&v.value);
    if (!p) throw std::runtime_error(ctx + ": expected array");
    return *p;
}

std::string as_string(const Json& v, const std::string& ctx) {
    auto p = std::get_if<std::string>(&v.value);
    if (!p) throw std::runtime_error(ctx + ": expected string");
    return *p;
}

double as_number(const Json& v, const std::string& ctx) {
    auto p = std::get_if<double>(&v.value);
    if (!p) throw std::runtime_error(ctx + ": expected number");
    return *p;
}

bool as_bool(const Json& v, const std::string& ctx) {
    auto p = std::get_if<bool>(&v.value);
    if (!p) throw std::runtime_error(ctx + ": expected bool");
    return *p;
}

const Json& get_key(const JsonObject& obj, const std::string& key, const std::string& ctx) {
    auto it = obj.find(key);
    if (it == obj.end()) throw std::runtime_error(ctx + ": missing key " + key);
    return it->second;
}

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

void assert_expected_dv(const std::string& case_id, const JsonObject& expect, const dv::DV& actual_dv, double abs_tol, double rel_tol) {
    const std::string exp_unit = as_string(get_key(expect, "unit", case_id), case_id + ".expect.unit");
    const double exp_value = as_number(get_key(expect, "value", case_id), case_id + ".expect.value");
    const double actual = exp_unit.empty() ? actual_dv.value() : actual_dv.value_in(exp_unit.c_str());
    if (!nearly_equal(actual, exp_value, abs_tol, rel_tol)) {
        throw std::runtime_error(case_id + ": expected " + std::to_string(exp_value) + ", got " + std::to_string(actual));
    }
}

void run_case(const JsonObject& test_case, double abs_tol, double rel_tol) {
    const std::string case_id = as_string(get_key(test_case, "id", "case"), "id");
    const std::string op = as_string(get_key(test_case, "op", case_id), "op");

    const JsonObject& expect = as_object(get_key(test_case, "expect", case_id), case_id + ".expect");
    const bool ok = as_bool(get_key(expect, "ok", case_id), case_id + ".expect.ok");

    if (op == "construct") {
        const JsonObject& input = as_object(get_key(test_case, "input", case_id), case_id + ".input");
        const double value = as_number(get_key(input, "value", case_id), case_id + ".input.value");
        const std::string unit = as_string(get_key(input, "unit", case_id), case_id + ".input.unit");

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
        const JsonObject& input = as_object(get_key(test_case, "input", case_id), case_id + ".input");
        const double value = as_number(get_key(input, "value", case_id), case_id + ".input.value");
        const std::string from_unit = as_string(get_key(input, "from_unit", case_id), case_id + ".input.from_unit");
        const std::string to_unit = as_string(get_key(input, "to_unit", case_id), case_id + ".input.to_unit");

        dv::DV v(value, from_unit.c_str());
        if (ok) {
            const double exp_value = as_number(get_key(expect, "value", case_id), case_id + ".expect.value");
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
        const JsonObject& input = as_object(get_key(test_case, "input", case_id), case_id + ".input");
        const double value = as_number(get_key(input, "value", case_id), case_id + ".input.value");
        const std::string unit = as_string(get_key(input, "unit", case_id), case_id + ".input.unit");
        return dv::DV(value, unit.c_str());
    };

    if (op == "add" || op == "sub" || op == "mul" || op == "div") {
        const JsonObject& left_obj = as_object(get_key(test_case, "left", case_id), case_id + ".left");
        const JsonObject& right_obj = as_object(get_key(test_case, "right", case_id), case_id + ".right");

        dv::DV left(
            as_number(get_key(left_obj, "value", case_id), case_id + ".left.value"),
            as_string(get_key(left_obj, "unit", case_id), case_id + ".left.unit").c_str()
        );
        dv::DV right(
            as_number(get_key(right_obj, "value", case_id), case_id + ".right.value"),
            as_string(get_key(right_obj, "unit", case_id), case_id + ".right.unit").c_str()
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
        int exponent = static_cast<int>(as_number(get_key(test_case, "exponent", case_id), case_id + ".exponent"));
        dv::DV result = input.powi(exponent);
        assert_expected_dv(case_id, expect, result, abs_tol, rel_tol);
        return;
    }

    if (op == "powf") {
        double exponent = as_number(get_key(test_case, "exponent", case_id), case_id + ".exponent");
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
    const Json root = JsonParser(content).parse();
    const JsonObject& root_obj = as_object(root, "root");

    double abs_tol = 1e-10;
    double rel_tol = 1e-10;
    auto defaults_it = root_obj.find("defaults");
    if (defaults_it != root_obj.end()) {
        const JsonObject& defaults = as_object(defaults_it->second, "defaults");
        abs_tol = as_number(get_key(defaults, "abs_tol", "defaults"), "defaults.abs_tol");
        rel_tol = as_number(get_key(defaults, "rel_tol", "defaults"), "defaults.rel_tol");
    }

    const JsonArray& cases = as_array(get_key(root_obj, "cases", "root"), "cases");
    for (const auto& c : cases) {
        run_case(as_object(c, "case"), abs_tol, rel_tol);
    }
}

int main() {
    try {
        run_vector_file("units_tests.json");
        run_vector_file("math_tests.json");
        std::cout << "Integration vectors passed for C++\\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Integration test failure: " << e.what() << "\\n";
        return 1;
    }
}
