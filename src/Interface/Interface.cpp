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
#include <iomanip>

/* some good references about c++ macros
https://isocpp.org/wiki/faq/misc-technical-issues#macros-with-if
 */

// Uncomment the line below to enable logging, comment to disable it
// #define ENABLE_LOGGING

#ifdef ENABLE_LOGGING
#include <chrono>
#include <ctime>
#include <fstream>
#include <sstream>
#endif

Interface::Input_error Interface::start_input_validation(Move &move,
  Gameboard &gb,
  const Player turn_color,
  bool &is_partially_pinned,
  Axis &axis)
{
  using IE = Interface::Input_error;
  Bearing &start = move.start;

  if (!(cin >> start)) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return IE::bad_input;
  }

  if (gb.at(start).empty()) { return IE::no_piece_selected; }

  const Player piece_color = gb.at(start).player;
  if (turn_color != piece_color) { return IE::cannot_move_enemy_piece; }

  const bool not_a_king =
    gb.at(start).symbol != Piece_symbols::white_king and gb.at(start).symbol != Piece_symbols::black_king;
  if (not_a_king and gb.is_absolutely_pinned(start, is_partially_pinned, axis)) { return IE::absolutely_pinned_piece; }

  if (gb.last_move_checked() and not_a_king and not gb.start_able_to_intercept(move)) {
    return IE::unable_to_intercept_menace;
  }

  if (not gb.available_movement_at(start)) { return IE::no_available_moves; }

  return IE::none;
}

Interface::Input_error
  Interface::end_input_validation(Move &move, Gameboard &gb, const bool is_partially_pinned, const Axis axis)
{
  using IE = Interface::Input_error;

  if (!(cin >> move.end)) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return IE::bad_input;
  }
  if (move.start == move.end) { return IE::start_is_equal_to_end; }

  const bool is_not_free = not gb.at(move.end).isFree;
  if (is_not_free and (gb.at(move.start).player == gb.at(move.end).player)) {// NOSONAR
    return IE::cannibalism;
  }

  const bool not_a_king =
    gb.at(move.start).symbol != Piece_symbols::white_king and gb.at(move.start).symbol != Piece_symbols::black_king;
  if (gb.last_move_checked() and not_a_king and not gb.end_able_to_intercept(move)) {
    return IE::menace_not_intercepted;
  }

  if (not gb.validMovement(move, is_partially_pinned, axis)) { return IE::illegal_movement; }

  return IE::none;
}

