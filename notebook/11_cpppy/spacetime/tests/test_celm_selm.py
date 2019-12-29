# Copyright (c) 2018, Yung-Yu Chen <yyc@solvcon.net>
# BSD 3-Clause License, see COPYING

import unittest

import numpy as np

import libst


class CelmTC(unittest.TestCase):

    def setUp(self):

        self.grid10 = libst.Grid(0, 10, 10)
        self.sol10 = libst.Solver(grid=self.grid10, nvar=1,
                                  time_increment=0.2)
        self.ce0 = self.sol10.celm(ielm=0)
        self.ce9 = self.sol10.celm(ielm=9)

    def test_str(self):

        self.assertEqual(
            "Celm(even, index=0, x=0.5, xneg=0, xpos=1)",
            str(self.ce0),
        )

    def test_grid(self):

        self.assertEqual(self.grid10, self.ce0.grid)

    def test_duplicate(self):

        golden_ce = self.ce0
        golden_str = "Celm(even, index=0, x=0.5, xneg=0, xpos=1)"
        self.assertEqual(golden_str, str(self.ce0))
        self.assertEqual(golden_ce, self.ce0.duplicate())
        self.assertEqual(golden_str, str(self.ce0.duplicate()))
        # "dup" attribute is a shorthand for duplicate() function.
        self.assertEqual(golden_ce, self.ce0.dup)
        self.assertEqual(golden_str, str(self.ce0.dup))

        # Duplicated celm has standalone index.
        elm = self.ce0.dup
        elm.move(1)
        self.assertEqual(
            "Celm(odd, index=0, x=1, xneg=0.5, xpos=1.5)",
            str(elm),
        )
        self.assertEqual(
            "Celm(even, index=0, x=0.5, xneg=0, xpos=1)",
            str(self.ce0),
        )

    def test_index(self):

        self.assertEqual(0, self.ce0.index)
        # Moving a half grid makes it on odd plane.
        self.assertEqual(0, self.ce0.dup.move_pos().index)
        # Moving a full grid makes it back on even plane.
        self.assertEqual(1, self.ce0.dup.move_right().index)

    def test_on_even_plane(self):

        self.assertTrue(self.ce0.on_even_plane)
        self.assertFalse(self.ce0.on_odd_plane)
        self.assertFalse(self.ce0.dup.move_pos().on_even_plane)
        self.assertTrue(self.ce0.dup.move_right().on_even_plane)

    def test_coordinates(self):

        self.assertEqual(0.5, self.ce0.x)
        self.assertEqual(0, self.ce0.xneg)
        self.assertEqual(1, self.ce0.xpos)
        self.assertEqual(self.ce0.xpos - self.ce0.xneg, self.ce0.dx)

    def test_move_is_inplace(self):

        # Movement is in-place.
        self.ce0.move(1)
        self.assertEqual(
            "Celm(odd, index=0, x=1, xneg=0.5, xpos=1.5)",
            str(self.ce0),
        )

    def test_move_all(self):

        # Move right/left for a full CE (compound CE).
        self.assertEqual(1.5, self.ce0.dup.move_right().x)
        self.assertEqual(0.5, self.ce0.dup.move_right().move_left().x)

        # Move pos(itive)/neg(itive) for a half CE (basic CE).
        self.assertEqual(1, self.ce0.dup.move_pos().x)
        self.assertEqual(0.5, self.ce0.dup.move_pos().move_neg().x)

        # Out of bound.
        ce0d = self.ce0.dup
        ce0d.move_neg()
        with self.assertRaisesRegex(
            IndexError,
            "Celm\(xindex=2\)::move_at\(offset=-1\): xindex = 1 "
            "outside the interval \[2, 23\)",
        ):
            ce0d.move_neg()
        ce9d = self.ce9.dup
        ce9d.move_pos()
        with self.assertRaisesRegex(
            IndexError,
            "Celm\(xindex=22\)::move_at\(offset=1\): xindex = 23 "
            "outside the interval \[2, 23\)",
        ):
            ce9d.move_pos()

    def test_bounding_se(self):

        self.assertEqual(0, self.ce0.selm_xn.index)
        self.assertTrue(self.ce0.selm_xn.on_even_plane)
        self.assertEqual(1, self.ce0.selm_xp.index)
        self.assertTrue(self.ce0.selm_xp.on_even_plane)
        self.assertEqual(0, self.ce0.selm_tp.index)
        self.assertTrue(self.ce0.selm_tp.on_odd_plane)
        self.assertEqual(0, self.ce0.selm_tn.index)
        self.assertTrue(self.ce0.selm_tn.on_odd_plane)

    def test_calc_so(self):

        self.assertEqual(0, self.ce0.calc_so0(0))
        self.assertEqual(0, self.ce0.calc_so1_alpha0(0))
        self.assertEqual(0, self.ce0.calc_so1_alpha1(0))
        self.assertEqual(0, self.ce0.calc_so1_alpha2(0))
        with self.assertRaises(AttributeError):
            self.ce0.calc_so1_alpha3


