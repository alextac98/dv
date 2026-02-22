#ifndef DimensionalVariable_HPP
#define DimensionalVariable_HPP

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


namespace diplomat {
namespace capi {
    extern "C" {

    typedef struct DimensionalVariable_new_result {union {diplomat::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_new_result;
    DimensionalVariable_new_result DimensionalVariable_new(double value, diplomat::capi::DiplomatStringView unit);

    typedef struct DimensionalVariable_asin_scalar_result {union {diplomat::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_asin_scalar_result;
    DimensionalVariable_asin_scalar_result DimensionalVariable_asin_scalar(double x);

    typedef struct DimensionalVariable_acos_scalar_result {union {diplomat::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_acos_scalar_result;
    DimensionalVariable_acos_scalar_result DimensionalVariable_acos_scalar(double x);

    typedef struct DimensionalVariable_atan_scalar_result {union {diplomat::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_atan_scalar_result;
    DimensionalVariable_atan_scalar_result DimensionalVariable_atan_scalar(double x);

    size_t DimensionalVariable_base_units_size(void);

    double DimensionalVariable_value(const diplomat::capi::DimensionalVariable* self);

    typedef struct DimensionalVariable_value_in_result {union {double ok; }; bool is_ok;} DimensionalVariable_value_in_result;
    DimensionalVariable_value_in_result DimensionalVariable_value_in(const diplomat::capi::DimensionalVariable* self, diplomat::capi::DiplomatStringView unit);

    bool DimensionalVariable_is_unitless(const diplomat::capi::DimensionalVariable* self);

    diplomat::capi::BaseUnits DimensionalVariable_base_units(const diplomat::capi::DimensionalVariable* self);

    typedef struct DimensionalVariable_add_result {union {diplomat::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_add_result;
    DimensionalVariable_add_result DimensionalVariable_add(const diplomat::capi::DimensionalVariable* self, const diplomat::capi::DimensionalVariable* other);

    typedef struct DimensionalVariable_sub_result {union {diplomat::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_sub_result;
    DimensionalVariable_sub_result DimensionalVariable_sub(const diplomat::capi::DimensionalVariable* self, const diplomat::capi::DimensionalVariable* other);

    diplomat::capi::DimensionalVariable* DimensionalVariable_mul(const diplomat::capi::DimensionalVariable* self, const diplomat::capi::DimensionalVariable* other);

    diplomat::capi::DimensionalVariable* DimensionalVariable_div(const diplomat::capi::DimensionalVariable* self, const diplomat::capi::DimensionalVariable* other);

    diplomat::capi::DimensionalVariable* DimensionalVariable_mul_scalar(const diplomat::capi::DimensionalVariable* self, double scalar);

    diplomat::capi::DimensionalVariable* DimensionalVariable_div_scalar(const diplomat::capi::DimensionalVariable* self, double scalar);

    diplomat::capi::DimensionalVariable* DimensionalVariable_rdiv_scalar(const diplomat::capi::DimensionalVariable* self, double scalar);

    diplomat::capi::DimensionalVariable* DimensionalVariable_powi(const diplomat::capi::DimensionalVariable* self, int32_t exp);

    typedef struct DimensionalVariable_powf_result {union {diplomat::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_powf_result;
    DimensionalVariable_powf_result DimensionalVariable_powf(const diplomat::capi::DimensionalVariable* self, double exp);

    typedef struct DimensionalVariable_sqrt_result {union {diplomat::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_sqrt_result;
    DimensionalVariable_sqrt_result DimensionalVariable_sqrt(const diplomat::capi::DimensionalVariable* self);

    typedef struct DimensionalVariable_ln_result {union {diplomat::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_ln_result;
    DimensionalVariable_ln_result DimensionalVariable_ln(const diplomat::capi::DimensionalVariable* self);

    typedef struct DimensionalVariable_log2_result {union {diplomat::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_log2_result;
    DimensionalVariable_log2_result DimensionalVariable_log2(const diplomat::capi::DimensionalVariable* self);

    typedef struct DimensionalVariable_log10_result {union {diplomat::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_log10_result;
    DimensionalVariable_log10_result DimensionalVariable_log10(const diplomat::capi::DimensionalVariable* self);

    typedef struct DimensionalVariable_sin_result {union {diplomat::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_sin_result;
    DimensionalVariable_sin_result DimensionalVariable_sin(const diplomat::capi::DimensionalVariable* self);

    typedef struct DimensionalVariable_cos_result {union {diplomat::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_cos_result;
    DimensionalVariable_cos_result DimensionalVariable_cos(const diplomat::capi::DimensionalVariable* self);

    typedef struct DimensionalVariable_tan_result {union {diplomat::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_tan_result;
    DimensionalVariable_tan_result DimensionalVariable_tan(const diplomat::capi::DimensionalVariable* self);

    typedef struct DimensionalVariable_asin_result {union {diplomat::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_asin_result;
    DimensionalVariable_asin_result DimensionalVariable_asin(const diplomat::capi::DimensionalVariable* self);

    typedef struct DimensionalVariable_acos_result {union {diplomat::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_acos_result;
    DimensionalVariable_acos_result DimensionalVariable_acos(const diplomat::capi::DimensionalVariable* self);

    typedef struct DimensionalVariable_atan_result {union {diplomat::capi::DimensionalVariable* ok; }; bool is_ok;} DimensionalVariable_atan_result;
    DimensionalVariable_atan_result DimensionalVariable_atan(const diplomat::capi::DimensionalVariable* self);

    diplomat::capi::DimensionalVariable* DimensionalVariable_neg(const diplomat::capi::DimensionalVariable* self);

    diplomat::capi::DimensionalVariable* DimensionalVariable_abs(const diplomat::capi::DimensionalVariable* self);

    bool DimensionalVariable_equals(const diplomat::capi::DimensionalVariable* self, const diplomat::capi::DimensionalVariable* other);

    bool DimensionalVariable_not_equals(const diplomat::capi::DimensionalVariable* self, const diplomat::capi::DimensionalVariable* other);

    typedef struct DimensionalVariable_less_than_result {union {bool ok; }; bool is_ok;} DimensionalVariable_less_than_result;
    DimensionalVariable_less_than_result DimensionalVariable_less_than(const diplomat::capi::DimensionalVariable* self, const diplomat::capi::DimensionalVariable* other);

    typedef struct DimensionalVariable_less_equal_result {union {bool ok; }; bool is_ok;} DimensionalVariable_less_equal_result;
    DimensionalVariable_less_equal_result DimensionalVariable_less_equal(const diplomat::capi::DimensionalVariable* self, const diplomat::capi::DimensionalVariable* other);

    typedef struct DimensionalVariable_greater_than_result {union {bool ok; }; bool is_ok;} DimensionalVariable_greater_than_result;
    DimensionalVariable_greater_than_result DimensionalVariable_greater_than(const diplomat::capi::DimensionalVariable* self, const diplomat::capi::DimensionalVariable* other);

    typedef struct DimensionalVariable_greater_equal_result {union {bool ok; }; bool is_ok;} DimensionalVariable_greater_equal_result;
    DimensionalVariable_greater_equal_result DimensionalVariable_greater_equal(const diplomat::capi::DimensionalVariable* self, const diplomat::capi::DimensionalVariable* other);

    diplomat::capi::DimensionalVariable* DimensionalVariable_clone_var(const diplomat::capi::DimensionalVariable* self);

    typedef struct DimensionalVariable_to_string_result { bool is_ok;} DimensionalVariable_to_string_result;
    DimensionalVariable_to_string_result DimensionalVariable_to_string(const diplomat::capi::DimensionalVariable* self, diplomat::capi::DiplomatWrite* write);

    void DimensionalVariable_destroy(DimensionalVariable* self);

    } // extern "C"
} // namespace capi
} // namespace

inline diplomat::result<diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>, diplomat::Utf8Error> DimensionalVariable::new_(double value, std::string_view unit) {
    if (!diplomat::capi::diplomat_is_str(unit.data(), unit.size())) {
    return diplomat::Err<diplomat::Utf8Error>();
  }
    auto result = diplomat::capi::DimensionalVariable_new(value,
        {unit.data(), unit.size()});
    return diplomat::Ok<diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>>(result.is_ok ? diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Ok<std::unique_ptr<DimensionalVariable>>(std::unique_ptr<DimensionalVariable>(DimensionalVariable::FromFFI(result.ok)))) : diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Err<std::monostate>()));
}

inline diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> DimensionalVariable::asin_scalar(double x) {
    auto result = diplomat::capi::DimensionalVariable_asin_scalar(x);
    return result.is_ok ? diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Ok<std::unique_ptr<DimensionalVariable>>(std::unique_ptr<DimensionalVariable>(DimensionalVariable::FromFFI(result.ok)))) : diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Err<std::monostate>());
}

inline diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> DimensionalVariable::acos_scalar(double x) {
    auto result = diplomat::capi::DimensionalVariable_acos_scalar(x);
    return result.is_ok ? diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Ok<std::unique_ptr<DimensionalVariable>>(std::unique_ptr<DimensionalVariable>(DimensionalVariable::FromFFI(result.ok)))) : diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Err<std::monostate>());
}

inline diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> DimensionalVariable::atan_scalar(double x) {
    auto result = diplomat::capi::DimensionalVariable_atan_scalar(x);
    return result.is_ok ? diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Ok<std::unique_ptr<DimensionalVariable>>(std::unique_ptr<DimensionalVariable>(DimensionalVariable::FromFFI(result.ok)))) : diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Err<std::monostate>());
}

inline size_t DimensionalVariable::base_units_size() {
    auto result = diplomat::capi::DimensionalVariable_base_units_size();
    return result;
}

inline double DimensionalVariable::value() const {
    auto result = diplomat::capi::DimensionalVariable_value(this->AsFFI());
    return result;
}

inline diplomat::result<diplomat::result<double, std::monostate>, diplomat::Utf8Error> DimensionalVariable::value_in(std::string_view unit) const {
    if (!diplomat::capi::diplomat_is_str(unit.data(), unit.size())) {
    return diplomat::Err<diplomat::Utf8Error>();
  }
    auto result = diplomat::capi::DimensionalVariable_value_in(this->AsFFI(),
        {unit.data(), unit.size()});
    return diplomat::Ok<diplomat::result<double, std::monostate>>(result.is_ok ? diplomat::result<double, std::monostate>(diplomat::Ok<double>(result.ok)) : diplomat::result<double, std::monostate>(diplomat::Err<std::monostate>()));
}

inline bool DimensionalVariable::is_unitless() const {
    auto result = diplomat::capi::DimensionalVariable_is_unitless(this->AsFFI());
    return result;
}

inline BaseUnits DimensionalVariable::base_units() const {
    auto result = diplomat::capi::DimensionalVariable_base_units(this->AsFFI());
    return BaseUnits::FromFFI(result);
}

inline diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> DimensionalVariable::add(const DimensionalVariable& other) const {
    auto result = diplomat::capi::DimensionalVariable_add(this->AsFFI(),
        other.AsFFI());
    return result.is_ok ? diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Ok<std::unique_ptr<DimensionalVariable>>(std::unique_ptr<DimensionalVariable>(DimensionalVariable::FromFFI(result.ok)))) : diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Err<std::monostate>());
}

inline diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> DimensionalVariable::sub(const DimensionalVariable& other) const {
    auto result = diplomat::capi::DimensionalVariable_sub(this->AsFFI(),
        other.AsFFI());
    return result.is_ok ? diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Ok<std::unique_ptr<DimensionalVariable>>(std::unique_ptr<DimensionalVariable>(DimensionalVariable::FromFFI(result.ok)))) : diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Err<std::monostate>());
}

inline std::unique_ptr<DimensionalVariable> DimensionalVariable::mul(const DimensionalVariable& other) const {
    auto result = diplomat::capi::DimensionalVariable_mul(this->AsFFI(),
        other.AsFFI());
    return std::unique_ptr<DimensionalVariable>(DimensionalVariable::FromFFI(result));
}

inline std::unique_ptr<DimensionalVariable> DimensionalVariable::div(const DimensionalVariable& other) const {
    auto result = diplomat::capi::DimensionalVariable_div(this->AsFFI(),
        other.AsFFI());
    return std::unique_ptr<DimensionalVariable>(DimensionalVariable::FromFFI(result));
}

inline std::unique_ptr<DimensionalVariable> DimensionalVariable::mul_scalar(double scalar) const {
    auto result = diplomat::capi::DimensionalVariable_mul_scalar(this->AsFFI(),
        scalar);
    return std::unique_ptr<DimensionalVariable>(DimensionalVariable::FromFFI(result));
}

inline std::unique_ptr<DimensionalVariable> DimensionalVariable::div_scalar(double scalar) const {
    auto result = diplomat::capi::DimensionalVariable_div_scalar(this->AsFFI(),
        scalar);
    return std::unique_ptr<DimensionalVariable>(DimensionalVariable::FromFFI(result));
}

inline std::unique_ptr<DimensionalVariable> DimensionalVariable::rdiv_scalar(double scalar) const {
    auto result = diplomat::capi::DimensionalVariable_rdiv_scalar(this->AsFFI(),
        scalar);
    return std::unique_ptr<DimensionalVariable>(DimensionalVariable::FromFFI(result));
}

inline std::unique_ptr<DimensionalVariable> DimensionalVariable::powi(int32_t exp) const {
    auto result = diplomat::capi::DimensionalVariable_powi(this->AsFFI(),
        exp);
    return std::unique_ptr<DimensionalVariable>(DimensionalVariable::FromFFI(result));
}

inline diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> DimensionalVariable::powf(double exp) const {
    auto result = diplomat::capi::DimensionalVariable_powf(this->AsFFI(),
        exp);
    return result.is_ok ? diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Ok<std::unique_ptr<DimensionalVariable>>(std::unique_ptr<DimensionalVariable>(DimensionalVariable::FromFFI(result.ok)))) : diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Err<std::monostate>());
}

inline diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> DimensionalVariable::sqrt() const {
    auto result = diplomat::capi::DimensionalVariable_sqrt(this->AsFFI());
    return result.is_ok ? diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Ok<std::unique_ptr<DimensionalVariable>>(std::unique_ptr<DimensionalVariable>(DimensionalVariable::FromFFI(result.ok)))) : diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Err<std::monostate>());
}

inline diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> DimensionalVariable::ln() const {
    auto result = diplomat::capi::DimensionalVariable_ln(this->AsFFI());
    return result.is_ok ? diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Ok<std::unique_ptr<DimensionalVariable>>(std::unique_ptr<DimensionalVariable>(DimensionalVariable::FromFFI(result.ok)))) : diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Err<std::monostate>());
}

inline diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> DimensionalVariable::log2() const {
    auto result = diplomat::capi::DimensionalVariable_log2(this->AsFFI());
    return result.is_ok ? diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Ok<std::unique_ptr<DimensionalVariable>>(std::unique_ptr<DimensionalVariable>(DimensionalVariable::FromFFI(result.ok)))) : diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Err<std::monostate>());
}

inline diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> DimensionalVariable::log10() const {
    auto result = diplomat::capi::DimensionalVariable_log10(this->AsFFI());
    return result.is_ok ? diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Ok<std::unique_ptr<DimensionalVariable>>(std::unique_ptr<DimensionalVariable>(DimensionalVariable::FromFFI(result.ok)))) : diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Err<std::monostate>());
}

inline diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> DimensionalVariable::sin() const {
    auto result = diplomat::capi::DimensionalVariable_sin(this->AsFFI());
    return result.is_ok ? diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Ok<std::unique_ptr<DimensionalVariable>>(std::unique_ptr<DimensionalVariable>(DimensionalVariable::FromFFI(result.ok)))) : diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Err<std::monostate>());
}

inline diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> DimensionalVariable::cos() const {
    auto result = diplomat::capi::DimensionalVariable_cos(this->AsFFI());
    return result.is_ok ? diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Ok<std::unique_ptr<DimensionalVariable>>(std::unique_ptr<DimensionalVariable>(DimensionalVariable::FromFFI(result.ok)))) : diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Err<std::monostate>());
}

inline diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> DimensionalVariable::tan() const {
    auto result = diplomat::capi::DimensionalVariable_tan(this->AsFFI());
    return result.is_ok ? diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Ok<std::unique_ptr<DimensionalVariable>>(std::unique_ptr<DimensionalVariable>(DimensionalVariable::FromFFI(result.ok)))) : diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Err<std::monostate>());
}

inline diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> DimensionalVariable::asin() const {
    auto result = diplomat::capi::DimensionalVariable_asin(this->AsFFI());
    return result.is_ok ? diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Ok<std::unique_ptr<DimensionalVariable>>(std::unique_ptr<DimensionalVariable>(DimensionalVariable::FromFFI(result.ok)))) : diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Err<std::monostate>());
}

inline diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> DimensionalVariable::acos() const {
    auto result = diplomat::capi::DimensionalVariable_acos(this->AsFFI());
    return result.is_ok ? diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Ok<std::unique_ptr<DimensionalVariable>>(std::unique_ptr<DimensionalVariable>(DimensionalVariable::FromFFI(result.ok)))) : diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Err<std::monostate>());
}

