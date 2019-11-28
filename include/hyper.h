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

#ifndef _SPROGAR_HYPERSPACE_H_
#define _SPROGAR_HYPERSPACE_H_

#include <cassert>
#include <cmath>
#include <functional>
#include <map>
#include <set>
#include <vector>

namespace sprogar {
namespace hyper {
    template <unsigned... XX>
    class location_iterator {
    protected:
        struct dummy {
            unsigned value;
            dummy(unsigned u)
                : value(u){};
        };
        size_t pos1d;

        location_iterator(dummy pos)
            : pos1d{ pos.value }
        {
        }

        template <unsigned>
        inline unsigned neighborhood_type() const
        {
            return 0;
        }
        inline void increment() {}
        inline size_t size() const { return 1; }

    public:
        location_iterator()
            : pos1d{ 0 }
        {
        }

        inline bool operator!=(const location_iterator& other) const { return pos1d != other.pos1d; }
        inline operator size_t() const { return pos1d; }

        inline bool prefix_inc()
        {
            pos1d += 1;
            return true;
        }
        inline unsigned operator[](unsigned) const { return -1; }

        static inline location_iterator begin() { return location_iterator(); }
        static inline location_iterator end() { return location_iterator(); }
        template <bool>
        inline unsigned make_offset(std::set<int>&, int, unsigned) const
        {
            return 1;
        }
    };

    template <unsigned X, unsigned... XX>
    class location_iterator<X, XX...> : public location_iterator<XX...> {
        unsigned coordinate;

    protected:
        template <typename... CC>
        location_iterator(typename location_iterator<XX...>::dummy pos, unsigned c, CC... cc)
            : location_iterator<XX...>(typename location_iterator<XX...>::dummy(pos.value * X + c), cc...)
            , coordinate(c)
        {
        }

        template <unsigned D>
        inline unsigned neighborhood_type() const
        {
            unsigned typ = 0;

            if(coordinate == 0)
                typ += 1 * D;
            else if(coordinate == X - 1)
                typ += 2 * D;

            typ += location_iterator<XX...>::template neighborhood_type<3 * D>();
            return typ;
        }
        inline size_t size() const { return X * location_iterator<XX...>::size(); }
        inline bool prefix_inc()
        {
            if(location_iterator<XX...>::prefix_inc()) {
                coordinate += 1;
                if(coordinate == X) {
                    coordinate = 0;
                    return true;
                }
            }
            return false;
        }

    public:
        location_iterator()
            : location_iterator<XX...>()
            , coordinate{ 0 }
        {
        }

        template <typename... CC>
        location_iterator(unsigned c, CC... cc)
            : location_iterator<XX...>(typename location_iterator<XX...>::dummy(c), cc...)
            , coordinate(c)
        {
        }

        inline unsigned operator[](unsigned d) const
        {
            if(d == sizeof...(XX))
                return coordinate;
            return location_iterator<XX...>::operator[](d);
        }

        inline bool operator!=(const location_iterator<X, XX...>& other) const
        {
            return location_iterator<>::pos1d != other.pos1d;
        }
        inline operator size_t() const { return location_iterator<>::pos1d; }

        inline unsigned type() const { return neighborhood_type<1>(); }

        inline location_iterator& operator++()
        {
            prefix_inc();
            return *this;
        }

        static inline location_iterator begin() { return location_iterator(); }
        static inline location_iterator end()
        {
            location_iterator loc;
            loc.pos1d = loc.size();
            return loc;
        }

        template <bool wrap>
        unsigned make_offset(std::set<int>& set, int offset, unsigned plane) const
        {
            plane = location_iterator<XX...>::template make_offset<wrap>(set, offset, plane);

            if(coordinate != 0) {
                unsigned P = location_iterator<XX...>::template make_offset<wrap>(set, offset - plane, plane);
                set.insert(offset - P);
            }
            else if(wrap) {
                unsigned P = location_iterator<XX...>::template make_offset<wrap>(set, offset + (X - 1) * plane, plane);
                set.insert(offset + (X - 1) * P);
            }

            set.insert(offset);

            if(coordinate != X - 1) {
                unsigned P = location_iterator<XX...>::template make_offset<wrap>(set, offset + plane, plane);
                set.insert(offset + P);
            }
            else if(wrap) {
                unsigned P = location_iterator<XX...>::template make_offset<wrap>(set, offset - (X - 1) * plane, plane);
                set.insert(offset - (X - 1) * P);
            }

            return X * plane;
        }
    };

    template <bool wrap>
    inline std::vector<int> make_offset(const location_iterator<>& loc)
    {
        return std::vector<int>();
    }
    template <bool wrap, unsigned X, unsigned... XX>
    std::vector<int> make_offset(const location_iterator<X, XX...>& loc)
    {
        std::set<int> set;

        loc.template make_offset<wrap>(set, 0, 1);

        set.erase(0);
        std::vector<int> vec;
        vec.reserve(set.size());
        for(auto x : set)
            vec.push_back(x);

        return vec;
    }

