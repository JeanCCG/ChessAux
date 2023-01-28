#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include "../Game_settings/Game_settings.hpp"
#include "../Move/Move.hpp"
#include "../enums/enums.h"
#include "../gameboard/gameboard.hpp"
#include "print.hpp"
#include <iostream>
#include <limits>
#include <ncurses.h>
#include <string>
#include <vector>

using namespace std;

class Interface
{
  Game_result game_result;
  enum class Input_error {
    // start
    bad_input,
    no_piece_selected,
    cannot_move_enemy_piece,
    absolutely_pinned_piece,
    no_available_moves,
    this_piece_cannot_uncheck_your_king,
    // end
    illegal_movement,
    cannibalism,
    none,
    start_is_equal_to_end,
    your_king_still_checked,
  };
  // Input_error input_validation(Bearing bearing, Gameboard gb) {}
  Input_error start_input_validation(Move &move, Gameboard gb, const Player turn_color)
  {
    if (!(cin >> move.start)) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      return Input_error::bad_input;
    }

    if (bool isFree = gb.at(move.start).isFree; isFree) { return Input_error::no_piece_selected; }

    Player piece_color = gb.at(move.start).player;
    if (turn_color != piece_color) { return Input_error::cannot_move_enemy_piece; }

    if (gb.is_absolutely_pinned(move.start)) { return Input_error::absolutely_pinned_piece; }
    bool availableMovement = gb.draw_piece_possibilities(move.start);
    if (!availableMovement) { return Input_error::no_available_moves; }

    return Input_error::none;
  }

  Input_error end_input_validation(Move &move, Gameboard gb)
  {
    if (!(cin >> move.end)) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      return Input_error::bad_input;
    }
    if (move.start == move.end) { return Input_error::start_is_equal_to_end; }

    const bool is_not_free = not gb.at(move.end).isFree;
    if (is_not_free and (gb.at(move.start).player == gb.at(move.end).player)) { return Input_error::cannibalism; }

    if (bool invalidMovement = not gb.validMovement(move); invalidMovement) { return Input_error::illegal_movement; }
    return Input_error::none;
  }


  void display_gameboard(Gameboard &gb, Player turn_perspective)
  {
    std::string blank_frame{ "   " };
    std::string blank_slot{ "       " };
    Style slot_color = Style::white_bg;
    // Style axis_color = Style::yellow;
    const auto width = gb.width;
    const auto height = gb.height;

    auto switch_slot_color = [&]() {
      slot_color = (slot_color == Style::white_bg) ? Style::black_bg : Style::white_bg;
    };

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
      slot_number[blank_slot.size() / 2] = i;
      cout << slot_number;
    }
    cout << '\n';
  }

public:
  void print_title(int option);

  Game_result get_player_move(Gameboard gb, Player my_player, bool is_human_player = true)
  {
    if (not is_human_player) { /* return IA(); */
      ;
    }

    Move move;

    bool invalid_start{ true };
    do {
      cout << "Input the start position letter and number:\n";
      switch (start_input_validation(move, gb, my_player)) {
      case Input_error::bad_input: cout << "bad input"; break;
      case Input_error::no_piece_selected: cout << "No piece was selected"; break;
      case Input_error::no_available_moves: cout << "No available moves for that piece"; break;
      case Input_error::absolutely_pinned_piece: cout << "That piece is pinned"; break;
      case Input_error::none:
      default: invalid_start = false; break;
      }
      cout << ".\n";
    } while (invalid_start);

    Piece piece_to_undraw = gb.at(move.start);
    bool invalid_end{ true };
    do {
      cout << "Input the end position letter and number:\n";
      switch (end_input_validation(move, gb)) {
      case Input_error::bad_input: cout << "bad input"; break;
      case Input_error::cannibalism: cout << "You can't capture your own pieces"; break;
      case Input_error::illegal_movement: cout << "That movement is not allowed"; break;
      case Input_error::start_is_equal_to_end: cout << "You must move"; break;
      case Input_error::none:
      default: invalid_end = false; break;
      }
      cout << "\n";

    } while (invalid_end);

    gb.undraw_piece_possibilities(move.start, piece_to_undraw);
    // return gb.check_end_conditions();
  }

  void game(const Game_settings &game_settings)
  {
    Gameboard gb(game_settings);
    Player turn{ Player::white };

    auto switch_turn = [&]() { turn = (turn == Player::white) ? Player::black : Player::white; };

    game_result = no_results_yet;
    do {
      game_result = get_player_move(gb, turn);

      // game_result = gb.make_move(get_player_move(gb, turn));
      switch_turn();
    } while (game_result == no_results_yet);

    game_results_interface();
    // Interface_state next_interface{ Interface_state::main};
    // return next_interface;
  }

  void static input_anything()
  {
    cout << "\t\tInput [anything] to continue:\n\t\t\t\t";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
  }

  void static clean_screen() { system("clear"); }// Flawfinder: ignore ; reason: it is what it is

  void display_interface(vector<string> options, unsigned option)
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

  enum class User_input : int { up, down, right, left, select, exit, none };
  User_input static get_user_key_input();

  enum class Interface_state {
    main,
    human_vs_computer,
    human_vs_human,
    game_results,
    end_program,
  };

  void init() { interface_state_machine(); }
  Interface_state main_interface();
  Interface_state human_vs_computer_interface();
  Interface_state human_vs_human_interface();
  Interface_state game_results_interface();


  void interface_state_machine(Interface_state state = Interface_state::main)
  {
    do {
      switch (state) {
      case Interface_state::main: state = main_interface(); break;
      case Interface_state::human_vs_computer: state = human_vs_computer_interface(); break;
      case Interface_state::human_vs_human: state = human_vs_human_interface(); break;
      case Interface_state::game_results: state = game_results_interface(); break;
      default:
      case Interface_state::end_program: state = Interface_state::end_program; break;
      }
    } while (state != Interface_state::end_program);
  }
};


#endif// __INTERFACE_H__
