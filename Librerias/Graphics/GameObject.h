#pragma once
#include "Game.h"

class GameObject {
public:
    GameObject(const char* texturesheet, int x, int y);
    ~GameObject();

    void Update();
    void Render(int a, int b);
    void RenderBackground();
    int piece_xpos();
    int piece_ypos();

private:

    int xpos, ypos;

    SDL_Texture *objTexture;
    SDL_Rect srcRect, destRect;

};
