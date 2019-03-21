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
#include <vector>

#include "../include/hyper.h"

namespace sprogar
{
namespace test
{
using namespace hyper;

typedef void (*voidfunc)();
std::vector<voidfunc> all_tests = {
    // null space neighbor offset
    []() {
        assert(wrapped_space_offsets<0>::neighbors_offsets(0) == std::vector<int>({}));
        assert(unwrapped_space_offsets<0>::neighbors_offsets(0) == std::vector<int>({}));
    },
    // single point space neighbor offset
    []() {
        assert(wrapped_space_offsets<1>::neighbors_offsets(0) == std::vector<int>({}));
        assert(unwrapped_space_offsets<1>::neighbors_offsets(0) == std::vector<int>({}));
    },
    // wrapped offsets 1-D
    []() {
        typedef wrapped_space_offsets<5> ws;

        // left point ws.id(0)
        assert(ws::neighbors_offsets(1) == std::vector<int>({1, 4}));

        // in-between points
        assert(ws::neighbors_offsets(0) == std::vector<int>({-1, 1}));

        // right point ws.id(4)
        assert(ws::neighbors_offsets(2) == std::vector<int>({-4, -1}));
    },
    // unwrapped offsets 1-D
    []() {
        typedef unwrapped_space_offsets<5> us;

        // left point ws.id(0)
        assert(us::neighbors_offsets(1) == std::vector<int>({1}));

        // in-between points
        assert(us::neighbors_offsets(0) == std::vector<int>({-1, 1}));

        // right point ws.id(4)
        assert(us::neighbors_offsets(2) == std::vector<int>({-1}));
    },
    // iterator default construction
    []() {
        unwrapped_space_offsets<4, 3, 2>::iterator it;
        assert(0 == it[0]);
        assert(0 == it[1]);
        assert(0 == it[2]);
    },
    // iterator construction
    []() {
        unwrapped_space_offsets<4, 3, 2>::iterator it(2, 0, 1);
        assert(1 == it[0]);
        assert(0 == it[1]);
        assert(2 == it[2]);
    },
    // iterator prefix increment 3-D
    []() {
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
    // debug neighbors' offsets 2-D
    []() {
        unwrapped_space_offsets<5, 10, 7>::iterator it(0, 0, 0);
        assert(make_offset<false>(it) == std::vector<int>({1, 7, 8, 70, 71, 77, 78}));
        assert(it.type() == 13);
        assert(it == 0);
    },
    // neighbors' offsets with neighbors_offsets link 1-D
    []() {
        typedef unwrapped_space_offsets<5> us5;

        assert(hyper::make_offset<false>(us5::iterator(0)) == us5::neighbors_offsets(1));
        assert(hyper::make_offset<false>(us5::iterator(1)) == us5::neighbors_offsets(0));
        assert(hyper::make_offset<false>(us5::iterator(4)) == us5::neighbors_offsets(2));
    },
    // neighborhood types 1-D
    []() {
        typedef unwrapped_space_offsets<5>::iterator it;

        assert(it(0).type() == 1);
        assert(it(1).type() == 0);
        assert(it(2).type() == 0);
        assert(it(3).type() == 0);
        assert(it(4).type() == 2);
    },
    // neighborhood types 2-D
    []() {
        typedef unwrapped_space_offsets<2, 4>::iterator it;
        assert(it(0, 0).type() == 4);
        assert(it(0, 1).type() == 1);
        assert(it(0, 3).type() == 7);
        assert(it(1, 0).type() == 5);
        assert(it(1, 1).type() == 2);
        assert(it(1, 3).type() == 8);
    },
    // neighbors' offsets 2-D
    []() {
        unwrapped_space_offsets<2, 4>::iterator it;  // [0,0]
        assert(hyper::make_offset<false>(it) == std::vector<int>({1, 4, 5}));
        assert(hyper::make_offset<true>(it) == std::vector<int>({1, 3, 4, 5, 7}));

        ++it;  // [0,1]
        assert(hyper::make_offset<false>(it) == std::vector<int>({-1, 1, 3, 4, 5}));
        assert(hyper::make_offset<true>(it) == std::vector<int>({-1, 1, 3, 4, 5}));

        ++it;  // [0,2]
        ++it;  // [0,3]
        ++it;  // [1,0]
        assert(hyper::make_offset<false>(it) == std::vector<int>({-4, -3, 1}));
        assert(hyper::make_offset<true>(it) == std::vector<int>({-4, -3, -1, 1, 3}));

        ++it;  // [1,1]
        ++it;  // [1,2]
        ++it;  // [1,3]
        assert(hyper::make_offset<false>(it) == std::vector<int>({-5, -4, -1}));
        assert(hyper::make_offset<true>(it) == std::vector<int>({-7, -5, -4, -3, -1}));
    },
    // initialize location with coordinate list
    []() {
        unwrapped_space_offsets<3, 4, 5>::iterator it(2, 1, 0);
        assert(it == 45U);
    },
    // neighbors' offsets and neighborhood types 3-D
    []() {
        unwrapped_space_offsets<3, 4, 5>::iterator it;  // [0,0,0]
        assert(hyper::make_offset<false>(it) == std::vector<int>({1, 5, 6, 20, 21, 25, 26}));
        assert(hyper::make_offset<true>(it) == std::vector<int>({1,  4,  5,  6,  9,  15, 16, 19, 20, 21, 24, 25, 26,
                                                                 29, 35, 36, 39, 40, 41, 44, 45, 46, 49, 55, 56, 59}));
        assert(it.type() == 13);
    },
    // neighbors' offsets and neighborhood types 3-D
    []() {
        unwrapped_space_offsets<3, 4, 5>::iterator it(0, 1, 1);  // [0,1,1]
        assert(hyper::make_offset<false>(it)
               == std::vector<int>({-6, -5, -4, -1, 1, 4, 5, 6, 14, 15, 16, 19, 20, 21, 24, 25, 26}));
        assert(hyper::make_offset<true>(it) == std::vector<int>({-6, -5, -4, -1, 1,  4,  5,  6,  14, 15, 16, 19, 20,
                                                                 21, 24, 25, 26, 34, 35, 36, 39, 40, 41, 44, 45, 46}));
        assert(it.type() == 1);
    },
    // neighbors' offsets of the first point in unwrapped 4-D space
    []() {
        typedef unwrapped_space_offsets<4, 4, 4, 4> space;
        space::iterator it;
        assert(space::neighbors_offsets(it.type())
               == std::vector<int>(
                      {1, 4, 5, 16, 17, 20, 21, 64, 64 + 1, 64 + 4, 64 + 5, 64 + 16, 64 + 17, 64 + 20, 64 + 21}));
    },
    // neighbors' offsets of the 80th point in unwrapped 4-D space
    []() {
        typedef unwrapped_space_offsets<4, 4, 4, 4> space;
        space::iterator it(1, 1, 0, 0);
        auto ans
            = std::vector<int>({-64 - 16, -64 - 15, -64 - 12, -64 - 11, -64 + 0, -64 + 1, -64 + 4, -64 + 5, -64 + 16,
                                -64 + 17, -64 + 20, -64 + 21, -16,      -15,     -12,     -11,     +1,      +4,
                                +5,       +16,      +17,      +20,      +21,     64 - 16, 64 - 15, 64 - 12, 64 - 11,
                                64 + 0,   64 + 1,   64 + 4,   64 + 5,   64 + 16, 64 + 17, 64 + 20, 64 + 21});

        assert(space::neighbors_offsets(it.type()) == ans);
    },
    // neighborhood types 3-D
    // extreme coordinates [0, X] are assigned respective weighs that are finally summed
    // dimension 1: 0=>1, X=>2
    // dimension 2: 0=>3, X=>6
    // dimension 3: 0=>9, X=>18
    // dimension 3: 0=>27, X=>54
    // ...
    []() {
        unwrapped_space_offsets<3, 4, 5>::iterator it0;
        assert(it0.type() == 13);  // touches the left(9), near(3) and bottom(1) sides;

        unwrapped_space_offsets<3, 4, 5>::iterator it15(0, 1, 1);
        assert(it15.type() == 1);  // touches the bottom(1) side only;

        unwrapped_space_offsets<3, 4, 5>::iterator it59(2, 3, 4);
        assert(it59.type() == 26);  // touches the right(18), far(6) and top(2) sides;
    },
    // another offsets 3-D
    []() {
        unwrapped_space_offsets<4, 4, 4>::iterator it(3, 3, 2);
        assert(hyper::make_offset<true>(it)
               == std::vector<int>({-61, -60, -59, -53, -52, -51, -49, -48, -47, -29, -28, -27, -21,
                                    -20, -19, -17, -16, -15, -13, -12, -11, -5,  -4,  -3,  -1,  +1}));
    },
    // non-existing coordinate
    []() {
        typedef unwrapped_space_offsets<2, 3> space;
        space::iterator it;
        assert(-1U == it[space::dimension()]);
    },
    // access to default location coordinates 2-D
    []() {
        typedef unwrapped_space_offsets<2, 3> space;
        space::iterator it;

        assert(0 == it[0]);
        assert(0 == it[1]);
    },
    // access to arbitrary location coordinates 3-D
    []() {
        typedef unwrapped_space_offsets<7, 5, 6> space;
        space::iterator it(2, 4, 1);
        assert(2 == it[2]);
        assert(4 == it[1]);
        assert(1 == it[0]);
    },
    // mapping arbitrary 3-D location to 1-D
    []() {
        hyper::location_iterator<7, 5, 6> it(2, 4, 1);
        assert(it == 2 * 5 * 6 + 4 * 6 + 1);
    },
    // default iterator is first location 2-D
    []() {
        unwrapped_space_offsets<2, 3>::iterator it;
        assert(it == 0U);
    },
    // placed iterator is at desired location 2-D
    []() {
        unwrapped_space_offsets<2, 3>::iterator it(1, 2);
        assert(it == 5U);
    },
    // iterator type and positioning
    []() {
        unwrapped_space_offsets<7, 3, 4>::iterator it0;
        assert(it0.type() == 13);
        assert(it0 == 0);
        unwrapped_space_offsets<7, 3, 4>::iterator it12(1, 0, 0);
        assert(it12.type() == 12);
        assert(it12 == 1 * 3 * 4);
    },
    // another offset type and positioning 2-D
    []() {
        unwrapped_space_offsets<7, 3>::iterator it17(5, 2);
        assert(it17.type() == 6);
        assert(it17 == 17U);
    },
    // yet another offsets 2-D
    []() {
        unwrapped_space_offsets<7, 3>::iterator it19(5, 2);
        assert(hyper::make_offset<false>(it19) == std::vector<int>({-4, -3, -1, +2, +3}));
        assert(hyper::make_offset<true>(it19) == std::vector<int>({-5, -4, -3, -2, -1, +1, +2, +3}));
    },
    // unwrapped 2-D neighborhood
    []() {
        auto ans = hyper::make_neighborhoods<false, 2, 3>();
        assert(ans.size() == 9);
        assert(ans[/*typ*/ 0].empty());
        assert(ans[/*typ*/ 1] == std::vector<int>({-1, 1, 2, 3, 4}));      // pos1d==1
        assert(ans[/*typ*/ 2] == std::vector<int>({-4, -3, -2, -1, +1}));  // pos1d==4
        assert(ans[/*typ*/ 3].empty());
        assert(ans[/*typ*/ 4] == std::vector<int>({1, 3, 4}));    // pos1d==0
        assert(ans[/*typ*/ 5] == std::vector<int>({-3, -2, 1}));  // pos1d==3
        assert(ans[/*typ*/ 6].empty());
        assert(ans[/*typ*/ 7] == std::vector<int>({-1, 2, 3}));    // pos1d==2
        assert(ans[/*typ*/ 8] == std::vector<int>({-4, -3, -1}));  // pos1d==5
    },
    // 3-D offsets, type and positioning
    []() {
        unwrapped_space_offsets<4, 5, 6>::iterator it(2, 0, 5);
        assert(hyper::make_offset<false>(it) == std::vector<int>({-31, -30, -25, -24, -1, 5, 6, 29, 30, 35, 36}));
        assert(it.type() == 21);
        assert(it == 65);
    },
    // space description
    []() {
        assert((wrapped_space_offsets<5, 2, 3, 1>::info()) == "5x2x3x1 wrapped");
        assert((unwrapped_space_offsets<1, 3, 2, 5>::info()) == "1x3x2x5 unwrapped");
    },
    // space dimension
    []() {
        typedef wrapped_space_offsets<5, 2, 3, 1> space;
        assert(space::dimension() == 4);
        assert(space::dimension(0) == 5);
        assert(space::dimension(1) == 2);
        assert(space::dimension(2) == 3);
        assert(space::dimension(3) == 1);
    },
    // space size
    []() {
        typedef wrapped_space_offsets<5, 2, 3, 1> space;
        assert(space::size() == 5 * 2 * 3 * 1);
    },
    // space positioning
    []() {
        typedef wrapped_space_offsets<5, 6, 7, 8> space;
        assert(0 == space::id(0, 0, 0, 0));
        assert(1 == space::id(1, 0, 0, 0));
        assert(4 + 1 * 5 == space::id(4, 1, 0, 0));
        assert(3 + 2 * 5 + 1 * 5 * 6 + 4 * 5 * 6 * 7 == space::id(3, 2, 1, 4));
        assert(space::size() - 1 == space::id(4, 5, 6, 7));
    },
    // space size initialization
    []() {
        wrapped_space<int, 1> spc;

        assert(spc.size() == 1);
    },
    // space initialization by const value
    []() {
        wrapped_space<int, 1> spc(42);

        assert(spc[0] == 42);
    },
    // space initialization by function
    []() {
        wrapped_space<int, 2> spc([]() {
            static int g = 42;
            return g++;
        });

        assert(42 == spc[0]);
        assert(43 == spc[1]);
    },
    // const_iterator read
    []() {
        wrapped_space<int, 2, 2> spc(7);

        int total = 0;
        for(auto it = spc.begin(); it != spc.end(); ++it)
            total += *it;

        assert(7 * 2 * 2 == total);
    },
    // iterator write
    []() {
        wrapped_space<int, 2, 2> spc;

        int i = 1;
        for(auto& x : spc)
            x = i++;

        int total = 0;
        for(auto x : spc)
            total += x;

        assert(1 + 2 + 3 + 4 == total);
    },
    // implicit move semantics
    []() {
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
    // implicit copy semantics
    []() {
        struct copyable {
            bool copied = false;
            copyable() {}
            copyable(const copyable&) : copied(true){};
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
    // README.md usage
    []() {
        hyper::wrapped_space<int, 5, 7> spc{0};
        spc(3, 4) = 42;

        assert(spc[0] == 0);
        assert(spc[25] == 42);
    },
    // README.md usage
    []() {
        std::array<int, 5 * 7> spc = {0};  // 1-D memory allocation ...

        location_iterator<5, 7> it(3, 4);
        spc[it] = 42;

        assert(spc[0] == 0);
        assert(spc[25] == 42);
    },

    // API functionality: default construction
    []() {
        hyper::unwrapped_space<int, 3> spc;
        assert(spc[0] == 0);
        assert(spc[1] == 0);
        assert(spc[2] == 0);
    },
    // API functionality: construction with initialization
    []() {
        hyper::unwrapped_space<int, 3> spc{42};
        assert(spc[0] == 42);
        assert(spc[1] == 42);
        assert(spc[2] == 42);
    },
    // API functionality: construction by lambda
    []() {
        hyper::unwrapped_space<int, 3> spc{[]() {
            static int i = 0;
            return i++;
        }};
        assert(spc[0] == 0);
        assert(spc[1] == 1);
        assert(spc[2] == 2);
    },
    // API functionality: random access syntax 1
    []() {
        hyper::unwrapped_space<int, 3, 4> spc{0};
        spc[1] = 42;

        assert(spc[0] == 0);
        assert(spc[1] == 42);
        assert(spc[11] == 0);
    },
    // API functionality: random access syntax 2
    []() {
        hyper::unwrapped_space<int, 3, 4> spc{0};
        spc(0, 1) = 42;

        assert(spc[0] == 0);
        assert(spc[1] == 42);
        assert(spc[11] == 0);
    },
    // API functionality: random access syntax 3
    []() {
        hyper::unwrapped_space<int, 3, 4> spc{0};
        auto cell = spc.at(0, 1);
        *cell = 42;

        assert(spc[0] == 0);
        assert(spc[1] == 42);
        assert(spc[11] == 0);
    },
    // API functionality: cell iteration syntax 1
    []() {
        hyper::unwrapped_space<int, 5> spc{0};
        for(auto it = spc.begin(); it != spc.end(); ++it)
            *it = 42;

        assert(spc[0] == 42);
        assert(spc[1] == 42);
        assert(spc[2] == 42);
        assert(spc[3] == 42);
        assert(spc[4] == 42);
    },
    // API functionality: cell iteration syntax 2
    []() {
        hyper::unwrapped_space<int, 5> spc{0};
        for(int& x : spc)
            x = 42;

        assert(spc[0] == 42);
        assert(spc[4] == 42);
    },
    // API functionality: neighbors access syntax 1
    []() {
        hyper::unwrapped_space<int, 5> spc{0};
        auto cell = spc.at(2);

        for(int off : cell.neighbors_offsets())
            spc[cell + off] = 42;

        assert(spc[0] == 0);
        assert(spc[1] == 42);
        assert(spc[2] == 0);
        assert(spc[3] == 42);
        assert(spc[4] == 0);
    },
    // API functionality:  neighbors access syntax 2
    []() {
        hyper::unwrapped_space<int, 5> spc{0};
        auto cell = spc.at(2);

        for(int off : cell)
            spc[cell + off] = 42;

        assert(spc[0] == 0);
        assert(spc[1] == 42);
        assert(spc[2] == 0);
        assert(spc[3] == 42);
        assert(spc[4] == 0);
    },
    // API functionality:  neighbors access syntax 3
    []() {
        hyper::unwrapped_space<int, 5> spc{0};
        auto cell = spc.at(2);

        for(auto off = cell.begin(); off != cell.end(); ++off)
            spc[cell + *off] = 42;

        assert(spc[0] == 0);
        assert(spc[1] == 42);
        assert(spc[2] == 0);
        assert(spc[3] == 42);
        assert(spc[4] == 0);
    },
    // API functionality: neighbors access syntax 4
    []() {
        hyper::unwrapped_space<int, 5> spc{0};
        auto cell = spc.at(2);

        for(int off : cell.neighbors_offsets())
            cell[off] = 42;

        assert(spc[0] == 0);
        assert(spc[1] == 42);
        assert(spc[2] == 0);
        assert(spc[3] == 42);
        assert(spc[4] == 0);
    },
    // API functionality:  neighbors access syntax 5
    []() {
        hyper::unwrapped_space<int, 5> spc{0};
        auto cell = spc.at(2);

        for(int x : cell)
            cell[x] = 42;

        assert(spc[0] == 0);
        assert(spc[1] == 42);
        assert(spc[2] == 0);
        assert(spc[3] == 42);
        assert(spc[4] == 0);
    },
    // API functionality:  neighbors access syntax 6
    []() {
        hyper::unwrapped_space<int, 5> spc{0};
        auto cell = spc.at(2);

        for(int& x : cell.neighbors())
            x = 42;

        assert(spc[0] == 0);
        assert(spc[1] == 42);
        assert(spc[2] == 0);
        assert(spc[3] == 42);
        assert(spc[4] == 0);
    },
    // ISSUE #1: failure to auto-deduce the type stored in the neighboring cells
    []() {
        // hyper::unwrapped_space<int, 5> spc{0};
        // auto cell = spc.at(2);
        
        // for(auto& x : cell.neighbors())
        //     x = 42;  // compile error
    },
    // ISSUE #2: vector<reference_wrapper<bool>> problem with range-for syntax
    []() {
         // hyper::unwrapped_space<bool, 5> spc{false};
         // auto cell = spc.at(2);

         // for(bool& x : cell.neighbors())    // compile error
         //    x = true;
    }
};  // namespace sprogar

void run()
{
    for(auto& t : all_tests)
        t();
}

}  // namespace test
}  // namespace sprogar
