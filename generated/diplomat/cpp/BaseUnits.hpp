#ifndef BaseUnits_HPP
#define BaseUnits_HPP

#include "BaseUnits.d.hpp"

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

} // namespace capi
} // namespace


inline diplomat::capi::BaseUnits BaseUnits::AsFFI() const {
    return diplomat::capi::BaseUnits {
        /* .m = */ m,
        /* .kg = */ kg,
        /* .s = */ s,
        /* .k = */ k,
        /* .a = */ a,
        /* .mol = */ mol,
        /* .cd = */ cd,
        /* .rad = */ rad,
    };
}

inline BaseUnits BaseUnits::FromFFI(diplomat::capi::BaseUnits c_struct) {
    return BaseUnits {
        /* .m = */ c_struct.m,
        /* .kg = */ c_struct.kg,
        /* .s = */ c_struct.s,
        /* .k = */ c_struct.k,
        /* .a = */ c_struct.a,
        /* .mol = */ c_struct.mol,
        /* .cd = */ c_struct.cd,
        /* .rad = */ c_struct.rad,
    };
}


#endif // BaseUnits_HPP
