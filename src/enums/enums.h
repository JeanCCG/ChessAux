#ifndef __ENUMS_H__
#define __ENUMS_H__

enum Piece_points {
  int_eaten = -1,
  int_free = 0,
  kingPoints = 1000,
  queenPoints = 9,
  rookPoints = 5,
  knightPoints = 3,
  bishopPoints = 3,
  pawnPoints = 1
};

enum Piece_symbols : char {
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

enum class Player : bool {
  white = true,
  black = false,
};

// bool operator==(const Player &lhs, const Player &rhs)
// {
//   // if (lhs ==)
//   // {
//   //   /* code */
//   // }

//   //
// }
Player operator!(const Player &obj) { return (obj == Player::white) ? Player::black : Player::white; }

// enum Game_state : bool {
//   white_turn,
//   black_turn,
//   // game_over,
// };
// bool operator!(Game_state param) { return param }

enum Game_result {
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

// enum class Player_color : bool { white, black };

namespace game {
unsigned difference(const unsigned lhs, const unsigned rhs) { return (lhs > rhs) ? lhs - rhs : rhs - lhs; }
void switch_player(Player &player) { player = (player == Player::white) ? Player::black : Player::white; }
}// namespace game

#endif// __ENUMS_H__
