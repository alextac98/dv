#pragma once
#include "dv_c.h"
#include <stdexcept>
#include <string>

namespace dvpp {

inline std::string last_error() {
    const char* msg = dv_last_error_message();
    return msg ? std::string(msg) : std::string();
}

class Var {
public:
    Var() : ptr_(nullptr) {}
    Var(double value, const char* unit) : ptr_(dv_var_new(value, unit)) {
        if (!ptr_) throw std::runtime_error(last_error());
    }
    Var(const Var&) = delete;
    Var& operator=(const Var&) = delete;
    Var(Var&& o) noexcept : ptr_(o.ptr_) { o.ptr_ = nullptr; }
    Var& operator=(Var&& o) noexcept { if (this != &o) { reset(); ptr_ = o.ptr_; o.ptr_ = nullptr; } return *this; }
    ~Var() { reset(); }

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
    friend Var operator+(const Var& a, const Var& b) { return from_new(dv_var_add(a.cptr(), b.cptr())); }
    friend Var operator-(const Var& a, const Var& b) { return from_new(dv_var_sub(a.cptr(), b.cptr())); }
    friend Var operator*(const Var& a, const Var& b) { return from_new(dv_var_mul(a.cptr(), b.cptr())); }
    friend Var operator/(const Var& a, const Var& b) { return from_new(dv_var_div(a.cptr(), b.cptr())); }

    friend Var operator*(const Var& a, double s) { return from_new(dv_var_mul_scalar(a.cptr(), s)); }
    friend Var operator*(double s, const Var& a) { return from_new(dv_var_mul_scalar(a.cptr(), s)); }
    friend Var operator/(const Var& a, double s) { return from_new(dv_var_div_scalar(a.cptr(), s)); }

    Var powi(int e) const { return from_new(dv_var_powi(cptr(), e)); }
    Var powf(double e) const { return from_new(dv_var_powf(cptr(), e)); }
    Var sqrt() const { return from_new(dv_var_sqrt(cptr())); }

private:
    static Var from_new(dv_var* p) {
        if (!p) throw std::runtime_error(last_error());
        Var v; v.ptr_ = p; return v;
    }

    dv_var* ptr_;
};

inline size_t base_units_size() { return dv_base_units_size(); }

} // namespace dvpp
