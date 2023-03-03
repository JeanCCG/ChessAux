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

  explicit Gameboard(const Game_settings &game_settings);
  // ~Gameboard() { clear_stack_vars(); }

  void move(const Move t_move);// end MUST BE FREE
  void capture(const Move t_move);
  void eat(const Move t_move) { capture(t_move); }
  bool validMovement(const Move t_move);

  template<class Do_if_movable, class Do_if_edible>
  bool available_movement_at(const Bearing place,
    Do_if_movable do_if_movable = do_nothing,
    Do_if_edible do_if_edible = do_nothing);

  bool available_movement_at(const Bearing place) { return available_movement_at(place, do_nothing, do_nothing); }

  bool available_movement_for_player(const Player player);

  bool draw_piece_possibilities(const Bearing place);

  bool is_absolutely_pinned(const Bearing place);
  bool movable_piece(const Bearing place) { return !is_absolutely_pinned(place); }

  void undraw_piece_possibilities(const Bearing place, const Piece piece);
  void move_or_capture(const Move t_move);
  void make_move(const Move t_move);

  bool isMenaced(const Player player, const Bearing place);

  bool king_is_menaced(const Player player, const Bearing place);

  Piece &at(Bearing bearing) { return slots[bearing.x][bearing.y]; }
  Game_result check_end_conditions();

  bool last_move_checked() const { return m_last_move_checked; }
  void set_last_move_checked(const bool value) { m_last_move_checked = value; }

  bool start_able_to_intercept(const Move move)// check key
  {
    const auto [start,end] = move;
    // there is at least one menace
    Piece_symbols s{ at(m_menaces.front()).symbol };
    const bool knight_case =
      m_menaces.size() == 1 and (s == Piece_symbols::black_knight or s == Piece_symbols::white_knight);
    if (knight_case) { return knight_interceptors.contains(start); }

    return not interceptor_map[start].empty();
  }

  bool end_able_to_intercept(const Move move)// check values
  {
    const auto [start, end] = move;
    // there is at least one menace
    Piece_symbols s{ at(m_menaces.front()).symbol };
    const bool knight_case =
      m_menaces.size() == 1 and (s == Piece_symbols::black_knight or s == Piece_symbols::white_knight);

    if (knight_case) { return m_menaces.front() == end; }
    return interceptor_map[start].contains(end);
  }

  // bool is_an_interceptor(const Bearing b) const
  // {
  //   if (m_menaces.size() >= 2) { return not interceptor_map[b].empty(); }
  // }

