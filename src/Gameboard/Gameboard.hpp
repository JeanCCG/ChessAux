#ifndef __GAMEBOARD_H__
#define __GAMEBOARD_H__


// #include "../Game_settings/Game_settings.cpp"
#include "../Game_settings/Game_settings.hpp"
// #include "../Move/Move.cpp"
#include "../Move/Move.hpp"
// #include "../Piece/Piece.cpp"
#include "../Piece/Piece.hpp"

// #include "../Bearing/Bearing.cpp"

#include <vector>

void clean_screen();


const auto do_nothing = []([[maybe_unused]] const Bearing b) {};
const auto do_nothing_false = []([[maybe_unused]] const Bearing b) { return false; };
const auto do_nothing_true = []([[maybe_unused]] const Bearing b) { return true; };

namespace game {
inline unsigned difference(const unsigned lhs, const unsigned rhs) { return (lhs > rhs) ? lhs - rhs : rhs - lhs; }
inline void switch_player(Player &player) { player = (player == Player::white) ? Player::black : Player::white; }
}// namespace game

class Gameboard
{
public:
  Piece slots[8][8];
  unsigned width{ 8 };
  unsigned height{ 8 };

  Bearing white_king_bearing;
  Bearing black_king_bearing;
  int white_score = 0;
  int black_score = 0;

  // Move last_move{ { 0U, 0U }, { 0U, 0U } };
  Move last_move{ { 0U, 0U }, { 0U, 0U } };

  // Gameboard() {}
  explicit Gameboard(const Game_settings &game_settings);

  void move(const Move t_move);// end MUST BE FREE
  void capture(const Move t_move);
  void eat(const Move t_move) { capture(t_move); }
  bool validMovement(const Move t_move);
  bool piecePossibilities(Bearing place);// ONLY white need visual reference

  template<class Do_if_movable, class Do_if_edible>
  bool available_movement_at(const Bearing place,
    Do_if_movable do_if_movable = do_nothing,
    Do_if_edible do_if_edible = do_nothing);

  bool available_movement_at(const Bearing place) { return available_movement_at(place, do_nothing, do_nothing); }

  bool draw_piece_possibilities(const Bearing place)
  {
    return available_movement_at(
      place, [&](const Bearing b) { drawDot(b); }, do_nothing);
  };

  bool is_absolutely_pinned(const Bearing place);
  bool movable_piece(const Bearing place) { return !is_absolutely_pinned(place); }

  void undraw_piece_possibilities(const Bearing place, const Piece piece);
  void move_or_capture(const Move t_move);
  void make_move(const Move t_move);

  bool isMenaced(const Player player, const Bearing place);


  Piece &at(Bearing bearing) { return slots[bearing.x][bearing.y]; }
  Game_result check_end_conditions()
  {
    const Player last_move_player = at(last_move.end).player;

    Player the_other_player{};
    Bearing the_other_king_bearing{};
    Game_result last_move_checkmates_the_other{};
    if (last_move_player == Player::white) {
      the_other_player = Player::black;
      the_other_king_bearing = black_king_bearing;
      last_move_checkmates_the_other = Game_result::white_wins;
    } else {
      the_other_king_bearing = white_king_bearing;
      the_other_player = Player::white;
      last_move_checkmates_the_other = Game_result::black_wins;
    }

    if (not available_movement_at(the_other_king_bearing)) {
      if (isMenaced(the_other_player, the_other_king_bearing)) {
        return last_move_checkmates_the_other;
      } else {
        return Game_result::stale_mate;
      }
    }
    // check insufficient material;
    return Game_result::no_results_yet;
  }

private:
  enum Direction { top, bot, left, right, top_left, top_right, bot_left, bot_right };


  struct Bearing_it
  {
    Bearing value;
    Bearing step;
    unsigned limit{ 0 };
    Bearing_it(const Bearing init_value, const Bearing t_step, const int t_limit = 0)
      : value{ init_value }, step{ t_step }, limit{ static_cast<unsigned>(t_limit) }
    {}

    void next() { value += step; }
  };


  bool legal_pawn(const Move t_move);
  bool legal_diagonal(const Move t_move);
  bool legal_straight(const Move t_move);
  // bool legal_straight(const Move t_move)
  //   __attribute__((analyzer_noreturn));// NOLINT; clang disregards input validation, start != end
  bool legal_king(const Move t_move);
  static bool legal_jump(const Move t_move);

