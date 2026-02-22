#include "diplomat_nanobind_common.hpp"


#include "BaseUnits.hpp"

namespace dv_pyo3 {
void add_BaseUnits_binding(nb::module_ mod) {
    nb::class_<dv_pyo3::BaseUnits>(mod, "BaseUnits")
        .def(nb::init<>())
        .def(nb::init<double, double, double, double, double, double, double, double>(), "m"_a.none(),  "kg"_a.none(),  "s"_a.none(),  "k"_a.none(),  "a"_a.none(),  "mol"_a.none(),  "cd"_a.none(),  "rad"_a.none())
        .def_rw("m", &dv_pyo3::BaseUnits::m)
        .def_rw("kg", &dv_pyo3::BaseUnits::kg)
        .def_rw("s", &dv_pyo3::BaseUnits::s)
        .def_rw("k", &dv_pyo3::BaseUnits::k)
        .def_rw("a", &dv_pyo3::BaseUnits::a)
        .def_rw("mol", &dv_pyo3::BaseUnits::mol)
        .def_rw("cd", &dv_pyo3::BaseUnits::cd)
        .def_rw("rad", &dv_pyo3::BaseUnits::rad);
}

} 