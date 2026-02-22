#ifndef BaseUnits_D_HPP
#define BaseUnits_D_HPP

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <memory>
#include <functional>
#include <optional>
#include <cstdlib>
#include "diplomat_runtime.hpp"


namespace diplomat {
namespace capi {
    struct BaseUnits {
      double m;
      double kg;
      double s;
      double k;
      double a;
      double mol;
      double cd;
      double rad;
    };

    typedef struct BaseUnits_option {union { BaseUnits ok; }; bool is_ok; } BaseUnits_option;
} // namespace capi
} // namespace


/**
 * See the [Rust documentation for `BASE_UNITS_SIZE`](https://docs.rs/dv_rs/latest/dv_rs/units/BASE_UNITS_SIZE/index.html) for more information.
 */
struct BaseUnits {
    double m;
    double kg;
    double s;
    double k;
    double a;
    double mol;
    double cd;
    double rad;

    inline diplomat::capi::BaseUnits AsFFI() const;
    inline static BaseUnits FromFFI(diplomat::capi::BaseUnits c_struct);
};


#endif // BaseUnits_D_HPP
