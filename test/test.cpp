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

#include "../include/hyper.h"

namespace sprogar {
namespace test {
    using namespace hyper;

    typedef void (*voidfunc)();
    std::vector<voidfunc> all_tests = {
        []() {
            std::clog << "null space neighbor offset test\n";
            assert(wrapped_space_offsets<0>::neighbors_offsets(0) == std::vector<int>({}));
            assert(unwrapped_space_offsets<0>::neighbors_offsets(0) == std::vector<int>({}));
        },
        []() {
            std::clog << "single point space neighbor offset test\n";
            assert(wrapped_space_offsets<1>::neighbors_offsets(0) == std::vector<int>({}));
            assert(unwrapped_space_offsets<1>::neighbors_offsets(0) == std::vector<int>({}));
        },
        []() {
            std::clog << "1-D wrapped offsets test\n";
            typedef wrapped_space_offsets<5> ws;

            // left point ws.id(0)
            assert(ws::neighbors_offsets(1) == std::vector<int>({ 1, 4 }));

            // in-between points
            assert(ws::neighbors_offsets(0) == std::vector<int>({ -1, 1 }));

            // right point ws.id(4)
            assert(ws::neighbors_offsets(2) == std::vector<int>({ -4, -1 }));
        },
        []() {
            std::clog << "1-D unwrapped offsets test\n";
            typedef unwrapped_space_offsets<5> us;

            // left point ws.id(0)
            assert(us::neighbors_offsets(1) == std::vector<int>({ 1 }));

            // in-between points
            assert(us::neighbors_offsets(0) == std::vector<int>({ -1, 1 }));

            // right point ws.id(4)
            assert(us::neighbors_offsets(2) == std::vector<int>({ -1 }));
        },
        []() {
            std::clog << "iterator default construction test\n";
            unwrapped_space_offsets<4, 3, 2>::iterator it;
            assert(0 == it[0]);
            assert(0 == it[1]);
            assert(0 == it[2]);
        },
        []() {
            std::clog << "iterator construction test\n";
            unwrapped_space_offsets<4, 3, 2>::iterator it(2, 0, 1);
            assert(1 == it[0]);
            assert(0 == it[1]);
            assert(2 == it[2]);
        },
        []() {
            std::clog << "iterator prefix increment 3-D test\n";
            unwrapped_space_offsets<4, 3, 2>::iterator it;
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
            std::clog << "debug neighbors' offsets 2-D test\n";

            unwrapped_space_offsets<5, 10, 7>::iterator it(0, 0, 0);
            assert(make_offset<false>(it) == std::vector<int>({ 1, 7, 8, 70, 71, 77, 78 }));
            assert(it.type() == 13);
            assert(it == 0);
        },
        []() {
            std::clog << "neighbors' offsets with neighbors_offsets link 1-D test\n";
            typedef unwrapped_space_offsets<5> us5;

            assert(hyper::make_offset<false>(us5::iterator(0)) == us5::neighbors_offsets(1));
            assert(hyper::make_offset<false>(us5::iterator(1)) == us5::neighbors_offsets(0));
            assert(hyper::make_offset<false>(us5::iterator(4)) == us5::neighbors_offsets(2));
        },
        []() {
            std::clog << "neighborhood types 1-D test\n";
            typedef unwrapped_space_offsets<5>::iterator it;

            assert(it(0).type() == 1);
            assert(it(1).type() == 0);
            assert(it(2).type() == 0);
            assert(it(3).type() == 0);
            assert(it(4).type() == 2);
        },
        []() {
            std::clog << "neighborhood types 2-D test\n";
            typedef unwrapped_space_offsets<2, 4>::iterator it;
            assert(it(0, 0).type() == 4);
            assert(it(0, 1).type() == 1);
            assert(it(0, 3).type() == 7);
            assert(it(1, 0).type() == 5);
            assert(it(1, 1).type() == 2);
            assert(it(1, 3).type() == 8);
        },
        []() {
            std::clog << "neighbors' offsets 2-D test\n";
            unwrapped_space_offsets<2, 4>::iterator it; // [0,0]
            assert(hyper::make_offset<false>(it) == std::vector<int>({ 1, 4, 5 }));
            assert(hyper::make_offset<true>(it) == std::vector<int>({ 1, 3, 4, 5, 7 }));

            ++it; // [0,1]
            assert(hyper::make_offset<false>(it) == std::vector<int>({ -1, 1, 3, 4, 5 }));
            assert(hyper::make_offset<true>(it) == std::vector<int>({ -1, 1, 3, 4, 5 }));

            ++it; // [0,2]
            ++it; // [0,3]
            ++it; // [1,0]
            assert(hyper::make_offset<false>(it) == std::vector<int>({ -4, -3, 1 }));
            assert(hyper::make_offset<true>(it) == std::vector<int>({ -4, -3, -1, 1, 3 }));

            ++it; // [1,1]
            ++it; // [1,2]
            ++it; // [1,3]
            assert(hyper::make_offset<false>(it) == std::vector<int>({ -5, -4, -1 }));
            assert(hyper::make_offset<true>(it) == std::vector<int>({ -7, -5, -4, -3, -1 }));
        },
        []() {
            std::clog << "initialize location with coordinate list test\n";
            unwrapped_space_offsets<3, 4, 5>::iterator it(2, 1, 0);
            assert(it == 45U);
        },
        []() {
            std::clog << "neighbors' offsets and neighborhood types 3-D test\n";
            unwrapped_space_offsets<3, 4, 5>::iterator it; // [0,0,0]
            assert(hyper::make_offset<false>(it) == std::vector<int>({ 1, 5, 6, 20, 21, 25, 26 }));
            assert(hyper::make_offset<true>(it) ==
                std::vector<int>({ 1, 4, 5, 6, 9, 15, 16, 19, 20, 21, 24, 25, 26, 29, 35, 36, 39, 40, 41, 44, 45, 46,
                    49, 55, 56, 59 }));
            assert(it.type() == 13);
        },
        []() {
            std::clog << "neighbors' offsets and neighborhood types 3-D test\n";
            unwrapped_space_offsets<3, 4, 5>::iterator it(0, 1, 1); // [0,1,1]
            assert(hyper::make_offset<false>(it) ==
                std::vector<int>({ -6, -5, -4, -1, 1, 4, 5, 6, 14, 15, 16, 19, 20, 21, 24, 25, 26 }));
            assert(hyper::make_offset<true>(it) ==
                std::vector<int>({ -6, -5, -4, -1, 1, 4, 5, 6, 14, 15, 16, 19, 20, 21, 24, 25, 26, 34, 35, 36, 39, 40,
                    41, 44, 45, 46 }));
            assert(it.type() == 1);
        },
        []() {
            std::clog << "neighbors' offsets of the first point in unwrapped 4-D space test\n";
            typedef unwrapped_space_offsets<4, 4, 4, 4> space;
            space::iterator it;
            assert(space::neighbors_offsets(it.type()) ==
                std::vector<int>(
                    { 1, 4, 5, 16, 17, 20, 21, 64, 64 + 1, 64 + 4, 64 + 5, 64 + 16, 64 + 17, 64 + 20, 64 + 21 }));
        },
        []() {
            std::clog << "neighbors' offsets of the 80th point in unwrapped 4-D space test\n";
            typedef unwrapped_space_offsets<4, 4, 4, 4> space;
            space::iterator it(1, 1, 0, 0);
            auto ans = std::vector<int>({ -64 - 16, -64 - 15, -64 - 12, -64 - 11, -64 + 0, -64 + 1, -64 + 4, -64 + 5,
                -64 + 16, -64 + 17, -64 + 20, -64 + 21, -16, -15, -12, -11, +1, +4, +5, +16, +17, +20, +21, 64 - 16,
                64 - 15, 64 - 12, 64 - 11, 64 + 0, 64 + 1, 64 + 4, 64 + 5, 64 + 16, 64 + 17, 64 + 20, 64 + 21 });

            assert(space::neighbors_offsets(it.type()) == ans);
        },
        // 
        // extreme coordinates [0, X] are assigned respective weighs that are finally summed
        // dimension 1: 0=>1, X=>2
        // dimension 2: 0=>3, X=>6
        // dimension 3: 0=>9, X=>18
        // dimension 3: 0=>27, X=>54
        // ...
        []() {
            std::clog << "neighborhood types 3-D test\n";
            unwrapped_space_offsets<3, 4, 5>::iterator it0;
            assert(it0.type() == 13); // touches the left(9), near(3) and bottom(1) sides;

            unwrapped_space_offsets<3, 4, 5>::iterator it15(0, 1, 1);
            assert(it15.type() == 1); // touches the bottom(1) side only;

            unwrapped_space_offsets<3, 4, 5>::iterator it59(2, 3, 4);
            assert(it59.type() == 26); // touches the right(18), far(6) and top(2) sides;
        },
        []() {
            std::clog << "another offsets 3-D test\n";
            unwrapped_space_offsets<4, 4, 4>::iterator it(3, 3, 2);
            assert(hyper::make_offset<true>(it) ==
                std::vector<int>({ -61, -60, -59, -53, -52, -51, -49, -48, -47, -29, -28, -27, -21, -20, -19, -17, -16,
                    -15, -13, -12, -11, -5, -4, -3, -1, +1 }));
        },
        []() {
            std::clog << "non-existing coordinate test\n";
            typedef unwrapped_space_offsets<2, 3> space;
            space::iterator it;
            assert(-1U == it[space::dimension()]);
        },
        []() {
            std::clog << "access to default location coordinates 2-D test\n";
            typedef unwrapped_space_offsets<2, 3> space;
            space::iterator it;

            assert(0 == it[0]);
            assert(0 == it[1]);
        },
        []() {
            std::clog << "access to arbitrary location coordinates 3-D test\n";
            typedef unwrapped_space_offsets<7, 5, 6> space;
            space::iterator it(2, 4, 1);
            assert(2 == it[2]);
            assert(4 == it[1]);
            assert(1 == it[0]);
        },
        []() {
            std::clog << "mapping arbitrary 3-D location to 1-D test\n";
            hyper::location_iterator<7, 5, 6> it(2, 4, 1);
            assert(it == 2 * 5 * 6 + 4 * 6 + 1);
        },
        []() {
            std::clog << "default iterator is first location 2-D test\n";
            unwrapped_space_offsets<2, 3>::iterator it;
            assert(it == 0U);
        },
        []() {
            std::clog << "placed iterator is at desired location 2-D test\n";
            unwrapped_space_offsets<2, 3>::iterator it(1, 2);
            assert(it == 5U);
        },
        []() {
            std::clog << "iterator type and positioning test\n";
            unwrapped_space_offsets<7, 3, 4>::iterator it0;
            assert(it0.type() == 13);
            assert(it0 == 0);
            unwrapped_space_offsets<7, 3, 4>::iterator it12(1, 0, 0);
            assert(it12.type() == 12);
            assert(it12 == 1 * 3 * 4);
        },
        []() {
            std::clog << "another offset type and positioning 2-D test\n";
            unwrapped_space_offsets<7, 3>::iterator it17(5, 2);
            assert(it17.type() == 6);
            assert(it17 == 17U);
        },
        []() {
            std::clog << "yet another offsets 2-D test\n";
            unwrapped_space_offsets<7, 3>::iterator it19(5, 2);
            assert(hyper::make_offset<false>(it19) == std::vector<int>({ -4, -3, -1, +2, +3 }));
            assert(hyper::make_offset<true>(it19) == std::vector<int>({ -5, -4, -3, -2, -1, +1, +2, +3 }));
        },
        []() {
            std::clog << "unwrapped 2-D neighborhood test\n";
            auto ans = hyper::make_neighborhoods<false, 2, 3>();
            assert(ans.size() == 9);
            assert(ans[/*typ*/ 0].empty());
            assert(ans[/*typ*/ 1] == std::vector<int>({ -1, 1, 2, 3, 4 }));     // pos1d==1
            assert(ans[/*typ*/ 2] == std::vector<int>({ -4, -3, -2, -1, +1 })); // pos1d==4
            assert(ans[/*typ*/ 3].empty());
            assert(ans[/*typ*/ 4] == std::vector<int>({ 1, 3, 4 }));   // pos1d==0
            assert(ans[/*typ*/ 5] == std::vector<int>({ -3, -2, 1 })); // pos1d==3
            assert(ans[/*typ*/ 6].empty());
            assert(ans[/*typ*/ 7] == std::vector<int>({ -1, 2, 3 }));   // pos1d==2
            assert(ans[/*typ*/ 8] == std::vector<int>({ -4, -3, -1 })); // pos1d==5
        },
        []() {
            std::clog << "3-D offsets, type and positioning test\n";
            unwrapped_space_offsets<4, 5, 6>::iterator it(2, 0, 5);
            assert(hyper::make_offset<false>(it) == std::vector<int>({ -31, -30, -25, -24, -1, 5, 6, 29, 30, 35, 36 }));
            assert(it.type() == 21);
            assert(it == 65);
        },
        []() {
            std::clog << "iterator API: neighborhood size test\n";
            unwrapped_space<bool, 3, 3> u2d;

            auto it = u2d.begin();
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
            unwrapped_space<bool, 3, 3> u2d;

            auto it = u2d.begin();
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
            std::clog << "space description test\n";
            assert((wrapped_space_offsets<5, 2, 3, 1>::info()) == "5x2x3x1 wrapped");
            assert((unwrapped_space_offsets<1, 3, 2, 5>::info()) == "1x3x2x5 unwrapped");
        },
        []() {
            std::clog << "space dimension test\n";
            typedef wrapped_space_offsets<5, 2, 3, 1> space;
            assert(space::dimension() == 4);
            assert(space::dimension(0) == 5);
            assert(space::dimension(1) == 2);
            assert(space::dimension(2) == 3);
            assert(space::dimension(3) == 1);
        },
        []() {
            std::clog << "space size test\n";
            typedef wrapped_space_offsets<5, 2, 3, 1> space;
            assert(space::size() == 5 * 2 * 3 * 1);
        },
        []() {
            std::clog << "space positioning test\n";
            typedef wrapped_space_offsets<5, 6, 7, 8> space;
            assert(0 == space::id(0, 0, 0, 0));
            assert(1 == space::id(1, 0, 0, 0));
            assert(4 + 1 * 5 == space::id(4, 1, 0, 0));
            assert(3 + 2 * 5 + 1 * 5 * 6 + 4 * 5 * 6 * 7 == space::id(3, 2, 1, 4));
            assert(space::size() - 1 == space::id(4, 5, 6, 7));
        },
        []() {
            std::clog << "space size initialization test\n";
            wrapped_space<int, 1> spc;

            assert(spc.size() == 1);
        },
        []() {
            std::clog << "space initialization by const value test\n";
            wrapped_space<int, 1> spc(42);

            assert(spc[0] == 42);
        },
        []() {
            std::clog << "space initialization by function test\n";
            wrapped_space<int, 2> spc([]() {
                static int g = 42;
                return g++;
            });

            assert(42 == spc[0]);
            assert(43 == spc[1]);
        },
        []() {
            std::clog << "const_iterator read test\n";
            wrapped_space<int, 2, 2> spc(7);

            int total = 0;
            for(auto it = spc.begin(); it != spc.end(); ++it)
                total += *it;

            assert(7 * 2 * 2 == total);
        },
        []() {
            std::clog << "iterator write test\n";
            wrapped_space<int, 2, 2> spc;

            int i = 1;
            for(auto& x : spc)
                x = i++;

            int total = 0;
            for(auto x : spc)
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
            wrapped_space<uncopyable, 3> spc1;
            wrapped_space<uncopyable, 3> spc2;

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
                    : copied(true){};
                copyable& operator=(const copyable& oth)
                {
                    copied = true;
                    return *this;
                }
            };
            wrapped_space<copyable, 3> spc1;
            assert(spc1[0].copied == false);
            wrapped_space<copyable, 3> spc2(spc1);
            assert(spc2[0].copied == true);

            spc1 = spc2;
            assert(spc1[0].copied == true);
        },
        []() {
            std::clog << "README.md usage test\n";
            hyper::wrapped_space<int, 5, 7> spc{ 0 };
            spc(3, 4) = 42;

            assert(spc[0] == 0);
            assert(spc[25] == 42);
        },
        []() {
            std::clog << "README.md usage test\n";
            std::array<int, 5 * 7> spc = { 0 }; // 1-D memory allocation ...

            location_iterator<5, 7> it(3, 4);
            spc[it] = 42;

            assert(spc[0] == 0);
            assert(spc[25] == 42);
        },

