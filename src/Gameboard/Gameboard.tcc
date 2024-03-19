// Copyright 2023 marcus
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/* inspired by
https://softwareengineering.stackexchange.com/questions/373916/c-preferred-method-of-dealing-with-implementation-for-large-templates
*/
#include "../Game_settings/Game_settings.hpp"
#include "../Map/Map.hpp"
#include "../Move/Move.hpp"
#include "../Piece_set/Piece_set.hpp"
#include "../Score/Score.hpp"
#include "./Gameboard.hpp"

#include <algorithm>
#include <unordered_map>
#include <vector>

/*****************************************************************************
 * Template implementation
 ****************************************************************************/

template<class Do_if_movable, class Do_if_edible>
bool Gameboard::manage_available_menace_interceptors(const Bearing place,
  Do_if_movable do_if_movable,
  Do_if_edible do_if_edible)
{
  const Piece_symbols menace_symbol = at(m_menaces.front()).symbol;
  const bool knight_case =
    (menace_symbol == Piece_symbols::black_knight or menace_symbol == Piece_symbols::white_knight);
  if (knight_case) { return not knight_interceptors.empty() and knight_interceptors.contains(place); }

  const bool valid_key = not interceptor_map[place].empty();
  if (valid_key) {
    auto perform = [this, &do_if_movable, &do_if_edible](const Bearing b) {
      if (at(b).empty()) {
        do_if_movable(b);
      } else {
        do_if_edible(b);
      }
    };
    interceptor_map[place].for_each(perform);
    return true;
  }
  return false;
}

template<class Do_if_movable, class Do_if_edible>
bool Gameboard::available_movement_at(const Bearing place,
  Do_if_movable do_if_movable,
  Do_if_edible do_if_edible,
  const bool is_partially_pinned,
  const Axis axis)
{
  bool available_movement{ false };
  const Player player = at(place).player;
  const Piece_symbols piece{ at(place).symbol };

  if (last_move_checked() and (piece != Piece_symbols::white_king and piece != Piece_symbols::black_king)
      and player != at(m_menaces.front()).player) {
    return manage_available_menace_interceptors(place, do_if_movable, do_if_edible);
  }

  auto perform = do_nothing_false;
  auto extra_condition = [this, &do_if_movable, &do_if_edible, &available_movement, &player](const Bearing b) {
    const bool end_loop{ false };
    const bool continue_loop{ true };
    if (at(b).empty()) {
      do_if_movable(b);
      available_movement = true;
      return continue_loop;
    }
    if (is_an_enemy_piece(player, b)) {
      do_if_edible(b);
      available_movement = true;
    }
    return end_loop;
  };

  if (is_partially_pinned) {
    switch (axis) {
    case Axis::x:
      iterate_from_to_and_perform(place, Direction::right, perform, extra_condition);
      iterate_from_to_and_perform(place, Direction::left, perform, extra_condition);
      break;
    case Axis::y:
      iterate_from_to_and_perform(place, Direction::top, perform, extra_condition);
      iterate_from_to_and_perform(place, Direction::bot, perform, extra_condition);
      break;
    case Axis::Q1_Q3_xy:
      iterate_from_to_and_perform(place, Direction::top_right, perform, extra_condition);
      iterate_from_to_and_perform(place, Direction::bot_left, perform, extra_condition);
      break;
    case Axis::Q2_Q4_xy:
      iterate_from_to_and_perform(place, Direction::top_left, perform, extra_condition);
      iterate_from_to_and_perform(place, Direction::bot_right, perform, extra_condition);
      break;

    default: break;
    }
    return available_movement;
  }

  switch (piece) {
  case Piece_symbols::white_king:
  case Piece_symbols::black_king:
    available_movement = evaluate_king_possibilities(place, do_if_movable, do_if_edible);
    break;

  case Piece_symbols::white_rook:
  case Piece_symbols::black_rook: straight_perform(place, perform, extra_condition); break;

  case Piece_symbols::white_bishop:
  case Piece_symbols::black_bishop: diagonal_perform(place, perform, extra_condition); break;

  case Piece_symbols::white_queen:
  case Piece_symbols::black_queen:
    straight_perform(place, perform, extra_condition);
    diagonal_perform(place, perform, extra_condition);
    break;

  case Piece_symbols::white_knight:
  case Piece_symbols::black_knight:
    available_movement = evaluate_jump_possibilities(place, do_if_movable, do_if_edible);
    break;

  case Piece_symbols::white_pawn:
  case Piece_symbols::black_pawn:
    available_movement = evaluate_pawn_possibilities(place, do_if_movable, do_if_edible);
    break;

  case Piece_symbols::empty: return false;
  default: break;
  }

  return available_movement;
}

