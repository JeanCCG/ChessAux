#include "Gameboard.hpp"
#include "../Interface/Interface.hpp"
#include <bitset>
#include <iostream>


using namespace std;


Gameboard::Gameboard(const Game_settings &game_settings)
{
  for (unsigned x = 0; x < width; x++) {
    for (unsigned y = 0; y < height; y++) {
      at({ x, y }) = Piece{ static_cast<Piece_symbols>(game_settings.board[x][y]) };
      if (at({ x, y }).symbol == Piece_symbols::white_king) {
        white_king_bearing = { x, y };
      } else if (at({ x, y }).symbol == Piece_symbols::black_king) {
        black_king_bearing = { x, y };
      }
    }
  }
}

void Gameboard::move_or_capture(const Move t_move)
{
  if (not at(t_move.end).isFree) {
    capture(t_move);
  } else {
    move(t_move);
  }
}

bool Gameboard::is_absolutely_pinned(const Bearing place, bool &is_partially_pinned, Axis &axis)
{
  const Player my_player = at(place).player;
  const Piece_symbols place_s = at(place).symbol;
  const Piece_set enemy{ not my_player };
  const Piece_set mine{ my_player };

  Bearing my_king_bearing{};
  if (my_player == Player::white) {
    my_king_bearing = white_king_bearing;
  } else {
    my_king_bearing = black_king_bearing;
  }

  auto perform_factory = [this, &place_s, &my_player, &is_partially_pinned](
                           const vector<Piece_symbols> &enemy_symbols, const vector<Piece_symbols> &exceptions) {
    return [this, &place_s, &my_player, &enemy_symbols, &exceptions, &is_partially_pinned](const Bearing b) {
      if (at(b).empty()) { return false; }

      auto equal_to_b = [this, &b](const Piece_symbols s) {
        return at(b).symbol == s;
      };

      if (is_an_enemy_piece(my_player, b) and any_of(enemy_symbols.begin(), enemy_symbols.end(), equal_to_b)) {
        auto equal_to_place = [&place_s](const Piece_symbols s) {
          return place_s == s;
        };

        if (any_of(exceptions.begin(), exceptions.end(), equal_to_place)) {
          is_partially_pinned = true;
          return false;
        } else {
          is_partially_pinned = false;
          return true;
        }
      }
      return false;
    };
  };

  const vector<Piece_symbols> enemy_rook_queen{ enemy.rook, enemy.queen };
  const vector<Piece_symbols> mine_rook_queen{ mine.rook, mine.queen };

  Direction direction{};
  if (place.x == my_king_bearing.x) {
    if (place.y > my_king_bearing.y) {
      direction = Direction::top;
    } else {
      direction = Direction::bot;
    }
    axis = Axis::y;
    return iterate_from_to_and_perform(place, direction, perform_factory(enemy_rook_queen, mine_rook_queen));
  }

  if (place.y == my_king_bearing.y) {
    if (place.x < my_king_bearing.x) {
      direction = Direction::right;
    } else {
      direction = Direction::left;
    }

    axis = Axis::x;
    return iterate_from_to_and_perform(place, direction, perform_factory(enemy_rook_queen, mine_rook_queen));
  }

  // check diagonals
  const unsigned x_d = place.x - my_king_bearing.x;
  const unsigned y_d = place.y - my_king_bearing.y;

  const vector<Piece_symbols> enemy_bishop_queen{ enemy.bishop, enemy.queen };
  const vector<Piece_symbols> mine_bishop_queen{ mine.bishop, mine.queen };

  if (x_d == y_d) {// top_right or bot_left
    if (place.x > my_king_bearing.x) {
      direction = Direction::top_right;
    } else {
      direction = Direction::top_left;
    }

    axis = Axis::Q1_Q3_xy;
    return iterate_from_to_and_perform(place, direction, perform_factory(enemy_bishop_queen, mine_bishop_queen));
  }

  if (x_d + y_d == 0) {// top_left or bot_right
    if (y_d > x_d) {
      direction = Direction::bot_right;
    } else {
      direction = Direction::top_left;
    }

    axis = Axis::Q2_Q4_xy;
    return iterate_from_to_and_perform(place, direction, perform_factory(enemy_bishop_queen, mine_bishop_queen));
  }

  return false;
}

void Gameboard::move(const Move t_move)
{
  at(t_move.end).setPiece(at(t_move.start));
  at(t_move.start).setFree();
  last_move = t_move;
}

