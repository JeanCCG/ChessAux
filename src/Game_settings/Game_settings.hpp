#ifndef __GAME_SETTINGS_H__
#define __GAME_SETTINGS_H__

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

#include "../enums/enums.hpp"
#include <string>
#include <vector>

struct player_configuration
{
  Player color{};
  Player_type player_type{};
  player_configuration(Player t_color, Player_type t_player_type);
  explicit player_configuration(Player t_color);
  explicit player_configuration(Player_type t_player_type);
};

struct Game_settings
{
  const char _ = to_char(Piece_symbols::empty);

  // white is always in minuscules
  const char r = to_char(Piece_symbols::white_king);
  const char q = to_char(Piece_symbols::white_queen);
  const char t = to_char(Piece_symbols::white_rook);
  const char c = to_char(Piece_symbols::white_knight);
  const char a = to_char(Piece_symbols::white_bishop);
  const char p = to_char(Piece_symbols::white_pawn);

  // black IS ALWAYS IN MAJUSCULES
  const char R = to_char(Piece_symbols::black_king);
  const char Q = to_char(Piece_symbols::black_queen);
  const char T = to_char(Piece_symbols::black_rook);
  const char C = to_char(Piece_symbols::black_knight);
  const char A = to_char(Piece_symbols::black_bishop);
  const char P = to_char(Piece_symbols::black_pawn);

  // //   testing board
  // std::vector<std::string> board{
  //   /* 0--+--+--+--+--+--+--+--+-> y axis */
  //   /* |*/ { _, p, _, _, _, _, P, _ },
  //   /* |*/ { _, _, _, _, _, _, _, _ },
  //   /* |*/ { _, _, _, _, _, _, _, R },
  //   /* |*/ { _, _, _, _, _, _, _, _ },
  //   /* |*/ { r, _, _, c, _, _, _, _ },
  //   /* |*/ { _, _, _, _, _, _, _, _ },
  //   /* |*/ { _, _, _, _, _, _, _, _ },
  //   /* |*/ { _, _, _, _, _, _, _, _ },
  //   // V x axis
  // };

  // std::vector<std::string> board{
  //   /* 0--+--+--+--+--+--+--+--+-> y axis */
  //   /* |*/ { t, p, _, _, _, _, P, T },
  //   /* |*/ { c, p, _, _, _, _, P, C },
  //   /* |*/ { a, p, _, _, _, _, P, A },
  //   /* |*/ { q, p, _, _, _, _, P, Q },
  //   /* |*/ { r, p, _, _, _, _, P, R },
  //   /* |*/ { a, p, _, _, _, _, P, A },
  //   /* |*/ { c, p, _, _, _, _, P, C },
  //   /* |*/ { t, p, _, _, _, _, P, T },
  //   // V x axis
  // };

  std::vector<std::string> board{
    /* 0--+--+--+--+--+--+--+--+-> y axis */
    /* |*/ { t, p, _, _, _, _, P, T },
    /* |*/ { c, p, _, _, _, _, P, C },
    /* |*/ { a, p, _, c, _, _, P, A },
    /* |*/ { q, p, _, _, _, _, P, Q },
    /* |*/ { r, p, _, _, _, _, P, R },
    /* |*/ { a, p, _, _, _, _, P, A },
    /* |*/ { c, p, _, _, _, _, P, C },
    /* |*/ { t, p, _, _, _, _, P, T },
    // V x axis
  };

  // std::vector<std::string> board{
  //   { "♜♞♝♛♚♝♞♜" },
  //   { "♟♟♟♟♟♟♟♟" },
  //   { "♙♙♙♙♙♙♙♙" },
  //   { "♖♘♗♕♔♗♘♖" },
  // };

  player_configuration white_config{ Player::white };
  player_configuration black_config{ Player::black };

  bool is_time_enabled{ false };
  unsigned white_time{};
  unsigned black_time{};

  unsigned height{ static_cast<unsigned>(board.size()) };
  unsigned width{ static_cast<unsigned>(board.front().size()) };
  Game_settings() = default;
};

#endif// __GAME_SETTINGS_H__
