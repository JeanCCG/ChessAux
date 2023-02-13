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

  static Input_error start_input_validation(Move &move, Gameboard gb, const Player turn_color);
  static Input_error end_input_validation(Move &move, Gameboard gb);

  static void display_gameboard(Gameboard &gb, Player turn_perspective);

  static void print_title(const int option);
  static Move get_player_move(Gameboard gb, Player my_player, Player_type player_type);

  // game_interface

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

  Interface_state main_interface() const;
  Interface_state human_vs_computer_interface() const;
  Interface_state human_vs_human_interface() const;
  Interface_state static game_results_interface(const Game_result game_result);

  Interface_state game(const Game_settings &game_settings) const
  {
    Gameboard gb(game_settings);
    Player turn{ Player::white };
    const Player_type black_player_type = game_settings.black_config.player_type;
    const Player_type white_player_type = game_settings.white_config.player_type;

    Game_result game_result{ Game_result::no_results_yet };
    do {
      const Player_type turn_player_type = turn == Player::white ? white_player_type : black_player_type;
      gb.make_move(get_player_move(gb, turn, turn_player_type));
      game_result = gb.check_end_conditions();
      game::switch_player(turn);
    } while (game_result == Game_result::no_results_yet);

    return game_results_interface(game_result);
  }

  void interface_state_machine(Interface_state state = Interface_state::main) const;

public:
  void init() const { interface_state_machine(); }
};


#endif// __INTERFACE_H__