class SelmTC(unittest.TestCase):

    def setUp(self):

        self.dt = 0.2
        self.grid10 = libst.Grid(0, 10, 10)
        self.sol10 = libst.Solver(grid=self.grid10, nvar=1,
                                  time_increment=self.dt)
        self.se0 = self.sol10.selm(0)
        self.se9 = self.sol10.selm(9)
        self.se10 = self.sol10.selm(10)
        # Set trivial solution.
        self.sol10.so0.fill(1)
        self.sol10.so1.fill(0)

    def test_str(self):

        self.assertEqual(
            "Selm(even, index=0, x=0, xneg=-0.5, xpos=0.5)",
            str(self.se0),
        )
        self.assertEqual(
            "Selm(even, index=9, x=9, xneg=8.5, xpos=9.5)",
            str(self.se9),
        )
        self.assertEqual(
            "Selm(even, index=10, x=10, xneg=9.5, xpos=10.5)",
            str(self.se10),
        )

    def test_grid(self):

        self.assertEqual(self.grid10, self.se0.grid)

    def test_duplicate(self):

        golden_se = self.se0
        golden_str = "Selm(even, index=0, x=0, xneg=-0.5, xpos=0.5)"
        self.assertEqual(golden_str, str(self.se0))
        self.assertEqual(golden_se, self.se0.duplicate())
        self.assertEqual(golden_str, str(self.se0.duplicate()))
        # "dup" attribute is a shorthand for duplicate() function.
        self.assertEqual(golden_se, self.se0.dup)
        self.assertEqual(golden_str, str(self.se0.dup))

        # Duplicated selm has standalone index.
        se = self.se0.dup
        se.move(1)
        self.assertEqual(
            "Selm(odd, index=0, x=0.5, xneg=0, xpos=1)",
            str(se),
        )
        self.assertEqual(
            "Selm(even, index=0, x=0, xneg=-0.5, xpos=0.5)",
            str(self.se0),
        )

    def test_index(self):

        self.assertEqual(0, self.se0.index)
        # Moving a half grid makes it on odd plane.
        self.assertEqual(0, self.se0.dup.move_pos().index)
        # Moving a full grid makes it back on even plane.
        self.assertEqual(1, self.se0.dup.move_right().index)

    def test_on_even_plane(self):

        self.assertTrue(self.se0.on_even_plane)
        self.assertFalse(self.se0.on_odd_plane)
        self.assertFalse(self.se0.dup.move_pos().on_even_plane)
        self.assertTrue(self.se0.dup.move_right().on_even_plane)

        self.assertFalse(self.sol10.selm(5, odd_plane=True).on_even_plane)
        self.assertTrue(self.sol10.selm(5, odd_plane=False).on_even_plane)

    def test_coordinates(self):

        self.assertEqual(0, self.se0.x)
        self.assertEqual(-0.5, self.se0.xneg)
        self.assertEqual(0.5, self.se0.xpos)
        self.assertEqual(self.se0.xpos - self.se0.xneg, self.se0.dx)
        self.assertEqual(self.dt, self.se0.dt)
        self.assertEqual(self.dt/2, self.se0.hdt)
        self.assertEqual(self.dt/4, self.se0.qdt)

    def test_move_is_inplace(self):

        # Movement is in-place.
        self.se0.move(1)
        self.assertEqual(
            "Selm(odd, index=0, x=0.5, xneg=0, xpos=1)",
            str(self.se0),
        )

    def test_move_all(self):

        # Move right/left for a full CE (compound CE).
        self.assertEqual(1, self.se0.dup.move_right().x)
        self.assertEqual(0, self.se0.dup.move_right().move_left().x)

        # Move pos(itive)/neg(itive) for a half CE (basic CE).
        self.assertEqual(0.5, self.se0.dup.move_pos().x)
        self.assertEqual(0, self.se0.dup.move_pos().move_neg().x)

        # Out of bound.
        se0d = self.se0.dup
        se0d.move_neg()
        with self.assertRaisesRegex(
            IndexError,
            "Selm\(xindex=1\)::move_at\(offset=-1\): xindex = 0 "
            "outside the interval \[1, 24\)",
        ):
            se0d.move_neg()
        se10d = self.se10.dup
        se10d.move_pos()
        with self.assertRaisesRegex(
            IndexError,
            "Selm\(xindex=23\)::move_at\(offset=1\): xindex = 24 "
            "outside the interval \[1, 24\)",
        ):
            se10d.move_pos()


