#include "diplomat_nanobind_common.hpp"


#include "BaseUnits.hpp"

namespace dv_py {
void add_BaseUnits_binding(nb::module_ mod) {
    nb::class_<dv_py::BaseUnits>(mod, "BaseUnits")
        .def(nb::init<>())
        .def(nb::init<double, double, double, double, double, double, double, double>(), "m"_a.none(),  "kg"_a.none(),  "s"_a.none(),  "k"_a.none(),  "a"_a.none(),  "mol"_a.none(),  "cd"_a.none(),  "rad"_a.none())
        .def_rw("m", &dv_py::BaseUnits::m)
        .def_rw("kg", &dv_py::BaseUnits::kg)
        .def_rw("s", &dv_py::BaseUnits::s)
        .def_rw("k", &dv_py::BaseUnits::k)
        .def_rw("a", &dv_py::BaseUnits::a)
        .def_rw("mol", &dv_py::BaseUnits::mol)
        .def_rw("cd", &dv_py::BaseUnits::cd)
        .def_rw("rad", &dv_py::BaseUnits::rad);
}

} 