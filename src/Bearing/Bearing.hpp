#ifndef __BEARING_H__
#define __BEARING_H__

// Copyright (C) 2022 José Enrique Vilca Campana
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

#include <iostream>

// struct Bearing
// {
//   unsigned x{};
//   unsigned y{};
//   Bearing() = default;
//   Bearing(unsigned t_x, unsigned t_y) : x{ t_x }, y{ t_y } {}
//   Bearing(char t_c, unsigned t_y) : x{ static_cast<unsigned>(t_c - 'a') }, y{ t_y - 1 } {}
//   bool operator==(const Bearing &rhs) { return (x == rhs.x) && (y == rhs.y); }
//   Bearing &operator+=(const Bearing &rhs)
//   {
//     x += rhs.x;
//     y += rhs.y;
//     return *this;
//   }
//   Bearing &operator++()// prefix
//   {
//     ++x, ++y;
//     return *this;
//   }

//   Bearing &operator++(int) { return operator++(); }// postfix, same as prefix
//   Bearing &operator--()// prefix
//   {
//     --x, --y;
//     return *this;
//   }
//   Bearing &operator--(int) { return operator--(); }// postfix, same as prefix
//   Bearing operator+(const Bearing &rhs) const { return Bearing{ x + rhs.x, y + rhs.y }; }
// };


// std::istream &operator>>(std::istream &is, Bearing &bearing)
// {
//   char c;
//   unsigned n;
//   if (is >> c) {
//     c = tolower(c);
//     if ('a' <= c && c <= 'h' && is >> n && 1 <= n && n <= 8) {
//       bearing = Bearing{ c, n };
//       return is;
//     }
//   }
//   is.setstate(std::ios_base::failbit);// Flawfinder: ignore // register the failure in the stream
//   return is;
// }

struct Bearing
{
  unsigned x{};
  unsigned y{};
  Bearing() = default;
  Bearing(unsigned t_x, unsigned t_y) noexcept;
  Bearing(char t_c, unsigned t_y) noexcept;
  bool operator==(const Bearing &rhs) const;
  bool operator!=(const Bearing &rhs) const;
  Bearing &operator+=(const Bearing &rhs);
  Bearing &operator++();// postfix

  Bearing operator++(int);// postfix, same as prefix
  Bearing &operator--();// prefix
  Bearing operator--(int);// postfix, same as prefix
  Bearing operator+(const Bearing &rhs) const;
  Bearing operator-(const Bearing &rhs) const;
};

std::istream &operator>>(std::istream &is, Bearing &bearing);


#endif// __BEARING_H__
