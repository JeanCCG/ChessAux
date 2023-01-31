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

#include "Interface.hpp"

Interface::Interface_state Interface::human_vs_human_interface()
{
  using Interface_state = Interface::Interface_state;
  enum Option : int {
    play = 0,
    time,// on, off
    swap_colors,
    modify_chess_board,
    return_to_main,
  } option{ play };

  const vector<string> options{
    "Play",
    "Time",
    "Swap colors",
    "Modify Chess Board",
  };

  Game_settings game_settings{};
  game_settings.is_time_enabled = false;
  game_settings.white_config.player_type = Player_type::human;
  game_settings.black_config.player_type = Player_type::human;

  User_input user_input{};
  do {
    display_interface(options, static_cast<unsigned>(option));
    user_input = get_user_key_input();
    switch (user_input) {
    case User_input::down:
      if (option == Option::modify_chess_board) {
        option = Option::play;
      } else {
        option = static_cast<Option>(static_cast<int>(option) + 1);
      }
      break;
    case User_input::up:
      if (option == Option::play) {
        option = Option::modify_chess_board;
      } else {
        option = static_cast<Option>(static_cast<int>(option) - 1);
      }
      break;
    case User_input::select: user_input = User_input::select; break;
    case User_input::exit: return Interface_state::main;
    default: break;
    }

  } while (user_input != User_input::select);

  Interface_state next_state{};
  switch (option) {
  case Option::play:
    game(game_settings);
    next_state = Interface_state::game_results;
    break;
  case Option::time: /* enable - disable (if enable, add option) */ break;
  case Option::swap_colors: cout << "\tGood bye 1\n"; return Interface_state::end_program;
  case Option::modify_chess_board: cout << "\tGood bye 3\n"; return Interface_state::end_program;
  default: break;
  }
  return next_state;
}