  void drawDot(const Bearing place) { at(place).symbol = Piece_symbols::dot; }
  void unDrawDot(const Bearing place) { at(place).symbol = Piece_symbols::empty; }
  bool is_an_enemy_piece(const Player player, const Bearing bearing) { return (at(bearing).player != player); }
  bool first_movement(const Bearing bearing) { return at(bearing).movements == 0; }

  template<class Do_if_movable, class Do_if_edible>
  bool evaluate_pawn_possibilities(const Bearing place, Do_if_movable do_if_movable, Do_if_edible do_if_edible);

  bool draw_pawn(const Bearing place);
  void undraw_pawn(const Bearing place);
  bool draw_jumps(const Bearing place);
  void undraw_jumps(const Bearing place);

  template<class Do_if_movable, class Do_if_edible>
  bool evaluate_jump_possibilities(const Bearing place, Do_if_movable do_if_movable, Do_if_edible do_if_edible);
  template<class Do_if_movable, class Do_if_edible>
  bool evaluate_king_possibilities(const Bearing king_bearing, Do_if_movable do_if_movable, Do_if_edible do_if_edible);

  void undraw_king(const Bearing king_bearing);

  template<class Functor, class Extra_condition>
  bool iterate_from_to_and_perform(Bearing place,
    Direction Direction,
    Functor perform,
    Extra_condition extra_condition = do_nothing_true);

  template<class Functor, class Condition>
  bool perform_jumps(const Bearing place, Functor perform, Condition condition);

  template<class Functor, class Extra_condition>
  bool straight_menace(const Bearing place, Functor perform, Extra_condition extra_condition)
  {
    if (iterate_from_to_and_perform(place, Direction::right, perform, extra_condition)) { return true; }
    if (iterate_from_to_and_perform(place, Direction::top, perform, extra_condition)) { return true; }
    if (iterate_from_to_and_perform(place, Direction::left, perform, extra_condition)) { return true; }
    if (iterate_from_to_and_perform(place, Direction::bot, perform, extra_condition)) { return true; }
    return false;
  }

  template<class Functor, class Extra_condition>
  bool diagonal_menace(const Bearing place, Functor perform, Extra_condition extra_condition)
  {
    if (iterate_from_to_and_perform(place, Direction::top_right, perform, extra_condition)) { return true; }
    if (iterate_from_to_and_perform(place, Direction::top_left, perform, extra_condition)) { return true; }
    if (iterate_from_to_and_perform(place, Direction::bot_left, perform, extra_condition)) { return true; }
    if (iterate_from_to_and_perform(place, Direction::bot_right, perform, extra_condition)) { return true; }
    return false;
  }


  bool pawn_menace(const Player player, const Bearing place);
};


template<class Functor, class Condition>
bool Gameboard::perform_jumps(const Bearing place, Functor perform, Condition condition)
{
  const auto [x, y] = place;
  // consider that the expresion (false and true) returns 'false' without evaluating the 'true' after 'and'.
  if (const Bearing b{ x - 2, y - 1 }; (x > 1) and (y > 0) and condition(b) and perform(b)) { return true; }
  if (const Bearing b{ x - 2, y + 1 }; (x > 1) and (y < 7) and condition(b) and perform(b)) { return true; }
  if (const Bearing b{ x - 1, y - 2 }; (x > 0) and (y > 1) and condition(b) and perform(b)) { return true; }
  if (const Bearing b{ x + 1, y - 2 }; (x < 7) and (y > 1) and condition(b) and perform(b)) { return true; }
  if (const Bearing b{ x + 2, y - 1 }; (x < 6) and (y > 0) and condition(b) and perform(b)) { return true; }
  if (const Bearing b{ x + 2, y + 1 }; (x < 6) and (y < 7) and condition(b) and perform(b)) { return true; }
  if (const Bearing b{ x - 1, y + 2 }; (x > 0) and (y < 6) and condition(b) and perform(b)) { return true; }
  if (const Bearing b{ x + 1, y + 2 }; (x < 7) and (y < 6) and condition(b) and perform(b)) { return true; }
  return false;
}

template<class Functor, class Extra_condition>
bool Gameboard::iterate_from_to_and_perform(Bearing place,
  Direction Direction,
  Functor perform,
  Extra_condition extra_condition)
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
    break;
    for (Bearing b = { x - 1, y }; b.x != -1U and extra_condition(b); b.x--) {
      if (perform(b)) { return true; }
    }
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

