#ifndef __SCORE_H__
#define __SCORE_H__

// Copyright (C) 2023 José Enrique Vilca Campana
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "../enums/enums.hpp"
struct Score
{
  int score[2]{ 0, 0 };
  int &white_score = score[0];
  int &black_score = score[1];
  Score()
  {
    score[0] = 0;
    score[1] = 0;
  }
  Score(int a, int b) : white_score{ a }, black_score{ b } {}
  Score(const Score &s)
  {
    score[0] = s.score[0];
    score[1] = s.score[1];
  }
  int &at(const Player player);
  // Score &operator=(const Score &)
  // {
  //   //
  //   return this;
  // }
};

#endif// __SCORE_H__