void Gameboard::capture(const Move t_move)
{
  if (at(t_move.start).player == Player::white) {// white
    white_score += at(t_move.start).points;
  } else {
    black_score += at(t_move.start).points;
  }
  at(t_move.end).setFree();
  move(t_move);
  last_move = t_move;
}

bool Gameboard::legal_diagonal(const Move t_move)
{
  const auto [start, end] = t_move;

  using game::difference;
  if (difference(start.x, end.x) != difference(start.y, end.y)) { return false; }
  // the loop checks all bearings between start and end, what if there is no values between?
  // perhaps this if can be erased;
  if ((difference(start.x, end.x) == 1) and (difference(start.y, end.y) == 1)) { return true; }

  Bearing direction;

  direction.x = (start.x < end.x) ? 1 : -1U;
  direction.y = (start.y < end.y) ? 1 : -1U;

  for (Bearing b = start + direction; (b.x != end.x) and (b.y != end.y); b += direction) {
    if (not at(b).empty()) { return false; }
  }

  return true;
}

bool Gameboard::legal_straight(Move t_move)
{
  const auto [start, end] = t_move;

  if (start.x == end.x) {// same x axis
    const Bearing step = (start.y < end.y) ? Bearing{ 0U, 1 } : Bearing{ 0U, -1U };
    // clang disregards input validation, start != end
    for (Bearing b = start + step; b.y != end.y; b += step) {// NOSONAR // NOLINT
      if (not at(b).empty()) { return false; }
    }
    return true;
  }

  if (start.y == end.y) {// same y axis
    const Bearing step = (start.x < end.x) ? Bearing{ 1U, 0U } : Bearing{ -1U, 0U };
    // clang disregards input validation, start != end
    for (Bearing b = start + step; b.x != end.x; b += step) {// NOSONAR // NOLINT
      if (not at(b).empty()) { return false; }
    }
    return true;
  }

  return false;
}

bool Gameboard::legal_jump(const Move t_move)
{
  const unsigned x_difference = game::difference(t_move.start.x, t_move.end.x);
  const unsigned y_difference = game::difference(t_move.start.y, t_move.end.y);

  return ((x_difference == 1) && (y_difference == 2)) || ((x_difference == 2) && (y_difference == 1));
}

bool Gameboard::legal_pawn(const Move t_move)
{
  const auto [start, end] = t_move;

  const Player my_player = at(start).player;
  unsigned direction{};
  unsigned first_movement_y{};
  Piece_symbols enemy_pawn{};

  if (my_player == Player::white) {
    direction = 1;
    enemy_pawn = Piece_symbols::black_pawn;
    first_movement_y = 1;
  } else {
    direction = -1U;
    enemy_pawn = Piece_symbols::white_pawn;
    first_movement_y = height - 2;
  }

  if (start.x == end.x and at(end).empty()) {
    // normal move;
    if (start.y + direction == end.y) { return true; }

    // first move;
    const bool one_step_forward_is_empty = at({ start.x, start.y + direction }).empty();
    if (start.y == first_movement_y and start.y + direction + direction == end.y and one_step_forward_is_empty) {
      return true;
    }
  }

  using game::difference;

  if (difference(start.x, end.x) == 1 and difference(start.y, end.y) == 1) {
    // diagonal capture
    if (not at(end).empty() and is_an_enemy_piece(my_player, end)) { return true; }

    // en passant
    const bool last_was_pawn_and_double_step =
      (at(last_move.end).symbol == enemy_pawn) and (difference(last_move.start.y, last_move.end.y) == 2);

    if (at(end).empty() and difference(last_move.end.x, start.x) == 1 and last_was_pawn_and_double_step) {
      return true;
    }
  }

  return false;
}

