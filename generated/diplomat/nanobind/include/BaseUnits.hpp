#ifndef DV_PYO3_BaseUnits_HPP
#define DV_PYO3_BaseUnits_HPP

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


namespace dv_pyo3 {
namespace capi {

} // namespace capi
} // namespace


inline dv_pyo3::capi::BaseUnits dv_pyo3::BaseUnits::AsFFI() const {
    return dv_pyo3::capi::BaseUnits {
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

inline dv_pyo3::BaseUnits dv_pyo3::BaseUnits::FromFFI(dv_pyo3::capi::BaseUnits c_struct) {
    return dv_pyo3::BaseUnits {
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


#endif // DV_PYO3_BaseUnits_HPP
