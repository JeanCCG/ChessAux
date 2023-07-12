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

using namespace std;

void Interface::display_gameboard(Gameboard &gb, const Player turn_perspective)
{
  const std::string blank_frame{ "   " };
  std::string blank_slot{ "       " };
  const std::string half_blank_slot{ "   " };

  const Style white_slot_color = Style::yellow_bg;
  const Style black_slot_color = Style::black_bg;

  Style slot_color = white_slot_color;
  const Style slot_letter_color = Style::black;
  // Style axis_color = Style::yellow;
  const auto width = gb.width;
  // const auto height = gb.height;

  auto switch_slot_color = [&]() {
    slot_color = (slot_color == white_slot_color) ? black_slot_color : white_slot_color;
  };

  auto print_blank_lane = [&]() {
    cout << blank_frame;
    for (unsigned x_it = 0; x_it < width; x_it++, switch_slot_color()) { print(slot_color, blank_slot); }
    cout << '\n';
    switch_slot_color();
  };

  auto print_lane = [&](Bearing b) {
    // cout << ' ' << static_cast<char>(b.y + '0') << ' ';
    cout << ' ' << static_cast<char>(b.y + '1') << ' ';
    switch_slot_color();
    for (; b.x < width; b.x++, switch_slot_color()) {
      string slot = blank_slot;
      slot[blank_slot.size() / 2] = static_cast<char>(gb.at(b).symbol);
      print(slot_color, half_blank_slot);
      print(slot_color, slot_letter_color, static_cast<char>(gb.at(b).symbol));
      print(slot_color, half_blank_slot);
    }
    cout << '\n';
  };

  const unsigned direction{ -1U };
  const unsigned limit{ -1U };
  const unsigned y_init{ width - 1 };

  // if (turn_perspective == Player::white) {
  //   direction = -1U;
  //   limit = -1U;
  //   y_init = width - 1;
  // } else {
  //   direction = +1U;
  //   limit = height;
  //   y_init = 0;
  // }


  for (Bearing b = { 0U, y_init }; b.y != limit; b.y += direction) {
    print_blank_lane();
    print_lane(b);
    print_blank_lane();
  }

  // print x axis bottom frame
  cout << blank_frame;
  for (unsigned i = 0; i < width; i++) {
    string slot_number = blank_slot;
    // slot_number[blank_slot.size() / 2] = static_cast<char>('0' + i);
    slot_number[blank_slot.size() / 2] = static_cast<char>('A' + i);
    cout << slot_number;
  }
  cout << '\n';
}
