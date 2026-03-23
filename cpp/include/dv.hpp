#pragma once

#include "DvError.hpp"
#include "DimensionalVariable.hpp"

#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>

namespace dv {

inline std::string last_error() {
    return "dv operation failed";
}

inline std::string last_error(std::monostate) {
    return last_error();
}

inline std::string last_error(diplomat::Utf8Error) {
    return "invalid UTF-8";
}

inline std::string last_error(const std::unique_ptr<DvError>& error) {
    if (!error) {
        return last_error();
    }

    auto message = error->to_string();
    auto text = std::move(message).ok();
    return text.has_value() && !text->empty() ? std::move(*text) : last_error();
}

inline std::string last_error(std::unique_ptr<DvError>&& error) {
    return last_error(static_cast<const std::unique_ptr<DvError>&>(error));
}

template <class T, class E>
inline T unwrap_or_throw(diplomat::result<T, E>&& result) {
    if (result.is_ok()) {
        auto v = std::move(result).ok();
        return std::move(*v);
    }

    auto error = std::move(result).err();
    throw std::runtime_error(error.has_value() ? last_error(std::move(*error)) : last_error());
}

template <class T>
inline T unwrap_utf8_and_throw(
    diplomat::result<diplomat::result<T, std::unique_ptr<DvError>>, diplomat::Utf8Error>&& result
) {
    if (result.is_err()) {
        auto error = std::move(result).err();
        throw std::runtime_error(error.has_value() ? last_error(std::move(*error)) : last_error());
    }

    auto outer = std::move(result).ok();
    auto inner_result = std::move(*outer);
    if (inner_result.is_ok()) {
        auto inner = std::move(inner_result).ok();
        return std::move(*inner);
    }

    auto nested = std::move(inner_result).err();
    throw std::runtime_error(nested.has_value() ? last_error(std::move(*nested)) : last_error());
}

class DV {
public:
    DV() = default;

    DV(double value, const char* unit)
        : ptr_(unwrap_utf8_and_throw<std::unique_ptr<DimensionalVariable>>(DimensionalVariable::new_(value, unit))) {}

    DV(const DV&) = delete;
    DV& operator=(const DV&) = delete;
    DV(DV&&) noexcept = default;
    DV& operator=(DV&&) noexcept = default;
    ~DV() = default;

    double value() const { return ptr_->value(); }
    bool is_unitless() const { return ptr_->is_unitless(); }

    double value_in(const char* unit) const {
        return unwrap_utf8_and_throw<double>(ptr_->value_in(unit));
    }

    DV powi(int e) const { return from_ptr(ptr_->powi(e)); }
    DV powf(double e) const { return from_ptr(unwrap_or_throw<std::unique_ptr<DimensionalVariable>>(ptr_->powf(e))); }
    DV sqrt() const { return from_ptr(unwrap_or_throw<std::unique_ptr<DimensionalVariable>>(ptr_->sqrt())); }
    DV ln() const { return from_ptr(unwrap_or_throw<std::unique_ptr<DimensionalVariable>>(ptr_->ln())); }
    DV log2() const { return from_ptr(unwrap_or_throw<std::unique_ptr<DimensionalVariable>>(ptr_->log2())); }
    DV log10() const { return from_ptr(unwrap_or_throw<std::unique_ptr<DimensionalVariable>>(ptr_->log10())); }
    DV sin() const { return from_ptr(unwrap_or_throw<std::unique_ptr<DimensionalVariable>>(ptr_->sin())); }
    DV cos() const { return from_ptr(unwrap_or_throw<std::unique_ptr<DimensionalVariable>>(ptr_->cos())); }
    DV tan() const { return from_ptr(unwrap_or_throw<std::unique_ptr<DimensionalVariable>>(ptr_->tan())); }
    DV abs() const { return from_ptr(ptr_->abs()); }
    DV asin() const { return from_ptr(unwrap_or_throw<std::unique_ptr<DimensionalVariable>>(ptr_->asin())); }
    DV acos() const { return from_ptr(unwrap_or_throw<std::unique_ptr<DimensionalVariable>>(ptr_->acos())); }
    DV atan() const { return from_ptr(unwrap_or_throw<std::unique_ptr<DimensionalVariable>>(ptr_->atan())); }

    std::string to_string() const { return unwrap_or_throw<std::string>(ptr_->to_string()); }

    friend DV operator+(const DV& a, const DV& b) {
        return from_ptr(unwrap_or_throw<std::unique_ptr<DimensionalVariable>>(a.ptr_->add(*b.ptr_)));
    }
    friend DV operator-(const DV& a, const DV& b) {
        return from_ptr(unwrap_or_throw<std::unique_ptr<DimensionalVariable>>(a.ptr_->sub(*b.ptr_)));
    }
    friend DV operator*(const DV& a, const DV& b) {
        return from_ptr(a.ptr_->mul(*b.ptr_));
    }
    friend DV operator/(const DV& a, const DV& b) {
        return from_ptr(a.ptr_->div(*b.ptr_));
    }
    friend DV operator*(const DV& a, double s) { return from_ptr(a.ptr_->mul_scalar(s)); }
    friend DV operator*(double s, const DV& a) { return from_ptr(a.ptr_->mul_scalar(s)); }
    friend DV operator/(const DV& a, double s) { return from_ptr(a.ptr_->div_scalar(s)); }
    friend DV operator/(double s, const DV& a) { return from_ptr(a.ptr_->rdiv_scalar(s)); }
    friend DV operator-(const DV& a) { return from_ptr(a.ptr_->neg()); }

    friend bool operator==(const DV& a, const DV& b) { return a.ptr_->equals(*b.ptr_); }
    friend bool operator!=(const DV& a, const DV& b) { return a.ptr_->not_equals(*b.ptr_); }
    friend bool operator<(const DV& a, const DV& b) { return unwrap_or_throw<bool>(a.ptr_->less_than(*b.ptr_)); }
    friend bool operator<=(const DV& a, const DV& b) { return unwrap_or_throw<bool>(a.ptr_->less_equal(*b.ptr_)); }
    friend bool operator>(const DV& a, const DV& b) { return unwrap_or_throw<bool>(a.ptr_->greater_than(*b.ptr_)); }
    friend bool operator>=(const DV& a, const DV& b) { return unwrap_or_throw<bool>(a.ptr_->greater_equal(*b.ptr_)); }

private:
    explicit DV(std::unique_ptr<DimensionalVariable> p) : ptr_(std::move(p)) {}

    static DV from_ptr(std::unique_ptr<DimensionalVariable> p) {
        if (!p) throw std::runtime_error("dv operation failed");
        return DV(std::move(p));
    }

    std::unique_ptr<DimensionalVariable> ptr_;

    friend DV asin(double x);
    friend DV acos(double x);
    friend DV atan(double x);
};

inline size_t base_units_size() { return DimensionalVariable::base_units_size(); }
inline DV asin(double x) { return DV::from_ptr(unwrap_or_throw<std::unique_ptr<DimensionalVariable>>(DimensionalVariable::asin_scalar(x))); }
inline DV acos(double x) { return DV::from_ptr(unwrap_or_throw<std::unique_ptr<DimensionalVariable>>(DimensionalVariable::acos_scalar(x))); }
inline DV atan(double x) { return DV::from_ptr(unwrap_or_throw<std::unique_ptr<DimensionalVariable>>(DimensionalVariable::atan_scalar(x))); }

} // namespace dv
