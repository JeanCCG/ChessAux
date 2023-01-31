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

using namespace std;

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
    // end
    illegal_movement,
    cannibalism,
    none,
    start_is_equal_to_end,
    your_king_still_checked,
  };

  // Input_error input_validation(Bearing bearing, Gameboard gb) {}
  static Input_error start_input_validation(Move &move, Gameboard gb, const Player turn_color);
  static Input_error end_input_validation(Move &move, Gameboard gb);

  static void display_gameboard(Gameboard &gb, Player turn_perspective);

  static void print_title(const int option);
  Move get_player_move(Gameboard gb, Player my_player, bool is_human_player = true);

  // game_interface
  void game(const Game_settings &game_settings)
  {
    Gameboard gb(game_settings);
    Player turn{ Player::white };

    Game_result game_result{ no_results_yet };
    do {
      gb.make_move(get_player_move(gb, turn));
      game_result = gb.check_end_conditions();
      game::switch_player(turn);
    } while (game_result == no_results_yet);

    // return game_results_interface(game_result);
  }

  static void input_anything();

  static void clean_screen();

  static void display_interface(vector<string> options, unsigned option);

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
  Interface_state human_vs_human_interface();
  Interface_state static game_results_interface(const Game_result game_result);

  void interface_state_machine(Interface_state state = Interface_state::main);

public:
  void init() { interface_state_machine(); }
};


#endif// __INTERFACE_H__
