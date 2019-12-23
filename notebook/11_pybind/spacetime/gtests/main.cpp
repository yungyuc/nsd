#include <gtest/gtest.h>

#include "spacetime.hpp"


namespace st = spacetime;

TEST(CopyTest, Grid)
{

    std::shared_ptr<st::Grid> grid=st::Grid::construct(0, 100, 100);
    st::Grid copied_grid(*grid);
    EXPECT_NE(grid.get(), &copied_grid);

}

TEST(CopyTest, Solver)
{

    std::shared_ptr<st::Grid> grid=st::Grid::construct(0, 100, 100);

    std::shared_ptr<st::Solver> sol=st::Solver::construct(grid, 1, 1);
    std::shared_ptr<st::Solver> cloned_sol=sol->clone();
    EXPECT_NE(sol.get(), cloned_sol.get());
    EXPECT_EQ(&sol->grid(), &cloned_sol->grid());

    std::shared_ptr<st::Solver> cloned_grid_sol=sol->clone(true);
    EXPECT_NE(sol.get(), cloned_grid_sol.get());
    EXPECT_NE(&sol->grid(), &cloned_grid_sol->grid());

}

TEST(SolverTest, Celm)
{

    std::shared_ptr<st::Grid> grid=st::Grid::construct(0, 100, 100);
    std::shared_ptr<st::Solver> sol=st::Solver::construct(grid, 1, 1);

    st::Celm ce0 = sol->celm(0, false);
    st::Celm ce99 = sol->celm(99, false);
    EXPECT_FALSE(ce0 == ce99);
    EXPECT_TRUE (ce0 != ce99);
    EXPECT_TRUE (ce0 <  ce99);
    EXPECT_TRUE (ce0 <= ce99);
    EXPECT_FALSE(ce0 >  ce99);
    EXPECT_FALSE(ce0 >= ce99);

}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/* vim: set et ts=4 sw=4: */
