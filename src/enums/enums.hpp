#ifndef __ENUMS_H__
#define __ENUMS_H__

enum class Piece_points {// NOSONAR
  int_eaten = -1,
  int_free = 0,
  kingPoints = 1000,
  queenPoints = 9,
  rookPoints = 5,
  knightPoints = 3,
  bishopPoints = 3,
  pawnPoints = 1
};

int to_int(const Piece_points points);

enum class Piece_symbols : char {
  empty = ' ',
  dot = '*',

  // white is always in minuscules
  white_king = 'r',
  white_queen = 'q',
  white_rook = 't',
  white_knight = 'c',
  white_bishop = 'a',
  white_pawn = 'p',

  // black IS ALWAYS IN MAJUSCULES
  black_king = 'R',
  black_queen = 'Q',
  black_rook = 'T',
  black_knight = 'C',
  black_bishop = 'A',
  black_pawn = 'P'
};

char to_char(const Piece_symbols symbol);

enum class Player : bool {
  white = false,
  black = true,
};

enum class Game_result {
  draw,
  no_results_yet,
  white_wins,
  black_wins,
  stale_mate,
  insufficient_material,
};

enum class Game_mode {
  human_vs_human,
  human_vs_computer,
};
enum class Player_type : bool { human, computer };

Player operator!(const Player rhs);


#endif// __ENUMS_H__
