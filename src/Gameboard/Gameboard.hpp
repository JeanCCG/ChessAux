#ifndef __GAMEBOARD_H__
#define __GAMEBOARD_H__


#include "../Game_settings/Game_settings.hpp"
#include "../Map/Map.hpp"
#include "../Move/Move.hpp"
#include "../Piece_set/Piece_set.hpp"
#include "../Score/Score.hpp"

#include <algorithm>
#include <unordered_map>
#include <vector>

const auto do_nothing = []([[maybe_unused]] const Bearing b) { /* do nothing */ };
const auto do_nothing_false = []([[maybe_unused]] const Bearing b) {
  return false;
};
const auto do_nothing_true = []([[maybe_unused]] const Bearing b) {
  return true;
};

namespace game {
inline unsigned difference(const unsigned lhs, const unsigned rhs) { return (lhs > rhs) ? lhs - rhs : rhs - lhs; }
inline void switch_player(Player &player) { player = (player == Player::white) ? Player::black : Player::white; }
}// namespace game


class Gameboard
{
public:
  unsigned width{ 8 };
  unsigned height{ 8 };
  Score score;
  enum Direction { right, top, left, bot, top_right, top_left, bot_left, bot_right };

  explicit Gameboard(const Game_settings &game_settings);
  // explicit Gameboard(const Gameboard &gb);
  ~Gameboard() { clear_stack_vars(); }

  void move(const Move t_move);// end MUST BE FREE
  void capture(const Move t_move);
  void eat(const Move t_move) { capture(t_move); }
  bool validMovement(const Move t_move, const bool is_partially_pinned, const Axis axis);

  template<class Do_if_movable, class Do_if_edible>
  bool
    manage_available_menace_interceptors(const Bearing place, Do_if_movable do_if_movable, Do_if_edible do_if_edible);

  template<class Do_if_movable, class Do_if_edible>
  bool available_movement_at(const Bearing place,
    Do_if_movable do_if_movable = do_nothing,
    Do_if_edible do_if_edible = do_nothing)
  {
    return available_movement_at(place, do_if_movable, do_if_edible, false, Axis{});
  }

  template<class Do_if_movable, class Do_if_edible> bool available_movement_at(const Bearing place)
  {
    return available_movement_at(place, do_nothing, do_nothing, false, Axis{});
  }

  template<class Do_if_movable, class Do_if_edible>
  bool available_movement_at(const Bearing place,
    Do_if_movable do_if_movable,
    Do_if_edible do_if_edible,
    const bool is_partially_pinned,
    const Axis axis);

  bool available_movement_at(const Bearing place) { return available_movement_at(place, do_nothing, do_nothing); }


  bool available_movement_for_player(const Player player);

  bool draw_piece_possibilities(const Bearing place, const bool is_partially_pinned, const Axis axis);

  bool is_absolutely_pinned(const Bearing place, bool &is_partially_pinned, Axis &axis);
  // bool movable_piece(const Bearing place) { return !is_absolutely_pinned(place); }

  void undraw_piece_possibilities(const Bearing place, const Piece piece);
  void move_or_capture(const Move t_move);
  void make_move(const Move t_move);

  bool isMenaced(const Player player, const Bearing place);

  bool king_is_menaced(const Player player, const Bearing place);

  Piece &at(Bearing bearing) { return slots[bearing.x][bearing.y]; }
  Game_result check_end_conditions();

  bool last_move_checked() const { return m_last_move_checked; }
  void set_last_move_checked(const bool value) { m_last_move_checked = value; }

  bool start_able_to_intercept(const Move move);// check key
  bool end_able_to_intercept(const Move move);// check values
  // TODO: move this to private after solving the 766 copy destructor bug
  Map interceptor_map{ nullptr };

private:
  Piece slots[8][8];
  Bearing white_king_bearing;
  Bearing black_king_bearing;
  int &white_score = score.score[0];
  int &black_score = score.score[1];
  Move last_move{ { 0U, 0U }, { 0U, 0U } };

  bool m_last_move_checked{ false };

  /*
  E: enemy knight
  *: possible interceptor
    _ _ _ _ _ _ _ _
    _ _ * _ * _ _ _
    _ * * * * * _ _
    _ _ * E * _ _ _
    _ * * * * * _ _
    _ _ * _ * _ _ _
    _ _ _ _ _ _ _ _
    _ _ _ _ _ _ _ _
    There are 15 possible interceptors, but it's easier to work with 16.
   */

  Page<16> knight_interceptors{ nullptr };

  std::vector<Bearing> m_menaces;

