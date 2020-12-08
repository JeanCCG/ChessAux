#include "Moves.h"

int position_x() {
    if (Game::event.motion.x >= 0 && Game::event.motion.x <100){
        return 0;
    }else if (Game::event.motion.x >=100 && Game::event.motion.x <200){
        return 100;
    }else if (Game::event.motion.x >=200 && Game::event.motion.x <300){
        return 200;
    }else if(Game::event.motion.x >= 300 && Game::event.motion.x <400){
        return 300;
    }else if (Game::event.motion.x >= 400 && Game::event.motion.x <500){
        return 400;
    }else if (Game::event.motion.x >= 500 && Game::event.motion.x <600){
        return 500;
    }else if (Game::event.motion.x >= 600 && Game::event.motion.x <700){
        return 600;
    }else if (Game::event.motion.x >= 700 && Game::event.motion.x <800){
        return 700;
    }
}

int position_y(){
    if(Game::event.motion.y >=0 && Game::event.motion.y <100){
        return 0;
    }else if(Game::event.motion.y >=100 && Game::event.motion.y <200){
        return 100;
    }else if(Game::event.motion.y >=200 && Game::event.motion.y <300){
        return 200;
    }else if(Game::event.motion.y >=300 && Game::event.motion.y <400){
        return 300;
    }else if(Game::event.motion.y >=400 && Game::event.motion.y <500){
        return 400;
    }else if(Game::event.motion.y >=500 && Game::event.motion.y <600){
        return 500;
    }else if(Game::event.motion.y >=600 && Game::event.motion.y <700){
        return 600;
    }else if(Game::event.motion.y >=700 && Game::event.motion.y <800){
        return 700;
    }
}

