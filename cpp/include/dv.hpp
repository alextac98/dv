#pragma once

#include "DimensionalVariable.hpp"

#include <optional>
#include <stdexcept>
#include <string>
#include <utility>

namespace dv {

inline std::string last_error() {
    return "dv operation failed";
}

template <class T, class E>
inline T unwrap_or_throw(diplomat::result<T, E>&& result, const char* msg = "dv operation failed") {
    auto v = std::move(result).ok();
    if (!v.has_value()) {
        throw std::runtime_error(msg);
    }
    return std::move(*v);
}

template <class T>
inline T unwrap_utf8_and_throw(
    diplomat::result<diplomat::result<T, std::monostate>, diplomat::Utf8Error>&& result,
    const char* msg = "dv operation failed"
) {
    auto outer = std::move(result).ok();
    if (!outer.has_value()) {
        throw std::runtime_error("invalid UTF-8");
    }
    auto inner = std::move(*outer).ok();
    if (!inner.has_value()) {
        throw std::runtime_error(msg);
    }
    return std::move(*inner);
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