  Direction get_direction(const Move from_to)
  {
    auto [from, to] = from_to;
    if (from.x == to.x) {
      if (from.y > to.y) {
        return Direction::bot;
      } else {
        return Direction::top;
      }
    }
    if (from.y == to.y) {
      if (from.x < to.x) {
        return Direction::right;
      } else {
        return Direction::left;
      }
    }

    // check diagonals
    if (from.x < to.x) {
      if (from.y < to.y) {
        return Direction::top_right;
      } else {
        return Direction::bot_right;
      }
    }

    if (from.y < to.y) {
      return Direction::top_left;
    } else {
      return Direction::bot_left;
    }
  }

  std::vector<Bearing> menaces() const { return m_menaces; }

  void clear_stack_vars()
  {
    knight_interceptors.clear();
    interceptor_map.clear();
  }

  auto get_number_of_menaces() const { return m_menaces.size(); }
  Piece_symbols get_menace_symbol() { return at(m_menaces.front()).symbol; }
  bool available_knight_interceptor(const Player interceptor_p, const Bearing place);
  bool is_interceptable(const Player interceptor_p, const Bearing place);
  bool pawn_intercepts(const Player interceptor_p, const Bearing place);
  bool available_menace_interceptor(const Bearing place);
  bool legal_pawn(const Move t_move);
  bool legal_diagonal(const Move t_move);
  bool legal_straight(const Move t_move);
  bool legal_king(const Move t_move);
  static bool legal_jump(const Move t_move);

  void drawDot(const Bearing place) { at(place).symbol = Piece_symbols::dot; }
  void unDrawDot(const Bearing place) { at(place).symbol = Piece_symbols::empty; }
  bool is_an_enemy_piece(const Player my_player, const Bearing bearing) { return (at(bearing).player != my_player); }
  bool first_movement(const Bearing bearing) { return at(bearing).movements == 0; }

  template<class Do_if_movable, class Do_if_edible>
  bool evaluate_pawn_possibilities(const Bearing place, Do_if_movable do_if_movable, Do_if_edible do_if_edible);

  bool draw_pawn(const Bearing place);
  void undraw_white_pawn(const Bearing place);
  void undraw_black_pawn(const Bearing place);
  bool draw_jumps(const Bearing place);
  void undraw_jumps(const Bearing place);

  template<class Do_if_movable, class Do_if_edible>
  bool evaluate_jump_possibilities(const Bearing place, Do_if_movable do_if_movable, Do_if_edible do_if_edible);
  template<class Do_if_movable, class Do_if_edible>
  bool evaluate_king_possibilities(const Bearing king_bearing, Do_if_movable do_if_movable, Do_if_edible do_if_edible);
  template<class Do_if_movable>
  bool evaluate_castlings(const Bearing king_bearing, Do_if_movable do_if_movable, const Player my_player);

  void undraw_king(const Bearing king_bearing);

  template<class Functor, class Extra_condition>
  bool iterate_from_to_and_perform(Bearing place,
    Direction Direction,
    Functor perform,
    Extra_condition extra_condition = do_nothing_true) const;

  template<class Functor> bool iterate_from_to_and_perform(Bearing place, Direction Direction, Functor perform) const;

  template<class Functor, class Condition>
  bool perform_jumps(const Bearing place, Functor perform, Condition condition) const;

  template<class Functor, class Extra_condition>
  bool straight_menace(const Bearing place, Functor perform, Extra_condition extra_condition);

  template<class Functor> void straight_menace(const Bearing place, Functor perform)
  {
    straight_menace(place, perform, do_nothing_true);
  }

  template<class Functor, class Extra_condition>
  bool diagonal_menace(const Bearing place, Functor perform, Extra_condition extra_condition);

  template<class Functor> bool diagonal_menace(const Bearing place, Functor perform)
  {
    return diagonal_menace(place, perform, do_nothing_true);
  }

  bool pawn_menace(const Player player, const Piece_symbols enemy_pawn, const Bearing place);

  template<class Do_if_menace>
  bool pawn_menace(const Player player, const Piece_symbols enemy_pawn, const Bearing place, Do_if_menace do_if_menace);

  static bool king_menace(const Bearing enemy_king_b, const Bearing place);

  template<class Functor, class Extra_condition>
  void diagonal_perform(const Bearing place, Functor perform, Extra_condition extra_condition);
  template<class Functor, class Extra_condition>
  void straight_perform(const Bearing place, Functor perform, Extra_condition extra_condition);
};


#include "Gameboard.tcc"

#endif// __GAMEBOARD_H__
