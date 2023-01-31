#ifndef __IA_H__
#define __IA_H__

// #include "../Gameboard/Gameboard.hpp"
// #include <array>

// using namespace std;

// struct Moves_and_points
// {
//   std::vector<Move> moves{};
//   int points;
// };

// struct IA
// {
//   static const unsigned max_available_ends = 7 * 4;
//   static std::array<Bearing, max_available_ends> ends;
//   // static vector<Bearing> ends(28);
// };


// Moves_and_points IA(Player player, Gameboard gb, int depth)
// {
//   // max_number_of_moves: in a 8x8 board = 7*4
//   // max_number_of_moves_general: width - 2 + height - 2 + lowest_of(height,width) - 2
//   const unsigned width = gb.width;
//   const unsigned height = gb.height;
//   const unsigned max_available_ends = 7 * 4;
//   const vector<Bearing> ends(max_available_ends);

//   for (unsigned x = 0; x < width - 1; x++) {
//     for (unsigned y = 0; y < height; y++) {
//       const Bearing start{ x, y };
//       const Piece_symbols piece{ gb.at(start).symbol };
//       if (piece == Piece_symbols::empty) { continue; }
//       // get_all_piece_ends
//       // for end in ends:
//       switch (piece) {
//       case Piece_symbols::empty: continue;

//       default: break;
//       }
//       //
//     }
//   }
// }

#endif// __IA_H__
