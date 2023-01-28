// Copyright (C) 2022 José Enrique Vilca Campana
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

#include <iostream>
#include <ncurses.h>
#include <string>
#include <vector>

using namespace std;

enum class Style {
  reset = 0,
  black = 30,
  red = 31,
  green = 32,
  yellow = 33,
  blue = 34,
  magenta = 35,
  cyan = 36,
  white = 37,
  black_bg = 40,
  red_bg = 41,
  green_bg = 42,
  yellow_bg = 43,
  blue_bg = 44,
  magenta_bg = 45,
  cyan_bg = 46,
  white_bg = 47,
  default_bg = 49,
  bold = 1,
  underline = 4,
  inverse = 7,
  bold_off = 21,
  underline_off = 24,
  inverse_off = 27
};

std::ostream &operator<<(ostream &os, Style style);

template<class T> void print(T param);

template<class T, class... Types> void print(T first, Types... rest);
template<class... Types> void print(Style style, Types... rest);
void clean_screen();

void print_title();

enum class User_input : int { next, previous, select, exit, none };

User_input get_user_input()
{
  initscr();// Initialize ncursed
  cbreak();// disable line buffering. // Note: it works with it disabled but might be needed in the future.
  // Which means characters typed by the user are immediately available to be read by the program.
  // Without this function, the user would need to press Enter before the input is sent to the program.
  noecho();// Disable echoing of characters on the screen.
  // Without this function, the characters typed by the user would be displayed on the terminal.
  keypad(stdscr, TRUE);// allow arrow keys, enter key , numeric keypad, control keys ...

  User_input option{};
  do {
    const unsigned KEY_SCAPE = 27;
    switch (getch()) {
    case KEY_UP:
    case KEY_LEFT: option = User_input::previous; break;
    case KEY_DOWN:
    case KEY_RIGHT: option = User_input::next; break;
    case KEY_ENTER:
    case '\n': option = User_input::select; break;
    case KEY_SCAPE: option = User_input::exit; break;// escape key
    default: option = User_input::none; break;
    }
  } while (option == User_input::none);

  endwin();// Clean up ncursed
  return option;
}

void display_interface(vector<string> options, unsigned option)
{
  clean_screen();
  print_title();
  for (size_t i = 0; i < options.size(); i++) {
    if (i == static_cast<unsigned>(option)) {
      print("\t> ", Style::yellow_bg, options.at(i), "\n");
      continue;
    }
    cout << "\t  " << options.at(i) << "\n";
  }
}

int main()
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


  while (true) {
    display_interface(options, static_cast<unsigned>(option));

    switch (get_user_input()) {
    case User_input::next:
      if (option == Option::quit) {
        option = Option::human_vs_human;
      } else {
        option = static_cast<Option>(static_cast<int>(option) + 1);
      }
      break;
    case User_input::previous:
      if (option == Option::human_vs_human) {
        option = Option::quit;
      } else {
        option = static_cast<Option>(static_cast<int>(option) - 1);
      }
      break;
    case User_input::select:
      switch (option) {
      case Option::human_vs_human: cout << "human_vs_human\n selected\n"; break;
      case Option::human_vs_computer: cout << "human_vs_computer\n selected\n"; break;
      case Option::quit: cout << "quit\n selected\n"; return 0;
      default: break;
      }
      break;
    case User_input::exit: return 0;
    default: break;
    }
  }

  return 0;
}

std::ostream &operator<<(ostream &os, Style style) { return os << static_cast<int>(style); }

template<class T> void print(T param) { cout << param << "\033[" << Style::reset << ";" << Style::default_bg << "m"; }
template<class T, class... Types> void print(T first, Types... rest)
{
  cout << first;
  print(rest...);
}
template<class... Types> void print(Style style, Types... rest)
{
  cout << "\033[" << style << "m";
  print(rest...);
}

void clean_screen()
{
  system("clear");// for unix
}

void print_title()
{
  cout << R"(
        /\\\\\\\\\   /\\\        /\\\   /\\\\\\\\\\\\\\\      /\\\\\\\\\\\        /\\\\\\\\\\\
      /\\\////////   \/\\\       \/\\\  \/\\\///////////     /\\\/////////\\\    /\\\/////////\\\
     /\\\/            \/\\\       \/\\\  \/\\\               \//\\\      \///    \//\\\      \///
     /\\\              \/\\\\\\\\\\\\\\\  \/\\\\\\\\\\\        \////\\\            \////\\\
     \/\\\              \/\\\/////////\\\  \/\\\///////            \////\\\            \////\\\
      \//\\\             \/\\\       \/\\\  \/\\\                      \////\\\            \////\\\
        \///\\\           \/\\\       \/\\\  \/\\\               /\\\      \//\\\    /\\\      \//\\\
           \////\\\\\\\\\  \/\\\       \/\\\  \/\\\\\\\\\\\\\\\  \///\\\\\\\\\\\/    \///\\\\\\\\\\\/
               \/////////   \///        \///   \///////////////     \///////////        \///////////)"
       << endl;
}