bool Gameboard::legal_king(const Move t_move)
{
  const auto [start, end] = t_move;
  const Player player = at(start).player;
  const unsigned x_distance = game::difference(start.x, end.x);
  const unsigned y_distance = game::difference(start.y, end.y);

  const bool normal_movement = x_distance <= 1 and y_distance <= 1;
  const bool castling_movement = x_distance == 2 and y_distance == 0;

  Bearing *my_king_bearing{};
  if (player == Player::white) {
    my_king_bearing = &white_king_bearing;
  } else {
    my_king_bearing = &black_king_bearing;
  }

  if (isMenaced(player, end)) { return false; }
  if (normal_movement) {
    *my_king_bearing = end;
    return true;
  }
  if (castling_movement) {// and not checked
    if (not first_movement(start)) { return false; }
    const Bearing king_rook_start{ 7U, start.y };
    const Bearing king_rook_end{ start.x + 1, start.y };
    const Piece_symbols my_rook = (player == Player::white) ? Piece_symbols::white_rook : Piece_symbols::black_rook;
    const Bearing queen_rook_start{ 0U, start.y };
    const Bearing queen_rook_end{ start.x - 1, start.y };

    if (first_movement(king_rook_start) and (at(king_rook_start).symbol == my_rook) and (end.x < start.x)
        and at(king_rook_end).isFree) {
      if (isMenaced(player, king_rook_end)) { return false; }
      *my_king_bearing = end;
      return true;
    }
    if (first_movement(queen_rook_start) and (at(queen_rook_start).symbol == my_rook) and (start.y < end.y)
        and at(queen_rook_end).isFree) {
      if (isMenaced(player, queen_rook_end)) { return false; }
      *my_king_bearing = end;
      return true;
    }
  }
  return false;
};

bool Gameboard::validMovement(const Move t_move, const bool is_partially_pinned, const Axis axis)
{
  bool valid{ false };

  const auto [start, end] = t_move;
  if (is_partially_pinned) {
    switch (axis) {
    case Axis::x:
      if (start.y != end.y) { return false; }
      break;
    case Axis::y:
      if (start.x != end.x) { return false; }
      break;

    case Axis::Q1_Q3_xy: {
      const unsigned x_d = start.x - end.x;
      const unsigned y_d = start.y - end.y;
      if (x_d != y_d) { return false; }
    } break;
    case Axis::Q2_Q4_xy: {
      const unsigned x_d = start.x - end.x;
      const unsigned y_d = start.y - end.y;
      if (x_d + y_d != 0) { return false; }
    } break;

    default: break;
    }
  }

  switch (const auto piece{ at(start).symbol }; piece) {
  case Piece_symbols::black_king:
  case Piece_symbols::white_king: valid = legal_king(t_move); break;
  case Piece_symbols::black_queen:
  case Piece_symbols::white_queen: valid = (legal_diagonal(t_move) or legal_straight(t_move)); break;
  case Piece_symbols::black_rook:
  case Piece_symbols::white_rook: valid = legal_straight(t_move); break;
  case Piece_symbols::black_knight:
  case Piece_symbols::white_knight: valid = legal_jump(t_move); break;
  case Piece_symbols::black_bishop:
  case Piece_symbols::white_bishop: valid = legal_diagonal(t_move); break;
  case Piece_symbols::black_pawn:
  case Piece_symbols::white_pawn: valid = legal_pawn(t_move); break;
  default: break;
  }

  return valid;
}

void Gameboard::make_move(const Move t_move)
{
  const auto [start, end] = t_move;
  using game::difference;

  switch (at(t_move.start).symbol) {
  case Piece_symbols::black_king:
  case Piece_symbols::white_king: {
    Bearing &king_bearing = at(t_move.start).player == Player::white ? white_king_bearing : black_king_bearing;
    king_bearing = end;

    const bool king_side_castling = start.x < end.x and end.x - start.x == 2;
    if (king_side_castling) {
      move(t_move);
      const Bearing rook_start{ width - 1, start.y };
      const Bearing rook_end{ start.x + 1, start.y };
      move({ rook_start, rook_end });
      return;
    }

    const bool queen_side_castling = end.x < start.x and start.x - end.x == 2;
    if (queen_side_castling) {
      move(t_move);
      const Bearing rook_start{ 0U, start.y };
      const Bearing rook_end{ start.x - 1, start.y };
      move({ rook_start, rook_end });
      return;
    }

    // normal move;
    move_or_capture(t_move);
  } break;
  case Piece_symbols::black_pawn:
  case Piece_symbols::white_pawn: {
    const Player my_player = at(start).player;
    const unsigned direction = (my_player == Player::white) ? 1 : -1U;
    if (difference(start.x, end.x) == 1 and at(end).empty()) {
      const Bearing en_passant_enemy_pawn_bearing = { end.x, end.y - direction };
      capture({ start, en_passant_enemy_pawn_bearing });
      move({ en_passant_enemy_pawn_bearing, end });
      return;
    }
    // promotion? perhaps a promote piece function for the interface class would be a better option.
    move_or_capture(t_move);
  } break;

  default: move_or_capture(t_move); break;
  }
  // should I check whether the moved_piece checks the enemy king?
}

