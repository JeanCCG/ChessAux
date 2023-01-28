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

using Interface_state = Interface::Interface_state;
Interface_state Interface::game_results_interface()
{
  switch (game_result) {
  case Game_result::draw: cout << "Tie 🤯 No one won the game!\n"; break;
  case Game_result::white_wins: cout << " \nwhite won the game!\n"; break;
  case Game_result::black_wins: cout << " \nblack won the game! \n"; break;
  default: break;
  }
  input_anything();

  Interface_state next_state{ Interface_state::main };
  return next_state;
}
