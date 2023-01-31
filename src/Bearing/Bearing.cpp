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

#include "Bearing.hpp"
//

Bearing::Bearing(unsigned t_x, unsigned t_y) noexcept : x{ t_x }, y{ t_y } {}
Bearing::Bearing(char t_c, unsigned t_y) noexcept : x{ static_cast<unsigned>(t_c - 'a') }, y{ t_y - 1 } {}
bool Bearing::operator==(const Bearing &rhs) const { return (x == rhs.x) && (y == rhs.y); }
Bearing &Bearing::operator+=(const Bearing &rhs)
{
  x += rhs.x;
  y += rhs.y;
  return *this;
}

Bearing &Bearing::operator++()// prefix
{
  ++x, ++y;
  return *this;
}

Bearing Bearing::operator++(int) { return operator++(); }// postfix, same as prefix
Bearing &Bearing::operator--()// prefix
{
  --x, --y;
  return *this;
}
Bearing Bearing::operator--(int) { return operator--(); }// postfix, same as prefix
Bearing Bearing::operator+(const Bearing &rhs) const { return Bearing{ x + rhs.x, y + rhs.y }; }


std::istream &operator>>(std::istream &is, Bearing &bearing)
{
  char c = 0;
  unsigned n = 0;
  if (is >> c) {
    c = tolower(c);// NOLINT
    if ('a' <= c && c <= 'h' && is >> n && 1 <= n && n <= 8) {
      bearing = Bearing{ c, n };
      return is;
    }
  }
  is.setstate(std::ios_base::failbit);// Flawfinder: ignore // register the failure in the stream
  return is;
}
