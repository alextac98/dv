#pragma once
#include "dv_c.h"
#include <stdexcept>
#include <string>
#include <ostream>

namespace dv {

inline std::string last_error() {
    const char* msg = dv_last_error_message();
    return msg ? std::string(msg) : std::string();
}

class DV {
public:
    DV() : ptr_(nullptr) {}
    DV(double value, const char* unit) : ptr_(dv_var_new(value, unit)) {
        if (!ptr_) throw std::runtime_error(last_error());
    }
    DV(const DV&) = delete;
    DV& operator=(const DV&) = delete;
    DV(DV&& o) noexcept : ptr_(o.ptr_) { o.ptr_ = nullptr; }
    DV& operator=(DV&& o) noexcept { if (this != &o) { reset(); ptr_ = o.ptr_; o.ptr_ = nullptr; } return *this; }
    ~DV() { reset(); }

    void reset() { if (ptr_) { dv_var_free(ptr_); ptr_ = nullptr; } }
    bool valid() const { return ptr_ != nullptr; }
    const dv_var* cptr() const { return ptr_; }
    dv_var* ptr() { return ptr_; }

    double value() const { return dv_var_value(ptr_); }
    bool is_unitless() const { return dv_var_is_unitless(ptr_) != 0; }
    double value_in(const char* unit) const {
        double out = 0.0;
        if (!dv_var_value_in(ptr_, unit, &out)) {
            throw std::runtime_error(last_error());
        }
        return out;
    }

    // Arithmetic: return new objects
    friend DV operator+(const DV& a, const DV& b) { return from_new(dv_var_add(a.cptr(), b.cptr())); }
    friend DV operator-(const DV& a, const DV& b) { return from_new(dv_var_sub(a.cptr(), b.cptr())); }
    friend DV operator*(const DV& a, const DV& b) { return from_new(dv_var_mul(a.cptr(), b.cptr())); }
    friend DV operator/(const DV& a, const DV& b) { return from_new(dv_var_div(a.cptr(), b.cptr())); }

    friend DV operator*(const DV& a, double s) { return from_new(dv_var_mul_scalar(a.cptr(), s)); }
    friend DV operator*(double s, const DV& a) { return from_new(dv_var_mul_scalar(a.cptr(), s)); }
    friend DV operator/(const DV& a, double s) { return from_new(dv_var_div_scalar(a.cptr(), s)); }

    DV powi(int e) const { return from_new(dv_var_powi(cptr(), e)); }
    DV powf(double e) const { return from_new(dv_var_powf(cptr(), e)); }
    DV sqrt() const { return from_new(dv_var_sqrt(cptr())); }
    DV ln() const { return from_new(dv_var_ln(cptr())); }
    DV log2() const { return from_new(dv_var_log2(cptr())); }
    DV log10() const { return from_new(dv_var_log10(cptr())); }
    DV sin() const { return from_new(dv_var_sin(cptr())); }
    DV cos() const { return from_new(dv_var_cos(cptr())); }
    DV tan() const { return from_new(dv_var_tan(cptr())); }
    DV abs() const { return from_new(dv_var_abs(cptr())); }
    friend DV operator-(const DV& a) { return from_new(dv_var_neg(a.cptr())); }

    /// Convert to string representation (e.g., "9.81 m/s^2")
    std::string to_string() const {
        char* s = dv_var_to_string(ptr_);
        if (!s) throw std::runtime_error(last_error());
        std::string result(s);
        dv_free_cstring(s);
        return result;
    }

    /// Stream output operator
    friend std::ostream& operator<<(std::ostream& os, const DV& v) {
        return os << v.to_string();
    }

    // Inverse trigonometric functions (return angle in radians)
    DV asin() const { return from_new(dv_var_asin(cptr())); }
    DV acos() const { return from_new(dv_var_acos(cptr())); }
    DV atan() const { return from_new(dv_var_atan(cptr())); }

private:
    static DV from_new(dv_var* p) {
        if (!p) throw std::runtime_error(last_error());
        DV v; v.ptr_ = p; return v;
    }

    dv_var* ptr_;

    friend DV asin(double x);
    friend DV acos(double x);
    friend DV atan(double x);
};

inline size_t base_units_size() { return dv_base_units_size(); }

// Free-standing inverse trigonometric functions (take raw double, return angle in radians)
inline DV asin(double x) { return DV::from_new(dv_asin(x)); }
inline DV acos(double x) { return DV::from_new(dv_acos(x)); }
inline DV atan(double x) { return DV::from_new(dv_atan(x)); }

} // namespace dv