inline diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> DimensionalVariable::atan() const {
    auto result = diplomat::capi::DimensionalVariable_atan(this->AsFFI());
    return result.is_ok ? diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Ok<std::unique_ptr<DimensionalVariable>>(std::unique_ptr<DimensionalVariable>(DimensionalVariable::FromFFI(result.ok)))) : diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>(diplomat::Err<std::monostate>());
}

inline std::unique_ptr<DimensionalVariable> DimensionalVariable::neg() const {
    auto result = diplomat::capi::DimensionalVariable_neg(this->AsFFI());
    return std::unique_ptr<DimensionalVariable>(DimensionalVariable::FromFFI(result));
}

inline std::unique_ptr<DimensionalVariable> DimensionalVariable::abs() const {
    auto result = diplomat::capi::DimensionalVariable_abs(this->AsFFI());
    return std::unique_ptr<DimensionalVariable>(DimensionalVariable::FromFFI(result));
}

inline bool DimensionalVariable::equals(const DimensionalVariable& other) const {
    auto result = diplomat::capi::DimensionalVariable_equals(this->AsFFI(),
        other.AsFFI());
    return result;
}

inline bool DimensionalVariable::not_equals(const DimensionalVariable& other) const {
    auto result = diplomat::capi::DimensionalVariable_not_equals(this->AsFFI(),
        other.AsFFI());
    return result;
}

