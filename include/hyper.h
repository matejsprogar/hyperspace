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
#include <string>

namespace sprogar
{
	using position_t = std::size_t;
	using offset_t = typename std::make_signed<position_t>::type;

	namespace hyper
	{
		template <unsigned R, unsigned... XX>
		class location_iterator
		{
		protected:
			struct dummy {
				position_t value;
				dummy(position_t u)
					: value(u) {};
			};
			position_t pos1d;

			location_iterator(position_t, dummy pos) : pos1d{ pos.value }
			{
			}
			location_iterator(dummy pos, position_t = 0) : pos1d{ pos.value }
			{
			}

			template <unsigned>
			inline unsigned neighborhood_type() const { return 0; }
			inline void increment() {}

		public:
			location_iterator() : pos1d{ 0 }
			{
			}

			inline bool operator!=(const location_iterator& other) const { return pos1d != other.pos1d; }

			template <class OtherIterator>
			inline bool equal_coordinates(const OtherIterator& other) const
			{
				return true;
			}
			inline operator position_t() const { return pos1d; }

			inline bool move_next()
			{
				pos1d += 1;
				return true;
			}
			inline bool move_prev()
			{
				pos1d -= 1;
				return true;
			}

			inline unsigned type() const { return -1; }

			inline location_iterator& operator++()
			{
				move_next();
				return *this;
			}
			inline location_iterator operator++(int)
			{
				auto old = *this;
				move_next();
				return old;
			}
			inline location_iterator& operator--()
			{
				move_prev();
				return *this;
			}
			inline location_iterator operator--(int)
			{
				auto old = *this;
				move_prev();
				return old;
			}

			inline static constexpr std::size_t size() { return 1; }
			inline unsigned operator[](unsigned) const { return -1; }

			static inline location_iterator begin() { return location_iterator(); }
			static inline location_iterator end() { return location_iterator(); }

			template <bool>
			inline unsigned make_offset(std::set<offset_t>&, offset_t, unsigned) const
			{
				return 1;
			}
		};



		template <unsigned R, unsigned X, unsigned... XX>
		class location_iterator<R, X, XX...> : public location_iterator<R, XX...>
		{
		public:
			typedef location_iterator<R> root;
			typedef location_iterator<R, XX...> base;

			unsigned coordinate;

		protected:
			// construct by coordinates
			template <typename... CC>
			location_iterator(typename root::dummy pos, unsigned c, CC... cc)
				: base(typename root::dummy(pos.value* X + c), cc...), coordinate(c)
			{
			}
			// construct by id
			location_iterator(unsigned c, typename root::dummy pos)
				: base((unsigned)(c% base::size()), pos), coordinate((unsigned)(c / base::size()))
			{
			}

			template <unsigned D>
			inline unsigned neighborhood_type() const
			{
				unsigned typ = 0;

				if (coordinate < R)
					typ = D * (1 + coordinate);
				else if (coordinate > X - R - 1)
					typ = 2 * R * D - D * (X - 1 - coordinate);

				typ += base::template neighborhood_type<D* (2 * R + 1)>();
				return typ;
			}
			inline bool move_next()
			{
				if (base::move_next()) {
					coordinate += 1;
					if (coordinate == X) {
						coordinate = 0;
						return true;
					}
				}
				return false;
			}
			inline bool move_prev()
			{
				if (base::move_prev()) {
					if (coordinate == 0) {
						coordinate = X - 1;
						return true;
					}
					coordinate -= 1;
				}
				return false;
			}

		public:
			location_iterator() : base(), coordinate(0)
			{
			}

			template <typename... CC>
			location_iterator(unsigned c, CC... cc) : base(typename root::dummy{ c }, cc...), coordinate(c)
			{
			}

			location_iterator(position_t pos) :
				base((unsigned)(pos% base::size()), typename root::dummy{ pos }), coordinate((unsigned)(pos / base::size()))
			{
			}

			inline static constexpr std::size_t size() { return X * base::size(); }

			inline unsigned operator[](unsigned d) const
			{
				if (d == sizeof...(XX))
					return coordinate;
				return base::operator[](d);
			}

			inline bool operator!=(const location_iterator& other) const
			{
				return root::pos1d != other.pos1d;
			}

			template <class OtherIterator>
			inline bool equal_coordinates(const OtherIterator& other) const
			{
				//assert(sizeof...(XX) == sizeof...(OtherIterator::XX));
				return coordinate == other.coordinate and base::equal_coordinates((const typename OtherIterator::base&)other);
			}