class NonUniformTC(unittest.TestCase):

    def setUp(self):

        xloc = np.array([0, 1, 3, 10])
        self.grid3 = libst.Grid(xloc=xloc)
        self.sol3 = libst.Solver(grid=self.grid3, nvar=1, time_increment=0.1)
        self.ce0 = self.sol3.celm(ielm=0)
        self.ce1 = self.sol3.celm(ielm=1)
        self.ce2 = self.sol3.celm(ielm=2)
        self.se0 = self.sol3.selm(ielm=0)
        self.se1 = self.sol3.selm(ielm=1)
        self.se2 = self.sol3.selm(ielm=2)
        self.se3 = self.sol3.selm(ielm=3)

    def test_ce_xcoord(self):

        def get_xcoord(elm):
            return [elm.x, elm.xneg, elm.xctr, elm.xpos]

        for it in range(3):
            ce = getattr(self, 'ce%d'%it)
            self.assertEqual(ce.xctr, ce.x)

        self.assertEqual([0.5, 0, 0.5, 1], get_xcoord(self.ce0))
        self.assertEqual([2, 1, 2, 3], get_xcoord(self.ce1))
        self.assertEqual([6.5, 3, 6.5, 10], get_xcoord(self.ce2))

    def test_se_xcoord(self):

        def get_xcoord(elm):
            return [elm.x, elm.xneg, elm.xctr, elm.xpos]

        for it in [0, 3]:
            se = getattr(self, 'se%d'%it)
            self.assertEqual(se.xctr, se.x)
        for it in [1, 2]:
            se = getattr(self, 'se%d'%it)
            self.assertNotEqual(se.xctr, se.x)

        self.assertEqual([0.0, -0.5, 0.0, 0.5], get_xcoord(self.se0))
        self.assertEqual([1.0, 0.5, 1.25, 2.0], get_xcoord(self.se1))
        self.assertEqual([3.0, 2.0, 4.25, 6.5], get_xcoord(self.se2))
        self.assertEqual([10.0, 6.5, 10.0, 13.5], get_xcoord(self.se3))