private:
  Bearing white_king_bearing;
  Bearing black_king_bearing;
  Piece slots[8][8];
  int &white_score = score.score[0];
  int &black_score = score.score[1];
  Move last_move{ { 0U, 0U }, { 0U, 0U } };

  bool m_last_move_checked{ false };

  Map interceptor_map{ nullptr };
  Page<16> knight_interceptors{ nullptr };

  std::vector<Bearing> m_menaces;

  enum Direction { right, top, left, bot, top_right, top_left, bot_left, bot_right };
  std::vector<Bearing> menaces() const { return m_menaces; }

  void clear_stack_vars()
  {
    knight_interceptors.clear();
    interceptor_map.clear();
  }

  auto get_number_of_menaces() const { return m_menaces.size(); }
  Piece_symbols get_menace_symbol() { return at(m_menaces.front()).symbol; }


  // Page<16>& get_available_knights(){}

  bool available_knight_interceptor(const Player interceptor_p, const Bearing place)
  {
    Piece_set interceptor{ interceptor_p };
    const std::vector<Piece_symbols> rook_queen{ interceptor.rook, interceptor.queen };
    const std::vector<Piece_symbols> bishop_queen{ interceptor.bishop, interceptor.queen };

    auto perform_factory = [this, &interceptor_p](const std::vector<Piece_symbols> &symbols) {
      return [this, &interceptor_p, &symbols](const Bearing b) {
        const bool m_continue{ false };
        const bool m_break{ true };
        if (at(b).empty()) { return m_continue; }

        if (is_an_enemy_piece(interceptor_p, b) and any_of(symbols.begin(), symbols.end(), [&](Piece_symbols p_s) {
              return at(b).symbol == p_s;
            })) {
          knight_interceptors.append(b);
          return m_break;
        }

        return m_continue;
      };
    };

    straight_menace(place, perform_factory(rook_queen));
    diagonal_menace(place, perform_factory(bishop_queen));

    auto jump_condition = [this, &interceptor](const Bearing b) {
      if (at(b).symbol == interceptor.knight) { knight_interceptors.append(b); }
      return true;
    };
    perform_jumps(place, do_nothing_false, jump_condition);
    // pawn

    auto do_if_menace = [this](const Bearing b) {
      knight_interceptors.append(b);
      return false;
    };

    pawn_menace(not interceptor_p, interceptor.pawn, place, do_if_menace);

    return not knight_interceptors.empty();
  }

  bool is_interceptable(const Player interceptor_p, const Bearing place)
  {
    bool available_interceptor{ false };
    Piece_set interceptor{ interceptor_p };

    const std::vector<Piece_symbols> rook_queen{ interceptor.rook, interceptor.queen };
    const std::vector<Piece_symbols> bishop_queen{ interceptor.bishop, interceptor.queen };

    auto perform_factory = [this, &interceptor_p, &place, &available_interceptor](
                             const std::vector<Piece_symbols> &symbols) {
      return [this, &interceptor_p, &place, &symbols, &available_interceptor](const Bearing b) {
        const bool m_continue{ false };
        const bool m_break{ true };
        if (at(b).empty()) { return m_continue; }

        if (is_an_enemy_piece(interceptor_p, b) and any_of(symbols.begin(), symbols.end(), [&](Piece_symbols p_s) {
              return at(b).symbol == p_s;
            })) {
          available_interceptor = true;
          interceptor_map[place].append(place);

          return m_break;
        }

        return m_continue;
      };
    };

    straight_menace(place, perform_factory(rook_queen));
    diagonal_menace(place, perform_factory(bishop_queen));
    auto jump_condition = [this, &interceptor, &available_interceptor](const Bearing b) {
      const bool m_continue{ false };
      if (at(b).symbol == interceptor.knight) { available_interceptor = true; }
      return m_continue;
    };
    perform_jumps(place, do_nothing_false, jump_condition);

    if (pawn_intercepts(interceptor_p, place)) { available_interceptor = true; }

    return available_interceptor;
  }

  bool pawn_intercepts(const Player interceptor_p, const Bearing place)
  {
    bool available_interceptor{ false };
    Piece_symbols interceptor_pawn{};
    unsigned direction{};
    if (interceptor_p == Player::white) {
      interceptor_pawn = Piece_symbols::white_pawn;
      direction = -1U;
    } else {
      interceptor_pawn = Piece_symbols::black_pawn;
      direction = +1;
    }

    if (at(place).empty()) {// move
      const Bearing one_behind{ place.x, place.y + direction };
      const Bearing two_behind{ place.x, place.y + direction + direction };
      const bool pawn_first_movement = first_movement(place) and at(two_behind).symbol == interceptor_pawn;
      if (const bool is_pawn = at(one_behind).symbol == interceptor_pawn; is_pawn) {
        available_interceptor = true;
        interceptor_map[one_behind].append(place);
      } else if (pawn_first_movement) {
        available_interceptor = true;
        interceptor_map[two_behind].append(place);
      }
    } else {// we assume that it's an enemy if not empty.
      const bool checkable_left = place.x > 0;
      const Bearing left_place = { place.x - 1, place.y + direction };
      const bool left_capture = checkable_left and is_an_enemy_piece(interceptor_p, left_place);
      if (left_capture) {
        interceptor_map[left_place].append(place);
        available_interceptor = true;
      }

      const bool checkable_right = place.x < width - 1;
      const Bearing right_place = { place.x + 1, place.y + direction };
      const bool right_capture = checkable_right and is_an_enemy_piece(interceptor_p, right_place);
      if (right_capture) {
        interceptor_map[right_place].append(place);
        available_interceptor = true;
      }
    }
    return available_interceptor;
  }


  bool available_menace_interceptor(const Bearing place)
  {
    bool available_interceptor{ false };

    Player enemy_player = at(place).player;
    const Bearing menace{ m_menaces.front() };

    if (at(menace).symbol == Piece_symbols::white_knight or at(menace).symbol == Piece_symbols::black_knight) {
      return available_knight_interceptor(enemy_player, menace);
    }

    Direction direction;

    if (place.x == menace.x) {
      if (place.y > menace.y) {
        direction = Direction::bot;
      } else {
        direction = Direction::top;
      }
    } else if (place.y == menace.y) {
      if (place.x < menace.x) {
        direction = Direction::right;
      } else {
        direction = Direction::left;
      }
    } else if (place.x < menace.x) {
      if (place.y < menace.y) {
        direction = Direction::top_right;
      } else {
        direction = Direction::bot_right;
      }
    } else {
      if (place.y < menace.y) {
        direction = Direction::top_left;
      } else {
        direction = Direction::bot_left;
      }
    }

    interceptor_map.reserve(64);

    auto perform = [this, &enemy_player, &available_interceptor](const Bearing b) {
      const bool m_continue{ false };
      const bool m_break{ true };

      if (is_interceptable(enemy_player, b)) { available_interceptor = true; }

      if (not at(b).empty() and at(b).player == enemy_player) { return m_break; }
      return m_continue;
    };

    iterate_from_to_and_perform(place, direction, perform);

    return available_interceptor;
  }

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
};