			inline operator position_t() const { return root::pos1d; }

			inline unsigned type() const { return neighborhood_type<1>(); }

			inline location_iterator& operator++()
			{
				move_next();
				return *this;
			}
			inline location_iterator operator++(int)
			{
				auto old = *this;
				move_next();
				return old;
			}
			inline location_iterator& operator--()
			{
				move_prev();
				return *this;
			}
			inline location_iterator operator--(int)
			{
				auto old = *this;
				move_prev();
				return old;
			}

			static inline location_iterator begin() { return location_iterator(); }
			static inline location_iterator end()
			{
				location_iterator loc;
				loc.pos1d = loc.size();
				return loc;
			}

			template <bool wrap>
			unsigned make_offset(std::set<offset_t>& set, offset_t offset, unsigned plane) const
			{
				plane = base::template make_offset<wrap>(set, offset, plane);

				if (coordinate != 0) {
					unsigned P = base::template make_offset<wrap>(set, offset - plane, plane);
					set.insert(offset - P);
				}
				else if (wrap) {
					unsigned P = base::template make_offset<wrap>(set, offset + (X - 1) * plane, plane);
					set.insert(offset + (X - 1) * P);
				}

				set.insert(offset);

				if (coordinate != X - 1) {
					unsigned P = base::template make_offset<wrap>(set, offset + plane, plane);
					set.insert(offset + P);
				}
				else if (wrap) {
					unsigned P = base::template make_offset<wrap>(set, offset - (X - 1) * plane, plane);
					set.insert(offset - (X - 1) * P);
				}

				return X * plane;
			}
			template <bool wrap>
			std::vector<offset_t> make_offset()
			{
				std::set<offset_t> total;
				make_offset<wrap>(total, 0, 1);
				total.erase(0);

				for (unsigned r = 1; r < R; ++r)
				{
					std::set<offset_t> todo(total);
					for (offset_t off : todo) {
						location_iterator it(root::pos1d + off);

						std::set<offset_t> ring;
						it.template make_offset<wrap>(ring, 0, 1);

						for (offset_t x : ring)
							total.insert(off + x);
						total.erase(0);
					}
				}

				std::vector<offset_t> vec;
				vec.insert(vec.begin(), total.begin(), total.end());

				return vec;
			}
		};




		template <bool wrap, unsigned R, unsigned... XX>
		class iterable_space
		{
		protected:
			template <unsigned>
			static inline unsigned id_helper() { return 0; }

		public:
			static inline constexpr std::string info() {
				const std::string msg = std::string("wrapped R") + std::to_string(R);
				return wrap ? msg : std::string("un") + msg;
			}
			static inline constexpr unsigned dimension() { return 0; }
			static inline constexpr unsigned dimension(unsigned) { return 0; }
			static inline constexpr position_t size() { return 1; }

			static inline constexpr location_iterator<R> begin() { return location_iterator<R>::begin(); }
			static inline constexpr location_iterator<R> end() { return location_iterator<R>::end(); }
		};

		template <bool wrap, unsigned R, unsigned X, unsigned... XX>
		class iterable_space<wrap, R, X, XX...> : public iterable_space<wrap, R, XX...>
		{
		protected:
			template <unsigned S, typename... UU>
			static inline unsigned id_helper(unsigned x, UU... uu)
			{
				assert(x < X);
				return x * S + iterable_space<wrap, R, XX...>::template id_helper<X* S>(uu...);
			}

		public:
			static inline constexpr std::string info()
			{
				return std::to_string(X) + (sizeof...(XX) > 0 ? "x" : " ") + iterable_space<wrap, R, XX...>::info();
			}
			static inline constexpr unsigned dimension() { return 1 + sizeof...(XX); }
			static inline constexpr unsigned dimension(unsigned D)
			{
				return D == sizeof...(XX) ? X : iterable_space<wrap, R, XX...>::dimension(D);
			}
			static inline constexpr position_t size() { return X * iterable_space<wrap, R, XX...>::size(); }
			template <typename... UU>
			static inline unsigned id(UU... uu)
			{
				assert(sizeof...(UU) == 1 + sizeof...(XX));
				return id_helper<1U>(static_cast<unsigned>(uu)...);
			}

			static inline constexpr location_iterator<R, X, XX...> begin() { return location_iterator<R, X, XX...>::begin(); }
			static inline constexpr location_iterator<R, X, XX...> end() { return location_iterator<R, X, XX...>::end(); }
		};

