#ifndef __PIECE_H__
#define __PIECE_H__


#include "../enums/enums.hpp"

class Piece
{
public:
  Piece_symbols symbol;
  bool isFree{ false };
  Player player{};
  int points{};
  int movements{ 0 };

  Piece() = default;
  explicit Piece(const char &&t_symbol);
  explicit Piece(const Piece_symbols &t_symbol);
  void constructor(const Piece_symbols t_symbol);

  void setFree();
  void setPiece(Piece);
  bool empty();
};


#endif// __PIECE_H__
