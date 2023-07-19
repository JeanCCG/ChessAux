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
#include <string>
#include <vector>

using namespace std;

Interface::Interface_state Interface::human_vs_computer_interface()
{
  enum class Option : int {
    play = 0,
    time,// on, off
    swap_colors,
    difficulty,
    modify_chess_board,
  };
  Option option{ Option::play };

  const vector<string> options{
    "Play",
    "Time",
    "Swap colors",
    "Difficulty",
    "Modify Chess Board",
  };

  Game_settings game_settings{};
  game_settings.is_time_enabled = false;
  game_settings.white_config.player_type = Player_type::human;
  game_settings.black_config.player_type = Player_type::computer;

  User_input input{ User_input::none };
  while (input != User_input::select) {
    display_interface(options, static_cast<unsigned>(option));
    input = get_user_key_input();
    switch (input) {
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
    case User_input::select: input = User_input::select; break;
    case User_input::exit: return Interface_state::main;
    default: break;
    }
  }

  Interface_state next_state{};
  switch (option) {
  case Option::play: next_state = game(game_settings); break;
  case Option::time: game_settings.is_time_enabled = not game_settings.is_time_enabled; break;
  case Option::swap_colors: cout << "\tGood bye 1\n"; return Interface_state::end_program;
  case Option::difficulty: cout << "\tGood bye 2\n"; return Interface_state::end_program;
  case Option::modify_chess_board:
    modify_chess_board_interface(game_settings);
    // modify_chess_board_interface()?
    cout << "\tGood bye 3\n";
    next_state = Interface_state::human_vs_computer;
    // input the matrix.
    break;
  default: break;
  }
  return next_state;
}