void Gameboard::undraw_king(const Bearing king_bearing)
{
  const bool inTheUpperBorder = king_bearing.x == 0;
  const bool inTheBottomBorder = king_bearing.x == height - 1;
  const bool inTheLeftBorder = king_bearing.y == 0;
  const bool inTheRightBorder = king_bearing.y == width - 1;
  bool available_queen_side_castling{ false };
  bool available_king_side_castling{ false };

  Bearing end{ 0U, 0U };
  unsigned iLimit = 3;
  unsigned jLimit = 3;
  if (inTheBottomBorder) { iLimit = 2; }
  if (inTheRightBorder) { jLimit = 2; }

  for (unsigned i = inTheUpperBorder ? 1 : 0; i < iLimit; i++) {
    end.x = king_bearing.x + (-1 + i);
    for (unsigned j = inTheLeftBorder ? 1 : 0; j < jLimit; j++) {
      end.y = king_bearing.y + (-1 + j);
      if (at(end).symbol == Piece_symbols::dot) { unDrawDot(end); }
    }
  }
  if (at(king_bearing).movements == 0) {
    if (slots[king_bearing.x][0].movements == 0) { available_queen_side_castling = true; }
    if (slots[king_bearing.x][width - 1].movements == 0) { available_king_side_castling = true; }
  }

  end.x = king_bearing.x;
  if (available_king_side_castling) {
    end.y = 2;
    if (at(end).symbol == Piece_symbols::dot) { unDrawDot(end); }
  }
  if (available_queen_side_castling) {
    end.y = width - 2;
    if (at(end).symbol == Piece_symbols::dot) { unDrawDot(end); }
  }
}

void Gameboard::undraw_white_pawn(const Bearing place)
{
  const Piece_symbols dot = Piece_symbols::dot;
  const auto [x, y] = place;

  const bool not_at_left_border = place.x > 0;
  if (const Bearing b{ x - 1, y + 1 }; not_at_left_border and at(b).symbol == dot) { unDrawDot(b); }

  const bool not_at_right_border = x < width - 1;
  if (const Bearing b{ x + 1, y + 1 }; not_at_right_border and at(b).symbol == dot) { unDrawDot(b); }

  if (const Bearing b{ x, y + 1 }; at(b).symbol == dot) { unDrawDot(b); }

  const bool is_two_steps_from_top_border = y < height - 1;
  if (const Bearing b{ x, y + 2 }; is_two_steps_from_top_border and at(b).symbol == dot) { unDrawDot(b); }
}
void Gameboard::undraw_black_pawn(const Bearing place)
{
  const Piece_symbols dot = Piece_symbols::dot;

  const bool not_at_left_border = place.x > 0;
  if (const Bearing b{ place.x - 1, place.y - 1 }; not_at_left_border and at(b).symbol == dot) { unDrawDot(b); }

  const bool not_at_right_border = place.x < width - 1;
  if (const Bearing b{ place.x + 1, place.y - 1 }; not_at_right_border and at(b).symbol == dot) { unDrawDot(b); }

  if (const Bearing b{ place.x, place.y - 1 }; at(b).symbol == dot) { unDrawDot(b); }

  const bool is_two_steps_from_top_border = place.y >= 2;
  if (const Bearing b{ place.x, place.y - 2 }; is_two_steps_from_top_border and at(b).symbol == dot) { unDrawDot(b); }
}

bool Gameboard::draw_jumps(const Bearing place)
{
  bool available_movement{ false };

  auto draw_dot_and_set_true = [this, &available_movement](const Bearing b) {
    drawDot(b);
    available_movement = true;
    return false;
  };
  auto condition = [this](const Bearing b) {
    return at(b).empty();
  };

  perform_jumps(place, draw_dot_and_set_true, condition);

  return available_movement;
}

void Gameboard::undraw_jumps(const Bearing place)
{
  auto perform = [this](const Bearing b) {
    unDrawDot(b);
    return false;
  };
  auto condition = [this](const Bearing b) {
    return at(b).symbol == Piece_symbols::dot;
  };
  perform_jumps(place, perform, condition);
}