inline diplomat::result<bool, std::monostate> DimensionalVariable::less_than(const DimensionalVariable& other) const {
    auto result = diplomat::capi::DimensionalVariable_less_than(this->AsFFI(),
        other.AsFFI());
    return result.is_ok ? diplomat::result<bool, std::monostate>(diplomat::Ok<bool>(result.ok)) : diplomat::result<bool, std::monostate>(diplomat::Err<std::monostate>());
}

inline diplomat::result<bool, std::monostate> DimensionalVariable::less_equal(const DimensionalVariable& other) const {
    auto result = diplomat::capi::DimensionalVariable_less_equal(this->AsFFI(),
        other.AsFFI());
    return result.is_ok ? diplomat::result<bool, std::monostate>(diplomat::Ok<bool>(result.ok)) : diplomat::result<bool, std::monostate>(diplomat::Err<std::monostate>());
}

inline diplomat::result<bool, std::monostate> DimensionalVariable::greater_than(const DimensionalVariable& other) const {
    auto result = diplomat::capi::DimensionalVariable_greater_than(this->AsFFI(),
        other.AsFFI());
    return result.is_ok ? diplomat::result<bool, std::monostate>(diplomat::Ok<bool>(result.ok)) : diplomat::result<bool, std::monostate>(diplomat::Err<std::monostate>());
}

