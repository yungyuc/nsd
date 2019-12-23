#pragma once

/*
 * Copyright (c) 2019, Yung-Yu Chen <yyc@solvcon.net>
 * BSD-style license; see COPYING
 */

#include <pybind11/pybind11.h> // Must be the first include.
#include <pybind11/numpy.h>
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

#include "modmesh/modmesh.hpp"

#ifdef __GNUG__
#  define MODMESH_PYTHON_WRAPPER_VISIBILITY __attribute__((visibility("hidden")))
#else
#  define MODMESH_PYTHON_WRAPPER_VISIBILITY
#endif

namespace modmesh
{

namespace python
{

/**
 * Helper template for pybind11 class wrappers.
 */
template
<
    class Wrapper
  , class Wrapped
  , class Holder = std::unique_ptr<Wrapped>
  , class WrappedBase = Wrapped
>
/*
 * Use CRTP to detect type error during compile time.
 */
class
MODMESH_PYTHON_WRAPPER_VISIBILITY
WrapBase
{

public:

    using wrapper_type = Wrapper;
    using wrapped_type = Wrapped;
    using wrapped_base_type = WrappedBase;
    using holder_type = Holder;
    using root_base_type = WrapBase
    <
        wrapper_type
      , wrapped_type
      , holder_type
      , wrapped_base_type
    >;
    using class_ = typename std::conditional_t
    <
        std::is_same< Wrapped, WrappedBase >::value
      , pybind11::class_< wrapped_type, holder_type >
      , pybind11::class_< wrapped_type, wrapped_base_type, holder_type >
    >;

    static wrapper_type & commit(pybind11::module & mod)
    {
        static wrapper_type derived(mod);
        return derived;
    }

    WrapBase() = delete;
    WrapBase(WrapBase const & ) = default;
    WrapBase(WrapBase       &&) = delete;
    WrapBase & operator=(WrapBase const & ) = default;
    WrapBase & operator=(WrapBase       &&) = delete;
    ~WrapBase() = default;

#define DECL_MM_PYBIND_CLASS_METHOD(METHOD) \
    template< class... Args > \
    /* NOLINTNEXTLINE(bugprone-macro-parentheses) */ \
    wrapper_type & METHOD(Args&&... args) \
    { \
        m_cls.METHOD(std::forward<Args>(args)...); \
        return *static_cast<wrapper_type*>(this); \
    }

    DECL_MM_PYBIND_CLASS_METHOD(def)
    DECL_MM_PYBIND_CLASS_METHOD(def_buffer)
    DECL_MM_PYBIND_CLASS_METHOD(def_readwrite)
    DECL_MM_PYBIND_CLASS_METHOD(def_property)
    DECL_MM_PYBIND_CLASS_METHOD(def_property_readonly)
    DECL_MM_PYBIND_CLASS_METHOD(def_property_readonly_static)

#undef DECL_MM_PYBIND_CLASS_METHOD

    class_ & cls() { return m_cls; }

protected:

    WrapBase(pybind11::module & mod)
      : m_cls(mod, wrapper_type::PYNAME, wrapper_type::PYDOC)
    {
        static_assert
        (
            std::is_convertible<decltype(wrapper_type::PYNAME), const char *>::value
          , "wrapper_type::PYNAME is not char *"
        );
        static_assert
        (
            std::is_convertible<decltype(wrapper_type::PYDOC), const char *>::value
          , "wrapper_type::PYDOC is not char *"
        );
    }

private:

    class_ m_cls;

}; /* end class WrapBase */

template< typename Wrapper, typename GT >
class
MODMESH_PYTHON_WRAPPER_VISIBILITY
WrapStaticGridBase
  : public WrapBase< Wrapper, GT >
{

public:

    using base_type = WrapBase< Wrapper, GT >;
    using wrapped_type = typename base_type::wrapped_type;

    using serial_type = typename wrapped_type::serial_type;
    using real_type = typename wrapped_type::real_type;

    friend typename base_type::root_base_type;

protected:

    WrapStaticGridBase(pybind11::module & mod) : base_type(mod)
    {

        namespace py = pybind11;

        (*this)
            .def_property_readonly_static
            (
                "NDIM"
              , [](py::object const &) { return wrapped_type::NDIM; }
            )
        ;

    }

}; /* end class WrapStaticGridBase */

class WrapStaticGrid1d
  : public WrapStaticGridBase< WrapStaticGrid1d, modmesh::StaticGrid1d >
{

public:

    static constexpr char PYNAME[] = "StaticGrid1d";
    static constexpr char PYDOC[] = "StaticGrid1d";

    friend root_base_type;

    using base_type = WrapStaticGridBase< WrapStaticGrid1d, StaticGrid1d >;

protected:

    WrapStaticGrid1d(pybind11::module & mod) : base_type(mod)
    {

        namespace py = pybind11;

        (*this)
            .def
            (
                py::init
                (
                    [](serial_type nx)
                    {
                        return new StaticGrid1d(nx);
                    }
                )
              , py::arg("nx")
            )
            .def
            (
                "__len__"
              , [](wrapped_type const & self) { return self.size(); }
            )
            .def
            (
                "__getitem__"
              , [](wrapped_type const & self, size_t it) { return self.at(it); }
            )
            .def
            (
                "__setitem__"
              , [](wrapped_type & self, size_t it, wrapped_type::real_type val)
                {
                    self.at(it) = val;
                }
            )
            .def_property_readonly
            (
                "nx"
              , [](wrapped_type const & self) { return self.nx(); }
            )
            .def_property
            (
                "coord"
              , [](wrapped_type & self)
                {
                    return py::array
                    (
                        py::detail::npy_format_descriptor<real_type>::dtype()
                      , { self.nx() }
                      , { sizeof(real_type) }
                      , self.coord()
                      , py::cast(self)
                    );
                }
              , [](wrapped_type & self, py::array_t<real_type> arr)
                {
                    for (size_t it=0 ; it < self.nx() ; ++it)
                    {
                        self.at(it) = arr.at(it);
                    }
                }
            )
            .def
            (
                "fill"
              , &wrapped_type::fill
              , py::arg("value")
            )
        ;

    }

}; /* end class WrapStaticGrid1d */

class WrapStaticGrid2d
  : public WrapStaticGridBase< WrapStaticGrid2d, modmesh::StaticGrid2d >
{

public:

    static constexpr char PYNAME[] = "StaticGrid2d";
    static constexpr char PYDOC[] = "StaticGrid2d";

    friend root_base_type;

    using base_type = WrapStaticGridBase< WrapStaticGrid2d, StaticGrid2d >;

protected:

    WrapStaticGrid2d(pybind11::module & mod) : base_type(mod)
    {}

}; /* end class WrapStaticGrid2d */

class WrapStaticGrid3d
  : public WrapStaticGridBase< WrapStaticGrid3d, modmesh::StaticGrid3d >
{

public:

    static constexpr char PYNAME[] = "StaticGrid3d";
    static constexpr char PYDOC[] = "StaticGrid3d";

    friend root_base_type;

    using base_type = WrapStaticGridBase< WrapStaticGrid3d, StaticGrid3d >;

protected:

    WrapStaticGrid3d(pybind11::module & mod) : base_type(mod)
    {}

}; /* end class WrapStaticGrid3d */

class WrapTimeRegistry
  : public WrapBase< WrapTimeRegistry, TimeRegistry >
{

public:

    static constexpr char PYNAME[] = "TimeRegistry";
    static constexpr char PYDOC[] = "TimeRegistry";

    friend root_base_type;

protected:

    WrapTimeRegistry(pybind11::module & mod) : root_base_type(mod)
    {

        namespace py = pybind11;

        (*this)
            .def_property_readonly_static("me", [](py::object const &) -> wrapped_type& { return wrapped_type::me(); })
            .def("report", &wrapped_type::report)
        ;

    }

}; /* end class WrapTimeRegistry */

} /* end namespace python */

} /* end namespace modmesh */

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
