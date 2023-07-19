#ifndef __IA_HPP__
#define __IA_HPP__

#include "../Gameboard/Gameboard.hpp"
#include <iostream>
#include <limits>
#include <vector>

// debug headers
#include <sstream>
#include <string>

using namespace std;

unsigned gb_counter = 0;

std::string print_gb(Gameboard &gb)
{
  // if (gb_counter < 763) { return ""; }
  std::ostringstream os;
  os << "gb_counter: " << gb_counter << "\n";

  for (unsigned x = 0; x < gb.width; x++) {
    for (unsigned y = 0; y < gb.height; y++) {
      const Bearing b{ x, y };
      const auto e = gb.at(b).symbol;
      if (e == Piece_symbols::empty) {
        os << "_";
      } else {
        os << static_cast<char>(e);
      }
    }
    os << "\n";
  }

  std::string str = os.str();
  return str;
}

const auto less_than = [](const int lhs, const int rhs) {
  return lhs < rhs;
};
const auto greater_than = [](const int lhs, const int rhs) {
  return lhs > rhs;
};

struct IA_functor
{
  enum class Minimax { min, max };
  Player my_player;

  unsigned max_movable_ends = 7 * 4;
  unsigned max_edible_ends = 8;

  explicit IA_functor(Player t_my_player /* , const Game_settings& settings */) : my_player{ t_my_player } {}
  Move operator()(const Gameboard &initial_gb, Player player, int depth)
  {
    vector<Move> t_path{};
    Gameboard gb_copy = initial_gb;
    recursive_IA(gb_copy, t_path, player, depth);
    return t_path.front();
  }

  int recursive_IA(Gameboard initial_gb,
    vector<Move> &t_path,
    Player player,
    int depth,
    Minimax minimax = Minimax::max);

  void my_perform(const Player player,
    const Minimax next_minimax,
    const int depth,
    const Player next_player,
    int &best_score_until_now,
    vector<Move> &best_path_until_now,
    const auto cmp,
    int my_initial_score,
    vector<Move> &t_path,
    Gameboard &gb,
    const Move move);
};


int diff_relative_to_player(Player player, Score score) { return score.at(player) - score.at(not player); }

unsigned sup = 0;

void IA_functor::my_perform(const Player player,
  const Minimax next_minimax,
  const int depth,
  const Player next_player,
  int &best_score_until_now,
  vector<Move> &best_path_until_now,
  const auto cmp,
  int my_initial_score,
  vector<Move> &t_path,
  Gameboard &gb,
  const Move move)
{
  t_path.push_back(move);
  int score = diff_relative_to_player(player, gb.score) - my_initial_score;
  // vector<Move> path(depth + 1);
  vector<Move> path{ t_path };

  std::string str = print_gb(gb);
  gb_counter = gb_counter;
  if (gb.check_end_conditions() == Game_result::no_results_yet) {
    gb_counter = gb_counter;
    sup++;
    sup = sup;
    Gameboard copy_gb = gb;
    sup = sup;
    score += recursive_IA(copy_gb, path, next_player, depth, next_minimax);//! first freed memory here
    sup = sup;//* NOTE: i suspect the operator=() in page, keys are different but somehow the pages are double-freed
  }//! second freed memory here

  if (cmp(score, best_score_until_now)) {
    best_score_until_now = score;
    best_path_until_now = path;
  }
}


int IA_functor::recursive_IA(Gameboard initial_gb, vector<Move> &t_path, Player player, int depth, Minimax minimax)
{
  depth--;
  const int my_initial_score = diff_relative_to_player(player, initial_gb.score);
  if (depth == 0) { return my_initial_score; }


  vector<Bearing> movable_ends{};// vector<Bearing> movable_ends(max_movable_ends);
  vector<Bearing> edible_ends{};// vector<Bearing> edible_ends(max_edible_ends);
  // What to do when analyzing the available movements.
  auto movable_append = [&movable_ends](const Bearing b) {
    movable_ends.push_back(b);
  };
  auto edible_append = [&edible_ends](const Bearing b) {
    edible_ends.push_back(b);
  };

  const Player next_player = not player;
  const Minimax next_minimax = minimax == Minimax::max ? Minimax::min : Minimax::max;

  auto cmp = minimax == Minimax::max ? greater_than : less_than;

  int best_score_until_now = minimax == Minimax::max ? numeric_limits<int>::min() : numeric_limits<int>::max();

  vector<Move> best_path_until_now{};

  for (unsigned x = 0; x < initial_gb.width; x++) {
    for (unsigned y = 0; y < initial_gb.height; y++) {
      const Bearing start{ x, y };

      bool partially_pinned{};
      Axis operative_axis{};
      const bool invalid_piece = initial_gb.at(start).empty() or initial_gb.at(start).player != player
                                 or initial_gb.is_absolutely_pinned(start, partially_pinned, operative_axis);
      if (invalid_piece) { continue; }

      //
      gb_counter = gb_counter;
      // at 52 it breaks
      std::string str = print_gb(initial_gb);

      if (not initial_gb.available_movement_at(start, movable_append, edible_append)) { continue; }

      // vector<Page<3> *> initial_gb_map_interceptors = initial_gb.interceptor_map.interceptors();
      //
      for (const auto end : edible_ends) {
        gb_counter++;
        Gameboard gb = initial_gb;
        // print_gb(gb);
        const Move move = { start, end };
        gb.capture(move);
        // vector<Page<3> *> gb_map_interceptors = initial_gb.interceptor_map.interceptors();
        // print_gb(gb);
        my_perform(player,
          next_minimax,
          depth,
          next_player,
          best_score_until_now,
          best_path_until_now,
          cmp,
          my_initial_score,
          t_path,
          gb,
          move);
      }
      for (const auto end : movable_ends) {
        Gameboard gb = initial_gb;
        const Move move = { start, end };
        gb.make_move(move);
        /* Here
        ! #8 frees the memory
        ! #8 double-frees the memory
        */
        my_perform(player,
          next_minimax,
          depth,
          next_player,
          best_score_until_now,
          best_path_until_now,
          cmp,
          my_initial_score,
          t_path,
          gb,
          move);
      }

      movable_ends.clear();
      edible_ends.clear();
    }
  }
  t_path = best_path_until_now;
  return best_score_until_now;
}


#endif// __IA_HPP__