template<class Do_if_movable, class Do_if_edible>
bool Gameboard::evaluate_king_possibilities(const Bearing king_bearing,
  Do_if_movable do_if_movable,
  Do_if_edible do_if_edible)
{
  const Player my_player = at(king_bearing).player;

  const bool in_the_left_border = (king_bearing.x == 0);
  const bool in_the_right_border = (king_bearing.x == height - 1);
  const bool in_the_bot_border = (king_bearing.y == 0);
  const bool in_the_top_border = (king_bearing.y == width - 1);

  const unsigned x_limit = in_the_right_border ? 2 : 3;
  const unsigned y_limit = in_the_top_border ? 2 : 3;


  bool available_movement{ false };

  // TODO: make this code clearer
  for (unsigned x_it = in_the_left_border ? 1U : 0U; x_it < x_limit; x_it++) {
    Bearing end;
    end.x = king_bearing.x + x_it + -1U;

    for (unsigned y_it = in_the_bot_border ? 1U : 0U; y_it < y_limit; y_it++) {
      end.y = king_bearing.y + y_it - 1U;

      if (isMenaced(my_player, end)) { continue; }
      if (at(end).empty()) {
        do_if_movable(end);
        available_movement = true;
      } else if (is_an_enemy_piece(my_player, end) and not isMenaced(my_player, end)) {
        do_if_edible(end);
        available_movement = true;
        break;
      }
    }
  }

  if (evaluate_castlings(king_bearing, do_if_movable, my_player)) { available_movement = true; }

  return available_movement;
}

template<class Do_if_movable>
bool Gameboard::evaluate_castlings(const Bearing king_bearing, Do_if_movable do_if_movable, const Player my_player)
{
  bool available_movement{ false };

  bool available_queen_side_castling{ true };
  bool available_king_side_castling{ true };


  if (first_movement(king_bearing)) {
    Piece_symbols rook =
      at(king_bearing).player == Player::white ? Piece_symbols::white_rook : Piece_symbols::black_rook;

    const Bearing queen_rook_bearing{ 0U, king_bearing.y };
    if (at(queen_rook_bearing).symbol == rook and first_movement(queen_rook_bearing)) {
      for (Bearing b = { king_bearing.x - 1, king_bearing.y }; b.x != 0; b.x--) {
        if (not at(b).isFree or isMenaced(my_player, b)) {
          available_queen_side_castling = false;
          break;
        }
      }
    } else {
      available_queen_side_castling = false;
    }

    const Bearing king_rook_bearing{ 7U, king_bearing.y };
    // here is the bug
    if (at(king_rook_bearing).symbol == rook and first_movement(king_rook_bearing)) {
      for (Bearing b = { king_bearing.x + 1, king_bearing.y }; b.x < width - 1; b.x++) {
        if (not at(b).isFree or isMenaced(my_player, b)) {
          available_king_side_castling = false;
          break;
        }
      }
    } else {
      available_king_side_castling = false;
    }
  } else {
    available_king_side_castling = false;
    available_queen_side_castling = false;
  }

  if (available_king_side_castling) {
    available_movement = true;
    do_if_movable({ king_bearing.x - 2U, king_bearing.y });
  }

  if (available_queen_side_castling) {
    available_movement = true;
    do_if_movable({ king_bearing.x + 2U, king_bearing.y });
  }

  return available_movement;
}

template<class Do_if_movable, class Do_if_edible>
bool Gameboard::evaluate_jump_possibilities(const Bearing place, Do_if_movable do_if_movable, Do_if_edible do_if_edible)
{
  bool available_movement{ false };

  const Player my_player = at(place).player;
  auto condition = do_nothing_true;
  auto perform = [this, &my_player, &do_if_movable, &do_if_edible, &available_movement](const Bearing b) {
    if (at(b).empty()) {
      do_if_movable(b);
      available_movement = true;
    }
    if (is_an_enemy_piece(my_player, b)) {
      do_if_edible(b);
      available_movement = true;
    }
    return false;
  };

  perform_jumps(place, perform, condition);

  return available_movement;
}