/*****************************************************************************
 * Template implementation
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
bool Gameboard::diagonal_menace(const Bearing place, Functor perform, Extra_condition extra_condition)
{
  using Direction = Gameboard::Direction;
  if (iterate_from_to_and_perform(place, Direction::top_right, perform, extra_condition)) { return true; }
  if (iterate_from_to_and_perform(place, Direction::top_left, perform, extra_condition)) { return true; }
  if (iterate_from_to_and_perform(place, Direction::bot_left, perform, extra_condition)) { return true; }
  if (iterate_from_to_and_perform(place, Direction::bot_right, perform, extra_condition)) { return true; }
  return false;
}


template<class Functor, class Condition>
bool Gameboard::perform_jumps(const Bearing place, Functor perform, Condition condition) const
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
    iterate_from_to_and_perform(place, Direction::top_right, perform, extra_condition);
    iterate_from_to_and_perform(place, Direction::top_left, perform, extra_condition);
    iterate_from_to_and_perform(place, Direction::bot_left, perform, extra_condition);
    iterate_from_to_and_perform(place, Direction::bot_right, perform, extra_condition);
  };

  const auto draw_lanes = [&]() {
    iterate_from_to_and_perform(place, Direction::right, perform, extra_condition);
    iterate_from_to_and_perform(place, Direction::top, perform, extra_condition);
    iterate_from_to_and_perform(place, Direction::left, perform, extra_condition);
    iterate_from_to_and_perform(place, Direction::bot, perform, extra_condition);
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
  case Piece_symbols::black_queen:
    draw_lanes();
    draw_diagonals();
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
    end.x = king_bearing.x + x_it + -1;

    for (unsigned y_it = in_the_bot_border ? 1U : 0U; y_it < y_limit; y_it++) {
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
  const bool left_capture = checkable_left and is_an_enemy_piece(my_player, { place.x - 1, place.y + direction });
  if (left_capture) {
    do_if_edible({ place.x - 1, place.y + direction });
    available_movement = true;
  }

  const bool checkable_right = place.x < width - 1;
  const bool right_capture = checkable_right and is_an_enemy_piece(my_player, { place.x + 1, place.y + direction });
  if (right_capture) {
    do_if_edible({ place.x + 1, place.y + direction });
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

  // FIXME
  // TODO
  // bug, no variable player
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

#endif// __GAMEBOARD_H__
