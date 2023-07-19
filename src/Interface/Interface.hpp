#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include "../Game_settings/Game_settings.hpp"
#include "../Gameboard/Gameboard.hpp"
#include "../Move/Move.hpp"
#include "../enums/enums.hpp"
#include "print.hpp"

#include <iostream>
#include <limits>
#include <ncurses.h>
#include <string>
#include <vector>

// DEBUG VARS
#include "../config/logging.hpp"

// Uncomment the line below to enable logging, comment to disable it
// #define ENABLE_LOGGING

class Interface
{
private:
  enum class Input_error {
    // start
    bad_input,
    no_piece_selected,
    cannot_move_enemy_piece,
    absolutely_pinned_piece,
    no_available_moves,
    this_piece_cannot_uncheck_your_king,
    unable_to_intercept_menace,
    // end
    illegal_movement,
    cannibalism,
    none,
    start_is_equal_to_end,
    your_king_still_checked,
    menace_not_intercepted,
  };

  static Input_error
    start_input_validation(Move &move, Gameboard &gb, const Player turn_color, bool &is_partially_pinned, Axis &axis);
  static Input_error end_input_validation(Move &move, Gameboard &gb, const bool is_partially_pinned, const Axis axis);

  static void display_gameboard(Gameboard &gb, Player turn_perspective);

  static void print_title(const int option);
  static Move get_player_move(Gameboard &gb, Player my_player, Player_type player_type);

  static void input_anything();

  static void clean_screen();

  static void display_interface(std::vector<std::string> options, unsigned option);

  enum class User_input : int { up, down, right, left, select, exit, none };
  User_input static get_user_key_input();

  enum class Interface_state {
    main,
    human_vs_computer,
    human_vs_human,
    game_results,
    end_program,
  };

  Interface_state main_interface();
  Interface_state human_vs_computer_interface();
  Interface_state human_vs_human_interface() const;
  Interface_state static game_results_interface(const Game_result game_result);
  // Game_settings modify_chess_board_interface(Game_settings game_settings);

  void modify_chess_board_interface(Game_settings &game_settings);

  void interface_state_machine(Interface_state state);

public:
  // public for testing
  Interface_state game(const Game_settings &game_settings) const;
  // public for testing

  void init() { interface_state_machine(Interface_state::main); }
};


#endif// __INTERFACE_H__
