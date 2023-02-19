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
#include "../IA/IA.hpp"

Interface::Input_error Interface::start_input_validation(Move &move, Gameboard gb, const Player turn_color)
{
  using IE = Interface::Input_error;
  if (!(cin >> move.start)) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return IE::bad_input;
  }

  if (gb.at(move.start).empty()) { return IE::no_piece_selected; }

  const Player piece_color = gb.at(move.start).player;
  if (turn_color != piece_color) { return IE::cannot_move_enemy_piece; }

  if (gb.is_absolutely_pinned(move.start)) { return IE::absolutely_pinned_piece; }

  if (not gb.available_movement_at(move.start)) { return IE::no_available_moves; }

  return IE::none;
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
  if (is_not_free and (gb.at(move.start).player == gb.at(move.end).player)) {// NOSONAR
    return Input_error::cannibalism;
  }

  if (not gb.validMovement(move)) { return Input_error::illegal_movement; }

  return Input_error::none;
}

Move Interface::get_player_move(Gameboard gb, Player my_player, Player_type player_type)
{
  if (player_type == Player_type::computer) {
    const int depth = 2;
    IA_functor IA{ my_player };
    return IA(gb, my_player, depth);
  }

  const string my_player_str = (my_player == Player::white) ? "white" : "black";
  Move move;

  // if my_king_is_menaced
  // it must not be menaced at the end of my turn.
  // problem: in the IA, it might sacrifice the king to gain more points in the end of it.
  //

  using IE = Input_error;
  bool invalid_start{ true };
  string error_message{};
  do {
    clean_screen();
    display_gameboard(gb, my_player);
    print(Style::red, error_message, ".\n");
    cout << my_player_str << "\n";

    cout << "Input the start position letter and number:\n";
    switch (start_input_validation(move, gb, my_player)) {
    case IE::none: invalid_start = false; break;
    case IE::bad_input: error_message = "bad input"; break;
    case IE::no_piece_selected: error_message = "No piece was selected"; break;
    case IE::no_available_moves: error_message = "No available moves for that piece"; break;
    case IE::absolutely_pinned_piece: error_message = "That piece is pinned"; break;
    case IE::cannot_move_enemy_piece: error_message = "That piece is not yours"; break;

    default: error_message = "unexpected error"; break;
    }
  } while (invalid_start);

  gb.draw_piece_possibilities(move.start);

  error_message.clear();
  const Piece piece_to_undraw = gb.at(move.start);
  bool invalid_end{ true };
  do {
    clean_screen();
    display_gameboard(gb, my_player);
    print(Style::red, error_message, ".\n");
    cout << my_player_str << "\n";

    cout << "Input the end position letter and number:\n";
    switch (end_input_validation(move, gb)) {
    case IE::none: invalid_end = false; break;
    case IE::bad_input: error_message = "bad input"; break;
    case IE::cannibalism: error_message = "You can't capture your own pieces"; break;
    case IE::illegal_movement: error_message = "That movement is not allowed"; break;
    case IE::start_is_equal_to_end: error_message = "You must move"; break;

    default: error_message = "unexpected error"; break;
    }
  } while (invalid_end);
  gb.undraw_piece_possibilities(move.start, piece_to_undraw);


  return move;
}


void Interface::input_anything()
{
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  cout << "\n\t\tInput [anything] to continue:\n\t\t\t\t";
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

void Interface::interface_state_machine(Interface_state state) const
{
  using Interface_state = Interface::Interface_state;
  do {
    switch (state) {
    case Interface_state::main: state = main_interface(); break;
    case Interface_state::human_vs_computer: state = human_vs_computer_interface(); break;
    case Interface_state::human_vs_human: state = human_vs_human_interface(); break;
    case Interface_state::end_program: state = Interface_state::end_program; break;
    default: break;
    }
  } while (state != Interface_state::end_program);

  clean_screen();
}

void Interface::clean_screen() { system("clear"); }// Flawfinder: ignore ; reason: it is what it is //NOLINT

Interface::Interface_state Interface::game(const Game_settings &game_settings) const
{
  Gameboard gb(game_settings);
  Player turn{ Player::white };
  const Player_type black_player_type = game_settings.black_config.player_type;
  const Player_type white_player_type = game_settings.white_config.player_type;

  Game_result game_result{ Game_result::no_results_yet };
  do {
    const Player_type turn_player_type = turn == Player::white ? white_player_type : black_player_type;
    const Move move = get_player_move(gb, turn, turn_player_type);
    // gb.make_move(get_player_move(gb, turn, turn_player_type));
    gb.make_move(move);
    game_result = gb.check_end_conditions();
    game::switch_player(turn);
  } while (game_result == Game_result::no_results_yet);

  return game_results_interface(game_result);
}
