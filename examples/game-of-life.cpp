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

namespace sprogar
{
namespace examples
{
template <typename T, unsigned D1, unsigned D0>
std::ostream& operator<<(std::ostream& os, const hyper::unwrapped_space<T, D1, D0>& spc)
{
    os << std::endl;
    int d0 = 1;
    for(auto b : spc) {
        os << (b ? " X" : " ·");
        if(d0++ % D0 == 0)
            os << '\n';
    }
    return os;
}

template <typename T, unsigned D2, unsigned D1, unsigned D0>
std::ostream& operator<<(std::ostream& os, const hyper::unwrapped_space<T, D2, D1, D0>& spc)
{
    std::vector<std::ostringstream> lines{D1};

    int d0 = 1, d1 = 0, d2 = 1;
    for(auto b : spc) {
        lines[d1] << (b ? " X" : " ·");
        if(d0++ % D0 == 0) {
            if(d2 % D2)
                lines[d1] << " |";
            d1 += 1;

            if(d1 == D1) {
                d1 = 0;
                d2 += 1;
            }
        }
    }

    os << std::endl;
    for(const auto& s : lines)
        os << s.str() << std::endl;

    return os;
}

void game_of_life_2D()
{
    std::cout << "Game of life 2D\n";

    hyper::unwrapped_space<bool, 5, 10> grid(false), old_grid(false);

    // create a glider
    grid(0, 1) = grid(1, 2) = grid(2, 0) = grid(2, 1) = grid(2, 2) = true;

    for(int i = 0; i < 5; ++i) {
        std::cout << i << grid;

        std::swap(grid, old_grid);
        for(auto it = old_grid.begin(); it != old_grid.end(); ++it) {
            int count = 0;
            // count the living neighbors
            for(auto off : it)
                count += it[off];

            // conway's rule
            grid[it] = count == 3 or (old_grid[it] and count == 2);
        }
    }
}

void game_of_life_3D()
{
    std::cout << "Game of life 3D\n";

    hyper::unwrapped_space<bool, 2, 5, 10> grid(false), old_grid(false);

    // a 3-D glider
    grid(0, 0, 1) = grid(0, 1, 2) = grid(0, 2, 0) = grid(0, 2, 1) = grid(0, 2, 2) = true;
    grid(1, 0, 1) = grid(1, 1, 2) = grid(1, 2, 0) = grid(1, 2, 1) = grid(1, 2, 2) = true;

    for(int i = 0; i < 5; ++i) {
        std::cout << i << grid;

        std::swap(grid, old_grid);
        for(auto it = old_grid.begin(); it != old_grid.end(); ++it) {
            int count = 0;
            // count the living neighbors
            for(auto off : it)
                count += it[off];

            // the rule
            grid[it] = count >= 5 and count <= 7;
        }
    }
}

}  // namespace examples
}  // namespace sprogar