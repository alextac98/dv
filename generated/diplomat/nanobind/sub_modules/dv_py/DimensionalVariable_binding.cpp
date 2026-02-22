#include "diplomat_nanobind_common.hpp"


#include "DimensionalVariable.hpp"

namespace dv_py {
void add_DimensionalVariable_binding(nb::module_ mod) {
    PyType_Slot dv_py_DimensionalVariable_slots[] = {
        {Py_tp_free, (void *)dv_py::DimensionalVariable::operator delete },
        {Py_tp_dealloc, (void *)diplomat_tp_dealloc},
        {0, nullptr}};
    
    nb::class_<dv_py::DimensionalVariable>(mod, "DimensionalVariable", nb::type_slots(dv_py_DimensionalVariable_slots))
        .def("abs", &dv_py::DimensionalVariable::abs)
        .def("acos", &dv_py::DimensionalVariable::acos)
        .def_static("acos_scalar", &dv_py::DimensionalVariable::acos_scalar, "x"_a)
        .def("add", &dv_py::DimensionalVariable::add, "other"_a)
        .def("asin", &dv_py::DimensionalVariable::asin)
        .def_static("asin_scalar", &dv_py::DimensionalVariable::asin_scalar, "x"_a)
        .def("atan", &dv_py::DimensionalVariable::atan)
        .def_static("atan_scalar", &dv_py::DimensionalVariable::atan_scalar, "x"_a)
        .def("base_units", &dv_py::DimensionalVariable::base_units)
        .def_static("base_units_size", &dv_py::DimensionalVariable::base_units_size)
        .def("clone_var", &dv_py::DimensionalVariable::clone_var)
        .def("cos", &dv_py::DimensionalVariable::cos)
        .def("div", &dv_py::DimensionalVariable::div, "other"_a)
        .def("div_scalar", &dv_py::DimensionalVariable::div_scalar, "scalar"_a)
        .def("equals", &dv_py::DimensionalVariable::equals, "other"_a)
        .def("greater_equal", &dv_py::DimensionalVariable::greater_equal, "other"_a)
        .def("greater_than", &dv_py::DimensionalVariable::greater_than, "other"_a)
        .def("is_unitless", &dv_py::DimensionalVariable::is_unitless)
        .def("less_equal", &dv_py::DimensionalVariable::less_equal, "other"_a)
        .def("less_than", &dv_py::DimensionalVariable::less_than, "other"_a)
        .def("ln", &dv_py::DimensionalVariable::ln)
        .def("log10", &dv_py::DimensionalVariable::log10)
        .def("log2", &dv_py::DimensionalVariable::log2)
        .def("mul", &dv_py::DimensionalVariable::mul, "other"_a)
        .def("mul_scalar", &dv_py::DimensionalVariable::mul_scalar, "scalar"_a)
        .def("neg", &dv_py::DimensionalVariable::neg)
        .def_static("new", &dv_py::DimensionalVariable::new_, "value"_a, "unit"_a)
        .def("not_equals", &dv_py::DimensionalVariable::not_equals, "other"_a)
        .def("powf", &dv_py::DimensionalVariable::powf, "exp"_a)
        .def("powi", &dv_py::DimensionalVariable::powi, "exp"_a)
        .def("rdiv_scalar", &dv_py::DimensionalVariable::rdiv_scalar, "scalar"_a)
        .def("sin", &dv_py::DimensionalVariable::sin)
        .def("sqrt", &dv_py::DimensionalVariable::sqrt)
        .def("sub", &dv_py::DimensionalVariable::sub, "other"_a)
        .def("tan", &dv_py::DimensionalVariable::tan)
        .def("to_string", &dv_py::DimensionalVariable::to_string)
        .def("value", &dv_py::DimensionalVariable::value)
        .def("value_in", &dv_py::DimensionalVariable::value_in, "unit"_a);
}

} 