Move Interface::get_player_move(Gameboard &gb, Player my_player, Player_type player_type)
{
  if (player_type == Player_type::computer) {
    const int depth = 2;
    IA_functor IA{ my_player };
    return IA(gb, my_player, depth);
  }

  const string my_player_str = (my_player == Player::white) ? "white" : "black";
  Move move;
  bool is_partially_pinned{ false };
  Axis axis{};

  using IE = Input_error;
  bool invalid_start{ true };
  string error_message{};
  do {
    clean_screen();
    display_gameboard(gb, my_player);
    print(Style::red, error_message, ".\n");
    cout << my_player_str << "\n";

    cout << "Input the start position letter and number:\n";
    switch (start_input_validation(move, gb, my_player, is_partially_pinned, axis)) {
    case IE::none: invalid_start = false; break;
    case IE::bad_input: error_message = "bad input"; break;
    case IE::no_piece_selected: error_message = "No piece was selected"; break;
    case IE::no_available_moves: error_message = "No available moves for that piece"; break;
    case IE::absolutely_pinned_piece: error_message = "That piece is pinned"; break;
    case IE::cannot_move_enemy_piece: error_message = "That piece is not yours"; break;
    case IE::unable_to_intercept_menace: error_message = "Unable to intercept the menace"; break;

    default: error_message = "unexpected error"; break;
    }
  } while (invalid_start);

  gb.draw_piece_possibilities(move.start, is_partially_pinned, axis);

  error_message.clear();
  const Piece piece_to_undraw = gb.at(move.start);
  bool invalid_end{ true };
  do {
    clean_screen();
    display_gameboard(gb, my_player);
    print(Style::red, error_message, ".\n");
    cout << my_player_str << "\t" << move.start << "\n";

    cout << "Input the end position letter and number:\n";
    switch (end_input_validation(move, gb, is_partially_pinned, axis)) {
    case IE::none: invalid_end = false; break;
    case IE::bad_input: error_message = "bad input"; break;
    case IE::cannibalism: error_message = "You can't capture your own pieces"; break;
    case IE::illegal_movement: error_message = "That movement is not allowed"; break;
    case IE::start_is_equal_to_end: error_message = "You must move"; break;
    case IE::menace_not_intercepted: error_message = "You must intercept the menace"; break;

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

void Interface::interface_state_machine(Interface_state state)
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

#ifdef ENABLE_LOGGING
std::string log_path()
{
  auto now = chrono::system_clock::now();
  time_t currentTime = chrono::system_clock::to_time_t(now);

  tm *local_time = localtime(&currentTime);// Convert the time_t object to a struct tm object
  ostringstream file_name;
  file_name << put_time(local_time, "log__%Y-%m-%d_%H-%M-%S.log");
  string logs_dir = "../logs/";
  // string logs_dir = "./logs/";
  string log_path = logs_dir + file_name.str();
  return log_path;
}
#endif


// Interface::Interface_state Interface::game(const Game_settings &game_settings, logging = false, ) const
Interface::Interface_state Interface::game(const Game_settings &game_settings) const
{
  // enable and disable logging in "interface.hpp" file
#ifdef ENABLE_LOGGING

  ofstream log_file{ log_path() };
  if (!log_file) { cerr << "couldn't open \"" << log_path << "\" for writing" << endl; }

// #define LOG(x) log_file << x << "\n";
#define LOG(x) log_file << x << " ";

#else

#define LOG(x)

#endif

  Gameboard gb(game_settings);
  Player turn{ Player::white };
  const Player_type black_player_type = game_settings.black_config.player_type;
  const Player_type white_player_type = game_settings.white_config.player_type;

  Game_result game_result{ gb.check_end_conditions() };
  while (game_result == Game_result::no_results_yet) {
    const Player_type turn_player_type = turn == Player::white ? white_player_type : black_player_type;
    const Move move = get_player_move(gb, turn, turn_player_type);
    LOG(move);
    gb.make_move(move);
    game_result = gb.check_end_conditions();

    game::switch_player(turn);
  }

#ifdef ENABLE_LOGGING
  LOG(static_cast<int>(game_result));
  log_file.close();
#endif

  return game_results_interface(game_result);
}

// Game_settings Interface::modify_chess_board_interface(Game_settings game_settings)
void Interface::modify_chess_board_interface(Game_settings &game_settings)
{
  clean_screen();
  /* ideal plan:
  message
  dimensions
  chess_pieces (input the matrix);
  */

  const std::vector<string> white_names{ "Pawn", "Bishop", "Knight", "Rook", "Queen", "King" };
  const string white_symbols{ "pactqr" };
  const std::vector<string> black_names{ "Pawn", "Bishop", "Knight", "Rook", "Queen", "King" };
  const string black_symbols{ "PACTQR" };

  cout << setw(15) << "White pieces\t" << setw(15) << "Black pieces" << endl;

  std::cout << std::left;
  for (int i = 0; i < 6; i++) {
    cout << setw(6) << white_names[i] << setw(2) << ":" << white_symbols[i] << "\t";
    cout << setw(6) << black_names[i] << setw(2) << ":" << black_symbols[i] << endl;
  }

  cout << "Input the 8x8 matrix:" << endl;
  for (auto &row : game_settings.board) {
    for (auto &e : row) {
      // TODO: modify this for integration testing and unit testing.
      cin >> e;
    }
  }
  // clean_screen();
  // return game_settings;
}
