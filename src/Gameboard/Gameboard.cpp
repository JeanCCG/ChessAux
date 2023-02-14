#include "Gameboard.hpp"
#include "../Interface/Interface.hpp"
// #include <cmath>
#include <bitset>
#include <iostream>

#include <algorithm>
// #include <ranges>

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
  if (!at(t_move.end).isFree) {
    capture(t_move);
  } else {
    move(t_move);
  }
}

bool Gameboard::is_absolutely_pinned(const Bearing place)
{
  const Player my_player = at(place).player;
  const Piece piece = at(place);
  const Bearing my_king_bearing = (my_player == Player::white) ? white_king_bearing : black_king_bearing;

  bool is_absolutely_pinned{ false };

  at(place).setFree();
  if (isMenaced(my_player, my_king_bearing)) { is_absolutely_pinned = true; }
  at(place) = piece;// put back the piece

  return is_absolutely_pinned;
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

//
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

bool Gameboard::isMenaced(const Player player, const Bearing place)
{
  Piece_symbols enemy_bishop{};
  Piece_symbols enemy_queen{};
  Piece_symbols enemy_rook{};
  Piece_symbols enemy_knight{};
  Piece_symbols my_king{};

  if (player == Player::white) {
    my_king = Piece_symbols::white_king;
    enemy_bishop = Piece_symbols::black_bishop;
    enemy_queen = Piece_symbols::black_queen;
    enemy_rook = Piece_symbols::black_rook;
    enemy_knight = Piece_symbols::black_knight;
  } else {
    my_king = Piece_symbols::black_king;
    enemy_bishop = Piece_symbols::white_bishop;
    enemy_queen = Piece_symbols::white_queen;
    enemy_rook = Piece_symbols::white_rook;
    enemy_knight = Piece_symbols::white_knight;
  }

  const std::vector<Piece_symbols> rook_or_queen{ enemy_rook, enemy_queen };

  auto perform_1 = [this, &player, &rook_or_queen](const Bearing b) {
    if (at(b).empty()) { return false; }
    if (is_an_enemy_piece(player, b) and any_of(rook_or_queen.begin(), rook_or_queen.end(), [&](Piece_symbols p_s) {
          return at(b).symbol == p_s;
        })) {
      return true;
    }
    return false;
  };
  auto extra_condition = [this, &player, &my_king](
                           const Bearing b) { return at(b).player == player and at(b).symbol != my_king; };
  if (straight_menace(place, perform_1, extra_condition)) { return true; }

  if (pawn_menace(player, place)) { return true; }

  const std::vector<Piece_symbols> bishop_or_queen{ enemy_bishop, enemy_queen };
  auto perform_2 = [this, &player, &bishop_or_queen](const Bearing b) {
    if (at(b).empty()) { return false; }
    if (is_an_enemy_piece(player, b) and any_of(bishop_or_queen.begin(), bishop_or_queen.end(), [&](Piece_symbols p_s) {
          return at(b).symbol == p_s;
        })) {
      return true;
    }
    return false;
  };
  if (diagonal_menace(place, perform_2, extra_condition)) { return true; }

  auto jump_condition = [this, &enemy_knight](const Bearing b) { return at(b).symbol == enemy_knight; };
  return perform_jumps(place, do_nothing_true, jump_condition);
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

  if (game::difference(start.x, end.x) == 1) {
    // diagonal capture
    if (not at(end).empty() and is_an_enemy_piece(my_player, end)) { return true; }
    // en passant
    const bool last_move_was_a_pawn_and_double_forward_step =
      (at(last_move.end).symbol == enemy_pawn) and (game::difference(last_move.start.y, last_move.end.y) == 2);
    if (at(end).empty() and last_move_was_a_pawn_and_double_forward_step) { return true; }
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

  if (isMenaced(player, end)) { return false; }
  if (normal_movement) {
    white_king_bearing = end;
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
      white_king_bearing = end;
      return true;
    }
    if (first_movement(queen_rook_start) and (at(queen_rook_start).symbol == my_rook) and (start.y < end.y)
        and at(queen_rook_end).isFree) {
      if (isMenaced(player, queen_rook_end)) { return false; }
      white_king_bearing = end;
      return true;
    }
  }
  return false;
};

bool Gameboard::validMovement(const Move t_move)
{
  bool valid{ false };

  const auto [start, end] = t_move;

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

bool Gameboard::draw_jumps(const Bearing place)// Knight Jumps
{
  bool available_movement{ false };

  auto draw_dot_and_set_true = [this, &available_movement](const Bearing b) {
    drawDot(b);
    available_movement = true;
    return false;
  };
  auto condition = [this](const Bearing b) { return at(b).empty(); };

  perform_jumps(place, draw_dot_and_set_true, condition);

  return available_movement;
}

void Gameboard::undraw_jumps(const Bearing place)
{
  auto perform = [this](const Bearing b) {
    unDrawDot(b);
    return false;
  };
  auto condition = [this](const Bearing b) { return at(b).symbol == Piece_symbols::dot; };
  perform_jumps(place, perform, condition);
}

void Gameboard::undraw_piece_possibilities(const Bearing place, const Piece piece)
{
  auto perform = [&](const Bearing b) {
    unDrawDot(b);
    return false;
  };
  auto extra_condition = [&](const Bearing b) { return at(b).symbol == Piece_symbols::dot; };

  const auto undraw_diagonals = [&]() {
    iterate_from_to_and_perform(place, Direction::right, perform, extra_condition);
    iterate_from_to_and_perform(place, Direction::top, perform, extra_condition);
    iterate_from_to_and_perform(place, Direction::left, perform, extra_condition);
    iterate_from_to_and_perform(place, Direction::bot, perform, extra_condition);
  };
  const auto undraw_lanes = [&]() {
    iterate_from_to_and_perform(place, Direction::top_right, perform, extra_condition);
    iterate_from_to_and_perform(place, Direction::top_left, perform, extra_condition);
    iterate_from_to_and_perform(place, Direction::bot_left, perform, extra_condition);
    iterate_from_to_and_perform(place, Direction::right, perform, extra_condition);
  };

  switch (piece.symbol) {
  case Piece_symbols::white_king:
  case Piece_symbols::black_king: undraw_king(place); break;

  case Piece_symbols::white_rook:
  case Piece_symbols::black_rook: undraw_lanes(); break;

  case Piece_symbols::white_bishop:
  case Piece_symbols::black_bishop: undraw_diagonals(); break;

  case Piece_symbols::white_queen:
  case Piece_symbols::black_queen:
    undraw_lanes();
    undraw_diagonals();
    break;

  case Piece_symbols::white_knight:
  case Piece_symbols::black_knight: undraw_jumps(place); break;

  case Piece_symbols::white_pawn: undraw_white_pawn(place); break;
  case Piece_symbols::black_pawn: undraw_black_pawn(place); break;

  default: break;
  }
}


bool Gameboard::pawn_menace(const Player player, const Bearing place)
{
  const auto [x, y] = place;
  const Piece_symbols enemy_pawn = (player == Player::white) ? Piece_symbols::black_pawn : Piece_symbols::white_pawn;
  // pawns
  if ((player == Player::white) and (x >= 1)) {
    if (const Bearing bottom_left{ x - 1, y - 1 }; (y > 0) and (at(bottom_left).symbol == enemy_pawn)) { return true; }
    if (const Bearing top_left{ x - 1, y + 1 }; (y < width) and (at(top_left).symbol == enemy_pawn)) { return true; }
  } else if (x < height - 1) {
    if (const Bearing bottom_right{ x + 1, y - 1 }; (y > 0) and (at(bottom_right).symbol == enemy_pawn)) {
      return true;
    }
    if (const Bearing top_right{ x + 1, y + 1 }; (y < width) and (at(top_right).symbol == enemy_pawn)) { return true; }
  }
  return false;
}


Game_result Gameboard::check_end_conditions()
{
  const Player last_move_player = at(last_move.end).player;

  Player the_other_player{};
  Bearing the_other_king_bearing{};
  Game_result last_move_checkmates_the_other{};
  int *last_move_score{ nullptr };
  if (last_move_player == Player::white) {
    the_other_player = Player::black;
    the_other_king_bearing = black_king_bearing;
    last_move_checkmates_the_other = Game_result::white_wins;
    last_move_score = &white_score;
  } else {
    the_other_king_bearing = white_king_bearing;
    the_other_player = Player::white;
    last_move_checkmates_the_other = Game_result::black_wins;
    last_move_score = &black_score;
  }

  if (not available_movement_at(the_other_king_bearing)) {
    if (isMenaced(the_other_player, the_other_king_bearing)) {
      *last_move_score += 1000;
      return last_move_checkmates_the_other;
    }
    if (not available_movement_for_player(the_other_player)) { return Game_result::stale_mate; }
  }
  // TODO: insufficient material

  return Game_result::no_results_yet;
}


bool Gameboard::available_movement_for_player(const Player player)
{
  for (unsigned x_it = 0; x_it < width; x_it++) {
    Bearing end;
    end.x = x_it;
    for (end.y = 0; end.y < height; end.y++) {
      if (at(end).player != player) { continue; }
      if (available_movement_at(end)) { return true; }
    }
  }
  return false;
}