		template <bool wrap, unsigned R, unsigned... XX>
		std::vector<std::vector<offset_t>> make_neighborhoods()
		{
			std::map<unsigned, std::vector<offset_t>> M;

			auto loc = location_iterator<R, XX...>::begin(), loc_end = location_iterator<R, XX...>::end();
			for (; loc != loc_end; ++loc) {
				unsigned typ = loc.type();
				if (M.find(typ) == M.end())
					M[typ] = loc.template make_offset<wrap>();
			}

			std::vector<std::vector<offset_t>> ret((unsigned)std::pow(2 * R + 1, sizeof...(XX)));

			for (auto&& pair : M)
				ret[pair.first] = std::move(pair.second);

			return ret;
		}



		template <bool wrap, unsigned R, unsigned... XX>
		class iterable_offsets : public iterable_space<wrap, R, XX...>
		{

		public:
			typedef hyper::location_iterator<R, XX...> iterator;

			static inline const std::vector<offset_t>& neighbors_offsets(unsigned hood_type)
			{
				static std::vector<std::vector<offset_t>> all_offsets = make_neighborhoods<wrap, R, XX...>();

				assert(hood_type < all_offsets.size());
				return all_offsets[hood_type];
			}
			static std::vector<const std::vector<offset_t>*> offsets()
			{
				using base = iterable_space<wrap, R, XX...>;
				std::vector<const std::vector<offset_t>*> ofs(base::size());

				for (auto loc = base::begin(); loc != base::end(); ++loc)
					ofs[(position_t)loc] = &neighbors_offsets(loc.type());

				return ofs;
			}
		};

		template <unsigned R, unsigned... XX>
		using wrapped_space_offsets = iterable_offsets<true, R, XX...>;

		template <unsigned R, unsigned... XX>
		using unwrapped_space_offsets = iterable_offsets<false, R, XX...>;






		template <typename T, unsigned R, bool wrap, unsigned... XX>
		class grid
		{
			std::vector<T> data; // std::array<> is not moveable

		public:
			typedef iterable_offsets<wrap, R, XX...> space_offsets;

			class iterator {
			public:
				using difference_type = std::ptrdiff_t;
				using value_type = T;
				using pointer = T*;
				using reference = T&;
				using iterator_category = std::bidirectional_iterator_tag;

				std::vector<T>& _data;
				location_iterator<R, XX...> _loc;

				iterator(std::vector<T>& d, const location_iterator<R, XX...>& loc)
					: _data(d)
					, _loc(loc)
				{
				}

				inline operator unsigned() const { return (unsigned)_loc; }
				inline typename std::vector<T>::reference operator*() { return _data[_loc]; }
				inline typename std::vector<T>::pointer operator->() { return &_data[_loc]; }

				inline std::vector<offset_t>::const_iterator begin() const
				{
					return space_offsets::neighbors_offsets(_loc.type()).begin();
				}
				inline std::vector<offset_t>::const_iterator end() const
				{
					return space_offsets::neighbors_offsets(_loc.type()).end();
				}
				inline position_t size() const { return space_offsets::neighbors_offsets(_loc.type()).size(); }
				inline unsigned type() const { return _loc.type(); }

				inline const std::vector<offset_t>& neighbors_offsets() const
				{
					return space_offsets::neighbors_offsets(_loc.type());
				}
				inline std::vector<std::reference_wrapper<T>> neighbors() const
				{
					std::vector<std::reference_wrapper<T>> ans;
					for (offset_t off : space_offsets::neighbors_offsets(_loc.type()))
						ans.push_back(std::ref(_data[_loc + off]));
					return ans;
				}

				inline iterator& operator++()
				{
					++_loc;
					return *this;
				}
				inline iterator& operator--()
				{
					--_loc;
					return *this;
				}
				inline bool operator!=(const iterator& rhs) const { return !(*this == rhs); }
				inline bool operator==(const iterator& rhs) const { return _loc == rhs._loc; }
				inline typename std::vector<T>::reference operator[](offset_t offset) { return _data[_loc + offset]; }
				inline unsigned coordinate(unsigned c) const { return _loc[c]; }
			};

			struct const_iterator {
				using difference_type = std::ptrdiff_t;
				using value_type = T;
				using pointer = T*;
				using reference = T&;
				using iterator_category = std::bidirectional_iterator_tag;

				const std::vector<T>& _data;
				location_iterator<R, XX...> _loc;