inline diplomat::result<bool, std::monostate> DimensionalVariable::greater_equal(const DimensionalVariable& other) const {
    auto result = diplomat::capi::DimensionalVariable_greater_equal(this->AsFFI(),
        other.AsFFI());
    return result.is_ok ? diplomat::result<bool, std::monostate>(diplomat::Ok<bool>(result.ok)) : diplomat::result<bool, std::monostate>(diplomat::Err<std::monostate>());
}

inline std::unique_ptr<DimensionalVariable> DimensionalVariable::clone_var() const {
    auto result = diplomat::capi::DimensionalVariable_clone_var(this->AsFFI());
    return std::unique_ptr<DimensionalVariable>(DimensionalVariable::FromFFI(result));
}

inline diplomat::result<std::string, std::monostate> DimensionalVariable::to_string() const {
    std::string output;
    diplomat::capi::DiplomatWrite write = diplomat::WriteFromString(output);
    auto result = diplomat::capi::DimensionalVariable_to_string(this->AsFFI(),
        &write);
    return result.is_ok ? diplomat::result<std::string, std::monostate>(diplomat::Ok<std::string>(std::move(output))) : diplomat::result<std::string, std::monostate>(diplomat::Err<std::monostate>());
}
template<typename W>
inline diplomat::result<std::monostate, std::monostate> DimensionalVariable::to_string_write(W& writeable) const {
    diplomat::capi::DiplomatWrite write = diplomat::WriteTrait<W>::Construct(writeable);
    auto result = diplomat::capi::DimensionalVariable_to_string(this->AsFFI(),
        &write);
    return result.is_ok ? diplomat::result<std::monostate, std::monostate>(diplomat::Ok<std::monostate>()) : diplomat::result<std::monostate, std::monostate>(diplomat::Err<std::monostate>());
}

inline const diplomat::capi::DimensionalVariable* DimensionalVariable::AsFFI() const {
    return reinterpret_cast<const diplomat::capi::DimensionalVariable*>(this);
}

inline diplomat::capi::DimensionalVariable* DimensionalVariable::AsFFI() {
    return reinterpret_cast<diplomat::capi::DimensionalVariable*>(this);
}

inline const DimensionalVariable* DimensionalVariable::FromFFI(const diplomat::capi::DimensionalVariable* ptr) {
    return reinterpret_cast<const DimensionalVariable*>(ptr);
}

inline DimensionalVariable* DimensionalVariable::FromFFI(diplomat::capi::DimensionalVariable* ptr) {
    return reinterpret_cast<DimensionalVariable*>(ptr);
}

inline void DimensionalVariable::operator delete(void* ptr) {
    diplomat::capi::DimensionalVariable_destroy(reinterpret_cast<diplomat::capi::DimensionalVariable*>(ptr));
}


#endif // DimensionalVariable_HPP
