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

#include "Piece.hpp"

Piece::Piece(const Piece_symbols &t_symbol) : symbol{ t_symbol } { constructor(t_symbol); }
Piece::Piece(const char &&t_symbol) : symbol{ static_cast<Piece_symbols>(t_symbol) } { constructor(symbol); }

bool Piece::empty() { return isFree; }

void Piece::constructor(const Piece_symbols t_symbol)
{
  switch (t_symbol) {
  case Piece_symbols::white_king: points = Piece_points::kingPoints; break;
  case Piece_symbols::black_king: points = Piece_points::kingPoints; break;
  case Piece_symbols::black_queen:
  case Piece_symbols::white_queen: points = Piece_points::queenPoints; break;
  case Piece_symbols::black_rook:
  case Piece_symbols::white_rook: points = Piece_points::rookPoints; break;
  case Piece_symbols::black_knight:
  case Piece_symbols::white_knight: points = Piece_points::knightPoints; break;
  case Piece_symbols::black_bishop:
  case Piece_symbols::white_bishop: points = Piece_points::bishopPoints; break;
  case Piece_symbols::black_pawn:
  case Piece_symbols::white_pawn: points = Piece_points::pawnPoints; break;
  default:
    points = Piece_points::int_free;
    isFree = true;
    break;
  }

  switch (t_symbol) {
  case Piece_symbols::white_king:
  case Piece_symbols::white_queen:
  case Piece_symbols::white_rook:
  case Piece_symbols::white_knight:
  case Piece_symbols::white_bishop:
  case Piece_symbols::white_pawn: player = Player::white; break;
  default: player = Player::black; break;
  }
}

void Piece::setFree()
{
  symbol = Piece_symbols::empty;
  isFree = true;
  points = 0;
  movements = 0;
}


void Piece::setPiece(Piece piece)
{
  symbol = piece.symbol;
  isFree = false;
  player = piece.player;
  points = piece.points;
  movements += piece.movements + 1;
}
