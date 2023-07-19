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

Interface::Interface_state Interface::game_results_interface(const Game_result game_result)
{
  using Interface_state = Interface::Interface_state;

  clean_screen();
  print_title(5);

  cout << "\n\n\t\t";
  switch (game_result) {
  case Game_result::draw: cout << "Tie 🤯 No one won the game!"; break;
  case Game_result::stale_mate: cout << " stale mate 🤯, no one won"; break;
  case Game_result::white_wins: cout << " white won the game!"; break;
  case Game_result::black_wins: cout << " black won the game! "; break;
  default: cout << "unexpected end"; break;
  }
  cout << "\n";

  input_anything();

  const Interface_state next_state{ Interface_state::main };
  return next_state;
}