    template <bool wrap, unsigned... XX>
    class iterable_space {
    protected:
        template <unsigned>
        static inline unsigned id_helper()
        {
            return 0;
        }

    public:
        static inline constexpr std::string info() { return wrap ? "wrapped" : "unwrapped"; }
        static inline constexpr unsigned dimension() { return 0; }
        static inline constexpr unsigned dimension(unsigned) { return 0; }
        static inline constexpr size_t size() { return 1; }

        static inline constexpr location_iterator<> begin() { return location_iterator<>::begin(); }
        static inline constexpr location_iterator<> end() { return location_iterator<>::end(); }
    };

    template <bool wrap, unsigned X, unsigned... XX>
    class iterable_space<wrap, X, XX...> : public iterable_space<wrap, XX...> {
    protected:
        template <unsigned S, typename... UU>
        static inline unsigned id_helper(unsigned x, UU... uu)
        {
            assert(x < X);
            return x * S + iterable_space<wrap, XX...>::template id_helper<X * S>(uu...);
        }

    public:
        static inline constexpr std::string info()
        {
            return std::to_string(X) + (sizeof...(XX) > 0 ? 'x' : ' ') + iterable_space<wrap, XX...>::info();
        }
        static inline constexpr unsigned dimension() { return 1 + sizeof...(XX); }
        static inline constexpr unsigned dimension(unsigned D)
        {
            return D == 0 ? X : iterable_space<wrap, XX...>::dimension(D - 1);
        }
        static inline constexpr size_t size() { return X * iterable_space<wrap, XX...>::size(); }
        template <typename... UU>
        static inline unsigned id(UU... uu)
        {
            assert(sizeof...(UU) == 1 + sizeof...(XX));
            return id_helper<1U>(static_cast<unsigned>(uu)...);
        }

        static inline constexpr location_iterator<X, XX...> begin() { return location_iterator<X, XX...>::begin(); }
        static inline constexpr location_iterator<X, XX...> end() { return location_iterator<X, XX...>::end(); }
    };

    template <bool wrap, unsigned... XX>
    std::vector<std::vector<int>> make_neighborhoods()
    {
        std::map<unsigned, std::vector<int>> M;

        auto loc = location_iterator<XX...>::begin(), loc_end = location_iterator<XX...>::end();
        for(; loc != loc_end; ++loc) {
            unsigned typ = loc.type();
            if(M.find(typ) == M.end())
                M[typ] = make_offset<wrap>(loc);
        }

        std::vector<std::vector<int>> ret;
        ret.resize(std::pow(3, sizeof...(XX)));

        for(auto&& pair : M)
            ret[pair.first] = std::move(pair.second);

        return ret;
    }

    template <bool wrap, unsigned... XX>
    class iterable_offsets  : public iterable_space<wrap, XX...> {
        static const std::vector<std::vector<int>> all_offsets;

    public:
        typedef hyper::location_iterator<XX...> iterator;

        static inline const std::vector<int>& neighbors_offsets(unsigned hood_type)
        {
            assert(hood_type < all_offsets.size());
            return all_offsets[hood_type];
        }
    };

    template <bool wrap, unsigned... XX>
    const std::vector<std::vector<int>> iterable_offsets<wrap, XX...>::all_offsets = make_neighborhoods<wrap, XX...>();

    template <unsigned... XX>
    using wrapped_space_offsets = iterable_offsets<true, XX...>;

    template <unsigned... XX>
    using unwrapped_space_offsets = iterable_offsets<false, XX...>;

    template <unsigned... XX>
    inline const std::vector<int>& wrapped_neighbors_offsets(const location_iterator<XX...>& it)
    {
        return wrapped_space_offsets<XX...>::neighbors_offsets(it.type());
    }
    template <unsigned... XX>
    inline const std::vector<int>& unwrapped_neighbors_offsets(const location_iterator<XX...>& it)
    {
        return unwrapped_space_offsets<XX...>::neighbors_offsets(it.type());
    }

    template <typename T, bool wrap, unsigned... XX>
    class grid {
        std::vector<T> data; // std::array<> is not moveable

    public:
        typedef iterable_offsets<wrap, XX...> space_offsets;

        struct iterator {
            std::vector<T>& _data;
            location_iterator<XX...> _loc;

            iterator(std::vector<T>& d, const location_iterator<XX...>& loc)
                : _data(d)
                , _loc(loc)
            {
            }

            inline operator unsigned() const { return (unsigned)_loc; }
            inline typename std::vector<T>::reference operator*() { return _data[_loc]; }
            inline typename std::vector<T>::pointer operator->() { return &_data[_loc]; }

