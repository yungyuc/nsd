# Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
# BSD 3-Clause License, see COPYING

import unittest

import numpy as np

import libst


class LinearScalarSolverTC(unittest.TestCase):

    @staticmethod
    def _build_solver(resolution):

        # Build grid.
        xcrd = np.arange(resolution+1) / resolution
        xcrd *= 2 * np.pi
        grid = libst.Grid(xcrd)
        dx = (grid.xmax - grid.xmin) / grid.ncelm

        # Build solver.
        time_stop = 2*np.pi
        cfl_max = 1.0
        dt_max = dx * cfl_max
        nstep = int(np.ceil(time_stop / dt_max))
        dt = time_stop / nstep
        cfl = dt / dx
        svr = libst.LinearScalarSolver(grid=grid, time_increment=dt)

        # Initialize.
        svr.set_so0(0, np.sin(xcrd))
        svr.set_so1(0, np.cos(xcrd))
        svr.setup_march()

        return nstep, xcrd, svr

    def setUp(self):

        self.resolution = 8
        self.nstep, self.xcrd, self.svr = self._build_solver(self.resolution)
        self.cycle = 10

    def test_xctr(self):

        # On even plane.
        self.assertEqual(len(self.svr.xctr()), self.svr.grid.ncelm+1)
        self.assertEqual(self.svr.xctr().tolist(), self.xcrd.tolist())
        self.assertEqual(self.svr.xctr().tolist(),
                         [e.xctr for e in self.svr.selms(odd_plane=False)])

        # On odd plane.
        self.assertEqual(len(self.svr.xctr(odd_plane=True)), self.svr.grid.ncelm)
        self.assertEqual(self.svr.xctr().tolist(), self.xcrd.tolist())
        self.assertEqual(self.svr.xctr(odd_plane=True).tolist(),
                         [e.xctr for e in self.svr.selms(odd_plane=True)])

    def test_nvar(self):

        self.assertEqual(1, self.svr.nvar)

    def test_array_getter(self):

        v1 = [e.get_so0(0) for e in self.svr.selms(odd_plane=False)]
        v2 = self.svr.get_so0(0).tolist()
        self.assertEqual(self.svr.grid.ncelm+1, len(v2))
        self.assertEqual(v1, v2)

        with self.assertRaisesRegex(IndexError, "out of nvar range"):
            self.svr.get_so0(1)
        with self.assertRaisesRegex(IndexError, "out of nvar range"):
            self.svr.get_so0(1, odd_plane=True)

        v1 = [e.get_so1(0) for e in self.svr.selms(odd_plane=False)]
        v2 = self.svr.get_so1(0).tolist()
        self.assertEqual(self.svr.grid.ncelm+1, len(v2))
        self.assertEqual(v1, v2)

        with self.assertRaisesRegex(IndexError, "out of nvar range"):
            self.svr.get_so1(1)
        with self.assertRaisesRegex(IndexError, "out of nvar range"):
            self.svr.get_so1(1, odd_plane=True)

        # The odd-plane value is uninitialized before marching.
        self.svr.march_alpha2(steps=1)

        v1 = [e.get_so0(0) for e in self.svr.selms(odd_plane=True)]
        v2 = self.svr.get_so0(0, odd_plane=True).tolist()
        self.assertEqual(self.svr.grid.ncelm, len(v2))
        self.assertEqual(v1, v2)

        v1 = [e.get_so1(0) for e in self.svr.selms(odd_plane=True)]
        v2 = self.svr.get_so1(0, odd_plane=True).tolist()
        self.assertEqual(self.svr.grid.ncelm, len(v2))
        self.assertEqual(v1, v2)

    def test_initialized(self):

        self.assertEqual(self.svr.get_so0(0).tolist(),
                         np.sin(self.xcrd).tolist())
        self.assertEqual(self.svr.get_so1(0).tolist(),
                         np.cos(self.xcrd).tolist())

    def test_march(self):

        self.svr.march_alpha2(self.nstep*self.cycle)
        np.testing.assert_allclose(self.svr.get_so0(0), np.sin(self.xcrd),
                                   rtol=0, atol=1.e-14)
        ones = np.ones(self.svr.grid.nselm, dtype='float64')
        np.testing.assert_allclose(self.svr.get_cfl(), ones,
                                   rtol=0, atol=1.e-14)

    def test_march_fine_interface(self):

        def _march():

            # first half step.
            self.svr.march_half_so0(odd_plane=False)
            self.svr.treat_boundary_so0()
            self.svr.update_cfl(odd_plane=True)
            self.svr.march_half_so1_alpha2(odd_plane=False)
            self.svr.treat_boundary_so1()
            # second half step.
            self.svr.march_half_so0(odd_plane=True)
            self.svr.update_cfl(odd_plane=False)
            self.svr.march_half_so1_alpha2(odd_plane=True)

        svr2 = self._build_solver(self.resolution)[-1]

        for it in range(self.nstep*self.cycle):
            _march()
            svr2.march_alpha2(steps=1)
            self.assertEqual(self.svr.get_so0(0).tolist(),
                            svr2.get_so0(0).tolist())

# vim: set et sw=4 ts=4:
