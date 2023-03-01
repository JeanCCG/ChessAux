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

public:
  ~Page() { clear(); }
  void clear();
  bool empty() const { return root == nullptr; }

  explicit Page(Bearing *t_page) : root{ t_page } {}
  explicit Page(const Page &t_page) : root{ t_page.root } {}
  explicit Page(const Page &&t_page) noexcept : root{ t_page.root } {}

  Page() : root{ new Bearing[Size] } { init_array(root, root + Size); }

  void init_array(Bearing *start, Bearing *end)
  {
    for (; start != end; start++) { *start = Bearing{ -1U, -1U }; }
  }

  void append(const Bearing value);
  bool contains(const Bearing b) const;
};

template<unsigned Size> void Page<Size>::clear()
{
  if (root == nullptr) { return; }
  delete[] root;
  root = nullptr;
}

template<unsigned Size> void Page<Size>::append(const Bearing value)
{
  if (root == nullptr) {
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
  explicit Map(const Map &map) : keys{ map.keys } {}
  explicit Map(const Map &&map) noexcept : keys{ map.keys } {}

  explicit Map(const unsigned size);
  // explicit Map(const unsigned size);
  ~Map() { clear(); }
  void clear()
  {
    if (keys == nullptr) { return; }
    // for (Page<3> *it = keys; it != keys + 64; it++) {
    //   if (it->empty()) { continue; }
    //   it->clear();
    // }

    delete[] keys;
    keys = nullptr;
  }

  bool empty() const { return keys == nullptr; }
  // bool has_values() const { return not empty(); }

  Page<3> &operator[](const Bearing b) const;
  Page<3> &at(const Bearing b) const;


private:
  Page<3> *keys{ nullptr };
};
