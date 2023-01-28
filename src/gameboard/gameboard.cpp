#include "gameboard.hpp"
#include "../Interface/Interface.hpp"
// #include <cmath>
#include <algorithm>
#include <bitset>
#include <iostream>

using namespace std;


Gameboard::Gameboard(const Game_settings &game_settings)
{
  for (unsigned x = 0; x < width; x++) {
    for (unsigned y = 0; y < height; y++) { slots[x][y] = Piece{ game_settings.board.at(x).at(y) }; }
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
  if ((difference(start.x, end.x) == 1) and (difference(start.y, end.y) == 1)) { return true; }

  Bearing direction;

  direction.x = (start.x < end.x) ? 1 : -1U;
  direction.y = (start.y < end.y) ? 1 : -1U;

  for (Bearing b = start + direction; (b.x != end.x) and (b.y != end.y); b += direction) {
    if (at(b).empty()) { return false; }
  }

  return true;
}

bool Gameboard::legal_straight(Move t_move)
{
  const auto [start, end] = t_move;

  if (start.x == end.x) {// same x axis
    const Bearing step = (start.y < end.y) ? Bearing{ 0U, 1 } : Bearing{ 0U, -1U };
    for (Bearing b = start + step; b.y != end.y; b += step) {
      if (not at(b).empty()) { return false; }
    }
    return true;
  }

  if (start.y == end.y) {// same y axis
    const Bearing step = (start.x < end.x) ? Bearing{ 1U, 0U } : Bearing{ -1U, 0U };
    for (Bearing b = start + step; b.x != end.x; b += step) {
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

  if (((x_difference != 1) or (y_difference != 2)) and ((x_difference != 2) or (y_difference != 1))) { return false; }

  move_or_capture(t_move);
  return true;
}

bool Gameboard::isMenaced(const Player player, const Bearing place)
{
  Piece_symbols enemy_bishop{};
  Piece_symbols enemy_queen{};
  Piece_symbols enemy_rook{};
  Piece_symbols enemy_knight{};
  Piece_symbols enemy_pawn{};
  Piece_symbols my_king{};

  if (player == Player::white) {
    my_king = Piece_symbols::white_king;
    enemy_bishop = Piece_symbols::black_bishop;
    enemy_queen = Piece_symbols::black_queen;
    enemy_rook = Piece_symbols::black_rook;
    enemy_knight = Piece_symbols::black_knight;
    enemy_pawn = Piece_symbols::black_knight;
  } else {
    my_king = Piece_symbols::black_king;
    enemy_pawn = Piece_symbols::white_bishop;
    enemy_bishop = Piece_symbols::white_bishop;
    enemy_queen = Piece_symbols::white_queen;
    enemy_rook = Piece_symbols::white_rook;
    enemy_knight = Piece_symbols::white_knight;
  }

  ////////////*
  //* straight
  ////////////*
  const std::vector<Piece_symbols> rook_or_queen{ enemy_rook, enemy_queen };
  auto perform_1 = [&](const Bearing b) {
    if (at(b).isFree) { return false; }
    if (is_an_enemy_piece(player, b)
        and any_of(rook_or_queen, [&](Piece_symbols p_s) { return at(b).symbol == p_s; })) {
      return true;
    }
  };
  auto extra_condition = [&](const Bearing b) { return at(b).player == player and at(b).symbol != my_king; };

  if (iterate_from_to_and_perform(place, Direction::right, perform_1, extra_condition)) { return true; }
  if (iterate_from_to_and_perform(place, Direction::top, perform_1, extra_condition)) { return true; }
  if (iterate_from_to_and_perform(place, Direction::left, perform_1, extra_condition)) { return true; }
  if (iterate_from_to_and_perform(place, Direction::bot, perform_1, extra_condition)) { return true; }

  const auto [x, y] = place;
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

  const std::vector<Piece_symbols> bishop_or_queen{ enemy_bishop, enemy_queen };
  auto perform_2 = [&](const Bearing b) {
    if (at(b).isFree) { return false; }
    if (is_an_enemy_piece(player, b)
        and any_of(bishop_or_queen, [&](Piece_symbols p_s) { return at(b).symbol == p_s; })) {
      return true;
    }
  };

  if (iterate_from_to_and_perform(place, Direction::top_right, perform_2, extra_condition)) { return true; }
  if (iterate_from_to_and_perform(place, Direction::top_left, perform_2, extra_condition)) { return true; }
  if (iterate_from_to_and_perform(place, Direction::bot_left, perform_2, extra_condition)) { return true; }
  if (iterate_from_to_and_perform(place, Direction::bot_right, perform_2, extra_condition)) { return true; }

  ////////////*
  //* Jumps
  ////////////*

  auto jump_condition = [&](const Bearing b) { return at(b).symbol == enemy_knight; };
  auto perform_3 = []([[maybe_unused]] const Bearing b) { return true; };

  return perform_jumps(place, perform_3, jump_condition);
}

bool Gameboard::legal_pawn(const Move t_move)
{
  const auto [start, end] = t_move;

  const Player my_player = at(start).player;
  Player enemy_player{};
  unsigned direction{};
  unsigned first_movement_y{};

  if (my_player == Player::white) {
    direction = 1;
    enemy_player = Player::black;
    first_movement_y = 1;
  } else {
    direction = -1U;
    enemy_player = Player::white;
    first_movement_y = height - 2;
  }

  using game::difference;

  // 0b binary number

  if (start.x == end.x and at(end).empty()) {
    if (start.y + direction == end.y) {
      move(t_move);
      return true;
    }

    const bool one_step_forward_is_empty = at({ start.x, start.y + direction }).empty();
    if (start.y == first_movement_y and one_step_forward_is_empty and start.y + direction + direction == end.y) {
      move(t_move);
      return true;
    }
  }

  if (difference(start.x, end.x) == 1) {
    // diagonal capture
    if (not at(end).empty() and is_an_enemy_piece(my_player, end)) {
      capture(t_move);
      return true;
    }
    // en passant
    if (at(end).empty() and last_move.end.y == first_movement_y + direction + direction) {
      capture({ t_move.start, last_move.end });
      move({ last_move.start, t_move.end });
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

  if (isMenaced(player, end)) { return false; }
  if (normal_movement) {
    move_or_capture(Move(t_move));
    white_king_bearing = end;
    return true;
  }
  if (castling_movement) {// and not checked
    if (not first_movement(start)) { return false; }
    const Bearing king_rook_start{ 7U, start.y };
    const Bearing king_rook_end{ start.x + 1, start.y };
    const Piece_symbols my_rook = (player == Player::white) ? white_rook : black_rook;
    const Bearing queen_rook_start{ 0U, start.y };
    const Bearing queen_rook_end{ start.x - 1, start.y };

    if (first_movement(king_rook_start) and (at(king_rook_start).symbol == my_rook) and (end.x < start.x)
        and at(king_rook_end).isFree) {
      if (isMenaced(player, king_rook_end)) { return false; }
      move(t_move);
      move({ king_rook_start, king_rook_end });
      white_king_bearing = end;
      return true;
    }
    if (first_movement(queen_rook_start) and (at(queen_rook_start).symbol == my_rook) and (start.y < end.y)
        and at(queen_rook_end).isFree) {
      if (isMenaced(player, queen_rook_end)) { return false; }
      move(t_move);
      move({ queen_rook_start, queen_rook_end });
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

  for (unsigned i = (inTheUpperBorder) ? 1 : 0; i < iLimit; i++) {
    end.x = king_bearing.x + (-1 + i);
    for (unsigned j = (inTheLeftBorder) ? 1 : 0; j < jLimit; j++) {
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


// bool Gameboard::draw_pawn(const Bearing place)
// {
//   bool available_movement{ false };
//   const Player my_player = at(place).player;
//   const Piece_symbols enemy_pawn = (my_player == Player::white) ? white_pawn : black_pawn;
//   const bool was_the_last_move_a_pawn{ at(last_move.end).symbol == enemy_pawn };
//   const unsigned direction = (my_player == Player::white) ? +1 : -1U;
//   const unsigned en_passant_y = (my_player == Player::white) ? height - 4 : 3;
//   const bool en_passant_capture_available =
//     was_the_last_move_a_pawn and (place.y == en_passant_y) and game::difference(place.x, last_move.end.x) == 1;

//   if (en_passant_capture_available) {
//     available_movement = true;
//     const unsigned y_behind_the_enemy_pawn = last_move.end.y + direction;
//     drawDot({ last_move.end.x, y_behind_the_enemy_pawn });
//   }

//   if (const bool normal_movement = at({ place.x, place.y + direction }).isFree; normal_movement) {
//     available_movement = true;
//     drawDot({ place.x, place.y + direction });
//     const bool first_double_movement = first_movement(place) and at({ place.x, place.y + 2 * direction }).isFree;
//     if (first_double_movement) { drawDot({ place.x, place.y + 2 * direction }); }
//   }

//   const bool left_capture = is_an_enemy_piece(my_player, { place.x - 1, place.y + direction });
//   if (left_capture) { available_movement = true; }

//   const bool right_capture = is_an_enemy_piece(my_player, { place.x + 1, place.y + direction });
//   if (right_capture) { available_movement = true; }

//   return available_movement;
// }

void Gameboard::undraw_pawn(const Bearing place)
{
  unsigned x = place.x - 1;
  const unsigned cas = place.x == 6 ? 2 : 1;
  for (unsigned i = 0; i < cas; i++) {
    const Bearing b{ x, place.y };
    if (at(b).symbol == Piece_symbols::dot) {
      unDrawDot(b);
      x--;
    }
    break;
  }
}

bool Gameboard::draw_jumps(const Bearing place)// Knight Jumps
{
  bool available_movement{ false };

  auto draw_dot_and_set_true = [&](const Bearing b) {
    drawDot(b);
    available_movement = true;
    return false;
  };
  auto condition = [&](const Bearing b) { return at(b).empty(); };

  perform_jumps(place, draw_dot_and_set_true, condition);

  return available_movement;
}

void Gameboard::undraw_jumps(const Bearing place)
{
  auto perform = [&](const Bearing b) {
    unDrawDot(b);
    return false;
  };
  auto condition = [&](const Bearing b) { return at(b).symbol == Piece_symbols::dot; };
  perform_jumps(place, perform, condition);
}


// bool Gameboard::draw_piece_possibilities(const Bearing place)
// {
//   bool available_movement{ false };
//   const Player player = at(place).player;
//   const Piece piece{ at(place) };

//   auto perform = [&]([[maybe_unused]] const Bearing b) { return false; };
//   auto extra_condition = [&](const Bearing b) {
//     if (at(b).isFree) {
//       drawDot(b);
//       available_movement = true;
//       return true;
//     } else if (is_an_enemy_piece(player, b)) {
//       available_movement = true;
//       return true;
//     }
//     return false;
//   };

//   const auto draw_diagonals = [&]() {
//     iterate_from_to_and_perform(place, Direction::right, perform, extra_condition);
//     iterate_from_to_and_perform(place, Direction::top, perform, extra_condition);
//     iterate_from_to_and_perform(place, Direction::left, perform, extra_condition);
//     iterate_from_to_and_perform(place, Direction::bot, perform, extra_condition);
//   };
//   const auto draw_lanes = [&]() {
//     iterate_from_to_and_perform(place, Direction::top_right, perform, extra_condition);
//     iterate_from_to_and_perform(place, Direction::top_left, perform, extra_condition);
//     iterate_from_to_and_perform(place, Direction::bot_left, perform, extra_condition);
//     iterate_from_to_and_perform(place, Direction::right, perform, extra_condition);
//   };

//   switch (piece.symbol) {
//   case Piece_symbols::white_king:
//   case Piece_symbols::black_king: draw_king(place); break;

//   case Piece_symbols::white_rook:
//   case Piece_symbols::black_rook: draw_lanes(); break;

//   case Piece_symbols::white_bishop:
//   case Piece_symbols::black_bishop: draw_diagonals(); break;

//   case Piece_symbols::white_queen:
//   case Piece_symbols::black_queen: draw_lanes(), draw_diagonals(); break;

//   case Piece_symbols::white_knight:
//   case Piece_symbols::black_knight: draw_jumps(place); break;

//   case Piece_symbols::white_pawn:
//   case Piece_symbols::black_pawn: draw_pawn(place); break;

//   // case Piece_symbols::empty: return false; break; // for IA?
//   default: break;
//   }

//   return available_movement;
// }


bool Gameboard::examine_king_movements(const Bearing king_bearing)
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
        drawDot(end);
        available_movement = true;
        // do_if_movable
      } else if (is_an_enemy_piece(my_player, end) and not isMenaced(my_player, end)) {
        // do_if_edible
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
    drawDot({ king_bearing.x, 2 });
  }

  if (available_queen_side_castling) {
    available_movement = true;
    drawDot({ king_bearing.x, width - 2 });
  }

  return available_movement;
}

bool Gameboard::undraw_piece_possibilities(const Bearing place, const Piece piece)
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
  case Piece_symbols::black_queen: undraw_lanes(), undraw_diagonals(); break;

  case Piece_symbols::white_knight:
  case Piece_symbols::black_knight: undraw_jumps(place); break;

  case Piece_symbols::white_pawn:
  case Piece_symbols::black_pawn: undraw_pawn(place); break;

  default: break;
  }
}

Game_result Gameboard::check_end_conditions()
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

  // if (not available_movements_at(the_other_king_bearing)) {
  // if is menaced: checkmate ;else: stalemate;
  // return last_move_checkmates_the_other;
  // }


  // check whether last_move_player checkmated the_other_king
  // check whether last_move_player stalemated the_other_king
  // check insufficient material;


  return Game_result::no_results_yet;
  // stalemate
  // white checkmates black
  // black checkmates white
  // white checks black
  // black checks white
  // white insufficient material
  // black insufficient material
}