class ComparisonTC(unittest.TestCase):

    def setUp(self):

        self.grid = libst.Grid(0, 10, 10)
        self.sol1 = libst.Solver(grid=self.grid, nvar=1, time_increment=0.2)
        self.sol2 = libst.Solver(grid=self.grid, nvar=1, time_increment=0.2)

        self.ce0s1 = self.sol1.celm(ielm=0)
        self.ce0s1p = self.sol1.celm(ielm=0)
        self.ce1s1 = self.sol1.celm(ielm=1)
        self.ce0s1o = self.sol1.celm(ielm=0, odd_plane=True)
        self.ce0s2 = self.sol2.celm(ielm=0)

        self.se0s1 = self.sol1.selm(ielm=0)
        self.se0s1p = self.sol1.selm(ielm=0)
        self.se1s1 = self.sol1.selm(ielm=1)
        self.se0s1o = self.sol1.selm(ielm=0, odd_plane=True)
        self.se0s2 = self.sol2.selm(ielm=0)

    def _check_copy(self, e1, e2):

        self.assertTrue (e1 == e2)
        self.assertFalse(e1 != e2)
        self.assertTrue (e1 <= e2)
        self.assertFalse(e1 <  e2)
        self.assertTrue (e1 >= e2)
        self.assertFalse(e1 >  e2)

        self.assertTrue (e2 == e1)
        self.assertFalse(e2 != e1)
        self.assertTrue (e2 <= e1)
        self.assertFalse(e2 <  e1)
        self.assertTrue (e2 >= e1)
        self.assertFalse(e2 >  e1)

    def test_celm_copy(self):

        self._check_copy(self.ce0s1, self.ce0s1p)

    def test_selm_copy(self):

        self._check_copy(self.se0s1, self.se0s1p)

    def _check_adjacent(self, e1, e2):

        self.assertFalse(e1 == e2)
        self.assertTrue (e1 != e2)
        self.assertTrue (e1 <= e2)
        self.assertTrue (e1 <  e2)
        self.assertFalse(e1 >= e2)
        self.assertFalse(e1 >  e2)

        self.assertFalse(e2 == e1)
        self.assertTrue (e2 != e1)
        self.assertFalse(e2 <= e1)
        self.assertFalse(e2 <  e1)
        self.assertTrue (e2 >= e1)
        self.assertTrue (e2 >  e1)

    def test_celm_adjacent(self):

        self._check_adjacent(self.ce0s1, self.ce1s1)

    def test_selm_adjacent(self):

        self._check_adjacent(self.se0s1, self.se1s1)

    def _check_odd_plane(self, e1, e2):

        self.assertFalse(e1 == e2)
        self.assertTrue (e1 != e2)
        self.assertTrue (e1 <= e2)
        self.assertTrue (e1 <  e2)
        self.assertFalse(e1 >= e2)
        self.assertFalse(e1 >  e2)

        self.assertFalse(e2 == e1)
        self.assertTrue (e2 != e1)
        self.assertFalse(e2 <= e1)
        self.assertFalse(e2 <  e1)
        self.assertTrue (e2 >= e1)
        self.assertTrue (e2 >  e1)

    def test_celm_odd_plane(self):

        self._check_odd_plane(self.ce0s1, self.ce0s1o)

    def test_selm_odd_plane(self):

        self._check_odd_plane(self.se0s1, self.se0s1o)

    def _check_different_solver(self, e1, e2):

        self.assertFalse(e1 == e2)
        self.assertTrue (e1 != e2)
        self.assertFalse(e1 <= e2)
        self.assertFalse(e1 <  e2)
        self.assertFalse(e1 >= e2)
        self.assertFalse(e1 >  e2)

        self.assertFalse(e2 == e1)
        self.assertTrue (e2 != e1)
        self.assertFalse(e2 <= e1)
        self.assertFalse(e2 <  e1)
        self.assertFalse(e2 >= e1)
        self.assertFalse(e2 >  e1)

    def test_celm_different_solver(self):

        self._check_different_solver(self.ce0s1, self.ce0s2)

    def test_selm_different_solver(self):

        self._check_different_solver(self.se0s1, self.se0s2)

    def test_celm_selm(self):

        e1 = self.ce0s1
        e2 = self.se0s1

        self.assertFalse(e1 == e2)
        self.assertTrue (e1 != e2)
        with self.assertRaises(TypeError):
            e1 <= e2
        with self.assertRaises(TypeError):
            e1 <  e2
        with self.assertRaises(TypeError):
            e1 >= e2
        with self.assertRaises(TypeError):
            e1 >  e2

        self.assertFalse(e2 == e1)
        self.assertTrue (e2 != e1)
        with self.assertRaises(TypeError):
            e2 <= e1
        with self.assertRaises(TypeError):
            e2 <  e1
        with self.assertRaises(TypeError):
            e2 >= e1
        with self.assertRaises(TypeError):
            e2 >  e1

# vim: set et sw=4 ts=4:
