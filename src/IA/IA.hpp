#ifndef __IA_HPP__
#define __IA_HPP__

#include "../Gameboard/Gameboard.hpp"
#include <limits>
#include <vector>

using namespace std;

auto less_than = [](const int lhs, const int rhs) {
  return lhs < rhs;
};
auto greater_than = [](const int lhs, const int rhs) {
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
    auto cmp,
    int my_initial_score,
    vector<Move> &t_path,
    Gameboard &gb,
    const Move start_end);
};


int diff_relative_to_player(Player player, Score score) { return score.at(player) - score.at(not player); }

void IA_functor::my_perform(const Player player,
  const Minimax next_minimax,
  const int depth,
  const Player next_player,
  int &best_score_until_now,
  vector<Move> &best_path_until_now,
  auto cmp,
  int my_initial_score,
  vector<Move> &t_path,
  Gameboard &gb,
  const Move start_end)
{
  auto [start, end] = start_end;
  int score = diff_relative_to_player(player, gb.score) - my_initial_score;
  // vector<Move> path(depth + 1);
  vector<Move> path{ t_path };

  if (gb.check_end_conditions() != Game_result::no_results_yet) {
    path.push_back({ start, end });
  } else {
    score += recursive_IA(gb, path, next_player, depth, next_minimax);
  }

  if (cmp(score, best_score_until_now)) {
    best_score_until_now = score;
    best_path_until_now = path;
    best_path_until_now.push_back({ start, end });
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

      if (not initial_gb.available_movement_at(start, movable_append, edible_append)) { continue; }

      for (const auto end : edible_ends) {
        Gameboard gb = initial_gb;
        gb.capture({ start, end });
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
          Move{ start, end });
      }
      for (const auto end : movable_ends) {
        Gameboard gb = initial_gb;
        gb.make_move({ start, end });
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
          Move{ start, end });
      }

      movable_ends.clear();
      edible_ends.clear();
    }
  }
  t_path = best_path_until_now;
  return best_score_until_now;
}


#endif// __IA_HPP__
