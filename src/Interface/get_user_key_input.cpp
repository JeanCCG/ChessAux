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

using User_input = Interface::User_input;
User_input Interface::get_user_key_input()
{
  initscr();// Initialize ncursed
  cbreak();// disable line buffering.
  // Which means characters typed by the user are immediately available to be read by the program.
  // Without this function, the user would need to press Enter before the input is sent to the program.
  noecho();// Disable echoing of characters on the screen.
  // Without this function, the characters typed by the user would be displayed on the terminal.
  keypad(stdscr, TRUE);// allow arrow keys, enter key , numeric keypad, control keys ...

  User_input option{};
  do {
    const unsigned KEY_SCAPE = 27;
    switch (getch()) {
    case KEY_UP: option = User_input::down; break;
    case KEY_LEFT: option = User_input::left; break;
    case KEY_DOWN: option = User_input::down; break;
    case KEY_RIGHT: option = User_input::right; break;
    case KEY_ENTER:
    case '\n': option = User_input::select; break;
    case KEY_SCAPE: option = User_input::exit; break;
    default: option = User_input::none; break;
    }
  } while (option == User_input::none);

  endwin();// Clean up ncursed
  return option;
}
