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

    // custom 2D grid output
    template <typename T, bool W, unsigned D1, unsigned D0>
    std::ostream& operator << (std::ostream& os, const hyper::grid<T, W, D1, D0>& spc)
    {
        int d0 = 1;
        for(auto b : spc) {
            os << (b ? " X" : " ·");
            if(d0++ % D0 == 0)
                os << '\n';
        }
        return os;
    }
    
    // central state-transition logic, usable for grids of all dimensions
    template <class AnyGrid>
    void game_of_life_iteration(AnyGrid& g, AnyGrid& old, bool (*rule)(int, bool))
    {
        std::swap(g, old);

        // for each cell in the grid
        for(auto it = old.begin(); it != old.end(); ++it) {
            int living_neighbors = 0;
            for(auto off : it)
                living_neighbors += it[off]; // bool -> 0 | 1

            // exercise the CA rule to determine the new state
            bool is_alive = it[0]; // or *it
            g[it] = rule(living_neighbors, is_alive);
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
                [](int count, bool alive) -> bool { return count == 3 or (alive and count == 2); });
            iterations += 1;
            
            // std::cout << grid << std::endl; // uncomment this line to display the moving glider
        } while(initial != grid);

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
            game_of_life_iteration(grid, old_grid,
                // rule to move the above 3D gliders
                [](int count, bool alive) -> bool { return count >= 5 and count <= 7; });
            iterations += 1;
        } while(initial != grid);

        assert(iterations == 40);
    }

} // namespace examples
} // namespace sprogar