void Gameboard::undraw_piece_possibilities(const Bearing place, const Piece piece)
{
  const bool not_a_king = piece.symbol != Piece_symbols::white_king and piece.symbol != Piece_symbols::black_king;
  if (last_move_checked() and not_a_king) {
    const Piece_symbols s{ at(m_menaces.front()).symbol };
    const bool knight_case =
      m_menaces.size() == 1 and (s == Piece_symbols::black_knight or s == Piece_symbols::white_knight);

    auto perform = [&](const Bearing b) {
      if (at(b).empty()) { unDrawDot(b); }
    };

    if (knight_case) {
      knight_interceptors.for_each(perform);
    } else {
      interceptor_map[place].for_each(perform);
    }
  }

  auto perform = [this](const Bearing b) {
    unDrawDot(b);
    return false;
  };

  auto extra_condition = [this](const Bearing b) {
    return at(b).symbol == Piece_symbols::dot;
  };

  switch (piece.symbol) {
  case Piece_symbols::white_king:
  case Piece_symbols::black_king: undraw_king(place); break;

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
  case Piece_symbols::black_knight: undraw_jumps(place); break;

  case Piece_symbols::white_pawn: undraw_white_pawn(place); break;
  case Piece_symbols::black_pawn: undraw_black_pawn(place); break;

  default: break;
  }
}

bool Gameboard::pawn_menace(const Player player, const Piece_symbols enemy_pawn, const Bearing place)
{
  return pawn_menace(player, enemy_pawn, place, do_nothing_true);
}

bool Gameboard::king_menace(const Bearing enemy_king_b, const Bearing place)
{
  return game::difference(place.x, enemy_king_b.x) <= 1 and game::difference(place.y, enemy_king_b.y) <= 1;
}

Game_result Gameboard::check_end_conditions()
{
  m_last_move_checked = false;
  clear_stack_vars();

  // last: l_
  // other: o_
  const Player l_player = at(last_move.end).player;

  Player o_player{};
  Bearing o_king_bearing{};
  Game_result l_checkmates_o{};
  int *l_score{ nullptr };

  if (l_player == Player::white) {
    o_player = Player::black;
    o_king_bearing = black_king_bearing;
    l_checkmates_o = Game_result::white_wins;
    l_score = &white_score;
  } else {
    o_king_bearing = white_king_bearing;
    o_player = Player::white;
    l_checkmates_o = Game_result::black_wins;
    l_score = &black_score;
  }

  // this might save some operations
  // m_last_move_checked = king_is_menaced(o_player, o_king_bearing);
  if (king_is_menaced(o_player, o_king_bearing)) {
    bool checkmate{ false };
    m_last_move_checked = true;

    // here is the bug, the king is able to eat, however it is not considered in the menace interceptor
    //
    const bool can_not_block_or_kill = not available_menace_interceptor(o_king_bearing);
    if (m_menaces.size() >= 2 and not available_movement_at(o_king_bearing)) {// flee
      checkmate = true;
    }
    if ((m_menaces.size() < 2 and can_not_block_or_kill) and not available_movement_at(o_king_bearing)) {// flee
      // if king is able to intercept: is not checkmate else is checkmate
      const unsigned x_distance = game::difference(o_king_bearing.x, menaces().front().x);
      const unsigned y_distance = game::difference(o_king_bearing.y, menaces().front().y);

      const bool king_is_not_able_to_intercept = not(x_distance <= 1 and y_distance <= 1);
      if (king_is_not_able_to_intercept) {
        checkmate = true;
      } else {
        return Game_result::no_results_yet;
      }
    }

    // if ((m_menaces.size() >= 2 or not available_menace_interceptor(o_king_bearing))
    //     and not available_movement_at(o_king_bearing)) {// flee
    //   checkmate = true;
    // }

    if (checkmate) {
      *l_score += 1000;
      return l_checkmates_o;
    }
  }

  if (not available_movement_for_player(o_player)) { return Game_result::stale_mate; }
  // TODO: insufficient material

  return Game_result::no_results_yet;
}

unsigned counter = 0;

bool Gameboard::available_movement_for_player(const Player player)
{
  for (unsigned x = 0; x < width; x++) {
    for (unsigned y = 0; y < height; y++) {
      const Bearing end{ x, y };
      if (at(end).empty() or at(end).player != player) { continue; }
      counter++;
      if (available_movement_at(end)) { return true; }
    }
  }
  return false;
}

