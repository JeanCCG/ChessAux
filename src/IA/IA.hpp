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
    recursive_IA(initial_gb, t_path, player, depth);
    return t_path.back();
  }
  int recursive_IA(Gameboard initial_gb,
    vector<Move> &t_path,
    Player player,
    int depth,
    Minimax minimax = Minimax::max);
};


int difference_relative_to_player(Player player, Score score) { return score.at(player) - score.at(not player); }


int IA_functor::recursive_IA(Gameboard initial_gb, vector<Move> &t_path, Player player, int depth, Minimax minimax)
{
  depth--;

  const int my_initial_score = difference_relative_to_player(player, initial_gb.score);

  // vector<Bearing> movable_ends(max_movable_ends);
  // vector<Bearing> edible_ends(max_edible_ends);
  vector<Bearing> movable_ends{};
  vector<Bearing> edible_ends{};


  const Player next_player = not player;
  const Minimax next_minimax = minimax == Minimax::max ? Minimax::min : Minimax::max;
  auto cmp = minimax == Minimax::max ? greater_than : less_than;

  int best_score_until_now = minimax == Minimax::max ? numeric_limits<int>::min() : numeric_limits<int>::max();

  vector<Move> best_path_until_now{};

  for (unsigned x = 0; x < initial_gb.width - 1; x++) {
    for (unsigned y = 0; y < initial_gb.height; y++) {
      const Bearing start{ x, y };

      bool partially_pinned{};
      Axis operative_axis{};
      const bool invalid_piece = initial_gb.at(start).empty() or initial_gb.at(start).player != player
or initial_gb.is_absolutely_pinned(start, partially_pinned, operative_axis);
      if (invalid_piece) { continue; }

      // What to do when analyzing the available movements.
      auto do_if_movable = [&movable_ends](const Bearing b) {
        movable_ends.push_back(b);
      };
      auto do_if_edible = [&edible_ends](const Bearing b) {
        edible_ends.push_back(b);
      };

      if (not initial_gb.available_movement_at(start, do_if_movable, do_if_edible)) { continue; }


      auto perform = [&](Gameboard &gb, const Bearing end) {
        int score = difference_relative_to_player(player, gb.score) - my_initial_score;
        // vector<Move> path(depth + 1);
        vector<Move> path;

        if (depth == 0 or Game_result::no_results_yet != gb.check_end_conditions()) {
          path.push_back({ start, end });
        } else {
          score += recursive_IA(gb, path, next_player, depth, next_minimax);
        }

        if (cmp(score, best_score_until_now)) {
          best_score_until_now = score;
          best_path_until_now = path;
          best_path_until_now.push_back({ start, end });
        }
      };

      for (const auto end : edible_ends) {
        Gameboard gb = initial_gb;
        gb.capture({ start, end });
        perform(gb, end);
      }
      for (const auto end : movable_ends) {
        Gameboard gb = initial_gb;
        gb.make_move({ start, end });
        perform(gb, end);
      }

      movable_ends.clear();
      edible_ends.clear();
    }
  }
  t_path = best_path_until_now;
  return best_score_until_now;
}


#endif// __IA_HPP__
