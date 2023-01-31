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

Interface::Input_error Interface::start_input_validation(Move &move, Gameboard gb, const Player turn_color)
{
  using Input_error = Interface::Input_error;
  if (!(cin >> move.start)) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return Input_error::bad_input;
  }

  if (const bool isFree = gb.at(move.start).isFree; isFree) { return Input_error::no_piece_selected; }

  const Player piece_color = gb.at(move.start).player;
  if (turn_color != piece_color) { return Input_error::cannot_move_enemy_piece; }

  if (gb.is_absolutely_pinned(move.start)) { return Input_error::absolutely_pinned_piece; }

  const bool availableMovement = gb.draw_piece_possibilities(move.start);
  if (!availableMovement) { return Input_error::no_available_moves; }

  return Input_error::none;
}

Interface::Input_error Interface::end_input_validation(Move &move, Gameboard gb)
{
  using Input_error = Interface::Input_error;

  if (!(cin >> move.end)) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return Input_error::bad_input;
  }
  if (move.start == move.end) { return Input_error::start_is_equal_to_end; }

  const bool is_not_free = not gb.at(move.end).isFree;
  if (is_not_free and (gb.at(move.start).player == gb.at(move.end).player)) { return Input_error::cannibalism; }

  const bool invalidMovement = not gb.validMovement(move);
  if (invalidMovement) { return Input_error::illegal_movement; }

  return Input_error::none;
}

void Interface::display_gameboard(Gameboard &gb, Player turn_perspective)
{
  std::string blank_frame{ "   " };
  std::string blank_slot{ "       " };
  Style slot_color = Style::white_bg;
  // Style axis_color = Style::yellow;
  const auto width = gb.width;
  const auto height = gb.height;

  auto switch_slot_color = [&]() { slot_color = (slot_color == Style::white_bg) ? Style::black_bg : Style::white_bg; };

  auto print_blank_lane = [&]() {
    cout << blank_frame;
    for (unsigned x_it = 0; x_it < width; x_it++, switch_slot_color()) { print(slot_color, blank_slot); }
    cout << '\n';
  };

  auto print_lane = [&](Bearing b) {
    cout << ' ' << static_cast<char>(b.y + 'A') << ' ';
    switch_slot_color();
    string slot = blank_slot;
    for (; b.x < width; b.x++, switch_slot_color()) {
      slot[blank_slot.size() / 2] = static_cast<char>(gb.at(b).symbol);
      print(slot_color, blank_slot);
    }
    cout << '\n';
  };

  const unsigned direction = (turn_perspective == Player::white) ? -1U : +1U;
  const unsigned limit = (turn_perspective == Player::white) ? -1U : height;
  for (Bearing b = { 0U, (turn_perspective == Player::white) ? width - 1 : 0 }; b.y != limit; b.y += direction) {
    print_blank_lane();
    print_lane(b);
    print_blank_lane();
  }

  // print x axis bottom frame
  cout << blank_frame;
  for (unsigned i = 1; i < width + 1; i++) {
    string slot_number = blank_slot;
    slot_number[blank_slot.size() / 2] = static_cast<char>('0' + i);
    cout << slot_number;
  }
  cout << '\n';
}

Move Interface::get_player_move(Gameboard gb, Player my_player, bool is_human_player)
{
  if (not is_human_player) { /* return IA(); */
    return { { 0U, 0U }, { 0U, 0U } };
  }

  Move move;


  using IE = Input_error;
  bool invalid_start{ true };
  do {
    cout << "Input the start position letter and number:\n";
    switch (start_input_validation(move, gb, my_player)) {
    case IE::bad_input: cout << "bad input"; break;
    case IE::no_piece_selected: cout << "No piece was selected"; break;
    case IE::no_available_moves: cout << "No available moves for that piece"; break;
    case IE::absolutely_pinned_piece: cout << "That piece is pinned"; break;

    case IE::none:
    default: invalid_start = false; break;
    }
    cout << ".\n";
  } while (invalid_start);

  const Piece piece_to_undraw = gb.at(move.start);
  bool invalid_end{ true };
  do {
    cout << "Input the end position letter and number:\n";
    switch (end_input_validation(move, gb)) {
    case IE::bad_input: cout << "bad input"; break;
    case IE::cannibalism: cout << "You can't capture your own pieces"; break;
    case IE::illegal_movement: cout << "That movement is not allowed"; break;
    case IE::start_is_equal_to_end: cout << "You must move"; break;

    case IE::none:
    default: invalid_end = false; break;
    }
    cout << "\n";

  } while (invalid_end);

  gb.undraw_piece_possibilities(move.start, piece_to_undraw);
  return move;
}


void Interface::input_anything()
{
  cout << "\t\tInput [anything] to continue:\n\t\t\t\t";
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void Interface::display_interface(vector<string> options, unsigned option)
{
  clean_screen();
  print_title(5);

  for (size_t i = 0; i < options.size(); i++) {
    if (i == option) {
      print("\t> ", Style::white_bg, options.at(i), "\n");
      continue;
    }
    cout << "\t  " << options.at(i) << "\n";
  }
}

void Interface::interface_state_machine(Interface_state state)
{
  using Interface_state = Interface::Interface_state;
  do {
    switch (state) {
    case Interface_state::main: state = main_interface(); break;
    case Interface_state::human_vs_computer: state = human_vs_computer_interface(); break;
    case Interface_state::human_vs_human: state = human_vs_human_interface(); break;
    // case Interface_state::game_results: state = game_results_interface(); break;
    default:
    case Interface_state::end_program: state = Interface_state::end_program; break;
    }
  } while (state != Interface_state::end_program);
}

void Interface::clean_screen() { system("clear"); }// Flawfinder: ignore ; reason: it is what it is //NOLINT
