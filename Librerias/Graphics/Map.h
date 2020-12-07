#pragma once

#include "Game.h"
class Map{
public:
    Map();
    ~Map();

    void LoadMap(int arr[8][8]);
    void DrawMap();

private:
    SDL_Rect src, dest;
    SDL_Texture *peon_B;
    SDL_Texture *peon_N;
    SDL_Texture *torre_B;
    SDL_Texture *torre_N;
    SDL_Texture *caballo_B;
    SDL_Texture *caballo_N;
    SDL_Texture *alfil_B;
    SDL_Texture *alfil_N;
    SDL_Texture *reina_B;
    SDL_Texture *reina_N;
    SDL_Texture *rey_B;
    SDL_Texture *rey_N;
    int map[8][8];
};