				const_iterator(const std::vector<T>& d, const location_iterator<R, XX...>& loc)
					: _data(d)
					, _loc(loc)
				{
				}

				inline operator unsigned() const { return (unsigned)_loc; }
				inline typename std::vector<T>::const_reference operator*() const { return _data[_loc]; }
				inline typename std::vector<T>::const_pointer operator->() const { return &_data[_loc]; }

				inline std::vector<offset_t>::const_iterator begin() const
				{
					return space_offsets::neighbors_offsets(_loc.type()).begin();
				}
				inline std::vector<offset_t>::const_iterator end() const
				{
					return space_offsets::neighbors_offsets(_loc.type()).end();
				}
				inline position_t size() const { return space_offsets::neighbors_offsets(_loc.type()).size(); }
				inline unsigned type() const { return _loc.type(); }

				inline const std::vector<offset_t>& neighbors_offsets() const
				{
					return space_offsets::neighbors_offsets(_loc.type());
				}

				inline const_iterator& operator++()
				{
					++_loc;
					return *this;
				}
				inline const_iterator& operator--()
				{
					--_loc;
					return *this;
				}
				inline bool operator!=(const const_iterator& rhs) const { return !(*this == rhs); }
				inline bool operator==(const const_iterator& rhs) const { return _loc == rhs._loc; }
				inline typename std::vector<T>::const_reference operator[](offset_t offset) const
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
			grid(T(*f)())
			{
				data.reserve(space_offsets::size());
				while (data.size() < space_offsets::size())
					data.push_back(f());
			}


			static inline constexpr std::string info() { return space_offsets::info(); }

			inline typename std::vector<T>::reference operator[](position_t pos) { return data[pos]; }
			inline typename std::vector<T>::const_reference operator[](position_t pos) const { return data[pos]; }

			inline static constexpr position_t size() { return space_offsets::size(); }

			inline static constexpr position_t dimension() { return sizeof...(XX); }
			inline static constexpr position_t dimension(unsigned D) { return space_offsets::dimension(D); }

			inline iterator begin() { return iterator(data, space_offsets::begin()); }
			inline iterator end() { return iterator(data, space_offsets::end()); }
			inline const_iterator begin() const { return const_iterator(data, space_offsets::begin()); }
			inline const_iterator end() const { return const_iterator(data, space_offsets::end()); }

			inline iterator imap(iterator it) { return iterator(data, it._loc); }
			inline const_iterator imap(const_iterator it) const { return const_iterator(data, it._loc); }

			template <typename... CC>
			inline iterator at(CC... cc)
			{
				static_assert(sizeof...(CC) == sizeof...(XX));
				return iterator(data, location_iterator<R, XX...>(cc...));
			}
			template <typename... CC>
			inline const_iterator at(CC... cc) const
			{
				static_assert(sizeof...(CC) == sizeof...(XX));
				return const_iterator(data, location_iterator<R, XX...>(cc...));
			}
			inline static const std::vector<offset_t>& neighbors_offsets(const location_iterator<R, XX...>& it)
			{
				return space_offsets::neighbors_offsets(it.type());
			}
			inline static const std::vector<offset_t>& neighbors_offsets(unsigned nhood_type)
			{
				return space_offsets::neighbors_offsets(nhood_type);
			}

			template <typename... CC>
			inline typename std::vector<T>::reference operator()(CC... cc)
			{
				static_assert(sizeof...(CC) == sizeof...(XX));
				// return data[typename space_offsets::iterator(cc...)];
				return data[(position_t)location_iterator<R, XX...>(cc...)];
			}
			template <typename... CC>
			inline typename std::vector<T>::const_reference operator()(CC... cc) const
			{
				static_assert(sizeof...(CC) == sizeof...(XX));
				// return data[typename space_offsets::iterator(cc...)];
				return data[(position_t)location_iterator<R, XX...>(cc...)];
			}

			friend void swap(grid& lhs, grid& rhs) noexcept { lhs.data.swap(rhs.data); }

			inline bool operator!=(const grid& oth) const { return !(*this == oth); }
			inline bool operator==(const grid& oth) const { return data == oth.data; }
		};

		template <typename T, unsigned Radius, unsigned... XX>
		using wrapped_space = grid<T, Radius, true, XX...>;

		template <typename T, unsigned Radius, unsigned... XX>
		using unwrapped_space = grid<T, Radius, false, XX...>;

	} // namespace hyper
} // namespace sprogar

#endif