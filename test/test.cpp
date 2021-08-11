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

#include <cassert>
#include <iostream>
#include <vector>
#include <array>

#include "../include/hyper.h"

namespace sprogar {
	namespace test {
		using namespace hyper;

		typedef void (*voidfunc)();
		std::vector<voidfunc> all_tests = {
		[]() {
			std::clog << "default iterator construction\n";
			location_iterator<1/*R*/, 40, 7> it;
			assert(it == 0);
			assert(0 == it[0]);
			assert(0 == it[1]);
		},
		[]() {
			std::clog << "iterator construction via id\n";
			location_iterator<1/*R*/, 40, 7> it(145);
			assert(it == 145U);
			assert(5 == it[0]);
			assert(20 == it[1]);
		},
		[]() {
			std::clog << "iterator construction via coordinates\n";
			location_iterator<1/*R*/, 40, 7> it(20,5);
			assert(it == 145U);
			assert(5 == it[0]);
			assert(20 == it[1]);
		},
		[]() {
			std::clog << "no spc dimension\n";
			wrapped_space<bool, 1/*R*/> no_space;
			assert(no_space.dimension() == 0);
		},
		[]() {
			std::clog << "empty spc dimension\n";
			wrapped_space<bool, 1/*R*/, 0> spc;
			assert(spc.dimension() == 1);
		},
		[]() {
			std::clog << "spc dimensions\n";
			wrapped_space<bool, 1/*R*/, 5> spc;
			assert(spc.dimension() == 1);
			assert(spc.dimension(0) == 5);
		},
		[]() {
			std::clog << "spc iterator default construction test\n";
			unwrapped_space<bool, 1/*R*/, 4, 3, 2> uws;
			auto it = uws.begin();
			assert(0 == it.coordinate(0));
			assert(0 == it.coordinate(1));
			assert(0 == it.coordinate(2));
		},
		[]() {
			std::clog << "spc iterator positioning test\n";
			unwrapped_space<bool, 1/*R*/, 4, 3, 2> uws;
			auto it = uws.at(2, 0, 1);
			assert(1 == it.coordinate(0));
			assert(0 == it.coordinate(1));
			assert(2 == it.coordinate(2));
		},
		[]() {
			std::clog << "iterator coordinate API\n";
			location_iterator<1/*R*/, 4, 3, 2> it(2, 0, 1);
			assert(1 == it[0]);
			assert(0 == it[1]);
			assert(2 == it[2]);
		},
		[]() {
			std::clog << "iterator prefix increment 1-D test\n";
			wrapped_space<int, 1/*R*/, 3> spc;
			auto it = spc.begin();
			assert(0 == it);
			++it;
			assert(1 == it);
			++it;
			assert(2 == it);
			++it;
			assert(spc.end() == it);
		},
		[]() {
			std::clog << "iterator prefix decrement 1-D test\n";
			wrapped_space<int, 1/*R*/, 3> spc;
			auto it = spc.begin();
			++it;
			--it;
			assert(spc.begin() == it);
		},
		[]() {
			std::clog << "iterator prefix increment 3-D test\n";
			location_iterator<1/*R*/, 4, 3, 2> it;
			assert(0 == it[0]);
			assert(0 == it[1]);
			assert(0 == it[2]);
			assert(0 == it);
			++it;
			assert(1 == it[0]);
			assert(0 == it[1]);
			assert(0 == it[2]);
			assert(1 == it);
			++it;
			assert(0 == it[0]);
			assert(1 == it[1]);
			assert(0 == it[2]);
			assert(2 == it);
			++it;
			assert(1 == it[0]);
			assert(1 == it[1]);
			assert(0 == it[2]);
			assert(3 == it);
			++it;
			assert(0 == it[0]);
			assert(2 == it[1]);
			assert(0 == it[2]);
			assert(4 == it);
		},
		[]() {
			std::clog << "null spc neighbor offset test\n";
			wrapped_space<bool, 1/*R*/, 0> ws;
			assert(ws.neighbors_offsets(0) == std::vector<int>());
			assert(ws.neighbors_offsets(0) == std::vector<int>());
		},
		[]() {
			std::clog << "single point spc neighbor offset test\n";
			assert((wrapped_space<bool, 1/*R*/, 1>::neighbors_offsets(0)) == std::vector<int>());
			assert((unwrapped_space<bool, 1/*R*/, 1>::neighbors_offsets(0)) == std::vector<int>());
		},
		[]() {
			std::clog << "radius 1 unwrapped offsets test\n";
			unwrapped_space<bool, 1/*R*/, 5> spc;
			assert(spc.at(0).neighbors_offsets() == std::vector<int>({ 1 }));
			assert(spc.at(1).neighbors_offsets() == std::vector<int>({ -1, 1 }));
			assert(spc.at(4).neighbors_offsets() == std::vector<int>({ -1 }));
		},
		[]() {
			std::clog << "radius 1 wrapped offsets test\n";
			wrapped_space<bool, 1/*R*/, 5> spc;
			assert(spc.at(0).neighbors_offsets() == std::vector<int>({ 1, 4 }));
			assert(spc.at(1).neighbors_offsets() == std::vector<int>({ -1, 1 }));
			assert(spc.at(4).neighbors_offsets() == std::vector<int>({ -4, -1 }));
		},
		[]() {
			std::clog << "radius 1 offsets in 3D test\n";
			unwrapped_space<bool, 1/*R*/, 5, 10, 7> spc;
			auto it = spc.begin();
			assert(it.neighbors_offsets() == std::vector<int>({ 1, 7, 8, 70, 71, 77, 78 }));
			assert(it.type() == 13);
			assert(it == 0);
		},
		[]() {
			std::clog << "radius 2 unwrapped offsets test\n";
			unwrapped_space<bool, 2/*R*/, 40, 7> spc;
			auto it = spc.begin();
			assert(it.neighbors_offsets() == std::vector<int>({ 1, 2, 7, 8, 9, 14, 15, 16 }));
		},
		[]() {
			std::clog << "radius 2 wrapped offsets test\n";
			wrapped_space<bool, 2/*R*/, 40, 7> spc;
			auto it = spc.begin();
			assert(it.neighbors_offsets() == std::vector<int>({ 1, 2, 5, 6, 7, 8, 9, 12, 13, 14, 15, 16,
				19, 20, 266, 267, 268, 271, 272, 273, 274, 275, 278, 279 }));
		},
		[]() {
			std::clog << "crosscheck test\n";
			unwrapped_space<bool, 1/*R*/, 2, 4> uws;
			auto it = uws.begin();
			assert(it.neighbors_offsets() == std::vector<int>({ 1, 4, 5 }));

			++it; // [0,1]
			assert(it.neighbors_offsets() == std::vector<int>({ -1, 1, 3, 4, 5 }));

			++it; // [0,2]
			++it; // [0,3]
			++it; // [1,0]
			assert(it.neighbors_offsets() == std::vector<int>({ -4, -3, 1 }));

			++it; // [1,1]
			++it; // [1,2]
			++it; // [1,3]
			assert(it.neighbors_offsets() == std::vector<int>({ -5, -4, -1 }));
			}, []() {
			std::clog << "neighbors' offsets 2-D test\n";
			wrapped_space<bool, 1/*R*/, 2, 4> ws;
			auto it = ws.begin();
			assert(it.neighbors_offsets() == std::vector<int>({ 1, 3, 4, 5, 7 }));

			++it; // [0,1]
			assert(it.neighbors_offsets() == std::vector<int>({ -1, 1, 3, 4, 5 }));

			++it; // [0,2]
			++it; // [0,3]
			++it; // [1,0]
			assert(it.neighbors_offsets() == std::vector<int>({ -4, -3, -1, 1, 3 }));

			++it; // [1,1]
			++it; // [1,2]
			++it; // [1,3]
			assert(it.neighbors_offsets() == std::vector<int>({ -7, -5, -4, -3, -1 }));
		},
		[]() {
			std::clog << "initialize location with coordinate list test\n";
			location_iterator<1/*R*/, 3, 4, 5> it(2, 1, 0);
			assert(it == 45U);
		},
		[]() {
			std::clog << "neighbors' offsets and neighborhood types 3-D test\n";
			unwrapped_space<bool, 1/*R*/, 3, 4, 5> uws;
			auto it = uws.begin(); // [0,0,0]
			assert(it.neighbors_offsets() == std::vector<int>({ 1, 5, 6, 20, 21, 25, 26 }));
		},
		[]() {
			std::clog << "neighbors' offsets and neighborhood types 3-D test\n";
			wrapped_space<bool, 1/*R*/, 3, 4, 5> ws;
			auto it = ws.begin(); // [0,0,0]
			assert(it.neighbors_offsets() == std::vector<int>({ 1, 4, 5, 6, 9, 15, 16, 19, 20, 21,
			24, 25, 26, 29, 35, 36, 39, 40, 41, 44, 45, 46, 49, 55, 56, 59 }));
		},
		[]() {
			std::clog << "neighbors' offsets of the first point in unwrapped 4-D spc test\n";
			unwrapped_space<bool, 1/*R*/, 4, 4, 4, 4> spc;
			auto it = spc.begin();
			assert(it.neighbors_offsets() == std::vector<int>(
			{ 1, 4, 5, 16, 17, 20, 21, 64, 64 + 1, 64 + 4, 64 + 5, 64 + 16, 64 + 17, 64 + 20, 64 + 21 }));
		},
		[]() {
			std::clog << "neighbors' offsets of the 80th point in unwrapped 4-D spc test\n";
			unwrapped_space<bool, 1/*R*/, 4, 4, 4, 4> spc;
			auto it = spc.at(1,1,0,0);
			auto ans = std::vector<int>({ -64 - 16, -64 - 15, -64 - 12, -64 - 11, -64 + 0, -64 + 1, -64 + 4, -64 + 5,
			-64 + 16, -64 + 17, -64 + 20, -64 + 21, -16, -15, -12, -11, +1, +4, +5, +16, +17, +20, +21, 64 - 16,
			64 - 15, 64 - 12, 64 - 11, 64 + 0, 64 + 1, 64 + 4, 64 + 5, 64 + 16, 64 + 17, 64 + 20, 64 + 21 });
			assert(it.neighbors_offsets() == ans);
		},
		[]() {
			std::clog << "another offsets 3-D test\n";
			wrapped_space<bool, 1/*R*/, 4, 4, 4> spc;
			auto it = spc.at(3, 3, 2);
			assert(it.neighbors_offsets() == std::vector<int>({ -61, -60, -59, -53, -52, -51, -49, -48, -47, -29, -28, -27, -21, -20, -19, -17, -16,
				-15, -13, -12, -11, -5, -4, -3, -1, +1 }));
		},
		[]() {
			std::clog << "(non)existing coordinate test\n";
			location_iterator<1/*R*/, 5, 6> it(2,3);
			assert(3 == it[0]);
			assert(2 == it[1]);
			assert(-1 == it[2]);
		},
		[]() {
			std::clog << "mapping arbitrary 3-D location to 1-D test\n";
			hyper::location_iterator<1/*R*/, 7, 5, 6> it(2, 4, 1);
			assert(it == 2 * 5 * 6 + 4 * 6 + 1);
		},
		[]() {
			std::clog << "placed iterator is at desired location 2-D test\n";
			auto it = unwrapped_space<bool, 1/*R*/, 2, 3>().at(1, 2);
			assert(it == 5U);
		},
		[]() {
			std::clog << "iterator API: neighborhood size test\n";
			unwrapped_space<bool, 1/*R*/, 3, 3> spc;
			auto it = spc.begin();
			assert(it.size() == 3); // corner
			++it;
			assert(it.size() == 5); // edge
			++it;
			assert(it.size() == 3); // corner
			++it;
			assert(it.size() == 5); // edge
			++it;
			assert(it.size() == 8); // center
		},
		[]() {
			std::clog << "iterator API: element coordinate test\n";
			unwrapped_space<bool, 1/*R*/, 3, 3> spc;

			auto it = spc.begin();
			assert(it.coordinate(0) == 0); // corner x
			assert(it.coordinate(1) == 0); // corner y
			++it;
			assert(it.coordinate(0) == 1); // edge x
			assert(it.coordinate(1) == 0); // edge y
			++it;
			assert(it.coordinate(0) == 2); // corner x
			assert(it.coordinate(1) == 0); // corner y
			++it;
			assert(it.coordinate(0) == 0); // edge x
			assert(it.coordinate(1) == 1); // edge y
			++it;
			assert(it.coordinate(0) == 1); // center x
			assert(it.coordinate(1) == 1); // center y
		},
		[]() {
			std::clog << "spc description test\n";
			assert((wrapped_space<bool, 1/*R*/, 5, 2, 3, 1>::info()) == "5x2x3x1 wrapped");
			assert((unwrapped_space<bool, 1/*R*/, 1, 3, 2, 5>::info()) == "1x3x2x5 unwrapped");
		},
		[]() {
			std::clog << "spc dimension test\n";
			typedef wrapped_space<bool, 1/*R*/, 5, 2, 3, 1> spc;
			assert(spc::dimension() == 4);
			assert(spc::dimension(0) == 1);
			assert(spc::dimension(1) == 3);
			assert(spc::dimension(2) == 2);
			assert(spc::dimension(3) == 5);
		},
		[]() {
			std::clog << "spc size test\n";
			typedef wrapped_space<bool, 1/*R*/, 5, 2, 3, 1> spc;
			assert(spc::size() == 5 * 2 * 3 * 1);
		},
		[]() {
			std::clog << "spc positioning test\n";
			typedef iterable_space<true, 1/*R*/, 5, 6, 7, 8> spc;
			assert(0 == spc::id(0, 0, 0, 0));
			assert(1 == spc::id(1, 0, 0, 0));
			assert(4 + 1 * 5 == spc::id(4, 1, 0, 0));
			assert(3 + 2 * 5 + 1 * 5 * 6 + 4 * 5 * 6 * 7 == spc::id(3, 2, 1, 4));
			assert(spc::size() - 1 == spc::id(4, 5, 6, 7));
		},
		[]() {
			std::clog << "spc size initialization test\n";
			wrapped_space<int, 1/*R*/, 1> spc;
			assert(spc.size() == 1);
		},
		[]() {
			std::clog << "spc initialization by const value test\n";
			wrapped_space<int, 1/*R*/, 2> spc(42);
			assert(spc[0] == 42);
			assert(spc[1] == 42);
		},
		[]() {
			std::clog << "spc initialization by function test\n";
			wrapped_space<int, 1/*R*/, 2> spc([]() {
				static int g = 42;
				return g++;
			});

			assert(42 == spc[0]);
			assert(43 == spc[1]);
		},
		[]() {
			std::clog << "const_iterator read test\n";
			wrapped_space<int, 1/*R*/, 2, 2> spc(7);

			int total = 0;
			for (auto it = spc.begin(); it != spc.end(); ++it)
			total += *it;
			assert(7 * 2 * 2 == total);
		},
		[]() {
			std::clog << "iterator write test\n";
			wrapped_space<int, 1/*R*/, 2, 2> spc;
			int i = 1;
			for (auto& x : spc)
			x = i++;
			int total = 0;
			for (auto x : spc)
			total += x;
			assert(1 + 2 + 3 + 4 == total);
		},
		[]() {
			std::clog << "implicit move semantics test\n";
			struct uncopyable {
				uncopyable() {}
				uncopyable(const uncopyable&) = delete;
					uncopyable& operator=(const uncopyable&) = delete;
			};
			wrapped_space<uncopyable, 1/*R*/, 3> spc1;
			wrapped_space<uncopyable, 1/*R*/, 3> spc2;

			const auto* p1 = &spc1[0];
			const auto* p2 = &spc2[0];
			assert(p1 != p2);

			std::swap(spc1, spc2);
			assert(&spc1[0] == p2);
			assert(&spc2[0] == p1);

			swap(spc2, spc1);
			assert(&spc1[0] == p1);
			assert(&spc2[0] == p2);

			spc1 = std::move(spc2);
			assert(&spc1[0] == p2);
		},
		[]() {
		std::clog << "implicit copy semantics test\n";
			struct copyable {
				bool copied = false;
				copyable() {}
				copyable(const copyable&)
					: copied(true) {};
				copyable& operator=(const copyable& oth)
				{
					copied = true;
					return *this;
				}
			};
			wrapped_space<copyable, 1/*R*/, 3> spc1;
			assert(spc1[0].copied == false);
			wrapped_space<copyable, 1/*R*/, 3> spc2(spc1);
			assert(spc2[0].copied == true);

			spc1 = spc2;
			assert(spc1[0].copied == true);
		},
		[]() {
			std::clog << "README.md usage test\n";
			hyper::wrapped_space<int, 1/*R*/, 5, 7> spc{ 0 };
			spc(3, 4) = 42;

			assert(spc[0] == 0);
			assert(spc[25] == 42);
		},
		[]() {
			std::clog << "README.md usage test\n";
			std::array<int, 5 * 7> spc = { 0 }; // 1-D memory allocation ...

			location_iterator<1/*R*/, 5, 7> it(3, 4);
			spc[it] = 42;

			assert(spc[0] == 0);
			assert(spc[25] == 42);
		},

		[]() {
			std::clog << "API functionality: default construction test\n";
			hyper::unwrapped_space<int, 1/*R*/, 3> spc;
			assert(spc[0] == 0);
			assert(spc[1] == 0);
			assert(spc[2] == 0);
		},
		[]() {
			std::clog << "API functionality: construction with initialization test\n";
			hyper::unwrapped_space<int, 1/*R*/, 3> spc{ 42 };
			assert(spc[0] == 42);
			assert(spc[1] == 42);
			assert(spc[2] == 42);
		},
		[]() {
			std::clog << "API functionality: construction by lambda test\n";
			hyper::unwrapped_space<int, 1/*R*/, 3> spc{ []() {
				static int i = 0;
				return i++;
				} 
			};
			assert(spc[0] == 0);
			assert(spc[1] == 1);
			assert(spc[2] == 2);
		},
		[]() {
			std::clog << "API functionality: random access syntax 1 test\n";
			hyper::unwrapped_space<int, 1/*R*/, 3, 4> spc{ 0 };
			spc[1] = 42;

			assert(spc[0] == 0);
			assert(spc[1] == 42);
			assert(spc[11] == 0);
		},
		[]() {
			std::clog << "API functionality: random access syntax 2 test\n";
			hyper::unwrapped_space<int, 1/*R*/, 3, 4> spc{ 0 };
			spc(0, 1) = 42;

			assert(spc[0] == 0);
			assert(spc[1] == 42);
			assert(spc[11] == 0);
		},
		[]() {
			std::clog << "API functionality: random access syntax 3 test\n";
			hyper::unwrapped_space<int, 1/*R*/, 3, 4> spc{ 0 };
			auto cell = spc.at(0, 1);
			*cell = 42;

			assert(spc[0] == 0);
			assert(spc[1] == 42);
			assert(spc[11] == 0);
		},
		[]() {
			std::clog << "API functionality: cell iteration syntax 1 test\n";
			hyper::unwrapped_space<int, 1/*R*/, 5> spc{ 0 };
			for (auto it = spc.begin(); it != spc.end(); ++it)
			*it = 42;

			assert(spc[0] == 42);
			assert(spc[1] == 42);
			assert(spc[2] == 42);
			assert(spc[3] == 42);
			assert(spc[4] == 42);
		},
		[]() {
			std::clog << "API functionality: cell iteration syntax 2 test\n";
			hyper::unwrapped_space<int, 1/*R*/, 5> spc{ 0 };
			for (int& x : spc)
			x = 42;

			assert(spc[0] == 42);
			assert(spc[4] == 42);
		},
		[]() {
			std::clog << "API functionality: neighbors access syntax 1 test\n";
			hyper::unwrapped_space<int, 1/*R*/, 5> spc{ 0 };
			auto cell = spc.at(2);

			for (int off : cell.neighbors_offsets())
			spc[cell + off] = 42;

			assert(spc[0] == 0);
			assert(spc[1] == 42);
			assert(spc[2] == 0);
			assert(spc[3] == 42);
			assert(spc[4] == 0);
		},
		[]() {
			std::clog << "API functionality:  neighbors access syntax 2 test\n";
			hyper::unwrapped_space<int, 1/*R*/, 5> spc{ 0 };
			auto cell = spc.at(2);

			for (int off : cell)
			spc[cell + off] = 42;

			assert(spc[0] == 0);
			assert(spc[1] == 42);
			assert(spc[2] == 0);
			assert(spc[3] == 42);
			assert(spc[4] == 0);
		},
		[]() {
			std::clog << "API functionality:  neighbors access syntax 3 test\n";
			hyper::unwrapped_space<int, 1/*R*/, 5> spc{ 0 };
			auto cell = spc.at(2);

			for (auto off = cell.begin(); off != cell.end(); ++off)
			spc[cell + *off] = 42;

			assert(spc[0] == 0);
			assert(spc[1] == 42);
			assert(spc[2] == 0);
			assert(spc[3] == 42);
			assert(spc[4] == 0);
		},
		[]() {
			std::clog << "API functionality: neighbors access syntax 4 test\n";
			hyper::unwrapped_space<int, 1/*R*/, 5> spc{ 0 };
			auto cell = spc.at(2);

			for (int off : cell.neighbors_offsets())
			cell[off] = 42;

			assert(spc[0] == 0);
			assert(spc[1] == 42);
			assert(spc[2] == 0);
			assert(spc[3] == 42);
			assert(spc[4] == 0);
		},
		[]() {
			std::clog << "API functionality:  neighbors access syntax 5 test\n";
			hyper::unwrapped_space<int, 1/*R*/, 5> spc{ 0 };
			auto cell = spc.at(2);

			for (int x : cell)
			cell[x] = 42;

			assert(spc[0] == 0);
			assert(spc[1] == 42);
			assert(spc[2] == 0);
			assert(spc[3] == 42);
			assert(spc[4] == 0);
		},
		[]() {
			std::clog << "API functionality:  neighbors access syntax 6 test\n";
			hyper::unwrapped_space<int, 1/*R*/, 5> spc{ 0 };
			auto cell = spc.at(2);

			for (int& x : cell.neighbors())
			x = 42;

			assert(spc[0] == 0);
			assert(spc[1] == 42);
			assert(spc[2] == 0);
			assert(spc[3] == 42);
			assert(spc[4] == 0);
		},
		[]() {
			std::clog << "grid iterator access test\n";
			wrapped_space<int, 1/*R*/, 2, 2> spc;
			wrapped_space<int, 1/*R*/, 2, 2>::iterator it = spc.begin();
			*it = 42;
			assert(spc[0] == 42);
		},
		[]() {
			std::clog << "grid const_iterator access test\n";
			const wrapped_space<int, 1/*R*/, 2, 2> spc;
			wrapped_space<int, 1/*R*/, 2, 2>::const_iterator it = spc.begin();
			assert(*it == 0);
		},
		[]() {
			std::clog << "grid iterator pointer access test\n";
			wrapped_space<std::pair<int, int>, 1/*R*/, 2, 2> spc;
			wrapped_space<std::pair<int, int>, 1/*R*/, 2, 2>::iterator it = spc.begin();
			it->first = 42;
			it->second = 1;
			assert(spc[0] == std::make_pair(42, 1));
		},
		[]() {
			std::clog << "grid const_iterator pointer access test\n";
			const wrapped_space<std::pair<int, int>, 1/*R*/, 2, 2> spc;
			wrapped_space<std::pair<int, int>, 1/*R*/, 2, 2>::const_iterator it = spc.begin();
			assert(*it == std::make_pair(0, 0));
		},
		[]() {
			// "ISSUE #1: failure to auto-deduce the type stored in the neighboring cells\n";
			// hyper::unwrapped_space<int, 1/*R*/, 5> spc;
			// auto cell = spc.at(2);
			// 
			// for(auto& x : cell.neighbors())
			//     x = 42;  // compile error
			},
		[]() {
			// "ISSUE #2: vector<reference_wrapper<bool>> problem with range-for syntax\n";
			// hyper::unwrapped_space<bool, 1/*R*/, 5> spc;
			// auto cell = spc.at(2);
			//
			// for(bool& x : cell.neighbors())    // compile error
			//    x = true;
		}
		}; // namespace sprogar

		void run()
		{
			for (auto& t : all_tests)
				t();
		}

	} // namespace test
} // namespace sprogar
