#include "PyVar.h"

#include <boost/format.hpp>
#include <boost/python.hpp>
#include <string>

#include "Halide.h"

#include "PyBinaryOperators.h"

using Halide::Expr;
using Halide::Var;

namespace py = boost::python;

namespace {

std::string var_repr(const Var &var) {
    boost::format f("<halide.Var '%s'>");
    return boost::str(f % var.name());
}

}  // namespace

void define_var() {
    bool (Var::*is_implicit_var)() const = &Var::is_implicit;
    bool (*is_implicit_string)(const std::string &) = &Var::is_implicit;
    int (Var::*implicit_index_var)() const = &Var::implicit_index;
    int (*implicit_index_string)(const std::string &) = &Var::implicit_index;
    bool (Var::*is_placeholder_var)() const = &Var::is_placeholder;
    bool (*is_placeholder_string)(const std::string &) = &Var::is_placeholder;
    Expr (Var::*as_expr_method)() const = &Var::operator Expr;

    auto var_class =
        py::class_<Var>("Var", py::init<>())
            .def(py::init<std::string>())
            .def("name", &Var::name, py::return_value_policy<py::copy_const_reference>())
            .def("same_as", &Var::same_as)
            .def("implicit", &Var::implicit)
            .def("is_implicit", is_implicit_var)
            .def("is_implicit", is_implicit_string)
            .def("implicit_index", implicit_index_var)
            .def("implicit_index", implicit_index_string)
            .def("is_placeholder", is_placeholder_var)
            .def("is_placeholder", is_placeholder_string)
            .def("outermost", &Var::outermost)
            .def("__repr__", &var_repr)
            .def("__str__", &Var::name, py::return_value_policy<py::copy_const_reference>())
            // TODO: Python doesn't have explicit type-conversion casting;
            // providing an explicit convert-to-Expr method here seems potentially
            // useful. Overthinking it? Is the best name 'as_expr()', 'expr()', or something else?
            .def("as_expr", as_expr_method)
    ;

    add_binary_operators(var_class);
    add_binary_operators_with<Expr>(var_class);

    py::implicitly_convertible<Var, Expr>();

    py::scope().attr("_") = py::object(Halide::_);
    py::scope().attr("_0") = py::object(Halide::_0);
    py::scope().attr("_1") = py::object(Halide::_1);
    py::scope().attr("_2") = py::object(Halide::_2);
    py::scope().attr("_3") = py::object(Halide::_3);
    py::scope().attr("_4") = py::object(Halide::_4);
    py::scope().attr("_5") = py::object(Halide::_5);
    py::scope().attr("_6") = py::object(Halide::_6);
    py::scope().attr("_7") = py::object(Halide::_7);
    py::scope().attr("_8") = py::object(Halide::_8);
    py::scope().attr("_9") = py::object(Halide::_9);
}