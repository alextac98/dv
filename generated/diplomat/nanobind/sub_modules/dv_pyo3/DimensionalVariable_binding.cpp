#include "diplomat_nanobind_common.hpp"


#include "DimensionalVariable.hpp"

namespace dv_pyo3 {
void add_DimensionalVariable_binding(nb::module_ mod) {
    PyType_Slot dv_pyo3_DimensionalVariable_slots[] = {
        {Py_tp_free, (void *)dv_pyo3::DimensionalVariable::operator delete },
        {Py_tp_dealloc, (void *)diplomat_tp_dealloc},
        {0, nullptr}};
    
    nb::class_<dv_pyo3::DimensionalVariable>(mod, "DimensionalVariable", nb::type_slots(dv_pyo3_DimensionalVariable_slots))
        .def("abs", &dv_pyo3::DimensionalVariable::abs)
        .def("acos", &dv_pyo3::DimensionalVariable::acos)
        .def_static("acos_scalar", &dv_pyo3::DimensionalVariable::acos_scalar, "x"_a)
        .def("add", &dv_pyo3::DimensionalVariable::add, "other"_a)
        .def("asin", &dv_pyo3::DimensionalVariable::asin)
        .def_static("asin_scalar", &dv_pyo3::DimensionalVariable::asin_scalar, "x"_a)
        .def("atan", &dv_pyo3::DimensionalVariable::atan)
        .def_static("atan_scalar", &dv_pyo3::DimensionalVariable::atan_scalar, "x"_a)
        .def("base_units", &dv_pyo3::DimensionalVariable::base_units)
        .def_static("base_units_size", &dv_pyo3::DimensionalVariable::base_units_size)
        .def("clone_var", &dv_pyo3::DimensionalVariable::clone_var)
        .def("cos", &dv_pyo3::DimensionalVariable::cos)
        .def("div", &dv_pyo3::DimensionalVariable::div, "other"_a)
        .def("div_scalar", &dv_pyo3::DimensionalVariable::div_scalar, "scalar"_a)
        .def("equals", &dv_pyo3::DimensionalVariable::equals, "other"_a)
        .def("greater_equal", &dv_pyo3::DimensionalVariable::greater_equal, "other"_a)
        .def("greater_than", &dv_pyo3::DimensionalVariable::greater_than, "other"_a)
        .def("is_unitless", &dv_pyo3::DimensionalVariable::is_unitless)
        .def("less_equal", &dv_pyo3::DimensionalVariable::less_equal, "other"_a)
        .def("less_than", &dv_pyo3::DimensionalVariable::less_than, "other"_a)
        .def("ln", &dv_pyo3::DimensionalVariable::ln)
        .def("log10", &dv_pyo3::DimensionalVariable::log10)
        .def("log2", &dv_pyo3::DimensionalVariable::log2)
        .def("mul", &dv_pyo3::DimensionalVariable::mul, "other"_a)
        .def("mul_scalar", &dv_pyo3::DimensionalVariable::mul_scalar, "scalar"_a)
        .def("neg", &dv_pyo3::DimensionalVariable::neg)
        .def_static("new", &dv_pyo3::DimensionalVariable::new_, "value"_a, "unit"_a)
        .def("not_equals", &dv_pyo3::DimensionalVariable::not_equals, "other"_a)
        .def("powf", &dv_pyo3::DimensionalVariable::powf, "exp"_a)
        .def("powi", &dv_pyo3::DimensionalVariable::powi, "exp"_a)
        .def("rdiv_scalar", &dv_pyo3::DimensionalVariable::rdiv_scalar, "scalar"_a)
        .def("sin", &dv_pyo3::DimensionalVariable::sin)
        .def("sqrt", &dv_pyo3::DimensionalVariable::sqrt)
        .def("sub", &dv_pyo3::DimensionalVariable::sub, "other"_a)
        .def("tan", &dv_pyo3::DimensionalVariable::tan)
        .def("to_string", &dv_pyo3::DimensionalVariable::to_string)
        .def("value", &dv_pyo3::DimensionalVariable::value)
        .def("value_in", &dv_pyo3::DimensionalVariable::value_in, "unit"_a);
}

} 