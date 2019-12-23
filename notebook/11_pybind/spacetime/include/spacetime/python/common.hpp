#pragma once

/*
 * Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
 * BSD 3-Clause License, see COPYING
 */

#include "pybind11/pybind11.h" // must be first
#include "pybind11/operators.h"
#include "pybind11/stl.h"
#include "xtensor-python/pyarray.hpp"

#include "spacetime.hpp"
#include "spacetime/python/WrapBase.hpp"

#include <functional>
#include <list>
#include <sstream>

namespace spacetime
{

namespace python
{

namespace detail {

template<class T> std::string to_str(T const & self) { return Formatter() << self >> Formatter::to_str; }

} /* end namespace detail */

template<class WT, class ET>
class
WrapElementBase
  : public WrapBase< WT, ET >
{

public:

    using base_type = WrapBase< WT, ET >;
    using wrapper_type = typename base_type::wrapper_type;
    using wrapped_type = typename base_type::wrapped_type;

    friend base_type;

protected:

    using base_type::base_type;

    WrapElementBase(pybind11::module * mod, const char * pyname, const char * clsdoc)
      : base_type(mod, pyname, clsdoc)
    {
        namespace py = pybind11;

        (*this)
            .def("__str__", &detail::to_str<wrapped_type>)
            .def(py::self == py::self) // NOLINT(misc-redundant-expression)
            .def(py::self != py::self) // NOLINT(misc-redundant-expression)
            .def(py::self != py::self) // NOLINT(misc-redundant-expression)
            .def(py::self <  py::self) // NOLINT(misc-redundant-expression)
            .def(py::self <= py::self) // NOLINT(misc-redundant-expression)
            .def(py::self >  py::self) // NOLINT(misc-redundant-expression)
            .def(py::self >= py::self) // NOLINT(misc-redundant-expression)
            .def("duplicate", &wrapped_type::duplicate)
            .def_property_readonly("dup", &wrapped_type::duplicate)
            .def_property_readonly("x", &wrapped_type::x)
            .def_property_readonly("dx", &wrapped_type::dx)
            .def_property_readonly("xneg", &wrapped_type::xneg)
            .def_property_readonly("xpos", &wrapped_type::xpos)
            .def_property_readonly("xctr", &wrapped_type::xctr)
            .def_property_readonly("index", &wrapped_type::index)
            .def_property_readonly("on_even_plane", &wrapped_type::on_even_plane)
            .def_property_readonly("on_odd_plane", &wrapped_type::on_odd_plane)
            .def_property_readonly("grid", &wrapped_type::grid)
            .def_property_readonly
            (
                "field"
              , static_cast<Field & (wrapped_type::*)()>(&wrapped_type::field)
            )
            .def_property_readonly("time_increment", &wrapped_type::time_increment)
            .def_property_readonly("dt", &wrapped_type::dt)
            .def_property_readonly("hdt", &wrapped_type::hdt)
            .def_property_readonly("qdt", &wrapped_type::qdt)
            .def("move" , [](wrapped_type & s, size_t v) { s.move_at(v); return s; } )
            .def("move_left" , [](wrapped_type & s) { s.move_left_at(); return s; } )
            .def("move_right" , [](wrapped_type & s) { s.move_right_at(); return s; } )
            .def("move_neg" , [](wrapped_type & s) { s.move_neg_at(); return s; } )
            .def("move_pos" , [](wrapped_type & s) { s.move_pos_at(); return s; } )
        ;
    }

}; /* end class WrapElementBase */

template< class Wrapper, class ET >
class
WrapCelmBase
  : public WrapElementBase< Wrapper, ET >
{

protected:

    using base_type = WrapElementBase< Wrapper, ET >;
    using wrapper_type = typename base_type::wrapper_type;
    using wrapped_type = typename base_type::wrapped_type;

    WrapCelmBase(pybind11::module * mod, const char * pyname, const char * clsdoc)
      : base_type(mod, pyname, clsdoc)
    {
        using se_getter_type = typename wrapped_type::selm_type (wrapped_type::*)();
        using calc_so_type = typename wrapped_type::value_type (wrapped_type::*)(size_t) const;
#define DECL_ST_WRAP_CALC_SO1_ALPHA(ALPHA) \
            .def \
            ( \
                "calc_so1_alpha" #ALPHA, \
                [](wrapped_type const & self, size_t iv) \
                { \
                    return self.template calc_so1_alpha<ALPHA>(iv); \
                } \
            )

        (*this)
            .def_property_readonly("selm_xn", static_cast<se_getter_type>(&wrapped_type::selm_xn))
            .def_property_readonly("selm_xp", static_cast<se_getter_type>(&wrapped_type::selm_xp))
            .def_property_readonly("selm_tn", static_cast<se_getter_type>(&wrapped_type::selm_tn))
            .def_property_readonly("selm_tp", static_cast<se_getter_type>(&wrapped_type::selm_tp))
            .def("calc_so0", static_cast<calc_so_type>(&wrapped_type::calc_so0))
            DECL_ST_WRAP_CALC_SO1_ALPHA(0)
            DECL_ST_WRAP_CALC_SO1_ALPHA(1)
            DECL_ST_WRAP_CALC_SO1_ALPHA(2)
        ;

#undef DECL_ST_WRAP_CALC_SO1_ALPHA
    }

}; /* end class WrapCelmBase */

template< class Wrapper, class ET >
class
WrapSelmBase
  : public WrapElementBase< Wrapper, ET >
{

protected:

    using base_type = WrapElementBase< Wrapper, ET >;
    using wrapper_type = typename base_type::wrapper_type;
    using wrapped_type = typename base_type::wrapped_type;

    WrapSelmBase(pybind11::module * mod, const char * pyname, const char * clsdoc)
      : base_type(mod, pyname, clsdoc)
    {
        using value_type = typename wrapped_type::value_type;
        using so_getter_type = value_type const & (wrapped_type::*)(size_t) const;
        using cfl_getter_type = value_type const & (wrapped_type::*)() const;
        (*this)
            .def_property_readonly("dxneg", &wrapped_type::dxneg)
            .def_property_readonly("dxpos", &wrapped_type::dxpos)
            .def("get_so0", static_cast<so_getter_type>(&wrapped_type::so0))
            .def("get_so1" , static_cast<so_getter_type>(&wrapped_type::so1))
            .def("get_cfl" , static_cast<cfl_getter_type>(&wrapped_type::cfl))
            .def
            (
                "set_so0"
              , [](wrapped_type & self, size_t it, value_type val) { self.so0(it) = val; }
            )
            .def
            (
                "set_so1"
              , [](wrapped_type & self, size_t it, value_type val) { self.so1(it) = val; }
            )
            .def
            (
                "set_cfl"
              , [](wrapped_type & self, value_type val) { self.cfl() = val; }
            )
            .def("xn", &wrapped_type::xn)
            .def("xp", &wrapped_type::xp)
            .def("tn", &wrapped_type::tn)
            .def("tp", &wrapped_type::tp)
            .def("update_cfl", &wrapped_type::update_cfl)
        ;
    }

}; /* end class WrapSelmBase */

template< typename ST >
class SolverElementIterator
{

public:

    using solver_type = ST;

    SolverElementIterator() = delete;
    SolverElementIterator(std::shared_ptr<ST> sol, bool odd_plane, size_t starting, bool selm)
      : m_solver(std::move(sol)), m_odd_plane(odd_plane), m_current(starting), m_selm(selm)
    {}

    typename ST::celm_type next_celm()
    {
        size_t ncelm = m_solver->grid().ncelm();
        if (m_odd_plane) { --ncelm; }
        if (m_current >= ncelm) { throw pybind11::stop_iteration(); }
        typename ST::celm_type ret = m_solver->celm(m_current, m_odd_plane);
        ++m_current;
        return ret;
    }

    typename ST::selm_type next_selm()
    {
        size_t nselm = m_solver->grid().nselm();
        if (m_odd_plane) { --nselm; }
        if (m_current >= nselm) { throw pybind11::stop_iteration(); }
        typename ST::selm_type ret = m_solver->selm(m_current, m_odd_plane);
        ++m_current;
        return ret;
    }

    bool is_selm() const { return m_selm; }
    bool on_odd_plane() const { return m_odd_plane; }
    size_t current() const { return m_current; }
    size_t nelem() const
    {
        size_t ret = is_selm() ? m_solver->grid().nselm() : m_solver->grid().ncelm();
        if (m_odd_plane) { --ret; }
        return ret;
    }

private:

