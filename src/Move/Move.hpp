#ifndef __MOVE_H__
#define __MOVE_H__

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

#include "../Bearing/Bearing.hpp"

struct Move
{
  Bearing start{};
  Bearing end{};
  Move(Bearing t_start, Bearing t_end) : start{ t_start }, end{ t_end } {}
  Move() = default;
};


// enum class type {
//     move,
//     capture,
//     queen_side_castling,
//     king_side_castling,
//     pawn_first_move_advance_twice,
//     en_passant,
//     promotion,
//   };


#endif// __MOVE_H__