template<class Do_if_movable, class Do_if_edible>
bool Gameboard::available_movement_at(const Bearing place, Do_if_movable do_if_movable, Do_if_edible do_if_edible)
{
  bool available_movement{ false };
  const Player player = at(place).player;
  const Piece piece{ at(place) };

  auto perform = do_nothing_false;
  auto extra_condition = [&](const Bearing b) {
    if (at(b).empty()) {
      do_if_movable(b);
      available_movement = true;
      return true;
    }
    if (is_an_enemy_piece(player, b)) {
      do_if_edible(b);
      available_movement = true;
      return true;
    }
    return false;
  };

  const auto draw_diagonals = [&]() {
    iterate_from_to_and_perform(place, Direction::right, perform, extra_condition);
    iterate_from_to_and_perform(place, Direction::top, perform, extra_condition);
    iterate_from_to_and_perform(place, Direction::left, perform, extra_condition);
    iterate_from_to_and_perform(place, Direction::bot, perform, extra_condition);
  };
  const auto draw_lanes = [&]() {
    iterate_from_to_and_perform(place, Direction::top_right, perform, extra_condition);
    iterate_from_to_and_perform(place, Direction::top_left, perform, extra_condition);
    iterate_from_to_and_perform(place, Direction::bot_left, perform, extra_condition);
    iterate_from_to_and_perform(place, Direction::right, perform, extra_condition);
  };

  switch (piece.symbol) {
  case Piece_symbols::white_king:
  case Piece_symbols::black_king:
    available_movement = evaluate_king_possibilities(place, do_if_movable, do_if_edible);
    break;

  case Piece_symbols::white_rook:
  case Piece_symbols::black_rook: draw_lanes(); break;

  case Piece_symbols::white_bishop:
  case Piece_symbols::black_bishop: draw_diagonals(); break;

  case Piece_symbols::white_queen:
  case Piece_symbols::black_queen: draw_lanes(), draw_diagonals(); break;

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

  const unsigned x_limit = (in_the_right_border) ? 2 : 3;
  const unsigned y_limit = (in_the_top_border) ? 2 : 3;


  bool available_movement{ false };

  // TODO: make this code clearer
  for (unsigned x_it = (in_the_left_border) ? 1U : 0U; x_it < x_limit; x_it++) {
    Bearing end;
    end.x = king_bearing.x + x_it + -1;

    for (unsigned y_it = (in_the_bot_border) ? 1U : 0U; y_it < y_limit; y_it++) {
      end.y = king_bearing.y + y_it - 1;

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

  bool available_queen_side_castling{ true };
  bool available_king_side_castling{ true };
  if (first_movement(king_bearing)) {
    const Bearing king_rook_bearing{ 0U, king_bearing.y };
    const Bearing queen_rook_bearing{ 7U, king_bearing.y };

    if (first_movement(queen_rook_bearing)) {
      for (Bearing b = { king_bearing.x, king_bearing.y - 1 }; b.y != -1U; b.y--) {
        if (not at(b).isFree or isMenaced(my_player, b)) {
          available_queen_side_castling = false;
          break;
        }
      }
    }

    if (first_movement(king_rook_bearing)) {
      for (Bearing b = { king_bearing.x, king_bearing.y + 1 }; b.y < width - 1; b.y++) {
        if (not at(b).isFree) {
          available_king_side_castling = false;
          break;
        }
      }
    }
  }

  if (available_king_side_castling) {
    available_movement = true;
    do_if_movable({ king_bearing.x, 2 });
  }

  if (available_queen_side_castling) {
    available_movement = true;
    do_if_movable({ king_bearing.x, width - 2 });
  }

  return available_movement;
}

template<class Do_if_movable, class Do_if_edible>
bool Gameboard::evaluate_jump_possibilities(const Bearing place, Do_if_movable do_if_movable, Do_if_edible do_if_edible)
{
  bool available_movement{ false };

  const Player my_player = at(place).player;
  auto condition = do_nothing_true;
  auto perform = [&](const Bearing b) {
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
    enemy_pawn = white_pawn;
    direction = +1;
    en_passant_y = height;
  } else {
    enemy_pawn = black_pawn;
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

    const bool left_capture = is_an_enemy_piece(my_player, { place.x - 1, place.y + direction });
    if (left_capture) {
      do_if_edible({ place.x - 1, place.y + direction });
      available_movement = true;
    }

    const bool right_capture = is_an_enemy_piece(my_player, { place.x + 1, place.y + direction });
    if (right_capture) {
      do_if_edible({ place.x + 1, place.y + direction });
      available_movement = true;
    }
  }

  return available_movement;
}

#endif// __GAMEBOARD_H__
