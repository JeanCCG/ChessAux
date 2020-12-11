#include "Moves.h"

int position_x() {
    return Game::event.motion.x / 100;
}

int position_y() {
    return Game::event.motion.y / 100;
}

