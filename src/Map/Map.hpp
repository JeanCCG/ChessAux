#ifndef __MAP_H__
#define __MAP_H__

// Copyright (C) 2023 José Enrique Vilca Campana
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "../Bearing/Bearing.hpp"
#include <vector>

template<unsigned Size = 3> class Page
{
private:
  Bearing *root{ nullptr };
  void init_array(Bearing *start, Bearing const *end) const
  {
    for (; start != end; start++) { *start = Bearing{ -1U, -1U }; }
  }

public:
  ~Page() { clear(); }
  void set_null() { root = nullptr; }
  void clear();
  bool empty() const { return root == nullptr; }
  void safe_delete()
  {
    delete[] root;
    root = nullptr;
  }

  Page() = default;
  explicit Page(Bearing *t_page) : root{ t_page } {}
  explicit Page(const Page &t_page)
  {
    if (t_page.empty()) { return; }
    root = new Bearing[Size];
    // Bearing *it = root;
    // for (Bearing const *t_it = t_page.root; t_it < t_page.root + Size; t_it++, it++) {//
    //   *it = *t_it;
    // }
    for (unsigned i = 0; i < Size; i++) {//
      root[i] = t_page.root[i];
    }
  }
  // perhaps i should add a "page operator=()""
  void copy_page(const Page &t_page)
  {
    if (t_page.empty()) {
      root = nullptr;//! check if it is safe to remove this line, then comment this.
      return;
    }
    root = new Bearing[Size];
    for (unsigned i = 0; i < Size; i++) {//
      root[i] = t_page.root[i];
    }
  }

  void reserve();
  void append(const Bearing value);
  bool contains(const Bearing b) const;
  template<class Functor> void for_each(Functor functor)
  {
    for (Bearing *it = root;//
         it != root + Size and//
         *it != Bearing{ -1U, -1U };//
         it++) {//
      functor(*it);
    }
  }
};

template<unsigned Size> void Page<Size>::reserve()
{
  clear();
  root = new Bearing[Size];
  init_array(root, root + Size);
}

template<unsigned Size> void Page<Size>::clear()
{
  if (empty()) { return; }
  safe_delete();
}

template<unsigned Size> void Page<Size>::append(const Bearing value)
{
  if (empty()) {
    root = new Bearing[Size];
    *root = value;
    init_array(root + 1, root + Size);
    return;
  }

  for (Bearing *it = root; it != root + Size; it++) {
    if (*it == Bearing{ -1U, -1U }) {
      *it = value;
      return;
    }
  }
}

template<unsigned Size> bool Page<Size>::contains(const Bearing b) const
{
  for (Bearing const *it = root; it != root + Size; it++) {
    if (*it == b) { return true; }
  }
  return false;
}


/*****************************************************************************
 * Map
 *****************************************************************************/

class Map
{
public:
  explicit Map(Page<3> *t_keys) : keys{ t_keys } {}
  explicit Map(const Map &t_map)
  {
    if (t_map.empty()) { return; }
    keys = new Page<3>[64];
    Page<3> *it = keys;
    for (Page<3> const *copy_it = t_map.keys; copy_it != t_map.keys + 64; copy_it++, it++) {//
      // if (copy_it->empty()) {
      //   it->root = nullptr;
      // } else {
      // }
      // *it = *copy_it;
      it->copy_page((*copy_it));
    }
  }

  explicit Map(const unsigned size);
  void reserve(const unsigned size);

  ~Map() { clear(); }
  void safe_delete()
  {
    delete[] keys;
    keys = nullptr;
  }
  void clear()
  {
    if (empty()) { return; }
    safe_delete();
  }

  bool empty() const { return keys == nullptr; }

  Page<3> &operator[](const Bearing b) const;
  Page<3> &at(const Bearing b) const;

  std::vector<Page<3> *> interceptors()
  {
    std::vector<Page<3> *> v;
    for (Page<3> *it = keys; it < keys + 64; it++) {
      if (not it->empty()) { v.push_back(it); }
    }
    return v;
  }

  Page<3> *keys{ nullptr };

private:
};

#endif// __MAP_H__