bool Gameboard::draw_piece_possibilities(const Bearing place, const bool is_partially_pinned, const Axis axis)
{
  return available_movement_at(
    place,
    [this](const Bearing b) {
      drawDot(b);
    },
    do_nothing,
    is_partially_pinned,
    axis);
}

bool Gameboard::king_is_menaced(const Player player, const Bearing place)
{
  bool is_menaced{ false };
  m_menaces.clear();

  using std::vector;
  Piece_set enemy{ not player };

  auto perform_factory = [this, &player, &is_menaced](const vector<Piece_symbols> &symbols) {
    return [this, &player, &symbols, &is_menaced](const Bearing b) {
      const bool m_continue{ false };
      const bool m_break{ true };
      if (at(b).empty()) { return m_continue; }

      if (is_an_enemy_piece(player, b) and any_of(symbols.begin(), symbols.end(), [&](Piece_symbols p_s) {
            return at(b).symbol == p_s;
          })) {
        is_menaced = true;
        m_menaces.push_back(b);
        return m_break;
      }

      return m_continue;
    };
  };
  auto until_is_my_piece = [this, &player](const Bearing b) {
    const bool m_break_loop{ false };
    const bool m_continue_loop{ true };
    if (at(b).player == player) { return m_break_loop; }
    return m_continue_loop;
  };

  const std::vector<Piece_symbols> rook_queen{ enemy.rook, enemy.queen };
  straight_menace(place, perform_factory(rook_queen), until_is_my_piece);
  const std::vector<Piece_symbols> bishop_queen{ enemy.bishop, enemy.queen };
  diagonal_menace(place, perform_factory(bishop_queen), until_is_my_piece);

  auto jump_condition = [this, &enemy, &is_menaced](const Bearing b) {
    const bool m_continue{ false };
    if (at(b).symbol == enemy.knight) {
      is_menaced = true;
      m_menaces.push_back(b);
    }
    return m_continue;
  };

  perform_jumps(place, do_nothing_false, jump_condition);

  pawn_menace(player, enemy.pawn, place);

  return is_menaced;
}

bool Gameboard::isMenaced(const Player my_player, const Bearing place)
{
  using std::vector;

  const Piece_set enemy{ not my_player };
  Piece_symbols my_king{};
  Bearing enemy_king_bearings{};

  if (my_player == Player::white) {
    my_king = Piece_symbols::white_king;
    enemy_king_bearings = black_king_bearing;
  } else {
    my_king = Piece_symbols::black_king;
    enemy_king_bearings = white_king_bearing;
  }


  auto while_not_mine_expect_my_king = [this, &my_king, &my_player](const Bearing b) {
    return at(b).empty() or at(b).player != my_player or at(b).symbol == my_king;
  };

  const std::vector<Piece_symbols> rook_queen{ enemy.rook, enemy.queen };

  auto perform_factory = [this, &my_player, &my_king](const vector<Piece_symbols> &symbols) {
    return [this, &my_player, &symbols, &my_king](const Bearing b) {
      // true: return true;
      const bool m_continue{ false };
      if (at(b).empty() or at(b).symbol == my_king) { return m_continue; }

      if (is_an_enemy_piece(my_player, b) and any_of(symbols.begin(), symbols.end(), [&](Piece_symbols p_s) {
            return at(b).symbol == p_s;
          })) {
        return true;
      }
      return m_continue;
    };
  };

  if (straight_menace(place, perform_factory(rook_queen), while_not_mine_expect_my_king)) { return true; }

  if (pawn_menace(my_player, enemy.pawn, place)) { return true; }
  if (king_menace(enemy_king_bearings, place)) { return true; }


  const std::vector<Piece_symbols> bishop_queen{ enemy.bishop, enemy.queen };
  if (diagonal_menace(place, perform_factory(bishop_queen), while_not_mine_expect_my_king)) { return true; }

  auto jump_condition = [this, &enemy](const Bearing b) {
    return at(b).symbol == enemy.knight;
  };

  return perform_jumps(place, do_nothing_true, jump_condition);
}

bool Gameboard::start_able_to_intercept(const Move move)
{
  const auto [start, end] = move;
  // there is at least one menace
  const Piece_symbols s{ at(m_menaces.front()).symbol };
  const bool knight_case =
    m_menaces.size() == 1 and (s == Piece_symbols::black_knight or s == Piece_symbols::white_knight);
  if (knight_case) { return knight_interceptors.contains(start); }

  return not interceptor_map[start].empty();
}

