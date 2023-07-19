#ifndef __PIECE_SET_H__
#define __PIECE_SET_H__

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
#include "../Piece/Piece.hpp"

struct Piece_set
{
  Piece_symbols bishop;
  Piece_symbols queen;
  Piece_symbols rook;
  Piece_symbols pawn;
  Piece_symbols knight;
  Piece_symbols king;
  explicit Piece_set(const Player player)
  {
    if (player == Player::white) {
      bishop = Piece_symbols::white_bishop;
      queen = Piece_symbols::white_queen;
      rook = Piece_symbols::white_rook;
      pawn = Piece_symbols::white_pawn;
      knight = Piece_symbols::white_knight;
      king = Piece_symbols::white_king;
    } else {
      bishop = Piece_symbols::black_bishop;
      queen = Piece_symbols::black_queen;
      rook = Piece_symbols::black_rook;
      pawn = Piece_symbols::black_pawn;
      knight = Piece_symbols::black_knight;
      king = Piece_symbols::black_king;
    }
  }
};

#endif // __PIECE_SET_H__