            inline std::vector<int>::const_iterator begin() const
            {
                return space_offsets::neighbors_offsets(_loc.type()).begin();
            }
            inline std::vector<int>::const_iterator end() const
            {
                return space_offsets::neighbors_offsets(_loc.type()).end();
            }
            inline size_t size() const { return space_offsets::neighbors_offsets(_loc.type()).size(); }
            inline unsigned type() const { return _loc.type(); }

            inline const std::vector<int>& neighbors_offsets() const
            {
                return space_offsets::neighbors_offsets(_loc.type());
            }
            inline std::vector<std::reference_wrapper<T>> neighbors() const
            {
                std::vector<std::reference_wrapper<T>> ans;
                for(int off : space_offsets::neighbors_offsets(_loc.type()))
                    ans.push_back(std::ref(_data[_loc + off]));
                return ans;
            }

            inline iterator& operator++()
            {
                ++_loc;
                return *this;
            }
            inline bool operator!=(const iterator& other) { return _loc != other._loc; }
            inline typename std::vector<T>::reference operator[](int offset) { return _data[_loc + offset]; }
            inline unsigned coordinate(unsigned c) const { return _loc[c]; }
        };

        struct const_iterator {
            const std::vector<T>& _data;
            location_iterator<XX...> _loc;

            const_iterator(const std::vector<T>& d, const location_iterator<XX...>& loc)
                : _data(d)
                , _loc(loc)
            {
            }

            inline operator unsigned() const { return (unsigned)_loc; }
            inline typename std::vector<T>::const_reference operator*() const { return _data[_loc]; }
            inline typename std::vector<T>::const_pointer operator->() const { return &_data[_loc]; }

            inline std::vector<int>::const_iterator begin() const
            {
                return space_offsets::neighbors_offsets(_loc.type()).begin();
            }
            inline std::vector<int>::const_iterator end() const
            {
                return space_offsets::neighbors_offsets(_loc.type()).end();
            }
            inline size_t size() const { return space_offsets::neighbors_offsets(_loc.type()).size(); }
            inline unsigned type() const { return _loc.type(); }

            inline const std::vector<int>& neighbors_offsets() const
            {
                return space_offsets::neighbors_offsets(_loc.type());
            }

            inline const_iterator& operator++()
            {
                ++_loc;
                return *this;
            }
            inline bool operator!=(const const_iterator& other) { return _loc != other._loc; }
            inline typename std::vector<T>::const_reference operator[](int offset) const
            {
                return _data[_loc + offset];
            }
            inline unsigned coordinate(unsigned c) const { return _loc[c]; }
        };

    public:
        grid()
            : data(space_offsets::size())
        {
        }
        grid(T _default)
            : data(space_offsets::size(), _default)
        {
        }
        grid(T (*f)())
        {
            data.reserve(space_offsets::size());
            while(data.size() < space_offsets::size())
                data.push_back(f());
        }

        inline typename std::vector<T>::reference operator[](unsigned pos) { return data[pos]; }
        inline typename std::vector<T>::const_reference operator[](unsigned pos) const { return data[pos]; }

        inline size_t size() const { return data.size(); }
        inline static constexpr size_t dimension() { return sizeof...(XX); }

        inline iterator begin() { return iterator(data, space_offsets::begin()); }
        inline iterator end() { return iterator(data, space_offsets::end()); }
        inline const_iterator begin() const { return const_iterator(data, space_offsets::begin()); }
        inline const_iterator end() const { return const_iterator(data, space_offsets::end()); }

        template <typename... CC>
        inline iterator at(CC... cc)
        {
            return iterator(data, location_iterator<XX...>(cc...));
        }
        template <typename... CC>
        inline const_iterator at(CC... cc) const
        {
            return const_iterator(data, location_iterator<XX...>(cc...));
        }
        inline static const std::vector<int>& neighbors_offsets(const location_iterator<XX...>& it)
        {
            return space_offsets::neighbors_offsets(it.type());
        }
        inline static const std::vector<int>& neighbors_offsets(unsigned nhood_type)
        {
            return space_offsets::neighbors_offsets(nhood_type);
        }

        template <typename... CC>
        inline typename std::vector<T>::reference operator()(CC... cc)
        {
            //return data[typename space_offsets::iterator(cc...)];
            return data[(size_t)location_iterator<XX...>(cc...)];
        }
        template <typename... CC>
        inline typename std::vector<T>::const_reference operator()(CC... cc) const
        {
            //return data[typename space_offsets::iterator(cc...)];
            return data[(size_t)location_iterator<XX...>(cc...)];
        }

        friend void swap(grid& lhs, grid& rhs) noexcept { lhs.data.swap(rhs.data); }
    };

    template <typename T, unsigned... XX>
    using wrapped_space = grid<T, true, XX...>;

    template <typename T, unsigned... XX>
    using unwrapped_space = grid<T, false, XX...>;

} // namespace hyper
} // namespace sprogar

#endif
