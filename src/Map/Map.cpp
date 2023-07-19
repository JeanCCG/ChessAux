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

#include "Map.hpp"

Page<3> &Map::at(const Bearing b) const
{
  const unsigned index = b.x * 8 + b.y;
  return keys[index];
}

Page<3> &Map::operator[](const Bearing b) const { return at(b); }

Map::Map(const unsigned size) : keys{ new Page<3>[size] }
{
  // for (Page<3> *it = keys; it < keys + size; it++) { it->set_null(); }
}

// cppcheck-suppress publicAllocationError; reason: i'll handle it.
void Map::reserve(const unsigned size)
{
  if (not empty()) { safe_delete(); }
  keys = new Page<3>[size];
}