bool Gameboard::end_able_to_intercept(const Move move)
{
  const auto [start, end] = move;
  // there is at least one menace
  const Piece_symbols s{ at(m_menaces.front()).symbol };
  const bool knight_case =
    m_menaces.size() == 1 and (s == Piece_symbols::black_knight or s == Piece_symbols::white_knight);

  if (knight_case) { return m_menaces.front() == end; }
  return interceptor_map[start].contains(end);
}

bool Gameboard::available_knight_interceptor(const Player interceptor_p, const Bearing place)
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

bool Gameboard::is_interceptable(const Player interceptor_p, const Bearing place)
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

      if (at(b).player == interceptor_p and any_of(symbols.begin(), symbols.end(), [&](Piece_symbols p_s) {
            return at(b).symbol == p_s;
          })) {
        available_interceptor = true;
        //! this allocated the memory
        interceptor_map[b].append(place);
        return m_break;
      }

      return m_continue;
    };
  };

  straight_menace(place, perform_factory(rook_queen));
  diagonal_menace(place, perform_factory(bishop_queen));

  auto jump_condition = [this, &place, &interceptor, &available_interceptor](const Bearing b) {
    const bool m_continue{ false };
    if (at(b).symbol == interceptor.knight) {
      interceptor_map[b].append(place);
      available_interceptor = true;
    }
    return m_continue;
  };
  perform_jumps(place, do_nothing_false, jump_condition);

  if (pawn_intercepts(interceptor_p, place)) { available_interceptor = true; }

  return available_interceptor;
}

bool Gameboard::pawn_intercepts(const Player interceptor_p, const Bearing place)
{
  bool available_interceptor{ false };
  Piece_symbols interceptor_pawn;
  unsigned direction;
  bool y_condition;
  if (interceptor_p == Player::white) {
    interceptor_pawn = Piece_symbols::white_pawn;
    direction = -1U;
    y_condition = place.y > 1;
  } else {
    interceptor_pawn = Piece_symbols::black_pawn;
    direction = +1;
    y_condition = place.y < height - 1;
  }

  if (at(place).empty()) {// move to block the enemy attack.
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
  } else {// we assume there is an enemy piece if not empty cause it is the direction of the menace.
    const bool checkable_left = place.x > 0 and y_condition;
    const Bearing left_place = { place.x - 1, place.y + direction };
    const bool left_capture = checkable_left and is_an_enemy_piece(interceptor_p, left_place);
    if (left_capture) {
      interceptor_map[left_place].append(place);
      available_interceptor = true;
    }

    const bool checkable_right = place.x < width - 1 and y_condition;
    const Bearing right_place = { place.x + 1, place.y + direction };
    const bool right_capture = checkable_right and at(right_place).player != interceptor_p;
    if (right_capture) {
      interceptor_map[right_place].append(place);
      available_interceptor = true;
    }
  }
  return available_interceptor;
}


bool Gameboard::available_menace_interceptor(const Bearing place)
{
  bool available_interceptor{ false };

  Player interceptor_p = at(place).player;
  const Bearing menace{ m_menaces.front() };

  if (at(menace).symbol == Piece_symbols::white_knight or at(menace).symbol == Piece_symbols::black_knight) {
    return available_knight_interceptor(interceptor_p, menace);
  }
  Direction direction = get_direction(Move{ place, menace });
  // Direction direction = get_direction(Move{ .start = place, .end = menace });

  interceptor_map.reserve(64);

  bool reached_enemy{ false };

  auto perform = [this, &reached_enemy, &interceptor_p, &available_interceptor](const Bearing b) {
    const bool m_continue{ false };
    const bool m_break{ true };
    if (is_interceptable(interceptor_p, b)) { available_interceptor = true; }
    if (reached_enemy) { return m_break; }
    return m_continue;
  };

  auto extra_condition = [this, &reached_enemy, &interceptor_p](const Bearing b) {
    const bool m_continue_loop{ true };
    if (not at(b).empty() and at(b).player != interceptor_p) { reached_enemy = true; }
    return m_continue_loop;
  };

  iterate_from_to_and_perform(place, direction, perform, extra_condition);

  return available_interceptor;
}
