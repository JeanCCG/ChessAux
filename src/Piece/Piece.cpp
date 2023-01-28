#include "Piece.hpp"
#include "../enums/enums.h"

// Piece::Piece() {}

// Piece(char, bool, bool, int);

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


// void setFree();
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
