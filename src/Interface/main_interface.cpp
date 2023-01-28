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

#include "./Interface.hpp"

using Interface_state = Interface::Interface_state;
Interface_state Interface::main_interface()
{
  enum Option : int {
    human_vs_human = 0,
    human_vs_computer,
    quit,
  } option{ human_vs_human };
  const vector<string> options{
    "Human vs Human",
    "Human vs Computer",
    "Quit",
  };

  User_input user_input{};
  do {
    display_interface(options, static_cast<unsigned>(option));
    switch (get_user_key_input()) {
    case User_input::down:
    case User_input::left:
      if (option == Option::quit) {
        option = Option::human_vs_human;
      } else {
        option = static_cast<Option>(static_cast<int>(option) + 1);
      }
      break;
    case User_input::right:
    case User_input::up:
      if (option == Option::human_vs_human) {
        option = Option::quit;
      } else {
        option = static_cast<Option>(static_cast<int>(option) - 1);
      }
      break;
    case User_input::select: user_input = User_input::select; break;
    case User_input::exit: return Interface_state::end_program;
    default: break;
    }
  } while (user_input != User_input::select);

  Interface_state next_state{};

  switch (option) {
  case Option::human_vs_human: next_state = human_vs_human_game_settings_interface(); break;
  case Option::human_vs_computer: next_state = human_vs_computer_game_settings_interface(); break;
  case Option::quit: return Interface_state::end_program;
  default: break;
  }

  return next_state;
}