template<class Do_if_movable, class Do_if_edible>
bool Gameboard::evaluate_pawn_possibilities(const Bearing place, Do_if_movable do_if_movable, Do_if_edible do_if_edible)
{
  bool available_movement{ false };
  const Player my_player = at(place).player;
  Piece_symbols enemy_pawn{};
  unsigned direction{};
  unsigned en_passant_y{};
  if (my_player == Player::white) {
    enemy_pawn = Piece_symbols::black_pawn;
    direction = +1;
    en_passant_y = height - 1 - 3;
  } else {
    enemy_pawn = Piece_symbols::white_pawn;
    direction = -1U;
    en_passant_y = 3;
  }

  const bool was_the_last_move_a_pawn{ at(last_move.end).symbol == enemy_pawn };
  const bool en_passant_capture_available =
    was_the_last_move_a_pawn and (place.y == en_passant_y) and game::difference(place.x, last_move.end.x) == 1;

  if (en_passant_capture_available) {
    available_movement = true;
    const unsigned y_behind_the_enemy_pawn = last_move.end.y + direction;
    do_if_movable({ last_move.end.x, y_behind_the_enemy_pawn });
  }

  if (const bool normal_movement = at({ place.x, place.y + direction }).isFree; normal_movement) {
    available_movement = true;
    do_if_movable({ place.x, place.y + direction });

    const bool first_double_movement = first_movement(place) and at({ place.x, place.y + 2 * direction }).isFree;
    if (first_double_movement) { do_if_movable({ place.x, place.y + 2 * direction }); }
  }

  const bool checkable_left = place.x > 0;
  const Bearing left = { place.x - 1, place.y + direction };
  const bool left_capture = checkable_left and not at(left).empty() and at(left).player != my_player;
  if (left_capture) {
    do_if_edible(left);
    available_movement = true;
  }

  const bool checkable_right = place.x < width - 1;
  const Bearing right = { place.x + 1, place.y + direction };
  const bool right_capture = checkable_right and not at(right).empty() and at(right).player != my_player;
  if (right_capture) {
    do_if_edible(right);
    available_movement = true;
  }

  return available_movement;
}


template<class Do_if_menace>
bool Gameboard::pawn_menace(const Player player,
  const Piece_symbols enemy_pawn,
  const Bearing place,
  Do_if_menace do_if_menace)
{
  const auto [x, y] = place;

  // TODO: bug, no variable player
  if ((player == Player::white) and (y < height - 1)) {
    if (const Bearing top_left{ x - 1, y + 1 };
        (x > 0) and (at(top_left).symbol == enemy_pawn) and do_if_menace(top_left)) {
      return true;
    }
    if (const Bearing top_right{ x + 1, y + 1 };
        (x < width - 1) and (at(top_right).symbol == enemy_pawn) and do_if_menace(top_right)) {
      return true;
    }
  } else if (y > 0) {
    if (const Bearing bot_left{ x - 1, y - 1 };
        (x > 0) and (at(bot_left).symbol == enemy_pawn) and do_if_menace(bot_left)) {
      return true;
    }
    if (const Bearing bot_right{ x + 1, y - 1 };
        (x > width - 1) and (at(bot_right).symbol == enemy_pawn) and do_if_menace(bot_right)) {
      return true;
    }
  }
  return false;
}


/*****************************************************************************
 * Chess abstract moves
 ****************************************************************************/

template<class Functor, class Extra_condition>
bool Gameboard::straight_menace(const Bearing place, Functor perform, Extra_condition extra_condition)
{
  using Direction = Gameboard::Direction;
  if (iterate_from_to_and_perform(place, Direction::right, perform, extra_condition)) { return true; }
  if (iterate_from_to_and_perform(place, Direction::top, perform, extra_condition)) { return true; }
  if (iterate_from_to_and_perform(place, Direction::left, perform, extra_condition)) { return true; }
  if (iterate_from_to_and_perform(place, Direction::bot, perform, extra_condition)) { return true; }
  return false;
}

template<class Functor, class Extra_condition>
void Gameboard::straight_perform(const Bearing place, Functor perform, Extra_condition extra_condition)
{
  using Direction = Gameboard::Direction;
  iterate_from_to_and_perform(place, Direction::right, perform, extra_condition);
  iterate_from_to_and_perform(place, Direction::top, perform, extra_condition);
  iterate_from_to_and_perform(place, Direction::left, perform, extra_condition);
  iterate_from_to_and_perform(place, Direction::bot, perform, extra_condition);
}

