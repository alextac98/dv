#ifndef DV_PYO3_DimensionalVariable_HPP
#define DV_PYO3_DimensionalVariable_HPP

#include "DimensionalVariable.d.hpp"

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <memory>
#include <functional>
#include <optional>
#include <cstdlib>
#include "BaseUnits.hpp"
#include "diplomat_runtime.hpp"


namespace dv_pyo3 {
namespace capi {
    extern "C" {

    typedef struct DimensionalVariable_new_result {union {dv_pyo3::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_new_result;
    DimensionalVariable_new_result DimensionalVariable_new(double value, dv_pyo3::diplomat::capi::DiplomatStringView unit);

    typedef struct DimensionalVariable_asin_scalar_result {union {dv_pyo3::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_asin_scalar_result;
    DimensionalVariable_asin_scalar_result DimensionalVariable_asin_scalar(double x);

    typedef struct DimensionalVariable_acos_scalar_result {union {dv_pyo3::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_acos_scalar_result;
    DimensionalVariable_acos_scalar_result DimensionalVariable_acos_scalar(double x);

    typedef struct DimensionalVariable_atan_scalar_result {union {dv_pyo3::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_atan_scalar_result;
    DimensionalVariable_atan_scalar_result DimensionalVariable_atan_scalar(double x);

    size_t DimensionalVariable_base_units_size(void);

    double DimensionalVariable_value(const dv_pyo3::capi::DimensionalVariable* self);

    typedef struct DimensionalVariable_value_in_result {union {double ok; }; bool is_ok;} DimensionalVariable_value_in_result;
    DimensionalVariable_value_in_result DimensionalVariable_value_in(const dv_pyo3::capi::DimensionalVariable* self, dv_pyo3::diplomat::capi::DiplomatStringView unit);

    bool DimensionalVariable_is_unitless(const dv_pyo3::capi::DimensionalVariable* self);

    dv_pyo3::capi::BaseUnits DimensionalVariable_base_units(const dv_pyo3::capi::DimensionalVariable* self);

    typedef struct DimensionalVariable_add_result {union {dv_pyo3::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_add_result;
    DimensionalVariable_add_result DimensionalVariable_add(const dv_pyo3::capi::DimensionalVariable* self, const dv_pyo3::capi::DimensionalVariable* other);

    typedef struct DimensionalVariable_sub_result {union {dv_pyo3::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_sub_result;
    DimensionalVariable_sub_result DimensionalVariable_sub(const dv_pyo3::capi::DimensionalVariable* self, const dv_pyo3::capi::DimensionalVariable* other);

    dv_pyo3::capi::DimensionalVariable* DimensionalVariable_mul(const dv_pyo3::capi::DimensionalVariable* self, const dv_pyo3::capi::DimensionalVariable* other);

    dv_pyo3::capi::DimensionalVariable* DimensionalVariable_div(const dv_pyo3::capi::DimensionalVariable* self, const dv_pyo3::capi::DimensionalVariable* other);

    dv_pyo3::capi::DimensionalVariable* DimensionalVariable_mul_scalar(const dv_pyo3::capi::DimensionalVariable* self, double scalar);

    dv_pyo3::capi::DimensionalVariable* DimensionalVariable_div_scalar(const dv_pyo3::capi::DimensionalVariable* self, double scalar);

    dv_pyo3::capi::DimensionalVariable* DimensionalVariable_rdiv_scalar(const dv_pyo3::capi::DimensionalVariable* self, double scalar);

    dv_pyo3::capi::DimensionalVariable* DimensionalVariable_powi(const dv_pyo3::capi::DimensionalVariable* self, int32_t exp);

    typedef struct DimensionalVariable_powf_result {union {dv_pyo3::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_powf_result;
    DimensionalVariable_powf_result DimensionalVariable_powf(const dv_pyo3::capi::DimensionalVariable* self, double exp);

    typedef struct DimensionalVariable_sqrt_result {union {dv_pyo3::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_sqrt_result;
    DimensionalVariable_sqrt_result DimensionalVariable_sqrt(const dv_pyo3::capi::DimensionalVariable* self);

    typedef struct DimensionalVariable_ln_result {union {dv_pyo3::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_ln_result;
    DimensionalVariable_ln_result DimensionalVariable_ln(const dv_pyo3::capi::DimensionalVariable* self);

    typedef struct DimensionalVariable_log2_result {union {dv_pyo3::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_log2_result;
    DimensionalVariable_log2_result DimensionalVariable_log2(const dv_pyo3::capi::DimensionalVariable* self);

    typedef struct DimensionalVariable_log10_result {union {dv_pyo3::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_log10_result;
    DimensionalVariable_log10_result DimensionalVariable_log10(const dv_pyo3::capi::DimensionalVariable* self);

    typedef struct DimensionalVariable_sin_result {union {dv_pyo3::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_sin_result;
    DimensionalVariable_sin_result DimensionalVariable_sin(const dv_pyo3::capi::DimensionalVariable* self);

    typedef struct DimensionalVariable_cos_result {union {dv_pyo3::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_cos_result;
    DimensionalVariable_cos_result DimensionalVariable_cos(const dv_pyo3::capi::DimensionalVariable* self);

    typedef struct DimensionalVariable_tan_result {union {dv_pyo3::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_tan_result;
    DimensionalVariable_tan_result DimensionalVariable_tan(const dv_pyo3::capi::DimensionalVariable* self);

    typedef struct DimensionalVariable_asin_result {union {dv_pyo3::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_asin_result;
    DimensionalVariable_asin_result DimensionalVariable_asin(const dv_pyo3::capi::DimensionalVariable* self);

    typedef struct DimensionalVariable_acos_result {union {dv_pyo3::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_acos_result;
    DimensionalVariable_acos_result DimensionalVariable_acos(const dv_pyo3::capi::DimensionalVariable* self);

    typedef struct DimensionalVariable_atan_result {union {dv_pyo3::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_atan_result;
    DimensionalVariable_atan_result DimensionalVariable_atan(const dv_pyo3::capi::DimensionalVariable* self);

    dv_pyo3::capi::DimensionalVariable* DimensionalVariable_neg(const dv_pyo3::capi::DimensionalVariable* self);

    dv_pyo3::capi::DimensionalVariable* DimensionalVariable_abs(const dv_pyo3::capi::DimensionalVariable* self);

    bool DimensionalVariable_equals(const dv_pyo3::capi::DimensionalVariable* self, const dv_pyo3::capi::DimensionalVariable* other);

    bool DimensionalVariable_not_equals(const dv_pyo3::capi::DimensionalVariable* self, const dv_pyo3::capi::DimensionalVariable* other);

    typedef struct DimensionalVariable_less_than_result {union {bool ok; }; bool is_ok;} DimensionalVariable_less_than_result;
    DimensionalVariable_less_than_result DimensionalVariable_less_than(const dv_pyo3::capi::DimensionalVariable* self, const dv_pyo3::capi::DimensionalVariable* other);

    typedef struct DimensionalVariable_less_equal_result {union {bool ok; }; bool is_ok;} DimensionalVariable_less_equal_result;
    DimensionalVariable_less_equal_result DimensionalVariable_less_equal(const dv_pyo3::capi::DimensionalVariable* self, const dv_pyo3::capi::DimensionalVariable* other);

    typedef struct DimensionalVariable_greater_than_result {union {bool ok; }; bool is_ok;} DimensionalVariable_greater_than_result;
    DimensionalVariable_greater_than_result DimensionalVariable_greater_than(const dv_pyo3::capi::DimensionalVariable* self, const dv_pyo3::capi::DimensionalVariable* other);

    typedef struct DimensionalVariable_greater_equal_result {union {bool ok; }; bool is_ok;} DimensionalVariable_greater_equal_result;
    DimensionalVariable_greater_equal_result DimensionalVariable_greater_equal(const dv_pyo3::capi::DimensionalVariable* self, const dv_pyo3::capi::DimensionalVariable* other);

    dv_pyo3::capi::DimensionalVariable* DimensionalVariable_clone_var(const dv_pyo3::capi::DimensionalVariable* self);

    typedef struct DimensionalVariable_to_string_result { bool is_ok;} DimensionalVariable_to_string_result;
    DimensionalVariable_to_string_result DimensionalVariable_to_string(const dv_pyo3::capi::DimensionalVariable* self, dv_pyo3::diplomat::capi::DiplomatWrite* write);

    void DimensionalVariable_destroy(DimensionalVariable* self);

    } // extern "C"
} // namespace capi
} // namespace

inline dv_pyo3::diplomat::result<dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>, dv_pyo3::diplomat::Utf8Error> dv_pyo3::DimensionalVariable::new_(double value, std::string_view unit) {
    if (!dv_pyo3::diplomat::capi::diplomat_is_str(unit.data(), unit.size())) {
    return dv_pyo3::diplomat::Err<dv_pyo3::diplomat::Utf8Error>();
  }
    auto result = dv_pyo3::capi::DimensionalVariable_new(value,
        {unit.data(), unit.size()});
    return dv_pyo3::diplomat::Ok<dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>>(result.is_ok ? dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Ok<std::unique_ptr<dv_pyo3::DimensionalVariable>>(std::unique_ptr<dv_pyo3::DimensionalVariable>(dv_pyo3::DimensionalVariable::FromFFI(result.ok)))) : dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Err<std::monostate>()));
}

inline dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> dv_pyo3::DimensionalVariable::asin_scalar(double x) {
    auto result = dv_pyo3::capi::DimensionalVariable_asin_scalar(x);
    return result.is_ok ? dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Ok<std::unique_ptr<dv_pyo3::DimensionalVariable>>(std::unique_ptr<dv_pyo3::DimensionalVariable>(dv_pyo3::DimensionalVariable::FromFFI(result.ok)))) : dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Err<std::monostate>());
}

inline dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> dv_pyo3::DimensionalVariable::acos_scalar(double x) {
    auto result = dv_pyo3::capi::DimensionalVariable_acos_scalar(x);
    return result.is_ok ? dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Ok<std::unique_ptr<dv_pyo3::DimensionalVariable>>(std::unique_ptr<dv_pyo3::DimensionalVariable>(dv_pyo3::DimensionalVariable::FromFFI(result.ok)))) : dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Err<std::monostate>());
}

inline dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> dv_pyo3::DimensionalVariable::atan_scalar(double x) {
    auto result = dv_pyo3::capi::DimensionalVariable_atan_scalar(x);
    return result.is_ok ? dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Ok<std::unique_ptr<dv_pyo3::DimensionalVariable>>(std::unique_ptr<dv_pyo3::DimensionalVariable>(dv_pyo3::DimensionalVariable::FromFFI(result.ok)))) : dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Err<std::monostate>());
}

inline size_t dv_pyo3::DimensionalVariable::base_units_size() {
    auto result = dv_pyo3::capi::DimensionalVariable_base_units_size();
    return result;
}

inline double dv_pyo3::DimensionalVariable::value() const {
    auto result = dv_pyo3::capi::DimensionalVariable_value(this->AsFFI());
    return result;
}

inline dv_pyo3::diplomat::result<dv_pyo3::diplomat::result<double, std::monostate>, dv_pyo3::diplomat::Utf8Error> dv_pyo3::DimensionalVariable::value_in(std::string_view unit) const {
    if (!dv_pyo3::diplomat::capi::diplomat_is_str(unit.data(), unit.size())) {
    return dv_pyo3::diplomat::Err<dv_pyo3::diplomat::Utf8Error>();
  }
    auto result = dv_pyo3::capi::DimensionalVariable_value_in(this->AsFFI(),
        {unit.data(), unit.size()});
    return dv_pyo3::diplomat::Ok<dv_pyo3::diplomat::result<double, std::monostate>>(result.is_ok ? dv_pyo3::diplomat::result<double, std::monostate>(dv_pyo3::diplomat::Ok<double>(result.ok)) : dv_pyo3::diplomat::result<double, std::monostate>(dv_pyo3::diplomat::Err<std::monostate>()));
}

inline bool dv_pyo3::DimensionalVariable::is_unitless() const {
    auto result = dv_pyo3::capi::DimensionalVariable_is_unitless(this->AsFFI());
    return result;
}

inline dv_pyo3::BaseUnits dv_pyo3::DimensionalVariable::base_units() const {
    auto result = dv_pyo3::capi::DimensionalVariable_base_units(this->AsFFI());
    return dv_pyo3::BaseUnits::FromFFI(result);
}

inline dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> dv_pyo3::DimensionalVariable::add(const dv_pyo3::DimensionalVariable& other) const {
    auto result = dv_pyo3::capi::DimensionalVariable_add(this->AsFFI(),
        other.AsFFI());
    return result.is_ok ? dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Ok<std::unique_ptr<dv_pyo3::DimensionalVariable>>(std::unique_ptr<dv_pyo3::DimensionalVariable>(dv_pyo3::DimensionalVariable::FromFFI(result.ok)))) : dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Err<std::monostate>());
}

inline dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> dv_pyo3::DimensionalVariable::sub(const dv_pyo3::DimensionalVariable& other) const {
    auto result = dv_pyo3::capi::DimensionalVariable_sub(this->AsFFI(),
        other.AsFFI());
    return result.is_ok ? dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Ok<std::unique_ptr<dv_pyo3::DimensionalVariable>>(std::unique_ptr<dv_pyo3::DimensionalVariable>(dv_pyo3::DimensionalVariable::FromFFI(result.ok)))) : dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Err<std::monostate>());
}

inline std::unique_ptr<dv_pyo3::DimensionalVariable> dv_pyo3::DimensionalVariable::mul(const dv_pyo3::DimensionalVariable& other) const {
    auto result = dv_pyo3::capi::DimensionalVariable_mul(this->AsFFI(),
        other.AsFFI());
    return std::unique_ptr<dv_pyo3::DimensionalVariable>(dv_pyo3::DimensionalVariable::FromFFI(result));
}

inline std::unique_ptr<dv_pyo3::DimensionalVariable> dv_pyo3::DimensionalVariable::div(const dv_pyo3::DimensionalVariable& other) const {
    auto result = dv_pyo3::capi::DimensionalVariable_div(this->AsFFI(),
        other.AsFFI());
    return std::unique_ptr<dv_pyo3::DimensionalVariable>(dv_pyo3::DimensionalVariable::FromFFI(result));
}

inline std::unique_ptr<dv_pyo3::DimensionalVariable> dv_pyo3::DimensionalVariable::mul_scalar(double scalar) const {
    auto result = dv_pyo3::capi::DimensionalVariable_mul_scalar(this->AsFFI(),
        scalar);
    return std::unique_ptr<dv_pyo3::DimensionalVariable>(dv_pyo3::DimensionalVariable::FromFFI(result));
}

inline std::unique_ptr<dv_pyo3::DimensionalVariable> dv_pyo3::DimensionalVariable::div_scalar(double scalar) const {
    auto result = dv_pyo3::capi::DimensionalVariable_div_scalar(this->AsFFI(),
        scalar);
    return std::unique_ptr<dv_pyo3::DimensionalVariable>(dv_pyo3::DimensionalVariable::FromFFI(result));
}

inline std::unique_ptr<dv_pyo3::DimensionalVariable> dv_pyo3::DimensionalVariable::rdiv_scalar(double scalar) const {
    auto result = dv_pyo3::capi::DimensionalVariable_rdiv_scalar(this->AsFFI(),
        scalar);
    return std::unique_ptr<dv_pyo3::DimensionalVariable>(dv_pyo3::DimensionalVariable::FromFFI(result));
}

inline std::unique_ptr<dv_pyo3::DimensionalVariable> dv_pyo3::DimensionalVariable::powi(int32_t exp) const {
    auto result = dv_pyo3::capi::DimensionalVariable_powi(this->AsFFI(),
        exp);
    return std::unique_ptr<dv_pyo3::DimensionalVariable>(dv_pyo3::DimensionalVariable::FromFFI(result));
}

inline dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> dv_pyo3::DimensionalVariable::powf(double exp) const {
    auto result = dv_pyo3::capi::DimensionalVariable_powf(this->AsFFI(),
        exp);
    return result.is_ok ? dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Ok<std::unique_ptr<dv_pyo3::DimensionalVariable>>(std::unique_ptr<dv_pyo3::DimensionalVariable>(dv_pyo3::DimensionalVariable::FromFFI(result.ok)))) : dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Err<std::monostate>());
}

inline dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> dv_pyo3::DimensionalVariable::sqrt() const {
    auto result = dv_pyo3::capi::DimensionalVariable_sqrt(this->AsFFI());
    return result.is_ok ? dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Ok<std::unique_ptr<dv_pyo3::DimensionalVariable>>(std::unique_ptr<dv_pyo3::DimensionalVariable>(dv_pyo3::DimensionalVariable::FromFFI(result.ok)))) : dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Err<std::monostate>());
}

inline dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> dv_pyo3::DimensionalVariable::ln() const {
    auto result = dv_pyo3::capi::DimensionalVariable_ln(this->AsFFI());
    return result.is_ok ? dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Ok<std::unique_ptr<dv_pyo3::DimensionalVariable>>(std::unique_ptr<dv_pyo3::DimensionalVariable>(dv_pyo3::DimensionalVariable::FromFFI(result.ok)))) : dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Err<std::monostate>());
}

inline dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> dv_pyo3::DimensionalVariable::log2() const {
    auto result = dv_pyo3::capi::DimensionalVariable_log2(this->AsFFI());
    return result.is_ok ? dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Ok<std::unique_ptr<dv_pyo3::DimensionalVariable>>(std::unique_ptr<dv_pyo3::DimensionalVariable>(dv_pyo3::DimensionalVariable::FromFFI(result.ok)))) : dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Err<std::monostate>());
}

inline dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> dv_pyo3::DimensionalVariable::log10() const {
    auto result = dv_pyo3::capi::DimensionalVariable_log10(this->AsFFI());
    return result.is_ok ? dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Ok<std::unique_ptr<dv_pyo3::DimensionalVariable>>(std::unique_ptr<dv_pyo3::DimensionalVariable>(dv_pyo3::DimensionalVariable::FromFFI(result.ok)))) : dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Err<std::monostate>());
}

inline dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> dv_pyo3::DimensionalVariable::sin() const {
    auto result = dv_pyo3::capi::DimensionalVariable_sin(this->AsFFI());
    return result.is_ok ? dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Ok<std::unique_ptr<dv_pyo3::DimensionalVariable>>(std::unique_ptr<dv_pyo3::DimensionalVariable>(dv_pyo3::DimensionalVariable::FromFFI(result.ok)))) : dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Err<std::monostate>());
}

inline dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> dv_pyo3::DimensionalVariable::cos() const {
    auto result = dv_pyo3::capi::DimensionalVariable_cos(this->AsFFI());
    return result.is_ok ? dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Ok<std::unique_ptr<dv_pyo3::DimensionalVariable>>(std::unique_ptr<dv_pyo3::DimensionalVariable>(dv_pyo3::DimensionalVariable::FromFFI(result.ok)))) : dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Err<std::monostate>());
}

inline dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> dv_pyo3::DimensionalVariable::tan() const {
    auto result = dv_pyo3::capi::DimensionalVariable_tan(this->AsFFI());
    return result.is_ok ? dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Ok<std::unique_ptr<dv_pyo3::DimensionalVariable>>(std::unique_ptr<dv_pyo3::DimensionalVariable>(dv_pyo3::DimensionalVariable::FromFFI(result.ok)))) : dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Err<std::monostate>());
}

inline dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> dv_pyo3::DimensionalVariable::asin() const {
    auto result = dv_pyo3::capi::DimensionalVariable_asin(this->AsFFI());
    return result.is_ok ? dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Ok<std::unique_ptr<dv_pyo3::DimensionalVariable>>(std::unique_ptr<dv_pyo3::DimensionalVariable>(dv_pyo3::DimensionalVariable::FromFFI(result.ok)))) : dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Err<std::monostate>());
}

inline dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> dv_pyo3::DimensionalVariable::acos() const {
    auto result = dv_pyo3::capi::DimensionalVariable_acos(this->AsFFI());
    return result.is_ok ? dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Ok<std::unique_ptr<dv_pyo3::DimensionalVariable>>(std::unique_ptr<dv_pyo3::DimensionalVariable>(dv_pyo3::DimensionalVariable::FromFFI(result.ok)))) : dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Err<std::monostate>());
}

inline dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> dv_pyo3::DimensionalVariable::atan() const {
    auto result = dv_pyo3::capi::DimensionalVariable_atan(this->AsFFI());
    return result.is_ok ? dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Ok<std::unique_ptr<dv_pyo3::DimensionalVariable>>(std::unique_ptr<dv_pyo3::DimensionalVariable>(dv_pyo3::DimensionalVariable::FromFFI(result.ok)))) : dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>(dv_pyo3::diplomat::Err<std::monostate>());
}

inline std::unique_ptr<dv_pyo3::DimensionalVariable> dv_pyo3::DimensionalVariable::neg() const {
    auto result = dv_pyo3::capi::DimensionalVariable_neg(this->AsFFI());
    return std::unique_ptr<dv_pyo3::DimensionalVariable>(dv_pyo3::DimensionalVariable::FromFFI(result));
}

inline std::unique_ptr<dv_pyo3::DimensionalVariable> dv_pyo3::DimensionalVariable::abs() const {
    auto result = dv_pyo3::capi::DimensionalVariable_abs(this->AsFFI());
    return std::unique_ptr<dv_pyo3::DimensionalVariable>(dv_pyo3::DimensionalVariable::FromFFI(result));
}

inline bool dv_pyo3::DimensionalVariable::equals(const dv_pyo3::DimensionalVariable& other) const {
    auto result = dv_pyo3::capi::DimensionalVariable_equals(this->AsFFI(),
        other.AsFFI());
    return result;
}

inline bool dv_pyo3::DimensionalVariable::not_equals(const dv_pyo3::DimensionalVariable& other) const {
    auto result = dv_pyo3::capi::DimensionalVariable_not_equals(this->AsFFI(),
        other.AsFFI());
    return result;
}

inline dv_pyo3::diplomat::result<bool, std::monostate> dv_pyo3::DimensionalVariable::less_than(const dv_pyo3::DimensionalVariable& other) const {
    auto result = dv_pyo3::capi::DimensionalVariable_less_than(this->AsFFI(),
        other.AsFFI());
    return result.is_ok ? dv_pyo3::diplomat::result<bool, std::monostate>(dv_pyo3::diplomat::Ok<bool>(result.ok)) : dv_pyo3::diplomat::result<bool, std::monostate>(dv_pyo3::diplomat::Err<std::monostate>());
}

inline dv_pyo3::diplomat::result<bool, std::monostate> dv_pyo3::DimensionalVariable::less_equal(const dv_pyo3::DimensionalVariable& other) const {
    auto result = dv_pyo3::capi::DimensionalVariable_less_equal(this->AsFFI(),
        other.AsFFI());
    return result.is_ok ? dv_pyo3::diplomat::result<bool, std::monostate>(dv_pyo3::diplomat::Ok<bool>(result.ok)) : dv_pyo3::diplomat::result<bool, std::monostate>(dv_pyo3::diplomat::Err<std::monostate>());
}

inline dv_pyo3::diplomat::result<bool, std::monostate> dv_pyo3::DimensionalVariable::greater_than(const dv_pyo3::DimensionalVariable& other) const {
    auto result = dv_pyo3::capi::DimensionalVariable_greater_than(this->AsFFI(),
        other.AsFFI());
    return result.is_ok ? dv_pyo3::diplomat::result<bool, std::monostate>(dv_pyo3::diplomat::Ok<bool>(result.ok)) : dv_pyo3::diplomat::result<bool, std::monostate>(dv_pyo3::diplomat::Err<std::monostate>());
}

inline dv_pyo3::diplomat::result<bool, std::monostate> dv_pyo3::DimensionalVariable::greater_equal(const dv_pyo3::DimensionalVariable& other) const {
    auto result = dv_pyo3::capi::DimensionalVariable_greater_equal(this->AsFFI(),
        other.AsFFI());
    return result.is_ok ? dv_pyo3::diplomat::result<bool, std::monostate>(dv_pyo3::diplomat::Ok<bool>(result.ok)) : dv_pyo3::diplomat::result<bool, std::monostate>(dv_pyo3::diplomat::Err<std::monostate>());
}

inline std::unique_ptr<dv_pyo3::DimensionalVariable> dv_pyo3::DimensionalVariable::clone_var() const {
    auto result = dv_pyo3::capi::DimensionalVariable_clone_var(this->AsFFI());
    return std::unique_ptr<dv_pyo3::DimensionalVariable>(dv_pyo3::DimensionalVariable::FromFFI(result));
}

inline dv_pyo3::diplomat::result<std::string, std::monostate> dv_pyo3::DimensionalVariable::to_string() const {
    std::string output;
    dv_pyo3::diplomat::capi::DiplomatWrite write = dv_pyo3::diplomat::WriteFromString(output);
    auto result = dv_pyo3::capi::DimensionalVariable_to_string(this->AsFFI(),
        &write);
    return result.is_ok ? dv_pyo3::diplomat::result<std::string, std::monostate>(dv_pyo3::diplomat::Ok<std::string>(std::move(output))) : dv_pyo3::diplomat::result<std::string, std::monostate>(dv_pyo3::diplomat::Err<std::monostate>());
}
template<typename W>
inline dv_pyo3::diplomat::result<std::monostate, std::monostate> dv_pyo3::DimensionalVariable::to_string_write(W& writeable) const {
    dv_pyo3::diplomat::capi::DiplomatWrite write = dv_pyo3::diplomat::WriteTrait<W>::Construct(writeable);
    auto result = dv_pyo3::capi::DimensionalVariable_to_string(this->AsFFI(),
        &write);
    return result.is_ok ? dv_pyo3::diplomat::result<std::monostate, std::monostate>(dv_pyo3::diplomat::Ok<std::monostate>()) : dv_pyo3::diplomat::result<std::monostate, std::monostate>(dv_pyo3::diplomat::Err<std::monostate>());
}

inline const dv_pyo3::capi::DimensionalVariable* dv_pyo3::DimensionalVariable::AsFFI() const {
    return reinterpret_cast<const dv_pyo3::capi::DimensionalVariable*>(this);
}

inline dv_pyo3::capi::DimensionalVariable* dv_pyo3::DimensionalVariable::AsFFI() {
    return reinterpret_cast<dv_pyo3::capi::DimensionalVariable*>(this);
}

inline const dv_pyo3::DimensionalVariable* dv_pyo3::DimensionalVariable::FromFFI(const dv_pyo3::capi::DimensionalVariable* ptr) {
    return reinterpret_cast<const dv_pyo3::DimensionalVariable*>(ptr);
}

inline dv_pyo3::DimensionalVariable* dv_pyo3::DimensionalVariable::FromFFI(dv_pyo3::capi::DimensionalVariable* ptr) {
    return reinterpret_cast<dv_pyo3::DimensionalVariable*>(ptr);
}

inline void dv_pyo3::DimensionalVariable::operator delete(void* ptr) {
    dv_pyo3::capi::DimensionalVariable_destroy(reinterpret_cast<dv_pyo3::capi::DimensionalVariable*>(ptr));
}


#endif // DV_PYO3_DimensionalVariable_HPP
