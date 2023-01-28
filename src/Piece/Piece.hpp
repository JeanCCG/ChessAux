#ifndef __PIECE_H__
#define __PIECE_H__


#include "../enums/enums.h"


class Piece
{
public:
  Piece_symbols symbol;
  bool isFree{ false };
  Player player;
  int points;
  int movements{ 0 };

  Piece();
  Piece(const char &&t_symbol) : symbol{ static_cast<Piece_symbols>(t_symbol) } { constructor(symbol); }
  Piece(const Piece_symbols &t_symbol) : symbol{ t_symbol } { constructor(t_symbol); }
  void constructor(const Piece_symbols t_symbol);
  // Piece(const char t_symbol);
  void setFree();
  void setPiece(Piece);
  bool empty() { return isFree; }
};

#endif// __PIECE_H__