        []() {
            std::clog << "API functionality: default construction test\n";
            hyper::unwrapped_space<int, 3> spc;
            assert(spc[0] == 0);
            assert(spc[1] == 0);
            assert(spc[2] == 0);
        },
        []() {
            std::clog << "API functionality: construction with initialization test\n";
            hyper::unwrapped_space<int, 3> spc{ 42 };
            assert(spc[0] == 42);
            assert(spc[1] == 42);
            assert(spc[2] == 42);
        },
        []() {
            std::clog << "API functionality: construction by lambda test\n";
            hyper::unwrapped_space<int, 3> spc{ []() {
                static int i = 0;
                return i++;
            } };
            assert(spc[0] == 0);
            assert(spc[1] == 1);
            assert(spc[2] == 2);
        },
        []() {
            std::clog << "API functionality: random access syntax 1 test\n";
            hyper::unwrapped_space<int, 3, 4> spc{ 0 };
            spc[1] = 42;

            assert(spc[0] == 0);
            assert(spc[1] == 42);
            assert(spc[11] == 0);
        },
        []() {
            std::clog << "API functionality: random access syntax 2 test\n";
            hyper::unwrapped_space<int, 3, 4> spc{ 0 };
            spc(0, 1) = 42;

            assert(spc[0] == 0);
            assert(spc[1] == 42);
            assert(spc[11] == 0);
        },
        []() {
            std::clog << "API functionality: random access syntax 3 test\n";
            hyper::unwrapped_space<int, 3, 4> spc{ 0 };
            auto cell = spc.at(0, 1);
            *cell = 42;

            assert(spc[0] == 0);
            assert(spc[1] == 42);
            assert(spc[11] == 0);
        },
        []() {
            std::clog << "API functionality: cell iteration syntax 1 test\n";
            hyper::unwrapped_space<int, 5> spc{ 0 };
            for(auto it = spc.begin(); it != spc.end(); ++it)
                *it = 42;

            assert(spc[0] == 42);
            assert(spc[1] == 42);
            assert(spc[2] == 42);
            assert(spc[3] == 42);
            assert(spc[4] == 42);
        },
        []() {
            std::clog << "API functionality: cell iteration syntax 2 test\n";
            hyper::unwrapped_space<int, 5> spc{ 0 };
            for(int& x : spc)
                x = 42;

            assert(spc[0] == 42);
            assert(spc[4] == 42);
        },
        []() {
            std::clog << "API functionality: neighbors access syntax 1 test\n";
            hyper::unwrapped_space<int, 5> spc{ 0 };
            auto cell = spc.at(2);

            for(int off : cell.neighbors_offsets())
                spc[cell + off] = 42;

            assert(spc[0] == 0);
            assert(spc[1] == 42);
            assert(spc[2] == 0);
            assert(spc[3] == 42);
            assert(spc[4] == 0);
        },
        []() {
            std::clog << "API functionality:  neighbors access syntax 2 test\n";
            hyper::unwrapped_space<int, 5> spc{ 0 };
            auto cell = spc.at(2);

            for(int off : cell)
                spc[cell + off] = 42;

            assert(spc[0] == 0);
            assert(spc[1] == 42);
            assert(spc[2] == 0);
            assert(spc[3] == 42);
            assert(spc[4] == 0);
        },
        []() {
            std::clog << "API functionality:  neighbors access syntax 3 test\n";
            hyper::unwrapped_space<int, 5> spc{ 0 };
            auto cell = spc.at(2);

            for(auto off = cell.begin(); off != cell.end(); ++off)
                spc[cell + *off] = 42;

            assert(spc[0] == 0);
            assert(spc[1] == 42);
            assert(spc[2] == 0);
            assert(spc[3] == 42);
            assert(spc[4] == 0);
        },
        []() {
            std::clog << "API functionality: neighbors access syntax 4 test\n";
            hyper::unwrapped_space<int, 5> spc{ 0 };
            auto cell = spc.at(2);

            for(int off : cell.neighbors_offsets())
                cell[off] = 42;

            assert(spc[0] == 0);
            assert(spc[1] == 42);
            assert(spc[2] == 0);
            assert(spc[3] == 42);
            assert(spc[4] == 0);
        },
        []() {
            std::clog << "API functionality:  neighbors access syntax 5 test\n";
            hyper::unwrapped_space<int, 5> spc{ 0 };
            auto cell = spc.at(2);

            for(int x : cell)
                cell[x] = 42;

            assert(spc[0] == 0);
            assert(spc[1] == 42);
            assert(spc[2] == 0);
            assert(spc[3] == 42);
            assert(spc[4] == 0);
        },
        []() {
            std::clog << "API functionality:  neighbors access syntax 6 test\n";
            hyper::unwrapped_space<int, 5> spc{ 0 };
            auto cell = spc.at(2);

            for(int& x : cell.neighbors())
                x = 42;

            assert(spc[0] == 0);
            assert(spc[1] == 42);
            assert(spc[2] == 0);
            assert(spc[3] == 42);
            assert(spc[4] == 0);
        },
        []() {
            std::clog << "grid iterator access test\n";
            wrapped_space<int, 2, 2> spc;
            wrapped_space<int, 2, 2>::iterator it = spc.begin();
            *it = 42;
            assert(spc[0] == 42);
        },
        []() {
            std::clog << "grid const_iterator access test\n";
            const wrapped_space<int, 2, 2> spc;
            wrapped_space<int, 2, 2>::const_iterator it = spc.begin();
            assert(*it == 0);
        },
        []() {
            std::clog << "grid iterator pointer access test\n";
            wrapped_space<std::pair<int, int>, 2, 2> spc;
            wrapped_space<std::pair<int, int>, 2, 2>::iterator it = spc.begin();
            it->first = 42;
            it->second = 1;
            assert(spc[0] == std::make_pair(42, 1));
        },
        []() {
            std::clog << "grid const_iterator pointer access test\n";
            const wrapped_space<std::pair<int, int>, 2, 2> spc;
            wrapped_space<std::pair<int, int>, 2, 2>::const_iterator it = spc.begin();
            assert(*it == std::make_pair(0, 0));
        },
        []() {
            std::clog << "ISSUE #1: failure to auto-deduce the type stored in the neighboring cells";
            // hyper::unwrapped_space<int, 5> spc{0};
            // auto cell = spc.at(2);

            // for(auto& x : cell.neighbors())
            //     x = 42;  // compile error
        },
        []() {
            std::clog << "ISSUE #2: vector<reference_wrapper<bool>> problem with range-for syntax";
            // hyper::unwrapped_space<bool, 5> spc{false};
            // auto cell = spc.at(2);

            // for(bool& x : cell.neighbors())    // compile error
            //    x = true;
        },
    }; // namespace sprogar

    void run()
    {
        for(auto& t : all_tests)
            t();
    }

} // namespace test
} // namespace sprogar
