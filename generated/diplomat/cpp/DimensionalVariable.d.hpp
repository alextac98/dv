#ifndef DimensionalVariable_D_HPP
#define DimensionalVariable_D_HPP

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <memory>
#include <functional>
#include <optional>
#include <cstdlib>
#include "diplomat_runtime.hpp"

struct BaseUnits;




namespace diplomat {
namespace capi {
    struct DimensionalVariable;
} // namespace capi
} // namespace

class DimensionalVariable {
public:

  inline static diplomat::result<diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate>, diplomat::Utf8Error> new_(double value, std::string_view unit);

  inline static diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> asin_scalar(double x);

  inline static diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> acos_scalar(double x);

  inline static diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> atan_scalar(double x);

  inline static size_t base_units_size();

  inline double value() const;

  inline diplomat::result<diplomat::result<double, std::monostate>, diplomat::Utf8Error> value_in(std::string_view unit) const;

  inline bool is_unitless() const;

  inline BaseUnits base_units() const;

  inline diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> add(const DimensionalVariable& other) const;

  inline diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> sub(const DimensionalVariable& other) const;

  inline std::unique_ptr<DimensionalVariable> mul(const DimensionalVariable& other) const;

  inline std::unique_ptr<DimensionalVariable> div(const DimensionalVariable& other) const;

  inline std::unique_ptr<DimensionalVariable> mul_scalar(double scalar) const;

  inline std::unique_ptr<DimensionalVariable> div_scalar(double scalar) const;

  inline std::unique_ptr<DimensionalVariable> rdiv_scalar(double scalar) const;

  inline std::unique_ptr<DimensionalVariable> powi(int32_t exp) const;

  inline diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> powf(double exp) const;

  inline diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> sqrt() const;

  inline diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> ln() const;

  inline diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> log2() const;

  inline diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> log10() const;

  inline diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> sin() const;

  inline diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> cos() const;

  inline diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> tan() const;

  inline diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> asin() const;

  inline diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> acos() const;

  inline diplomat::result<std::unique_ptr<DimensionalVariable>, std::monostate> atan() const;

  inline std::unique_ptr<DimensionalVariable> neg() const;

  inline std::unique_ptr<DimensionalVariable> abs() const;

  inline bool equals(const DimensionalVariable& other) const;

  inline bool not_equals(const DimensionalVariable& other) const;

  inline diplomat::result<bool, std::monostate> less_than(const DimensionalVariable& other) const;

  inline diplomat::result<bool, std::monostate> less_equal(const DimensionalVariable& other) const;

  inline diplomat::result<bool, std::monostate> greater_than(const DimensionalVariable& other) const;

  inline diplomat::result<bool, std::monostate> greater_equal(const DimensionalVariable& other) const;

  inline std::unique_ptr<DimensionalVariable> clone_var() const;

  inline diplomat::result<std::string, std::monostate> to_string() const;
  template<typename W>
  inline diplomat::result<std::monostate, std::monostate> to_string_write(W& writeable_output) const;

    inline const diplomat::capi::DimensionalVariable* AsFFI() const;
    inline diplomat::capi::DimensionalVariable* AsFFI();
    inline static const DimensionalVariable* FromFFI(const diplomat::capi::DimensionalVariable* ptr);
    inline static DimensionalVariable* FromFFI(diplomat::capi::DimensionalVariable* ptr);
    inline static void operator delete(void* ptr);
private:
    DimensionalVariable() = delete;
    DimensionalVariable(const DimensionalVariable&) = delete;
    DimensionalVariable(DimensionalVariable&&) noexcept = delete;
    DimensionalVariable operator=(const DimensionalVariable&) = delete;
    DimensionalVariable operator=(DimensionalVariable&&) noexcept = delete;
    static void operator delete[](void*, size_t) = delete;
};


#endif // DimensionalVariable_D_HPP
