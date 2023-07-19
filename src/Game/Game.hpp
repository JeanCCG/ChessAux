#ifndef __GAME_H__
#define __GAME_H__

// Copyright (C) 2022 José Enrique Vilca Campana
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

class Game
{
private:
  bool game_is_running{};

public:
  Game(/* args */);
  ~Game();
  void init()
  {
    // Interface
  }
};

// game_state: tie/white_turn/black_turn

/** Types of tie
 * Stalemate
 * Dead position
 * Insufficient Material
 ** king vs king
 ** king and bishop vs king
 ** king and knight vs king
 ** king and bishop vs king and bishop of the same color.
 * threefold repetition (optional)
 * 50-Move Rule (optional)
 */


Game::Game(/* args */) {}

Game::~Game() {}

#endif// __GAME_H__
