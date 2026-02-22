#ifndef DV_PYO3_DimensionalVariable_D_HPP
#define DV_PYO3_DimensionalVariable_D_HPP

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <memory>
#include <functional>
#include <optional>
#include <cstdlib>
#include "diplomat_runtime.hpp"
namespace dv_pyo3 {
namespace capi { struct DimensionalVariable; }
class DimensionalVariable;
struct BaseUnits;
} // namespace dv_pyo3



namespace dv_pyo3 {
namespace capi {
    struct DimensionalVariable;
} // namespace capi
} // namespace

namespace dv_pyo3 {
class DimensionalVariable {
public:

  inline static dv_pyo3::diplomat::result<dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate>, dv_pyo3::diplomat::Utf8Error> new_(double value, std::string_view unit);

  inline static dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> asin_scalar(double x);

  inline static dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> acos_scalar(double x);

  inline static dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> atan_scalar(double x);

  inline static size_t base_units_size();

  inline double value() const;

  inline dv_pyo3::diplomat::result<dv_pyo3::diplomat::result<double, std::monostate>, dv_pyo3::diplomat::Utf8Error> value_in(std::string_view unit) const;

  inline bool is_unitless() const;

  inline dv_pyo3::BaseUnits base_units() const;

  inline dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> add(const dv_pyo3::DimensionalVariable& other) const;

  inline dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> sub(const dv_pyo3::DimensionalVariable& other) const;

  inline std::unique_ptr<dv_pyo3::DimensionalVariable> mul(const dv_pyo3::DimensionalVariable& other) const;

  inline std::unique_ptr<dv_pyo3::DimensionalVariable> div(const dv_pyo3::DimensionalVariable& other) const;

  inline std::unique_ptr<dv_pyo3::DimensionalVariable> mul_scalar(double scalar) const;

  inline std::unique_ptr<dv_pyo3::DimensionalVariable> div_scalar(double scalar) const;

  inline std::unique_ptr<dv_pyo3::DimensionalVariable> rdiv_scalar(double scalar) const;

  inline std::unique_ptr<dv_pyo3::DimensionalVariable> powi(int32_t exp) const;

  inline dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> powf(double exp) const;

  inline dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> sqrt() const;

  inline dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> ln() const;

  inline dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> log2() const;

  inline dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> log10() const;

  inline dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> sin() const;

  inline dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> cos() const;

  inline dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> tan() const;

  inline dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> asin() const;

  inline dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> acos() const;

  inline dv_pyo3::diplomat::result<std::unique_ptr<dv_pyo3::DimensionalVariable>, std::monostate> atan() const;

  inline std::unique_ptr<dv_pyo3::DimensionalVariable> neg() const;

  inline std::unique_ptr<dv_pyo3::DimensionalVariable> abs() const;

  inline bool equals(const dv_pyo3::DimensionalVariable& other) const;

  inline bool not_equals(const dv_pyo3::DimensionalVariable& other) const;

  inline dv_pyo3::diplomat::result<bool, std::monostate> less_than(const dv_pyo3::DimensionalVariable& other) const;

  inline dv_pyo3::diplomat::result<bool, std::monostate> less_equal(const dv_pyo3::DimensionalVariable& other) const;

  inline dv_pyo3::diplomat::result<bool, std::monostate> greater_than(const dv_pyo3::DimensionalVariable& other) const;

  inline dv_pyo3::diplomat::result<bool, std::monostate> greater_equal(const dv_pyo3::DimensionalVariable& other) const;

  inline std::unique_ptr<dv_pyo3::DimensionalVariable> clone_var() const;

  inline dv_pyo3::diplomat::result<std::string, std::monostate> to_string() const;
  template<typename W>
  inline dv_pyo3::diplomat::result<std::monostate, std::monostate> to_string_write(W& writeable_output) const;

    inline const dv_pyo3::capi::DimensionalVariable* AsFFI() const;
    inline dv_pyo3::capi::DimensionalVariable* AsFFI();
    inline static const dv_pyo3::DimensionalVariable* FromFFI(const dv_pyo3::capi::DimensionalVariable* ptr);
    inline static dv_pyo3::DimensionalVariable* FromFFI(dv_pyo3::capi::DimensionalVariable* ptr);
    inline static void operator delete(void* ptr);
private:
    DimensionalVariable() = delete;
    DimensionalVariable(const dv_pyo3::DimensionalVariable&) = delete;
    DimensionalVariable(dv_pyo3::DimensionalVariable&&) noexcept = delete;
    DimensionalVariable operator=(const dv_pyo3::DimensionalVariable&) = delete;
    DimensionalVariable operator=(dv_pyo3::DimensionalVariable&&) noexcept = delete;
    static void operator delete[](void*, size_t) = delete;
};

} // namespace
#endif // DV_PYO3_DimensionalVariable_D_HPP