    std::shared_ptr<solver_type> m_solver;
    bool m_odd_plane;
    size_t m_current = 0;
    bool m_selm = false;

}; /* end class SolverElementIterator */

template< typename ST >
std::ostream& operator<<(std::ostream& os, const SolverElementIterator<ST> & seiter)
{
    os
        << "SolverElementIterator("
        << (seiter.is_selm() ? "selm" : "celm")
        << ", " << (seiter.on_odd_plane() ? "on_odd_plane" : "on_even_plane")
        << ", current=" << seiter.current() << ", nelem=" << seiter.nelem() << ")"
    ;
    return os;
}

template<class WT, class ST>
class
WrapSolverBase
  : public WrapBase< WT, ST, std::shared_ptr<ST> >
{

public:

    using base_type = WrapBase< WT, ST, std::shared_ptr<ST> >;
    using wrapper_type = typename base_type::wrapper_type;
    using wrapped_type = typename base_type::wrapped_type;

    friend base_type;

protected:

    WrapSolverBase(pybind11::module * mod, const char * pyname, const char * clsdoc)
      : base_type(mod, pyname, clsdoc)
    {

        namespace py = pybind11;

        using elm_iter_type = SolverElementIterator<wrapped_type>;
        std::string elm_pyname = std::string(pyname) + "ElementIterator";
        pybind11::class_< elm_iter_type >(*mod, elm_pyname.c_str())
            .def("__str__", &detail::to_str<elm_iter_type>)
            .def("__iter__", [](elm_iter_type & self){ return self; })
            .def(
                "__next__"
              , [](elm_iter_type & self)
                {
                    py::object ret;
                    if (self.is_selm()) { ret = py::cast(self.next_selm()); }
                    else                { ret = py::cast(self.next_celm()); }
                    return ret;
                }
            )
        ;

        using celm_getter = typename wrapped_type::celm_type (wrapped_type::*)(sindex_type, bool);
        using selm_getter = typename wrapped_type::selm_type (wrapped_type::*)(sindex_type, bool);

#define DECL_ST_WRAP_ARRAY_ACCESS_0D(NAME) \
    .def_property_readonly(#NAME, static_cast<typename wrapped_type::array_type & (wrapped_type::*)()>(&wrapped_type::NAME)) \
    .def("get_" #NAME, &wrapped_type::get_ ## NAME, py::arg("odd_plane")=false) \
    .def \
    ( \
        "set_" #NAME \
      , [](wrapped_type & self, xt::pyarray<typename wrapped_type::value_type> & arr, bool odd_plane) \
        { self.set_ ## NAME(arr, odd_plane); } \
      , py::arg("arr"), py::arg("odd_plane")=false \
    )
#define DECL_ST_WRAP_ARRAY_ACCESS_1D(NAME) \
    .def_property_readonly(#NAME, static_cast<typename wrapped_type::array_type & (wrapped_type::*)()>(&wrapped_type::NAME)) \
    .def("get_" #NAME, &wrapped_type::get_ ## NAME, py::arg("iv"), py::arg("odd_plane")=false) \
    .def \
    ( \
        "set_" #NAME \
      , [](wrapped_type & self, size_t iv, xt::pyarray<typename wrapped_type::value_type> & arr, bool odd_plane) \
        { self.set_ ## NAME(iv, arr, odd_plane); } \
      , py::arg("iv"), py::arg("arr"), py::arg("odd_plane")=false \
    )
#define DECL_ST_WRAP_MARCH_ALPHA(ALPHA) \
    .def \
    ( \
        "march_half_so1_alpha"#ALPHA \
      , [](wrapped_type & self, bool odd_plane) \
        { return self.template march_half_so1_alpha<ALPHA>(odd_plane); } \
      , py::arg("odd_plane") \
    ) \
    .def \
    ( \
        "march_half1_alpha"#ALPHA \
      , [](wrapped_type & self) { self.template march_half1_alpha<ALPHA>(); } \
    ) \
    .def \
    ( \
        "march_half2_alpha"#ALPHA \
      , [](wrapped_type & self) { self.template march_half2_alpha<ALPHA>(); } \
    ) \
    .def \
    ( \
        "march_alpha"#ALPHA \
      , [](wrapped_type & self, size_t steps) { self.template march_alpha<ALPHA>(steps); } \
      , py::arg("steps") \
    )

        (*this)
            .def("__str__", &detail::to_str<wrapped_type>)
            .def("clone", &wrapped_type::clone, py::arg("grid")=false)
            .def_property_readonly("grid", [](wrapped_type & self){ return self.grid().shared_from_this(); })
            .def("x", &wrapped_type::x, py::arg("odd_plane")=false)
            .def("xctr", &wrapped_type::xctr, py::arg("odd_plane")=false)
            .def_property_readonly("nvar", &wrapped_type::nvar)
            .def_property(
                "time_increment"
              , &wrapped_type::time_increment
              , &wrapped_type::set_time_increment
             )
            .def_property_readonly("dt", &wrapped_type::dt)
            .def_property_readonly("hdt", &wrapped_type::hdt)
            .def_property_readonly("qdt", &wrapped_type::qdt)
            .def("celm" , static_cast<celm_getter>(&wrapped_type::celm_at)
               , py::arg("ielm"), py::arg("odd_plane")=false)
            .def("selm" , static_cast<selm_getter>(&wrapped_type::selm_at)
               , py::arg("ielm"), py::arg("odd_plane")=false)
            .def
            (
                "celms"
              , [](wrapped_type & self, bool odd_plane)
                { return elm_iter_type(self.shared_from_this(), odd_plane, 0, false); }
              , py::arg("odd_plane")=false
            )
            .def
            (
                "selms"
              , [](wrapped_type & self, bool odd_plane)
                { return elm_iter_type(self.shared_from_this(), odd_plane, 0, true); }
              , py::arg("odd_plane")=false
            )
            .def("get_so0p", &wrapped_type::get_so0p, py::arg("iv"), py::arg("odd_plane")=false)
            DECL_ST_WRAP_ARRAY_ACCESS_0D(cfl)
            DECL_ST_WRAP_ARRAY_ACCESS_1D(so0)
            DECL_ST_WRAP_ARRAY_ACCESS_1D(so1)
            .def("update_cfl", &wrapped_type::update_cfl, py::arg("odd_plane"))
            .def("march_half_so0", &wrapped_type::march_half_so0, py::arg("odd_plane"))
            .def("treat_boundary_so0", &wrapped_type::treat_boundary_so0)
            .def("treat_boundary_so1", &wrapped_type::treat_boundary_so1)
            .def("setup_march", &wrapped_type::setup_march)
            DECL_ST_WRAP_MARCH_ALPHA(0)
            DECL_ST_WRAP_MARCH_ALPHA(1)
            DECL_ST_WRAP_MARCH_ALPHA(2)
        ;

#undef DECL_ST_WRAP_MARCH_ALPHA
#undef DECL_ST_WRAP_ARRAY_ACCESS_1D
#undef DECL_ST_WRAP_ARRAY_ACCESS_0D

    }

}; /* end class WrapSolverBase */

class ModuleInitializer {

public:

    using init_type = std::function<PyObject*(pybind11::module*)>;

    ModuleInitializer(ModuleInitializer const & ) = delete;
    ModuleInitializer(ModuleInitializer       &&) = delete;
    ModuleInitializer & operator=(ModuleInitializer const & ) = delete;
    ModuleInitializer & operator=(ModuleInitializer       &&) = delete;

    static ModuleInitializer & get_instance()
    {
        static ModuleInitializer inst;
        return inst;
    }

    void initialize(pybind11::module * topmod)
    {
        if (!m_initialized)
        {
            for (init_type const & initializer : m_initializers)
            {
                initializer(topmod);
            }
        }
        m_initialized = true;
    }

    bool is_initialized() const { return m_initialized; }

    ModuleInitializer & add(init_type const & init)
    {
        m_initializers.push_back(init);
        return *this;
    }

    template< typename WST, typename WCET, typename WSET >
    ModuleInitializer & add_solver(pybind11::module * mod, const std::string & name, const std::string & desc)
    {
        using namespace spacetime::python; // NOLINT(google-build-using-namespace)

        WST::commit
        (
            mod
          , (name + "Solver").c_str()
          , ("Solving algorithm of " + desc).c_str()
        );
        WCET::commit
        (
            mod
          , (name + "Celm").c_str()
          , ("Conservation element of " + desc).c_str()
        );
        WSET::commit
        (
            mod
          , (name + "Selm").c_str()
          , ("Solution element of " + desc).c_str()
        );

        return *this;
    }

private:

    ModuleInitializer() = default;
    ~ModuleInitializer() = default;

    bool m_initialized = false;
    std::list<init_type> m_initializers;

}; /* end class ModuleInitializer */

} /* end namespace python */

} /* end namespace spacetime */

/* vim: set et ts=4 sw=4: */
