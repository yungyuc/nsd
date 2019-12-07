# Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
# BSD 3-Clause License, see COPYING

import unittest

import numpy as np

import libst


class InviscidBurgersSolverTC(unittest.TestCase):

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
        svr = libst.InviscidBurgersSolver(grid=grid, time_increment=dt)

        # Initialize.
        svr.set_so0(0, np.sin(xcrd))
        svr.set_so1(0, np.cos(xcrd))
        svr.setup_march()

        return nstep, xcrd, svr

    def setUp(self):

        self.resolution = 8
        self.nstep, self.xcrd, self.svr = self._build_solver(self.resolution)
        self.cycle = 2

    def test_nvar(self):

        self.assertEqual(1, self.svr.nvar)

    def test_result_bound(self):

        for it in range(self.nstep*self.cycle):
            res = self.svr.get_so0(0)
            self.assertLessEqual(res.max(), 1)
            self.assertGreaterEqual(res.min(), -1)

# vim: set et sw=4 ts=4:
