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

#include "examples/examples.h"
#include "test/test.h"

/*
 * GCC:
 * $ g++ -std=c++11 main.cpp examples/game-of-life.cpp test/test.cpp
 * 
 * Other environments of choice:
 * Create a Console project/App, add all three .cpp files, check the c++11 flag and compile
 *
 * */
int main()
{
    sprogar::test::run();

    sprogar::examples::game_of_life_2D();
    sprogar::examples::game_of_life_3D();
	
	std::clog << "All tests passed.\n";
}