#include "DimensionalVariable.hpp"

#include <cmath>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>

template <class T, class E>
T expect_ok(diplomat::result<T, E>&& result, const char* what) {
    auto v = std::move(result).ok();
    if (!v.has_value()) {
        throw std::runtime_error(what);
    }
    return std::move(*v);
}

template <class T>
T expect_utf8_and_ok(diplomat::result<diplomat::result<T, std::monostate>, diplomat::Utf8Error>&& result, const char* what) {
    auto outer = std::move(result).ok();
    if (!outer.has_value()) {
        throw std::runtime_error("utf8 error");
    }
    auto inner = std::move(*outer).ok();
    if (!inner.has_value()) {
        throw std::runtime_error(what);
    }
    return std::move(*inner);
}

int main() {
    auto mass = expect_utf8_and_ok<std::unique_ptr<DimensionalVariable>>(DimensionalVariable::new_(10.0, "kg"), "mass");
    auto accel = expect_utf8_and_ok<std::unique_ptr<DimensionalVariable>>(DimensionalVariable::new_(9.81, "m/s^2"), "accel");
    auto force = mass->mul(*accel);
    auto force_lbf = expect_utf8_and_ok<double>(force->value_in("lbf"), "force.value_in");
    std::cout << "force = " << force_lbf << " lbf\n";

    auto angle_deg = expect_utf8_and_ok<std::unique_ptr<DimensionalVariable>>(DimensionalVariable::new_(45.0, "deg"), "angle_deg");
    auto angle_rad = expect_utf8_and_ok<std::unique_ptr<DimensionalVariable>>(DimensionalVariable::new_(M_PI / 4.0, "rad"), "angle_rad");
    std::cout << "45 degrees = " << expect_utf8_and_ok<double>(angle_deg->value_in("rad"), "deg->rad") << " radians\n";
    std::cout << "pi/4 radians = " << expect_utf8_and_ok<double>(angle_rad->value_in("deg"), "rad->deg") << " degrees\n";

    auto asin_angle = expect_ok<std::unique_ptr<DimensionalVariable>>(DimensionalVariable::asin_scalar(0.5), "asin");
    auto acos_angle = expect_ok<std::unique_ptr<DimensionalVariable>>(DimensionalVariable::acos_scalar(0.5), "acos");
    auto atan_angle = expect_ok<std::unique_ptr<DimensionalVariable>>(DimensionalVariable::atan_scalar(1.0), "atan");
    std::cout << "asin(0.5) = " << expect_utf8_and_ok<double>(asin_angle->value_in("rad"), "asin rad") << " rad\n";
    std::cout << "acos(0.5) = " << expect_utf8_and_ok<double>(acos_angle->value_in("deg"), "acos deg") << " deg\n";
    std::cout << "atan(1.0) = " << expect_utf8_and_ok<double>(atan_angle->value_in("deg"), "atan deg") << " deg (should be 45)\n";

    auto force_str = expect_ok<std::string>(force->to_string(), "to_string");
    std::cout << "force string = " << force_str << "\n";

    return 0;
}