template<class Functor, class Extra_condition>
bool Gameboard::diagonal_menace(const Bearing place, Functor perform, Extra_condition extra_condition)
{
  using Direction = Gameboard::Direction;
  if (iterate_from_to_and_perform(place, Direction::top_right, perform, extra_condition)) { return true; }
  if (iterate_from_to_and_perform(place, Direction::top_left, perform, extra_condition)) { return true; }
  if (iterate_from_to_and_perform(place, Direction::bot_left, perform, extra_condition)) { return true; }
  if (iterate_from_to_and_perform(place, Direction::bot_right, perform, extra_condition)) { return true; }
  return false;
}

template<class Functor, class Extra_condition>
void Gameboard::diagonal_perform(const Bearing place, Functor perform, Extra_condition extra_condition)
{
  using Direction = Gameboard::Direction;
  iterate_from_to_and_perform(place, Direction::top_right, perform, extra_condition);
  iterate_from_to_and_perform(place, Direction::top_left, perform, extra_condition);
  iterate_from_to_and_perform(place, Direction::bot_left, perform, extra_condition);
  iterate_from_to_and_perform(place, Direction::bot_right, perform, extra_condition);
}


template<class Functor, class Condition>
bool Gameboard::perform_jumps(const Bearing place, Functor perform, Condition condition) const
{
  const auto [x, y] = place;
  using cBearing = const Bearing;
  // consider that the expresion (false and true) returns 'false' without evaluating the 'true' after 'and'.
  if (cBearing b{ x - 2, y - 1 }; (x > 1) and (y > 0) and condition(b) and perform(b)) { return true; }
  if (cBearing b{ x - 2, y + 1 }; (x > 1) and (y < 7) and condition(b) and perform(b)) { return true; }
  if (cBearing b{ x - 1, y - 2 }; (x > 0) and (y > 1) and condition(b) and perform(b)) { return true; }
  if (cBearing b{ x + 1, y - 2 }; (x < 7) and (y > 1) and condition(b) and perform(b)) { return true; }
  if (cBearing b{ x + 2, y - 1 }; (x < 6) and (y > 0) and condition(b) and perform(b)) { return true; }
  if (cBearing b{ x + 2, y + 1 }; (x < 6) and (y < 7) and condition(b) and perform(b)) { return true; }
  if (cBearing b{ x - 1, y + 2 }; (x > 0) and (y < 6) and condition(b) and perform(b)) { return true; }
  if (cBearing b{ x + 1, y + 2 }; (x < 7) and (y < 6) and condition(b) and perform(b)) { return true; }
  return false;
}


template<class Functor>
bool Gameboard::iterate_from_to_and_perform(Bearing place, Direction Direction, Functor perform) const
{
  return iterate_from_to_and_perform(place, Direction, perform, do_nothing_true);
}

template<class Functor, class Extra_condition>
bool Gameboard::iterate_from_to_and_perform(Bearing place,
  Direction Direction,
  Functor perform,
  Extra_condition extra_condition) const
{
  const auto [x, y] = place;
  switch (Direction) {
  case right:
    for (Bearing b = { x + 1, y }; b.x < width and extra_condition(b); b.x++) {
      if (perform(b)) { return true; }
    }
    break;
  case top:
    for (Bearing b = { x, y + 1 }; b.y < height and extra_condition(b); b.y++) {
      if (perform(b)) { return true; }
    }
    break;
  case left:
    for (Bearing b = { x - 1, y }; b.x != -1U and extra_condition(b); b.x--) {
      if (perform(b)) { return true; }
    }
    break;
  case bot:
    for (Bearing b = { x, y - 1 }; b.y != -1U and extra_condition(b); b.y--) {
      if (perform(b)) { return true; }
    }
    break;
  case top_right:
    for (Bearing b = { x + 1, y + 1 }; b.x < width and b.y < height and extra_condition(b); ++b) {
      if (perform(b)) { return true; }
    }
    break;
  case top_left:
    for (Bearing b = { x - 1, y + 1 }; b.x != -1U and b.y < height and extra_condition(b); b.x--, b.y++) {
      if (perform(b)) { return true; }
    }
    break;
  case bot_left:
    for (Bearing b = { x - 1, y - 1 }; b.x != -1U and b.y != -1U and extra_condition(b); b--) {
      if (perform(b)) { return true; }
    }
    break;
  case bot_right:
    for (Bearing b = { x + 1, y - 1 }; b.x < width and b.y != -1U and extra_condition(b); b.x++, b.y--) {
      if (perform(b)) { return true; }
    }
    break;
  default: break;
  }
  return false;
}
