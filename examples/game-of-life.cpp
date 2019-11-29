/*
 * Copyright 2018 Matej Sprogar <matej.sprogar@gmail.com>
 *
 * This file is part of Hyperspace.
 *
 * Hyperspace is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Hyperspace is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Hyperspace.  If not, see <http://www.gnu.org/licenses/>.
 *
 * */

#include <iostream>
#include <sstream>

#include "../include/hyper.h"

namespace sprogar {
namespace examples {

    template <class Grid, class Func>
    void game_of_life_iteration(Grid& g1, Grid& g2, Func fun)
    {
        std::swap(g1, g2);

        for(auto it = g2.begin(); it != g2.end(); ++it) {
            int count = 0;
            // count the living neighbors
            for(auto off : it)
                count += it[off];

            // conway's rule
            g1[it] = fun(count, *it);
        }
    }

    void game_of_life_2D()
    {
        std::cout << "Game of life 2D test\n";

        hyper::wrapped_space<bool, 5, 10> grid(false), old_grid(false);

        // create a glider
        grid(0, 1) = grid(1, 2) = grid(2, 0) = grid(2, 1) = grid(2, 2) = true;
        const auto initial = grid;

        assert(initial == grid);
        int iterations = 0;
        do {
            game_of_life_iteration(grid, old_grid,
                // Conway's rule
                [](int count, bool alive) -> bool { return count == 3 or (alive and count == 2); }
            );
            iterations += 1;
        } while(initial != grid);

        assert(initial == grid);
        assert(iterations == 40);
    }

    void game_of_life_3D()
    {
        std::cout << "Game of life 3D test\n";

        hyper::wrapped_space<bool, 2, 5, 10> grid(false), old_grid(false);

        // a 3-D glider
        grid(0, 0, 1) = grid(0, 1, 2) = grid(0, 2, 0) = grid(0, 2, 1) = grid(0, 2, 2) = true;
        grid(1, 0, 1) = grid(1, 1, 2) = grid(1, 2, 0) = grid(1, 2, 1) = grid(1, 2, 2) = true;
        const auto initial = grid;

        assert(initial == grid);
        int iterations = 0;
        do {
            game_of_life_iteration(
                grid, old_grid, 
                // 3D gliding rule
                [](int count, bool alive) -> bool { return count >= 5 and count <= 7; }
            );
            iterations += 1;
        } while(initial != grid);

        assert(initial == grid);
        assert(iterations == 40);
    }

} // namespace examples
} // namespace sprogar